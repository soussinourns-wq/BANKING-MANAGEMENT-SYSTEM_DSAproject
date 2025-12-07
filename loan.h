#ifndef LOAN_H
#define LOAN_H

#include <iostream>
#include <string>
#include "utils.h"

using namespace std;
 
struct Loan {
    int loanID;
    string loanType;
    double principal;
    double interestRate;
    double amountPaid;
    double remaining;
    string startDate;
    string endDate;
    string status;
    int yearRequest;
    int monthRequest;
    int dayRequest;
    Loan* next;
    Loan* prev;
};

struct LoansList {
    Loan* loansHead;
    Loan* loansTail;
    int numberLoans;
};

// Add a struct for the requested loans
struct LoanStruct {
    string customerID;
    Loan* loanInfo;  // Pointer to the loan details
    LoanStruct* next;  // For FIFO queue
};


extern LoanStruct* requestedLoansFront;  // Front of FIFO queue
extern LoanStruct* requestedLoansRear;   // Rear of FIFO queue
extern LoanStruct* completedLoansHead;  // Singly linked list for completed loans
extern int nextLoanID;                      // Starting loan ID


void requestLoan(string customerID);
void processLoanRequests();
void addLoanToCustomer(int accountIndex, Loan* loan);

void displayLoanDetails(Loan* current);

void changeLoanStatus();
void moveCompletedLoans();

void viewCustomerLoans(int indexCustomer);

#endif