#ifndef STATISTICS_H
#define STATISTICS_H

#include "account.h"
#include "employee.h"

void statisticsMenu();
int totalNumberOfLoans();
int numberOfLoansByType(const string& type);
int numberOfLoansByStatus(const string& status);
void activeLoansInRange(int startD, int startM, int startY, int endD, int endM, int endY);
bool isDateInRange(int y, int m, int d, int startY, int startM, int startD, int endY, int endM, int endD);

int customerWithMostLoans(int& maxLoans);

int customerWithHighestBalance();
int customerWithLowestBalance();

int employeesPerBranch(const string& branch);

#endif