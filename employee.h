#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "utils.h"

#include <algorithm>
#include <iostream>
using namespace std;

//the struct Employee
struct Employee {
    string id;
    string name;
    string lastname;
    string address;
    double salary;
    int hireYear;
    int hireMonth;
    int hireDay;
    string branchcode; 
};



//declaration of the array of employees and the counter
extern Employee employees[100];
extern int employeeCount;

//functions prototypes
bool employeeLogin();
void employeeMenu();
void employeeTransactionsMenu();

void addEmployee();
void deleteEmployee();
void modifyEmployee();

void showEmployee(int index);

void showEmployeesSorted();
void showEmployeesByBranch();

int employeeIdExists(const string &id);

void showFirstRecruitedEmployee();
void showLastRecruitedEmployee();

bool isEarlierDate(int y1, int m1, int d1, int y2, int m2, int d2);
bool isLaterDate(int y1, int m1, int d1, int y2, int m2, int d2);

#endif