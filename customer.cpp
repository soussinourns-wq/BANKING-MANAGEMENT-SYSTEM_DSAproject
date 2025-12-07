#include <iostream>
#include <string>
#include "customer.h"

using namespace std;

bool customerLogin(int& indexCustomer) {
    string acc, pass;
    cout << "Enter account number: ";
    cin >> acc;
    cout << "Enter password: ";
    cin >> pass;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].status !=3 && accounts[i].accNumber == acc && accounts[i].password == pass) {
            indexCustomer=i;
            return true;
        }
    }
    return false;
}

void customerMenu() {
    cout << "|---------------------------|\n";
    cout << "|------ Customer Menu ------|\n";
    cout << "|---------------------------|\n";
    cout << "1. View loans\n";
    cout << "2. Request loan\n";
    cout << "3. Withdraw\n";
    cout << "4. Deposit\n";
    cout << "5. View today's transactions\n";
    cout << "6. Undo last transaction\n";
    cout << "7. Change password\n";
    cout << "8. Back\n";
}

