#include <limits>

#include <iostream>

#include "menus.h"
#include "customer.h"
#include "employee.h"
#include "statistics.h"
#include "persistence.h"

using namespace std;

void manageTransactions();
void statistics();

string accNumber;
int indexCustomer;
int choice, cc, ce, ct, year, month, day;
char other;

string fileName;
    
time_t now = time(0);
tm *ltm = localtime(&now);

double amount;

int main() {
    loadAll();

    while (true) {
        mainMenu();
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:                
                if (customerLogin(indexCustomer)) {
                    
                    do {
                        cout << "\nHello " << accounts[indexCustomer].holder << "\n\n";
                        customerMenu();
                        cout << "Choice: ";
                        cin >> cc;
                        switch (cc) {
                            case 1: //view loans
                                viewCustomerLoans(indexCustomer);
                                pressEnterToContinue();
                                break;
                            case 2: //request a loan
                                requestLoan(accounts[indexCustomer].accNumber);
                                pressEnterToContinue();
                                break;
                            case 3:
                                cout << "Enter the amount to withdraw: ";
                                cin >> amount;
                                withdraw(indexCustomer, amount);
                                pressEnterToContinue();
                                break;
                            case 4:
                                cout << "Enter the amount to deposit: ";
                                cin >> amount;
                                deposit(indexCustomer, amount);
                                pressEnterToContinue();
                                break;
                            case 5:
                                viewTodaysTransactionsOneCustomer(indexCustomer);
                                pressEnterToContinue();
                                break;
                            case 6:
                                undoLastTransaction(indexCustomer);
                                pressEnterToContinue();
                                break;
                            case 7:
                                changePassword(indexCustomer);
                                pressEnterToContinue();
                            default:
                                break;
                        }
                    } while (cc != 8);
                }
                else
                    cout << "Login failed!\n";
                break;
            case 2:{
                if (employeeLogin()) {
                    do {
                        employeeMenu();
                        cout << "Choice: ";
                        cin >> ce;
                        switch (ce){
                            case 1: //add employee
                                do {
                                    addEmployee();
                                    cout << "\nDo you want to add another employee? (y/n): ";
                                    cin >> other;

                                } while (other == 'y' || other == 'Y');
                                break;
                            case 2: //delete employee
                                if (employeeCount!=0){
                                    do {
                                        deleteEmployee();
                                        cout << "\nDo you want to delete another employee? (y/n): ";
                                        cin >> other;

                                    } while (other == 'y' || other == 'Y');
                                }
                                else{
                                    cout << "No employees to delete!\n";
                                    pressEnterToContinue();
                                }
                                break;
                            case 3: //modify employee
                                if (employeeCount!=0)
                                    modifyEmployee();
                                else
                                    cout << "No employees to modify!\n";                
                                pressEnterToContinue();
                                break;    
                            case 4: //show employees sorted by name
                                showEmployeesSorted();
                                pressEnterToContinue();
                                break;
                            case 5: //show employees by bank branch
                                showEmployeesByBranch();
                                pressEnterToContinue();
                                break;
                            case 6: //show last recruited employee
                                showFirstRecruitedEmployee();
                                pressEnterToContinue();
                                break;
                            case 7: //show last recruited employee
                                showLastRecruitedEmployee();
                                pressEnterToContinue();
                                break;
                            case 8: //add customet account
                                do {
                                    addCustomerAccount();
                                    cout << "\nDo you want to add another account? (y/n): ";
                                    cin >> other;

                                } while (other == 'y' || other == 'Y');
                                break;
                            case 9:  //list of accounts
                                if (accountCount!=0){
                                    cout << "\nList of accounts:\n\n";
                                    displayAccounts(accounts,accountCount);
                                    cout << "\nEnd of the list of  accounts.\n\n";
                                }
                                else
                                    cout << "No account found.\n";
                                pressEnterToContinue();
                                break;
                            case 10: //change account status
                                changeAccountStatus();
                                pressEnterToContinue();
                                break;
                            case 11: //archive the closed accounts
                                archiveClosedAccounts();
                                pressEnterToContinue();
                                break;
                            case 12: //show the archived accounts
                                if (archivedAccountCount!=0){
                                    cout << "\nList of archived accounts:\n\n";
                                    displayAccounts(archivedAccounts,archivedAccountCount);
                                    cout << "\nEnd of the list of archived accounts.\n\n";
                                }
                                else
                                    cout << "No archived account found.\n";
                                pressEnterToContinue();
                                break;
                            case 13: //display cutomer loans
                                cout << "Enter the account number: ";
                                cin >> accNumber;

                                // Get the account index using the accountIndex function
                                indexCustomer = accountIndex(accNumber);
                                if (indexCustomer == -1) {
                                    cout << "Account not found!" << endl;
                                } 
                                else{
                                    viewCustomerLoans(indexCustomer);
                                }
                                pressEnterToContinue();
                                break;
                            case 14: //change loan status
                                changeLoanStatus();
                                pressEnterToContinue();
                                break;
                            case 15: //move completed loans
                                moveCompletedLoans();
                                pressEnterToContinue();
                                break;
                            case 16: //manage loan requests
                                processLoanRequests();
                                pressEnterToContinue();
                                break;
                            case 17: //finalize day, the employee id is admin for now
                                now = time(0);
                                ltm = localtime(&now);
                                year  = 1900 + ltm->tm_year;
                                month = 1 + ltm->tm_mon;
                                day   = ltm->tm_mday;
                                finalizeDay(year, month, day, "admin");
                                pressEnterToContinue();
                                break;
                            
                            case 18: //manage transactions  
                                manageTransactions();                            
                                break;
                            
                            default:
                                break;
                        }
                    } while (ce != 19);
                } 
                else
                    cout << "Login failed!\n";               
                break;
            }
            case 3:
                statistics();
                break;
            case 4:
                cout << "Goodbye!\n";
                saveAll();
                return 0;
            default:
                cout << "Invalid.\n";
                break;
        }
    }
}

void manageTransactions(){
    do{
        employeeTransactionsMenu();
        cout << "Choice: ";    
        cin >> ct;
                                
        switch (ct){
            case 1: //Display all the transactions for one customer
                cout << "Enter the account number: ";
                cin >> accNumber;
                // Get the account index using the accountIndex function
                indexCustomer = accountIndex(accNumber);
                if (indexCustomer == -1) {
                    cout << "Account not found!" << endl;
                } 
                else{
                    viewAllTransactionsOneCustomer(indexCustomer);
                }
                pressEnterToContinue();
                break;
            case 2: //Display today's transactions for one customer
                cout << "Enter the account number: ";
                cin >> accNumber;
                // Get the account index using the accountIndex function
                indexCustomer = accountIndex(accNumber);
                if (indexCustomer == -1) {
                    cout << "Account not found!" << endl;
                } 
                else{
                    viewTodaysTransactionsOneCustomer(indexCustomer);
                }
                pressEnterToContinue();                                        
                break;
            case 3: //Display today's transactions of all customers
                viewTodaysTransactionsAllCustomers();
                pressEnterToContinue(); 
                break;
            case 4: //Display the finalized transactions
                showFinalizedTransactions();
                pressEnterToContinue(); 
                break;
            case 5: //Save finalized transactions to a file
                cout << "Enter the file name: ";
                cin >> fileName;
                saveFinalizedTransactionsToFile(fileName);
                pressEnterToContinue(); 
                break;
            case 6: //Load finalized transactions from a file
                cout << "Enter the file name: ";
                cin >> fileName;
                loadFinalizedTransactionsFromFile(fileName);
                pressEnterToContinue(); 
                break;
            default:
                break;
            }
    }while(ct != 7);
}

void statistics(){
    string loanType, loanStatus;
    if (employeeLogin()) 
        do {
            statisticsMenu();
            cout << "Choice: ";
            cin >> ce;
            switch (ce){
                case 1: //total number of loans
                    cout << "The total number of loans is: " 
                        << totalNumberOfLoans() 
                        << endl;
                    pressEnterToContinue();
                    break;

                case 2: // number of loans by type
                    cout << "Enter the loans' type: ";
                    cin >> loanType;
                    cout << "The number of loans with type: "
                        << loanType
                        << " is: " 
                        << numberOfLoansByType(loanType) 
                        << endl;
                    pressEnterToContinue();
                    break;

                case 3: // number of loans by status
                    cout << "Enter the loans' status: ";
                    cin >> loanStatus;
                    cout << "The number of loans with status: "
                        << loanStatus
                        << " is: "
                        << numberOfLoansByStatus(loanStatus) 
                        << endl;
                    pressEnterToContinue();
                    break;

                case 4: //active loans in a given range
                {
                    //get the range

                    int startY, startM, startD, endY, endM, endD;
                    cout << "\nEnter the start day: \n";
                    cin >> startD;
                    cout << "\nEnter the start month: \n";
                    cin >> startM;
                    cout << "\nEnter the start year: \n";
                    cin >> startY;

                    cout << "\nEnter the last day: \n";
                    cin >> endD;
                    cout << "\nEnter the last month: \n";
                    cin >> endM;
                    cout << "\nEnter the last year: \n";
                    cin >> endY;

                    activeLoansInRange(startD, startM, startY, endD, endM, endY);
                    pressEnterToContinue();
                    break;
                }
                
                case 5:{
                    int maxLoans;
                    int index = customerWithMostLoans(maxLoans);

                    if (index != -1) {
                        cout << "\nCustomer with most loans: " << accounts[index].holder << endl;
                        cout << "\nAccount number: " << accounts[index].accNumber << endl;
                        cout << "\nNumber of loans: " << maxLoans << endl;
                    } else {
                        cout << "\nNo accounts available.\n";
                    }
                    pressEnterToContinue();
                    break;
                }

                case 6:{
                    int index = customerWithHighestBalance();

                    if (index != -1) {
                        cout << "\nCustomer with highest balance: " << accounts[index].holder << endl;
                        cout << "\nAccount number: " << accounts[index].accNumber << endl;
                        cout << "\nBalance: " << accounts[index].balance << endl;
                    } else {
                        cout << "\nNo accounts available.\n";
                    }
                    pressEnterToContinue();
                    break;
                }

                case 7:{
                    int index = customerWithLowestBalance();

                    if (index != -1) {
                        cout << "\nCustomer with lowest balance: " << accounts[index].holder << endl;
                        cout << "\nAccount number: " << accounts[index].accNumber << endl;
                        cout << "\nBalance: " << accounts[index].balance << endl;
                    } else {
                        cout << "\nNo accounts available.\n";
                    }
                    pressEnterToContinue();
                    break;
                }

                case 8: //total number of employees
                    cout << "\nThe total number of employees is: " 
                        << employeeCount
                        << endl;
                    pressEnterToContinue();
                    break;

                case 9:{ // number of employees by branch
                    string branchCode;
                    cout << "\nEnter the branch' code: ";
                    cin >> branchCode;
                    cout << "\nThe number of employees in branch "
                        << branchCode
                        << " is: "
                        << employeesPerBranch(branchCode) 
                        << endl;
                    pressEnterToContinue();
                    break;
                }

                default:
                    break;
            }
        }while (ce != 10);
    else
        cout << "\nLogin failed!\n";  
}