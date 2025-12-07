#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "utils.h"
#include <string>

using namespace std;

struct Transaction {
    string id;
    string type;
    double amount;
    int year;
    int month;
    int day;

    //add a status variable to track if the transaction was canceled by the customer or finalized by an employee
    string status; //active, canceled, or finalized;

    Transaction* next;
};

struct FinalizedTransaction {
    string id;
    string type;
    double amount;
    int year;
    int month;
    int day;
    string accountNb;
    string employeeId;
    FinalizedTransaction* next;
};

struct FinalizedTransactionList {
    FinalizedTransaction* head;
    int size;
};

extern FinalizedTransactionList finalizedTransactionsList;
extern int lastIdTransaction;

void addTransaction(int accIndex, const string &type, double amount);

void deposit(int accIndex, double amount);
void withdraw(int accIndex, double amount);
void viewTodaysTransactionsOneCustomer(int accIndex);
void viewAllTransactionsOneCustomer(int accIndex);
void viewTodaysTransactionsAllCustomers();

void undoLastTransaction(int accIndex);
void finalizeDay(int year, int month, int day, const string& employeeId);

void showFinalizedTransactions();
void saveFinalizedTransactionsToFile(const string& filename);
void loadFinalizedTransactionsFromFile(const string& filename);

string twoDigits(int n);
string fourDigits(int n);

#endif