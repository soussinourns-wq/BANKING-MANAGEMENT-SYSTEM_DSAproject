#include "statistics.h"

void statisticsMenu() {
    cout << "|---------------------------|\n";
    cout << "|----- Statistics Menu -----|\n";
    cout << "|---------------------------|\n";
    cout << "1. Total number of loans\n";
    cout << "2. Number of loans by type\n";
    cout << "3. Number of loans by status\n";
    cout << "4. Active loans within a specified date range\n";
    cout << "5. Customer with the highest number of loans\n";
    cout << "6. Customer with the highest account balance\n";
    cout << "7. Customer with the lowest account balance\n";
    cout << "8. Total number of employees\n";
    cout << "9. Number of employees per bank branch\n";
    cout << "10. Back\n";
}

int totalNumberOfLoans() {
    int total = 0;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].loans != nullptr)
            total += accounts[i].loans->numberLoans;
    }
    return total;
}

int numberOfLoansByType(const string& type) {
    int count = 0;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].loans == nullptr) continue;

        Loan* current = accounts[i].loans->loansHead;
        while (current != nullptr) {
            if (current->loanType == type)
                count++;
            current = current->next;
        }
    }
    return count;
}

int numberOfLoansByStatus(const string& status) {
    int count = 0;

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].loans == nullptr) continue;

        Loan* current = accounts[i].loans->loansHead;
        while (current != nullptr) {
            if (current->status == status)
                count++;
            current = current->next;
        }
    }
    return count;
}

void activeLoansInRange(int startD, int startM, int startY, int endD, int endM, int endY)
{
    int count = 0;

    cout << "\n-----------------------------------------\n";
    cout << "     Active loans in the given range\n";
    cout << "-----------------------------------------\"n";

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].loans == nullptr) continue;

        Loan* current = accounts[i].loans->loansHead;

        while (current != nullptr) {

            // Check if the loan is active and its request date is in the specified range
            if (current->status == "active" &&
                isDateInRange(current->yearRequest, current->monthRequest, current->dayRequest,
                              startY, startM, startD,
                              endY, endM, endD))
            {
                count++;

                //show the loan's information
                cout << "\nLoan number: " << current->loanID << " (Active)\n";
                cout << "--------------------------------------------\n";
                cout << "Holder:        " << accounts[i].holder << "\n";
                cout << "Account No.:   " << accounts[i].accNumber << "\n";
                cout << "Amount Paid:   " << current->amountPaid << "\n";
                cout << "Remaining:     " << current->remaining << "\n";
                cout << "Start Date:    " << current->startDate << "\n";
                cout << "End Date:      " << current->endDate << "\n";
                cout << "Request Date:  "
                     << current->dayRequest << "-" 
                     << current->monthRequest << "-" 
                     << current->yearRequest << "\n";
            }

            current = current->next;
        }
    }

    cout << "\n----------------------------------------------------\n";
    cout << "The total active loans in the specified range is: \n" << count;
    cout << "\n---------------------------------------------------\n";

}

bool isDateInRange(int y, int m, int d, int startY, int startM, int startD, int endY, int endM, int endD)
{
    if (y < startY || (y == startY && m < startM) ||
        (y == startY && m == startM && d < startD))
        return false;

    if (y > endY || (y == endY && m > endM) ||
        (y == endY && m == endM && d > endD))
        return false;

    return true;
}



int customerWithMostLoans(int& maxLoans) {
    int customerIndex = -1;
    maxLoans = -1;

    for (int i = 0; i < accountCount; i++) {
        int n = (accounts[i].loans ? accounts[i].loans->numberLoans : 0);
        if (n > maxLoans) {
            maxLoans = n;
            customerIndex = i;
        }
    }

    return customerIndex;  // returns index in accounts[]
}

int customerWithHighestBalance() {
    if (accountCount == 0) return -1;

    int maxIndex = 0;
    for (int i = 1; i < accountCount; i++) {
        if (accounts[i].balance > accounts[maxIndex].balance)
            maxIndex = i;
    }
    return maxIndex;
}

int customerWithLowestBalance() {
    if (accountCount == 0) return -1;

    int minIndex = 0;
    for (int i = 1; i < accountCount; i++) {
        if (accounts[i].balance < accounts[minIndex].balance)
            minIndex = i;
    }
    return minIndex;
}

int employeesPerBranch(const string& branch) {
    int count = 0;

    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].branchcode == branch)
            count++;
    }

    return count;
}
