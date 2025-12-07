#include "account.h"
#include "persistence.h"

FinalizedTransactionList finalizedTransactionsList = { nullptr, 0 };
int lastIdTransaction=0;

void addTransaction(int accIndex, const string &type, double amount) {

    Transaction* t=new Transaction;

    t->type = type;
    t->amount = amount;

    time_t now = time(0);
    tm *ltm = localtime(&now);
    t->year  = 1900 + ltm->tm_year;
    t->month = 1 + ltm->tm_mon;
    t->day   = ltm->tm_mday;

    // ID incremented for every transaction added
    t->id = lastIdTransaction++;

    t->status="active";

    t->next = accounts[accIndex].transTop;
    accounts[accIndex].transTop = t;
}

string twoDigits(int n) {
    if (n < 10) return "0" + to_string(n);
    return to_string(n);
}

string fourDigits(int n) {
    if (n < 10) return "000" + to_string(n);
    if (n < 100) return "00" + to_string(n);
    if (n < 1000) return "0" + to_string(n);
    return to_string(n);
}



void deposit(int accIndex, double amount) {
    if (amount <= 0) {
        cout << "Invalid deposit amount!\n";
        return;
    }

    accounts[accIndex].balance += amount;
    addTransaction(accIndex, "Deposit", amount);

    cout << "Deposited " << amount
         << " to account " << accounts[accIndex].accNumber
         << ". New balance: " << accounts[accIndex].balance << endl;
}

void withdraw(int accIndex, double amount) {
    if (amount <= 0) {
        cout << "Invalid withdraw amount!\n";
        return;
    }

    if (amount > accounts[accIndex].balance) {
        cout << "Insufficient balance for this withdrawal!\n";
        return;
    }

    accounts[accIndex].balance -= amount;
    addTransaction(accIndex, "Withdraw", amount);

    cout << "Withdrawal " << amount
         << " from account " << accounts[accIndex].accNumber
         << ". New balance: " << accounts[accIndex].balance << endl;
}

void viewTodaysTransactionsOneCustomer(int accIndex) {
    // Get today's date
    time_t now = time(0);
    tm *ltm = localtime(&now);

    int todayYear  = 1900 + ltm->tm_year;
    int todayMonth = 1 + ltm->tm_mon;
    int todayDay   = ltm->tm_mday;

    Transaction* t = accounts[accIndex].transTop;
    bool found = false;

    cout << "\n=== Today's Transactions for Account "
         << accounts[accIndex].accNumber << " ===\n";

    while (t != nullptr) {
        if (t->year == todayYear &&
            t->month == todayMonth &&
            t->day == todayDay) {

            cout << "-----\nTransaction ID: " << t->id
                 << "\n  | Type: " << t->type
                 << "\n  | Amount: " << t->amount
                 << "\n  | Date: " << t->day << "/" << t->month << "/" << t->year
                 << "\n  | Status: " << t->status << endl;

            found = true;
        }
        t = t->next;
    }

    if (!found)
        cout << "No transactions today.\n";
}

void viewAllTransactionsOneCustomer(int accIndex) {
    Transaction* t = accounts[accIndex].transTop;
    bool found = false;

    cout << "\n----- Transactions for Account "
         << accounts[accIndex].accNumber << " -----\n";

    while (t != nullptr) {
        cout << "-----\nTransaction ID: " << t->id
            << "\n  | Type: " << t->type
            << "\n  | Amount: " << t->amount
            << "\n  | Date: " << t->day << "/" << t->month << "/" << t->year
            << "\n  | Status: " << t->status << endl;
        found = true;
        t = t->next;
    }

    if (!found)
        cout << "No transactions found.\n";
}

void viewTodaysTransactionsAllCustomers() {
    // Get today's date
    time_t now = time(0);
    tm *ltm = localtime(&now);

    int todayYear  = 1900 + ltm->tm_year;
    int todayMonth = 1 + ltm->tm_mon;
    int todayDay   = ltm->tm_mday;
    
    bool found = false;

    for(int i=0; i<accountCount; i++){
        Transaction* t = accounts[i].transTop;
        if (t == nullptr)
            continue; //skip the rest of the code for this iteration and go to the next iteration
        cout << "\n----- Today's Transactions for Account "
            << accounts[i].accNumber << " -----\n";

        while (t != nullptr) {
            if (t->year == todayYear &&
                t->month == todayMonth &&
                t->day == todayDay) {

                cout << "-----\nTransaction ID: " << t->id
                    << "\n  | Type: " << t->type
                    << "\n  | Amount: " << t->amount
                    << "\n  | Date: " << t->day << "/" << t->month << "/" << t->year
                    << "\n  | Status: " << t->status << endl;

                found = true;
            }
            t = t->next;
        }
    }

    if (!found)
        cout << "No transactions today.\n";
}

void undoLastTransaction(int accIndex) {
    
    Transaction* t = accounts[accIndex].transTop;

    if (t == nullptr) {
        cout << "No transactions to undo.\n";
        return;
    }

    if (t->status == "canceled"){
        cout << "The last transaction was canceled!\n";
        return;
    }

    if (t->status == "finalized"){
        cout << "Impossible to undo the last transaction, please contact the bank!\n";
        return;
    }

    
    // Reverse its effect
    if (t->type == "Deposit") {
        accounts[accIndex].balance -= t->amount;
    } 
    else if (t->type == "Withdraw") {
        accounts[accIndex].balance += t->amount;
    }

    //change its status to canceled
    t->status="canceled";

    cout << "The last transaction is canceled (ID: " << t->id
         << ", transaction type: " << t->type << ", amount: " << t->amount << ").\n"
         << "Balance: " << accounts[accIndex].balance << endl;

    //this is to delete the last transaction permanently
    //accounts[accIndex].transTop = t->next;

}

void finalizeDay(int year, int month, int day, const string& employeeId)
{
    int totalFinalized=0;
    
    // Loop over all accounts
    for (int i = 0; i < accountCount; i++) 
    {
        Account& acc = accounts[i];
        Transaction* t = acc.transTop;

        

        while (t != nullptr) 
        {
            // Only finalize active transactions from this exact date
            if (t->status == "active" &&
                t->year  == year  &&
                t->month == month &&
                t->day   == day)
            {
                // ---- Create a finalized transaction copy ----
                FinalizedTransaction* f = new FinalizedTransaction;
                f->id = t->id;
                f->type = t->type;
                f->amount = t->amount;
                f->year = t->year;
                f->month = t->month;
                f->day = t->day;
                f->accountNb = acc.accNumber;
                f->employeeId = employeeId;

                // Insert at head of finalizedTransactionsList (singly linked)
                f->next = finalizedTransactionsList.head;
                finalizedTransactionsList.head = f;
                
                finalizedTransactionsList.size++;

                // Mark transaction as finalized
                t->status = "finalized";
                totalFinalized++;
            }

            t = t->next;
        }
    }

    cout << "Finalization done for: " 
         << day << "/" << month << "/" << year 
         << " By: " << employeeId
         << ". \nTotal finalized: " << totalFinalized << endl;
}

void showFinalizedTransactions()
{
    cout << "\n----- Finalized Transactions -----\n";

    if (finalizedTransactionsList.head == nullptr) {
        cout << "No finalized transactions.\n";
        return;
    }

    FinalizedTransaction* curr = finalizedTransactionsList.head;
    int num = 1;

    while (curr != nullptr)
    {
        cout << "\nTransaction number: " << num++ << endl;
        cout << "   | ID: "      << curr->id  << endl;
        cout << "   | Account: "      << curr->accountNb  << endl;
        cout << "   | Type: "         << curr->type       << endl;
        cout << "   | Amount: "       << curr->amount     << endl;
        cout << "   | Date: "         << curr->day << "/" << curr->month << "/" << curr->year << endl;
        cout << "   | Employee who finalized: "  << curr->employeeId << endl;

        curr = curr->next;
    }
}

void saveFinalizedTransactionsToFile(const string& filename)
{
    ofstream fout(filename);

    if (!fout) {
        cout << "Error opening file for writing: " << filename << endl;
        return;
    }

    FinalizedTransaction* curr = finalizedTransactionsList.head;

    while (curr != nullptr)
    {
        fout << curr->id << ","
             << curr->type << ","
             << curr->amount << ","
             << curr->year << ","
             << curr->month << ","
             << curr->day << ","
             << curr->accountNb << ","
             << curr->employeeId
             << "\n";

        curr = curr->next;
    }

    fout.close();
    cout << "Finalized transactions saved to " << filename << endl;
}

void loadFinalizedTransactionsFromFile(const string& filename)
{
    ifstream fin(filename);

    // If file does not exist → auto-create an empty file
    if (!fin) {
        ofstream fout(filename);
        return;
    }

    // Reset existing list (clear memory)
    FinalizedTransaction* cur = finalizedTransactionsList.head;
    while (cur) {
        FinalizedTransaction* temp = cur;
        cur = cur->next;
        delete temp;
    }
    finalizedTransactionsList.head = nullptr;
    finalizedTransactionsList.size = 0;

    string line;
    char delimiter=',';
    while (getline(fin, line)) {
        if (line.empty()) continue;

        vector<string> t = split(line,delimiter);

        // CSV format required:
        // 0:id , 1:type , 2:amount , 3:year , 4:month , 5:day , 6:accountNb , 7:employeeId
        if (t.size() < 8) {
            cout << "Skipped corrupt finalized transaction line: " << line << "\n";
            continue;
        }
        
        FinalizedTransaction* f = new FinalizedTransaction;

        try {
            f->id        = t[0];
            f->type      = t[1];
            f->amount    = stod(t[2]);
            f->year      = stoi(t[3]);
            f->month     = stoi(t[4]);
            f->day       = stoi(t[5]);
            f->accountNb = t[6];
            f->employeeId = t[7];
        }
        catch (...) {
            cout << "Skipped corrupt numeric data in: " << line << "\n";
            delete f;
            continue;
        }

        // Insert at head (simple linked list insert)
        f->next = finalizedTransactionsList.head;
        finalizedTransactionsList.head = f;
        finalizedTransactionsList.size++;
    }

    fin.close();
}
