// CMSC 341 - Spring 2023 - Project 4
#include "vdetect.h"
VDetect::VDetect(int size, hash_fn hash, prob_t probing = DEFPOLCY){
    
    //Initializes all member variables
    m_hash = hash;          
    m_newPolicy = NONE;      

    bool prime = isPrime(size);
    if(prime == false){

        m_currentCap = findNextPrime(size);

    }else if(prime == true && (size >= MINPRIME && size <= MAXPRIME) ){

        m_currentCap = size;

    }else if(size < MINPRIME){

        m_currentCap = MINPRIME;

    }else if(size > MAXPRIME){

        m_currentCap = MAXPRIME;

    }

    m_currentTable = new Virus[m_currentCap];  
        
    m_currentSize = 0;   
    
    m_currNumDeleted = 0;
    m_currProbing = probing;   

    m_oldCap = 0;   
    m_oldTable = nullptr;      
         
    m_oldSize = 0;       
    
    m_oldNumDeleted = 0; 
    m_oldProbing = NONE;    

}

VDetect::~VDetect(){

    delete [] m_currentTable;
    delete [] m_oldTable;

}

void VDetect::changeProbPolicy(prob_t policy){
    
   m_newPolicy = policy; //Sets the new policy to be the inputted policy

}

bool VDetect::insert(Virus virus){
    
    //Returns false if the virus is out of range
    if(virus.m_id < MINID || virus.m_id > MAXID){

        return false;

    }

    //Returns false if there already exist this virus in the table
    if(virus == getVirus(virus.m_key, virus.m_id)){
        
        return false;
    
    }
    
    bool isInserted = false; //Sets isInserted to false
    int index = m_hash(virus.m_key) % m_currentCap; //Initialize an index

    //If the current index is empty, insert the value
    if(m_currentTable[index] == EMPTY){

        m_currentTable[index] = virus;
        m_currentSize++;
        isInserted = true;

    }else{
        
        //If collision occurs, do quadratic probing and double hash and insert accordingly
        if(m_currProbing == QUADRATIC){

            int i = 0;
            while(i < m_currentCap/2){

                index = ((m_hash(virus.m_key) % m_currentCap) + (i * i)) % m_currentCap;
                if(m_currentTable[index] == EMPTY){

                    m_currentTable[index] = virus;
                    m_currentSize++;
                    isInserted = true;
                    break;

                }

                i++;

            }


        }else if(m_currProbing == DOUBLEHASH){

            int i = 0;
            while(i < m_currentCap){

                index = ((m_hash(virus.m_key) % m_currentCap) + i * (11 - (m_hash(virus.m_key) % 11))) % m_currentCap; 
                if(m_currentTable[index] == EMPTY){

                    m_currentTable[index] = virus;
                    m_currentSize++;
                    isInserted = true;
                    break;

                }

                i++;

            }

        }

    }

    //If insertion is successful, rehash
    if(isInserted){

        rehash(lambda() > 0.5);

    }

    return isInserted;

}

//Helper function for insert, used in the rehashing function, same behavior as insert without additional boundary checking
bool VDetect::insertHelper(Virus virus){

    bool isInserted = false;
    int index = m_hash(virus.m_key) % m_currentCap;

    if(m_currentTable[index] == EMPTY){

        m_currentTable[index] = virus;
        m_currentSize++;
        isInserted = true;

    }else{

        if(m_currProbing == QUADRATIC){

            int i = 0;
            while(i < m_currentCap/2){

                index = ((m_hash(virus.m_key) % m_currentCap) + (i * i)) % m_currentCap;
                if(m_currentTable[index] == EMPTY){

                    m_currentTable[index] = virus;
                    m_currentSize++;
                    isInserted = true;
                    break;

                }

                i++;

            }


        }else if(m_currProbing == DOUBLEHASH){

            int i = 0;
            while(i < m_currentCap){

                index = ((m_hash(virus.m_key) % m_currentCap) + i * (11 - (m_hash(virus.m_key) % 11))) % m_currentCap; 
                if(m_currentTable[index] == EMPTY){

                    m_currentTable[index] = virus;
                    m_currentSize++;
                    isInserted = true;
                    break;

                }

                i++;

            }

        }

    }

    return isInserted;


}

bool VDetect::remove(Virus virus){
    
    bool isRemoved = false; //Sets the isRemoved to false

    int index = findCurrent(virus); //Sets teh index to find current index in the table
    if(index != -1){

        //If index is not -1, set the index to be deleted if it is not 
        if(!(m_currentTable[index] == DELETED)){

            m_currentTable[index] = DELETED;
            m_currNumDeleted++;
            m_currentSize--;
            isRemoved = true;

        }

    }

    //If there is an old table
    if(m_oldTable != nullptr){

        int index = findOld(virus); //Sets the index to find the virus in the old table

        //If the index doesn't evaluate to -1
        if(index != -1){

            //If the index in the old table is not deleted, then delete the index
            if(!(m_oldTable[index] == DELETED)){

                m_oldTable[index] = DELETED;
                m_oldNumDeleted++;
                m_oldSize--;
                isRemoved = true;

            }

        }

    }

    if(isRemoved){

        
        rehash(deletedRatio() > 0.8); //Rehash if the removal is successful

    }

    return isRemoved;



}

//Helper function to remove an object from the table, same algorithm as the remove
bool VDetect::removeHelper(Virus virus){

    bool isRemoved = false;

    int index = findCurrent(virus);
    if(index != -1){

        if(!(m_currentTable[index] == DELETED)){

            m_currentTable[index] = DELETED;
            m_currNumDeleted++;
            m_currentSize--;
            isRemoved = true;

        }

    }

    if(m_oldTable != nullptr){

        int index = findOld(virus);
        if(index != -1){

            if(!(m_oldTable[index] == DELETED)){

                m_oldTable[index] = DELETED;
                m_oldNumDeleted++;
                m_oldSize--;
                isRemoved = true;

            }

        }

    }

    return isRemoved;

}

void VDetect::rehash(bool checkFactor){

    //If the factor is true and there are no table,    
    if(checkFactor && !m_oldTable){
        
        //IF the currentCap is greater than the max prime, then do nothiing
        if(m_currentCap >= MAXPRIME){

            return;

        }

        //Sets the m_crrent information to be the information for m_old table
        m_oldTable = m_currentTable;
        m_oldCap = m_currentCap;
        m_oldSize = m_currentSize;
        m_oldNumDeleted = m_currNumDeleted;

        //Sets the data point to be the curr size - the curr num deleted, then use datapoint to mulyiply by 4, and set the index to find the next prime
        int datapoint = m_currentSize - m_currNumDeleted;
        int numBeforePrime = 4*datapoint;
        int index = findNextPrime(numBeforePrime);

        m_currentCap = index; //Sets the current cap to this index

        //construct a new current table
        m_currentTable = new Virus[m_currentCap];
        m_currentSize = 0;
        m_currNumDeleted = 0;
        m_currProbing = (m_newPolicy == NONE) ? m_currProbing : m_newPolicy;

        //Set the transferred size to be the oldsize/4
        int transfer = m_oldSize/4;
        int rehash = 0; //Sets rehash to 0

        //Loops through the index maximized at oldCap
        for(int i = 0; i < m_oldCap; i++){

            //Break if rehash is the same value as transferred
            if(rehash == transfer){

                break;

            }

            //Break if the old num deleted is greater than the old size
            if(m_oldNumDeleted >= m_oldSize){

                break;

            }

            //If the index is empty or deleted, proceed
            if(m_oldTable[i] == EMPTY || m_oldTable[i] == DELETED){

                continue;

            }

            //Create a temp insert index to be the old table index
            Virus tempInsert = m_oldTable[i]; 
            removeHelper(m_oldTable[i]); //Remove the old table index
            insertHelper(tempInsert); //Insert the temp insert
            rehash++; //Increment the rehash size

        }

        //If the old num deleted is bigger than the old size, delete the old table
        if(m_oldNumDeleted >= m_oldSize){

            delete[] m_oldTable;
            m_oldTable = nullptr;
            m_oldCap = 0;
            m_oldSize = 0;
            m_oldNumDeleted = 0;
            m_oldProbing = NONE;

        }

    }

}

Virus VDetect::getVirus(string key, int id) const{
    
    Virus findVirus = Virus(key, id); //Create the findVirus object based on the key and id inputted
    Virus virus = Virus(); //Create the empty virus object

    int index = findCurrent(findVirus); //Sets the index to find the index in the current table of this findVirus information

    if(index != -1){

        return m_currentTable[index]; //Returns the index if the index is not -1

    }

    //Do the same thing as above if the old table is not null
    if(m_oldTable != nullptr){

        int index = findOld(findVirus);
        if(index != -1){

            return m_oldTable[index];

        }      

    }

    return virus; //Returns the virus

}

//Helper function to find the current virus in the table
int VDetect::findCurrent(Virus find) const{
   
   int index = m_hash(find.m_key) % m_currentCap; //Sets the index

   //If the index is not empty. them check to see if the id and the key are the same
   if(!(m_currentTable[index] == EMPTY)){

        Virus virus = m_currentTable[index];
        if(find.m_id == virus.m_id){

            if(find.m_key == virus.m_key){

                return index;

            }

        }

   }

   int i = 0; //Initialize the i value

   //While the i is less than the size of the table halved
   while(i < m_currentCap/2){

    index = ((m_hash(find.m_key) % m_currentCap) + (i * i)) % m_currentCap; //Probe the index using quadratic probing

    //If the table index is occupied, then check to see if the virus is in this index
    if(!(m_currentTable[index] == EMPTY)){

        Virus virus = m_currentTable[index];
        if(find.m_id == virus.m_id){

            if(find.m_key == virus.m_key){

                return index;

            }

        }
   }

    i++;

   }

   i = 0;
   while(i < m_currentCap){

    index = ((m_hash(find.m_key) % m_currentCap) + i * (11 - (m_hash(find.m_key) % 11))) % m_currentCap; //Probe the index using double hash probing

    //If the table is occupied, then check if there is the object in this index
    if(!(m_currentTable[index] == EMPTY)){

        Virus virus = m_currentTable[index];
        if(find.m_id == virus.m_id){

            if(find.m_key == virus.m_key){

                return index;

            }

        }
   }

    i++;
   }

   return -1; //Returns -1 if the virus is not found

}

//Helper function to find the virus in the old table
int VDetect::findOld(Virus find) const{

    //Same algorithm as the findCurrent function but this function deals with old table
    int index = m_hash(find.m_key) % m_oldCap;
   if(!(m_oldTable[index] == EMPTY)){

        Virus virus = m_oldTable[index];
        if(find.m_id == virus.m_id){

            if(find.m_key == virus.m_key){

                return index;

            }

        }

   }

   int i = 0;
   while(i < m_oldCap/2){

    index = ((m_hash(find.m_key) % m_oldCap) + (i * i)) % m_oldCap; 
    if(!(m_oldTable[index] == EMPTY)){

        Virus virus = m_oldTable[index];
        if(find.m_id == virus.m_id){

            if(find.m_key == virus.m_key){

                return index;

            }

        }
   }
    i++;
   }

   i = 0;
   while(i < m_oldCap){

    index = ((m_hash(find.m_key) % m_oldCap) + i * (11 - (m_hash(find.m_key) % 11))) % m_oldCap; 
    if(!(m_oldTable[index] == EMPTY)){

        Virus virus = m_oldTable[index];
        if(find.m_id == virus.m_id){

            if(find.m_key == virus.m_key){

                return index;

            }

        }
   }
    i++;
   }

   return -1;
}

float VDetect::lambda() const {
    
    //Sets the occupied bucket to be the toal of the size and the num deleted
    float occupiedBucket = (float)(m_currentSize + m_currNumDeleted);
    return occupiedBucket/(float) m_currentCap; //returns occupied bucket over total size

}

float VDetect::deletedRatio() const {
    
    //Sets the occupied bucket to be the total of the current size and current number deleted
    float occupiedBucket = (float)(m_currentSize + m_currNumDeleted); 
    return (float) m_currNumDeleted/(float) occupiedBucket; //Returns the current number deleted over the occupied buckets

}

void VDetect::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool VDetect::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int VDetect::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

ostream& operator<<(ostream& sout, const Virus &virus ) {
    if (!virus.m_key.empty())
        sout << virus.m_key << " (ID " << virus.m_id << ")";
    else
        sout << "";
  return sout;
}

bool operator==(const Virus& lhs, const Virus& rhs){
    return ((lhs.m_key == rhs.m_key) && (lhs.m_id == rhs.m_id));
}
