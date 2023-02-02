//=========================================================================================================================================
//Name: Hakim Chughtai
//Project: Loan Calculator
//Description: This program will acheive the task of taking in a loan amount, interest of the loan and the duration of the loan and
//then calculate the total loan interest, amount, and the monthly payment of the loan
//=========================================================================================================================================

#include <iostream>
using namespace std;

int main()
{
    char input; //Creates a variable called input which takes in letters as input

    do {

        cout << "Enter loan amount: "; //Prompts the user to enter loan amount
        double face; //Creates a variable called face to represent the face value
        cin >> face; //Stores the loan amount in a variable called face

        cout << "Enter interest rate: "; //Prompts the user to enter interest rate
        double interestRate; //Creates a variable called interestRate to represent the rate of interest on this loan
        cin >> interestRate; //Stores the interest rate to a variable called interestRate

        interestRate = (interestRate / 100);


        cout << "Enter duration of the loan (in months): ";
        double loanDuration; //Creates a variable called loan duration 
        cin >> loanDuration; //Stores the amount of time the loan is in a variable callled loanDuration

        double yearDuration = (loanDuration / 12); //Creates a variable called yearDuration and set it to the loan duration in months over 12
        double interest = face * interestRate * yearDuration; //Calculates the interest by multiplying the face by the interest rate by the year duration
        double monthlyPayment = (face / loanDuration); //Calculates the monthly payment by taking the face value and dividing it by the duration of the loan
        double loan = (face - interest); //Calculates the loan by taking the face value and subtracting it by the interest

        cout.setf(ios::fixed);
        cout.setf(ios::showpoint);
        cout.precision(2);

        cout << "Total loan interest is: $" << interest << "\n" ; //Prints out the total loan interest
        cout << "Given amount: $" << loan << "\n"; //Prints out the given amount
        cout << "Monthly Payment: $" << monthlyPayment << " for " << loanDuration << " months" << "\n"; //Prints out the monthly payment

        cout << "Would you like to repeat this procedure (Enter Y or N)"; //Asks if the user would like to repeat the process
        cin >> input; //Takes in the input and stores it in a variable called input

    } while (input == 'Y' || input == 'y');

}


