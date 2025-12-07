#include "persistence.h"

//helper function
vector<string> split(const string& line, char delimiter = ',') {
    vector<string> parts;
    string part;
    stringstream ss(line);

    while (getline(ss, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}

//save accounts
void saveAccounts(const string& filename) {
    ofstream fout(filename);
    if (!fout) return;

    for (int i = 0; i < accountCount; i++) {
        fout << accounts[i].accNumber << ","
             << accounts[i].type << ","
             << accounts[i].iban << ","
             << accounts[i].branchCode << ","
             << accounts[i].holder << ","
             << accounts[i].openYear << ","
             << accounts[i].openMonth << ","
             << accounts[i].openDay << ","
             << accounts[i].status << ","
             << accounts[i].balance << ","
             << accounts[i].password
             << "\n";
    }

    fout.close();
}


//load accounts
void loadAccounts(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Could not open " << filename << " ... creating empty file.\n";
        ofstream create(filename); // auto-create
        return;
    }

    accountCount = 0;
    string line;

    while (getline(fin, line)) {

        // Remove CR (Windows)
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        vector<string> t = split(line);

        if (t.size() != 11) {
            cout << "Skipped corrupt line: " << line << endl;
            continue;
        }

        Account& acc = accounts[accountCount++];

        acc.accNumber           = t[0];
        acc.type                = t[1];
        acc.iban                = t[2];
        acc.branchCode          = t[3];
        acc.holder              = t[4];
        acc.openYear            = stoi(t[5]);
        acc.openMonth           = stoi(t[6]);
        acc.openDay             = stoi(t[7]);
        acc.status              = stoi(t[8]);
        acc.balance             = stod(t[9]);
        acc.password            = t[10];

        // Initialize pointers
        acc.loans = new LoansList{nullptr, nullptr, 0};
        acc.transTop = nullptr;
    }

    fin.close();
    cout << "Loaded " << accountCount << " accounts.\n";
}

//save archived accounts
void saveArchivedAccounts(const string& filename) {
    ofstream fout(filename);
    if (!fout) return;

    for (int i = 0; i < archivedAccountCount; i++) {
        fout << archivedAccounts[i].accNumber << ","
             << archivedAccounts[i].type << ","
             << archivedAccounts[i].iban << ","
             << archivedAccounts[i].branchCode << ","
             << archivedAccounts[i].holder << ","
             << archivedAccounts[i].openYear << ","
             << archivedAccounts[i].openMonth << ","
             << archivedAccounts[i].openDay << ","
             << archivedAccounts[i].status << ","
             << archivedAccounts[i].balance << ","
             << archivedAccounts[i].password
             << "\n";
    }
}

//load archived accounts
void loadArchivedAccounts(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Could not open " << filename << " ... creating empty file.\n";
        ofstream create(filename); // auto-create
        return;
    }

    archivedAccountCount = 0;
    string line;

    while (getline(fin, line)) {
        vector<string> t = split(line);
        if (t.size() < 11) continue;

        Account& acc = archivedAccounts[archivedAccountCount++];
        acc.accNumber = t[0];
        acc.type = t[1];
        acc.iban = t[2];
        acc.branchCode = t[3];
        acc.holder = t[4];
        acc.openYear = stoi(t[5]);
        acc.openMonth = stoi(t[6]);
        acc.openDay = stoi(t[7]);
        acc.status = stoi(t[8]);
        acc.balance = stod(t[9]);
        acc.password = t[10];

        acc.loans = nullptr;
        acc.transTop = nullptr;
    }
}

//save employees
void saveEmployees(const string& filename) {
    ofstream fout(filename);
    if (!fout) return;

    for (int i = 0; i < employeeCount; i++) {
        fout << employees[i].id << ","
             << employees[i].name << ","
             << employees[i].lastname << ","
             << employees[i].address << ","
             << employees[i].salary << ","
             << employees[i].hireYear << ","
             << employees[i].hireMonth << ","
             << employees[i].hireDay << ","
             << employees[i].branchcode
             << "\n";
    }
}

//load employees
void loadEmployees(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Could not open " << filename << " ... creating empty file.\n";
        ofstream create(filename); // auto-create
        return;
    }

    employeeCount = 0;
    string line;

    while (getline(fin, line)) {
        vector<string> t = split(line);
        if (t.size() < 9) continue;

        Employee& e = employees[employeeCount++];
        e.id = t[0];
        e.name = t[1];
        e.lastname = t[2];
        e.address = t[3];
        e.salary = stod(t[4]);
        e.hireYear = stoi(t[5]);
        e.hireMonth = stoi(t[6]);
        e.hireDay = stoi(t[7]);
        e.branchcode = t[8];
    }
}


//save requested loans
void saveRequestedLoans(const string& filename) {
    ofstream fout(filename);
    if (!fout) return;

    LoanStruct* curr = requestedLoansFront;

    while (curr != nullptr) {
        Loan* ln = curr->loanInfo;

        fout << curr->customerID << ","
             << ln->loanID << ","
             << ln->loanType << ","
             << ln->principal << ","
             << ln->interestRate << ","
             << ln->amountPaid << ","
             << ln->remaining << ","
             << ln->startDate << ","
             << ln->endDate << ","
             << ln->status << ","
             << ln->yearRequest << ","
             << ln->monthRequest << ","
             << ln->dayRequest 
             << "\n";

        curr = curr->next;
    }

    fout.close();
}

//load requested loans
void loadRequestedLoans(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Could not open " << filename << " ... creating empty file.\n";
        ofstream create(filename); // auto-create
        return;
    }

    requestedLoansFront = requestedLoansRear = nullptr;

    string line;
    while (getline(fin, line)) {
        vector<string> t = split(line);
        if (t.size() < 13) continue;

        LoanStruct* node = new LoanStruct;
        node->customerID = t[0];

        Loan* ln = new Loan;
        ln->loanID = stoi(t[1]);
        ln->loanType = t[2];
        ln->principal = stod(t[3]);
        ln->interestRate = stod(t[4]);
        ln->amountPaid = stod(t[5]);
        ln->remaining = stod(t[6]);
        ln->startDate = t[7];
        ln->endDate = t[8];
        ln->status = t[9];
        ln->yearRequest = stoi(t[10]);
        ln->monthRequest = stoi(t[11]);
        ln->dayRequest = stoi(t[12]);

        ln->next = ln->prev = nullptr;
        node->loanInfo = ln;
        node->next = nullptr;

        if (requestedLoansRear == nullptr) {
            requestedLoansFront = requestedLoansRear = node;
        } else {
            requestedLoansRear->next = node;
            requestedLoansRear = node;
        }
    }

    fin.close();
}

//save completed loans
void saveCompletedLoans(const string& filename) {
    ofstream fout(filename);
    if (!fout) return;

    LoanStruct* curr = completedLoansHead;

    while (curr != nullptr) {
        Loan* ln = curr->loanInfo;

        fout << curr->customerID << ","
             << ln->loanID << ","
             << ln->loanType << ","
             << ln->principal << ","
             << ln->interestRate << ","
             << ln->amountPaid << ","
             << ln->remaining << ","
             << ln->startDate << ","
             << ln->endDate << ","
             << ln->status << ","
             << ln->yearRequest << ","
             << ln->monthRequest << ","
             << ln->dayRequest
             << "\n";

        curr = curr->next;
    }

    fout.close();
}

//load completed loans
void loadCompletedLoans(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Could not open " << filename << " ... creating empty file.\n";
        ofstream create(filename); // auto-create
        return;
    }

    completedLoansHead = nullptr;

    string line;
    while (getline(fin, line)) {
        vector<string> t = split(line);
        if (t.size() < 13) continue;

        LoanStruct* node = new LoanStruct;
        node->customerID = t[0];

        Loan* ln = new Loan;
        ln->loanID = stoi(t[1]);
        ln->loanType = t[2];
        ln->principal = stod(t[3]);
        ln->interestRate = stod(t[4]);
        ln->amountPaid = stod(t[5]);
        ln->remaining = stod(t[6]);
        ln->startDate = t[7];
        ln->endDate = t[8];
        ln->status = t[9];
        ln->yearRequest = stoi(t[10]);
        ln->monthRequest = stoi(t[11]);
        ln->dayRequest = stoi(t[12]);

        ln->next = ln->prev = nullptr;
        node->loanInfo = ln;

        node->next = completedLoansHead;
        completedLoansHead = node;
    }

    fin.close();
}

//save next loan ID
void saveLoanMetadata(const string& filename) {
    ofstream fout(filename);
    if (!fout) return;
    fout << nextLoanID << "\n";
}

//load next loan ID
void loadLoanMetadata(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Could not open " << filename << " ... creating empty file.\n";
        ofstream create(filename); // auto-create
        return;
    }
    
    fin >> nextLoanID;
}

//one call to save all
void saveAll() {
    saveAccounts("data/accounts.csv");
    saveArchivedAccounts("data/archived_accounts.csv");
    saveEmployees("data/employees.csv");
    saveRequestedLoans("data/requested_loans.csv");
    saveCompletedLoans("data/completed_loans.csv");
    saveLoanMetadata("data/loan_metadata.csv");

    cout << "All data saved.\n";
}

//one call to load all
void loadAll() {
    cout << "\n----- Loading the data -----\n";

    // 1. ACCOUNTS --------------------------------------------------
    if (!ensureFileExists("data/accounts.csv"))
        cout << "Created missing file: accounts.csv\n";
    loadAccounts("data/accounts.csv");

    // 2. ARCHIVED ACCOUNTS -----------------------------------------
    if (!ensureFileExists("data/archived_accounts.csv"))
        cout << "Created missing file: archived_accounts.csv\n";
    loadArchivedAccounts("data/archived_accounts.csv");

    // 3. EMPLOYEES -------------------------------------------------
    if (!ensureFileExists("data/employees.csv"))
        cout << "Created missing file: employees.csv\n";
    loadEmployees("data/employees.csv");

    // 4. REQUESTED LOANS QUEUE -------------------------------------
    if (!ensureFileExists("data/requested_loans.csv"))
        cout << "Created missing file: requested_loans.csv\n";
    loadRequestedLoans("data/requested_loans.csv");

    // 5. COMPLETED LOANS -------------------------------------------
    if (!ensureFileExists("data/completed_loans.csv"))
        cout << "Created missing file: completed_loans.csv\n";
    loadCompletedLoans("data/completed_loans.csv");

    // 6. LOAN METADATA (nextLoanID) --------------------------------
    if (!ensureFileExists("data/loan_metadata.csv")) {
        cout << "Created missing file: loan_metadata.csv\n";
        nextLoanID = 1;
        saveLoanMetadata("data/loan_metadata.csv");
    } else {
        loadLoanMetadata("data/loan_metadata.csv");
        if (nextLoanID <= 0) nextLoanID = 1; // ensure valid
    }

    cout << "=== Loading completed ===\n\n";
}

//ensure the file exists, if not create it
bool ensureFileExists(const string& filename) {
    ifstream fin(filename);
    if (fin.good()) {
        fin.close();
        return true; // file exists
    }

    // otherwise create it
    ofstream fout(filename);
    if (!fout) return false;

    fout.close();
    return false; // newly created
}
