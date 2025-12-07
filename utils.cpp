#include "utils.h"

double enterDouble() {
    double number;
    while (true) {
        cout << "number: ";
        cin >> number;

        if (cin.fail()) {
            cout << "Invalid input! Please enter a numeric value.\n";
            cin.clear();                // Clear error state
            cin.ignore(10000, '\n');    // Remove invalid characters
        }
        else {
            cin.ignore(10000, '\n'); // Clean leftover input
            return number;
        }
    }
}

void pressEnterToContinue() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(); 
    cin.get();
}

