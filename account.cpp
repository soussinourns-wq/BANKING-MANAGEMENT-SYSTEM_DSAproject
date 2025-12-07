#include <limits>

#include <string>

#include "account.h"

#include "utils.h"

using namespace std;

Account accounts[100];
int accountCount;

Account archivedAccounts[100];
int archivedAccountCount;

void addCustomerAccount() {
    if (accountCount >= 100) {
        cout << "The maximum number of accounts is reached!\n";
        return;
    }

    cout << "\n--- Add Customer Account ---\n";

    Account a;

    //add an account number only if it is not already used
    string tempNb;
    do{
        cout << "Add a non existant account number: ";
        cin >> tempNb;
    }while(accountIndex(tempNb)!=-1 || indexInArchived(tempNb)!=-1);
    a.accNumber=tempNb;
    
    cout << "Account type: ";
    cin >> a.type;

    cout << "Account IBAN: ";
    cin >> a.iban;

    cout << "Account branch: ";
    cin >> a.branchCode;

    cout << "Account holder: ";
    cin >> a.holder;

    time_t now = time(0);
    tm *ltm = localtime(&now);
    a.openYear  = 1900 + ltm->tm_year;
    a.openMonth = 1 + ltm->tm_mon;
    a.openDay   = ltm->tm_mday;

    cout << "Initial balance: ";
    a.balance=enterDouble();

    //add a status with the verification made in the function enterStatus
    a.status=enterStatus();

    a.loans = nullptr;

    cout << "Create password: ";
    cin >> a.password;

    // add to the passed array
    accounts[accountCount] = a;
    accountCount++;
    cout << "Account added successfully\n";
}

void displayAccounts(Account accounts[], int& accountCount) {
    for (int i = 0; i < accountCount; i++) {
        cout << "---------------\nAccount " << i+1 << ": \n---------------\n";
        cout << " \n  Account number:  " << accounts[i].accNumber;
        cout << " \n  Account type:  " << accounts[i].type;
        cout << " \n  Account IBAN:  " << accounts[i].iban;
        cout << " \n  Account branch code:  " << accounts[i].branchCode;
        cout << " \n  Holder:  " << accounts[i].holder ;
        cout << " \n  Open date:  " << accounts[i].openDay << "/" << accounts[i].openMonth << "/" << accounts[i].openYear;
        cout << " \n  Status:  " ;
        if (accounts[i].status==1)
            cout << "Active";
        else if  (accounts[i].status==2)
            cout << "Inactive";
        else    
            cout << "Closed";
        
        cout << " \n  Balance:  "  << accounts[i].balance ;

        cout << endl;
    }
}

int accountIndex(const string &accountNb) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accNumber == accountNb) {
            return i;
        }
    }
    return -1;
}

int indexInArchived(const string &accountNb) {
    for (int i = 0; i < archivedAccountCount; i++) {
        if (archivedAccounts[i].accNumber == accountNb) {
            return i;
        }
    }
    return -1;
}

void changeAccountStatus() {
    string accountNb;
    int newStatus;
    int index;
    cout << "Enter the account number: ";
    cin >> accountNb;
    index=accountIndex(accountNb);
    if (index!=-1){
        cout << "\n The actual status is: " ;
        if (accounts[index].status==1)
            cout << "Active";
        else if  (accounts[index].status==2)
            cout << "Inactive";
        else    
            cout << "Closed";

        cout << "\n The new status: \n  ";
        accounts[index].status=enterStatus();
;
        cout << "\n Status changed\n" ;
    }
    else
        cout << "\n The account number entered does not exist!\n ";  
}

//Delete all accounts whose status is closed and store them in an array of archived accounts. 
void archiveClosedAccounts(){
    bool foundClosed=false;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].status == 3) {

            //add it to the array of archived accounts
            archivedAccounts[archivedAccountCount] = accounts[i];  
            archivedAccountCount++;

            //delete the account from the array of accounts
            for (int j = i; j < accountCount - 1; j++)
                accounts[j] = accounts[j + 1];
            accountCount--;

            foundClosed=true;
        }
    }
    if (foundClosed)
        cout << "\n The closed accounts are archived.\n ";  
    else
        cout << "\n No closed account to archive in this operation!\n ";  
    return;
}


int enterStatus(){
    int tempStatus;
    bool validInput=false;
    do{
        cout << "Choose the value of the status: \n   1-Active \n   2-Inactive \n   3-Closed\n   ";
        if (cin >> tempStatus){
            // Clear any extra input after the number
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
            if (tempStatus==1 || tempStatus==2 || tempStatus==3)
                return tempStatus ;
        }
        else{
            // Clear error and discard bad input
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cout << "Invalid number! Please enter 1, 2, or 3.\n";
    }while (!validInput);
    return tempStatus;
}

void changePassword(int accIndex){
    string oldPass,newPass,confirmedPass;
    cout << "Enter your current password";
    cin >> oldPass;
    if (oldPass==accounts[accIndex].password){
        do{
            cout << "Enter your new password: ";
            cin >> newPass;
            cout << "Confirm your new password: ";
            cin >> confirmedPass;
            if (newPass!=confirmedPass)
                cout << "Passwords not matching!\n";
        }while (newPass!=confirmedPass);
        
        accounts[accIndex].password=newPass;
        cout << "Password changed successfully.\n";
    }
    else
        cout << "Wrong password!\n";
}