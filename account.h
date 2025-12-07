#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "loan.h"
#include "transaction.h"

//define the structure Account
struct Account {
    string accNumber;
    string type;
    string iban;
    string branchCode;
    string holder;
    int openYear;
    int openMonth;
    int openDay;
    int status;
    double balance;
    LoansList* loans;
    Transaction* transTop;

    string password;

};


//declaration of the accounts array and the counter

extern Account accounts[100];
extern int accountCount;

extern Account archivedAccounts[100];
extern int archivedAccountCount;

//add the functions prototypes
void addCustomerAccount();
void displayAccounts(Account accounts[], int& accountCount);
int accountIndex(const string &accountNb);
int indexInArchived(const string &accountNb);
void changeAccountStatus();
void archiveClosedAccounts();
int enterStatus();

void changePassword(int accIndex);

#endif