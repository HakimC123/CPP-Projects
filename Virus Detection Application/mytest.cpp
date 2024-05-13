#include "vdetect.h"
#include <random>
#include <vector>
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{

    bool getVirus();
    bool getVirus1();
    bool checkRemove();

};

bool Tester::getVirus(){

    vector<Virus> dataList;
    VDetect vdetect(MINPRIME, hashCode, DOUBLEHASH);
    bool result = true;

    Virus virus1 = Virus("CGGAT", 2344);
    dataList.push_back(virus1);
    vdetect.insert(virus1);

    Virus virus2 = Virus("CAGAT", 2334);
    dataList.push_back(virus2);
    vdetect.insert(virus2);

    Virus virus3 = Virus("CAAAT", 2554);
    dataList.push_back(virus3);
    vdetect.insert(virus3);

    Virus virus4 = Virus("GGAAT", 6454);
    dataList.push_back(virus4);
    vdetect.insert(virus4);

    Virus virus5 = Virus("AGAAT", 6554);
    dataList.push_back(virus5);
    vdetect.insert(virus5);

    for (vector<Virus>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == vdetect.getVirus((*it).getKey(), (*it).getID()));
    }

    if(result && vdetect.getVirus("ZFGDA", 3455).getKey() == "ZFGDA" && vdetect.getVirus("ZFGDA", 3455).getId() == 3455 ){

        return true;

    }else{

        return false;

    }

}

bool Tester::getVirus1(){

    vector<Virus> dataList;
    VDetect vdetect(MINPRIME, hashCode, QUADRATIC);
    bool result = true;

    Virus virus1 = Virus("CTTAT", 1234);
    dataList.push_back(virus1);
    vdetect.insert(virus1);

    Virus virus2 = Virus("ACCAC", 4534);
    dataList.push_back(virus2);
    vdetect.insert(virus2);

    Virus virus3 = Virus("TAAAC", 2533);
    dataList.push_back(virus3);
    vdetect.insert(virus3);

    Virus virus4 = Virus("GGGAG", 9654);
    dataList.push_back(virus4);
    vdetect.insert(virus4);

    Virus virus5 = Virus("AGCAG", 6594);
    dataList.push_back(virus5);
    vdetect.insert(virus5);

    for (vector<Virus>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == vdetect.getVirus((*it).getKey(), (*it).getID()));
    }

    if(result && vdetect.getVirus("GGGAG", 9654).getKey() == "GGGAG" && vdetect.getVirus("GGGAG", 9654).getId() == 9654 ){

        return true;

    }else{

        return false;

    }

}

bool Tester::checkRemove(){

    vector<Virus> dataList;
    VDetect vdetect(MINPRIME, hashCode, QUADRATIC);
    bool result = true;

    Virus virus1 = Virus("CTTAT", 1234);
    dataList.push_back(virus1);
    vdetect.insert(virus1);

    Virus virus2 = Virus("ACCAC", 4534);
    dataList.push_back(virus2);
    vdetect.insert(virus2);

    Virus virus3 = Virus("TAAAC", 2533);
    dataList.push_back(virus3);
    vdetect.insert(virus3);

    Virus virus4 = Virus("GGGAG", 9654);
    dataList.push_back(virus4);
    vdetect.insert(virus4);

    Virus virus5 = Virus("AGCAG", 6594);
    dataList.push_back(virus5);
    vdetect.insert(virus5);

    vdetect.remove(virus3);

    for (vector<Virus>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == vdetect.getVirus((*it).getKey(), (*it).getID()));
    }

    if(result && vdetect.getVirus("TAAAC", 2533).getKey() == "TAAAC" && vdetect.getVirus("TAAAC", 2533).getId() == 2533 ){

        return true;

    }else{

        return false;

    }


}


unsigned int hashCode(const string str);
string sequencer(int size, int seedNum);

int main(){
    
    Tester tester;

    vector<Virus> dataList;
    vector<Virus> dataList1;
    vector<Virus> dataList2;
    vector<Virus> dataList3;

    Random RndID(MINID,MAXID);
    VDetect vdetect(MINPRIME, hashCode, DOUBLEHASH);
    bool result = true;
    for (int i=0;i<49;i++){
        // generating random data
        Virus dataObj = Virus(sequencer(5, i), RndID.getRandNum());
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the VDetect object
        vdetect.insert(dataObj);
    }
    // dumping the data points to the standard output
    vdetect.dump();
    // checking whether all data are inserted
    for (vector<Virus>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == vdetect.getVirus((*it).getKey(), (*it).getID()));
    }
    if (result)
        cout << "All data points exist in the VDetect object!\n";
    else
        cout << "Some data points are missing in the VDetect object\n";
    
    
    VDetect vdetect1(MAXPRIME, hashCode, DOUBLEHASH);
    bool result1 = true;
    for (int i=0;i<100;i++){
        // generating random data
        Virus dataObj1 = Virus(sequencer(5, i), RndID.getRandNum());
        // saving data for later use
        dataList1.push_back(dataObj1);
        // inserting data in to the VDetect object
        vdetect1.insert(dataObj1);
    }
    // dumping the data points to the standard output
    vdetect1.dump();
    // checking whether all data are inserted
    for (vector<Virus>::iterator it = dataList1.begin(); it != dataList1.end(); it++){
        result1 = result1 && (*it == vdetect1.getVirus((*it).getKey(), (*it).getID()));
    }
    if (result1)
        cout << "All data points exist in the VDetect object!\n";
    else
        cout << "Some data points are missing in the VDetect object\n";

    VDetect vdetect2(MINPRIME, hashCode, QUADRATIC);
    bool result2 = true;
    for (int i=0;i<213;i++){
        // generating random data
        Virus dataObj2 = Virus(sequencer(5, i), RndID.getRandNum());
        // saving data for later use
        dataList2.push_back(dataObj2);
        // inserting data in to the VDetect object
        vdetect2.insert(dataObj2);
    }
    // dumping the data points to the standard output
    vdetect2.dump();
    // checking whether all data are inserted
    for (vector<Virus>::iterator it = dataList2.begin(); it != dataList2.end(); it++){
        result2 = result2 && (*it == vdetect2.getVirus((*it).getKey(), (*it).getID()));
    }
    if (result2)
        cout << "All data points exist in the VDetect object!\n";
    else
        cout << "Some data points are missing in the VDetect object\n";
    
    
    VDetect vdetect3(MAXPRIME, hashCode, QUADRATIC);
    bool result3 = true;
    for (int i=0;i<63;i++){
        // generating random data
        Virus dataObj3 = Virus(sequencer(5, i), RndID.getRandNum());
        // saving data for later use
        dataList3.push_back(dataObj3);
        // inserting data in to the VDetect object
        vdetect3.insert(dataObj3);
    }
    // dumping the data points to the standard output
    vdetect3.dump();
    // checking whether all data are inserted
    for (vector<Virus>::iterator it = dataList3.begin(); it != dataList3.end(); it++){
        result3 = result3 && (*it == vdetect3.getVirus((*it).getKey(), (*it).getID()));
    }
    if (result3)
        cout << "All data points exist in the VDetect object!\n";
    else
        cout << "Some data points are missing in the VDetect object\n";

    cout << "Testing getVirus...." << endl;
    if(tester.getVirus()){

        cout << "getVirus test worked!" << endl;

    }else{

        cout << "getVirus test did not work :(" << endl;

    }
    cout << "Testing getVirus1())...." << endl;
    if(tester.getVirus1()){

        cout << "getVirus1 test worked!" << endl;

    }else{

        cout << "getVirus1 test did not work :(" << endl;

    }

    cout << "Testing checkRemove())...." << endl;
    if(tester.checkRemove()){

        cout << "checkRemove test worked!" << endl;

    }else{

        cout << "checkRemove test did not work :(" << endl;

    }


    return 0;
}

unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

string sequencer(int size, int seedNum){
    //this function returns a random DNA sequence
    string sequence = "";
    Random rndObject(0,3);
    rndObject.setSeed(seedNum);
    for (int i=0;i<size;i++){
        sequence = sequence + ALPHA[rndObject.getRandNum()];
    }
    return sequence;
}
