#include "account.h"
#include <limits>
#include <string>
using namespace std;

LoanStruct* requestedLoansFront = nullptr;  // Front of FIFO queue
LoanStruct* requestedLoansRear = nullptr;   // Rear of FIFO queue
LoanStruct* completedLoansHead = nullptr;   // Singly linked list for completed loans
int nextLoanID=0;                        // Starting loan ID

void requestLoan(string customerID) {
    //we will use the account number as customer ID

    // Create the loan details
    Loan* newLoan = new Loan;
    newLoan->loanID = nextLoanID++;
    cout << "Loan type  (car, home, student, business): ";
    cin >> newLoan->loanType;

    cout << "Principal amount: ";
    cin >> newLoan->principal;

    cout << "Interest rate: ";
    cin >> newLoan->interestRate;

    newLoan->amountPaid = 0.0;
    newLoan->remaining = newLoan->principal;

    cout << "Start date: ";
    cin >> newLoan->startDate;

    cout << "End date: ";
    cin >> newLoan->endDate;

    newLoan->status = "Pending";

    time_t now = time(0);
    tm *ltm = localtime(&now);
    newLoan->yearRequest  = 1900 + ltm->tm_year;
    newLoan->monthRequest = 1 + ltm->tm_mon;
    newLoan->dayRequest   = ltm->tm_mday;
    
    
    newLoan->next = nullptr;
    newLoan->prev = nullptr;
    
    // Create the requested loan node
    LoanStruct* newRequest = new LoanStruct;
    newRequest->customerID = customerID;
    newRequest->loanInfo = newLoan;
    newRequest->next = nullptr;
    
    // Add to the FIFO queue
    if (requestedLoansFront == nullptr) {
        // Queue is empty
        requestedLoansFront = newRequest;
        requestedLoansRear = newRequest;
    } else {
        // Add to rear of queue
        requestedLoansRear->next = newRequest;
        requestedLoansRear = newRequest;
    }
    
    cout << "Loan request submitted for customer " << customerID 
         << ". Loan ID: " << newLoan->loanID << endl;
}

void processLoanRequests() {
    //Processing loan requests (FIFO)

    cout << "\nProcessing loan requests..." << endl;
    cout.flush();
    
    if (requestedLoansFront == nullptr) {
        cout << "No pending loan requests." << endl;
        cout.flush();
        return;
    }
    
    while (requestedLoansFront != nullptr) {
        LoanStruct* currentRequest = requestedLoansFront;
        Loan* loan = currentRequest->loanInfo;
        string customerID = currentRequest->customerID;
        
        // Display request details
        cout << "\n--- Processing Loan Request ---" << endl;
        cout << "Customer ID: " << customerID << endl;
        cout << "Loan ID: " << loan->loanID << endl;
        cout << "Type: " << loan->loanType << endl;
        cout << "Amount: " << loan->principal << endl;
        cout << "Request Date: " << loan->dayRequest << "/" 
             << loan->monthRequest << "/" << loan->yearRequest << endl;
        cout.flush();
        
        // Ask for decision
        char decision;
        cout << "Accept this loan? (y/n): ";
        cout.flush();
        cin >> decision;
        
        if (decision == 'y' || decision == 'Y') {
            // Accept the loan
            loan->status = "active";
            
            // use accountIndex to find the customer's account
            int accIndex = accountIndex(customerID);
            
            if (accIndex != -1) {
                // Add loan to customer's loan list
                addLoanToCustomer(accIndex, loan);
                cout << "Loan accepted and added to customer's account." << endl;
            } else {
                cout << "Error: Customer account not found!" << endl;
                cout.flush();
            }
        } else {
            // Decline the loan - permanently delete
            cout << "Loan declined and permanently deleted." << endl;
            delete loan;  // Delete the loan details
        }
        
        // Remove from requested loans queue
        LoanStruct* toDelete = requestedLoansFront;
        requestedLoansFront = requestedLoansFront->next;
        
        // If queue becomes empty, update rear
        if (requestedLoansFront == nullptr) {
            requestedLoansRear = nullptr;
        }
        
        delete toDelete;  // Delete the request node
        
        // Ask if user wants to process next request
        if (requestedLoansFront != nullptr) {
            char continueProcessing;
            cout << "\nProcess next loan request? (y/n): ";
            cout.flush();
            cin >> continueProcessing;
            if (continueProcessing != 'y' && continueProcessing != 'Y') {
                break;
            }
        }
    }
    cout << "Loan request processing completed." << endl;
    cout.flush();
}

// function to add loan to customer's doubly linked list
void addLoanToCustomer(int accountIndex, Loan* loan) {
    if (accountIndex < 0 || accountIndex >= accountCount) {
        cout << "Invalid account index!" << endl;
        return;
    }
    
    // Reset pointers for the loan to add to customer's list
    loan->next = nullptr;
    loan->prev = nullptr;
    
    // Add to customer's loan list (doubly linked)
    if (accounts[accountIndex].loans == nullptr) {
        // First loan for this customer - allocate loans structure
        accounts[accountIndex].loans = new LoansList;
        accounts[accountIndex].loans->loansHead = loan;
        accounts[accountIndex].loans->loansTail = loan;
        accounts[accountIndex].loans->numberLoans = 1;
    } else if (accounts[accountIndex].loans->loansHead == nullptr) {
        // Loans structure exists but is empty
        accounts[accountIndex].loans->loansHead = loan;
        accounts[accountIndex].loans->loansTail = loan;
        accounts[accountIndex].loans->numberLoans = 1;
    } else {
        // Add to the end of list
        if (accounts[accountIndex].loans->loansTail != nullptr) {
            loan->prev = accounts[accountIndex].loans->loansTail;
            accounts[accountIndex].loans->loansTail->next = loan;
            accounts[accountIndex].loans->loansTail = loan;
            accounts[accountIndex].loans->numberLoans++;
        }
    }
    cout << "Loan added to customer " << accountIndex << "'s account." << endl;
}

void changeLoanStatus(){
    string accountNumber;
    int loanID;
    cout << "Enter the account number: ";
    cin >> accountNumber;

    cout << "Enter the loan ID: ";
    cin >> loanID;

    // Get the account index using the accountIndex function
    int accIndex = accountIndex(accountNumber);
    
    if (accIndex == -1) {
        cout << "Account not found!" << endl;
        return;
    }
      
    // Search for the loan in customer's loan list
    Loan* current = accounts[accIndex].loans->loansHead;
    Loan* foundLoan = nullptr;
    
    while (current != nullptr) {
        if (current->loanID == loanID) {
            foundLoan = current;
            break;
        }
        current = current->next;
    }
    
    if (foundLoan != nullptr){ 
        // Found the loan, proceed to update status
        //ask for the new state
        string newStatus;
        cout << "\n Enter new state (active, overdue, completed): ";
        cin >> newStatus;

        // Validate new status
        if (newStatus != "active" && newStatus != "overdue" && newStatus != "completed") {
            cout << "Invalid status!" << endl;
            return;
        }

        string oldStatus = foundLoan->status;
        foundLoan->status = newStatus;
        cout << "Loan " << loanID << " status changed from '" << oldStatus << "' to '" << newStatus << "'" << endl;
    }
    else
        cout << "Loan not found or not active !" << endl;
 }

 void moveCompletedLoans() {
    cout << "--------------------------------------" << endl;
    cout << "      Archiving completed loans " << endl;
    cout << "--------------------------------------\n" << endl;
    
    int totalMoved = 0;
    double totalAmountArchived = 0.0;
    
    // Iterate through all accounts
    for (int i = 0; i < accountCount; i++) {
        // Skip accounts without loans
        if (accounts[i].loans->loansHead == nullptr) {
            continue;
        }
        
        Loan* current = accounts[i].loans->loansHead;
        Loan* prev = nullptr;
        
        while (current != nullptr) {
            if (current->status == "completed") {
                // Store next pointer before we disconnect
                Loan* nextLoan = current->next;
                
                // Remove from customer's doubly linked list
                if (prev == nullptr) {
                    // Removing head
                    accounts[i].loans->loansHead = current->next;
                    if (accounts[i].loans->loansHead != nullptr) {
                        accounts[i].loans->loansHead->prev = nullptr;
                    } else {
                        accounts[i].loans->loansTail = nullptr;
                    }
                } else {
                    prev->next = current->next;
                    if (current->next != nullptr) {
                        current->next->prev = prev;
                    } else {
                        accounts[i].loans->loansTail = prev;
                    }
                }
                
                // Create a new LoanStruct node with customer info
                LoanStruct* newCompleted = new LoanStruct;
                newCompleted->customerID = accounts[i].accNumber;  // Store account number as customerID
                newCompleted->loanInfo = current;  // Point to the loan
                
                // Add to completedLoansHead (singly linked list)
                newCompleted->next = completedLoansHead;
                completedLoansHead = newCompleted;
                
                // Log the movement
                cout << "✓ Moved loan ID " << current->loanID 
                     << " from account " << accounts[i].accNumber 
                     << " to completed loans history." << endl;
                
                totalMoved++;
                totalAmountArchived += current->principal;
                
                // Reset the loan's pointers to prevent accidental traversal
                current->next = nullptr;
                current->prev = nullptr;
                
                // Move to next loan
                current = nextLoan;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
    
    cout << "--------------------------------------" << endl;
    if (totalMoved > 0) {
        cout << "SUCCESS: Moved " << totalMoved << " completed loans" << endl;
    } else {
        cout << "No completed loans found to move." << endl;
    }
    cout << "--------------------------------------" << endl;
}

void viewCustomerLoans(int indexCustomer) {

    Account& customer = accounts[indexCustomer];
    
    cout << "\n--------------------------------------" << endl;
    cout << "       Loans of: " << customer.holder << endl;
    cout << "       Account: " << customer.accNumber << endl;
    cout << "--------------------------------------" << endl;
    
    //look for the customer's requested loans 
    
    int requestedCount = 0;
    
    LoanStruct* currentRequest = requestedLoansFront;
        
    while (currentRequest != nullptr) {
        // Check if this request belongs to the current customer
        if (currentRequest->customerID == customer.accNumber) {
            requestedCount++;
            Loan* loan = currentRequest->loanInfo;
                
            // Display loan details
            cout << "\n Requestes loan " << requestedCount << ":" << endl;
            displayLoanDetails(loan);                
        }
        currentRequest = currentRequest->next;
    }
        
    if (requestedCount == 0) 
        cout << "No pending loan requests." << endl;
        
    
    
    // look for the loans in the customer's doubly linked list
    cout << "-------------------------" << endl;

    
    int activeCount = 0;

    
    if (customer.loans == nullptr || customer.loans->loansHead == nullptr) {
        cout << "No current loans (active or inactive)." << endl;
    } else {
        Loan* current = customer.loans->loansHead;
        int loanNum = 1;
        
        while (current != nullptr) {
            activeCount++;
            
            // Display loan details
            cout << "\n" << " Current loan number:" << loanNum;
            displayLoanDetails(current);
                        
            current = current->next;
            loanNum++;
        }
    }
    
    // look for the loans in the completed loans (from completedLoansHead)
    cout << "-------------------------" << endl;
    
    int completedCount = 0;
    
    // Search for completed loans belonging to this customer
    LoanStruct* currentCompleted = completedLoansHead;
    bool foundCompleted = false;
    
    while (currentCompleted != nullptr) {
        // Check if this completed loan belongs to the current customer
        if (currentCompleted->customerID == customer.accNumber) {
            foundCompleted = true;
            completedCount++;
            Loan* loan = currentCompleted->loanInfo;
            
            cout << "\n" << " Completed loan number:" << completedCount;
            displayLoanDetails(loan);
            
        }
        currentCompleted = currentCompleted->next;
    }
    
    if (!foundCompleted) {
        cout << "No completed loans found." << endl;
    }
    
    
    }

    void displayLoanDetails(Loan* current){
        cout << " (ID: " << current->loanID << ")" << endl;
        cout << "   |- Type:          " << current->loanType << endl;
        cout << "   |- Principal:     " << current->principal << endl;
        cout << "   |- Interest Rate: " << current->interestRate << "%" << endl;
        cout << "   |- Amount Paid:   " << current->amountPaid << endl;
        cout << "   |- Remaining:     " << current->remaining << endl;
        cout << "   |- Status:        " << current->status << endl;
        cout << "   |- Dates:         " << current->startDate << " to " << current->endDate << endl;
        cout << "   |- Requested on:     " << current->monthRequest << "/" << current->dayRequest << "/" << current->yearRequest << endl;
    }