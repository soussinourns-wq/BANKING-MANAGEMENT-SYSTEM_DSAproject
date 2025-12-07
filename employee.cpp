#include "employee.h"
#include "utils.h"

Employee employees[100];
int employeeCount;

//Employee login function

bool employeeLogin() {
    string id, pass;
    cout << "Employee ID: ";
    cin >> id;
    cout << "Password: ";
    cin >> pass;
    return (pass=="admin" && id=="admin");
}

//Display the menu for employees

void employeeMenu() {
    cout << "|---------------------------|\n";
    cout << "|------ Employee Menu ------|\n";
    cout << "|---------------------------|\n";
    cout << "1. Add employee\n";
    cout << "2. Delete employee\n";
    cout << "3. Modify employee\n";
    cout << "4. Show employees sorted\n";
    cout << "5. Show employees by bank\n";
    cout << "6. Show first recruited employee\n";
    cout << "7. Show last recruited employee\n";
    cout << "8. Add customer account\n";
    cout << "9. Display the list of accounts\n";
    cout << "10. Change account status\n";
    cout << "11. Archive closed accounts\n";
    cout << "12. Display the archived accounts\n";
    cout << "13. Show customer loans\n";
    cout << "14. Change loan status\n";
    cout << "15. Move completed loans\n";
    cout << "16. Manage loan requests\n";
    cout << "17. Finalize day\n";
    cout << "18: Manage transactions\n";
    cout << "19. Back\n";
}

void employeeTransactionsMenu() {
    cout << "|---------------------------|\n";
    cout << "|------ Transactions Menu ------|\n";
    cout << "|---------------------------|\n";
    cout << "1. Show a customer transactions (all)\n";
    cout << "2. Show a customer transactions (today)\n";
    cout << "3. Show all transactions of the day\n";
    cout << "4. Show finalized transactions\n";
    cout << "5. Save finalized transactions to a file\n";
    cout << "6. Load finalized transactions from a file\n";
    cout << "7. Back\n";
}
// Add employee

void addEmployee() {
    if (employeeCount >= 100) {
        cout << "The maximum number of employees is reached!\n";
        return;
    }

    cout << "\n------Add Employee------\n";

    Employee e;

    //the ID to add must not be added to another employee
    string tempId;
    do{
        cout << "Add a non exixtant ID: ";
        cin >> tempId;
    }while(employeeIdExists(tempId)!=-1);
    e.id=tempId;

    cout << "Name: ";
    cin >> e.name;
    cout << "Last name: ";
    cin >> e.lastname;
    cout << "Address: ";
    cin >> e.address;

    cout << "Salary: "; 
    e.salary=enterDouble();
    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    e.hireYear  = 1900 + ltm->tm_year;
    e.hireMonth = 1 + ltm->tm_mon;
    e.hireDay   = ltm->tm_mday;

    cout << "Branch code: ";
    cin >> e.branchcode;

    employees[employeeCount++] = e;

    cout << "Employee added successfully.\n";
}

//Delete employee (by ID)

void deleteEmployee() {
    cout << "\n------Delete Employee------\n";
    string id;
    cout << "Enter the ID of the employee to delete: ";
    cin >> id;

    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].id == id) {
            // shift left
            for (int j = i; j < employeeCount - 1; j++)
                employees[j] = employees[j + 1];

            employeeCount--;
            cout << "Employee deleted.\n";
            return;
        }
    }
    cout << "Employee not found.\n";
}

//Modify employee (by ID)

void modifyEmployee() {
    cout << "\n------Modify Employee------\n";

    string id;
    cout << "Enter the ID of the employee to modify: ";
    cin >> id;

    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].id == id) {
            char modify;
            cout << "\nModify name? (y/n): ";
            cin >> modify;
            if (modify=='y' or modify=='Y'){
                cout << "New name: ";
                cin >> employees[i].name;
            }
            cout << "\nModify last name? (y/n): ";
            cin >> modify;
            if (modify=='y' or modify=='Y'){
                cout << "New lastname: ";
                cin >> employees[i].lastname;
            }
            cout << "\nModify address? (y/n): ";
            cin >> modify;
            if (modify=='y' or modify=='Y'){
                cout << "New address: ";
                cin >> employees[i].address;
            }
            cout << "\nModify salary? (y/n): ";
            cin >> modify;
            if (modify=='y' || modify=='Y'){
                employees[i].salary=enterDouble();
            }
            cout << "\nModify hire date? (y/n): ";
            cin >> modify;
            if (modify=='y' or modify=='Y'){
                cout << "New hire date: \n";
                cout << " Year: ";
                cin >> employees[i].hireYear;
                cout << "\n Month: ";
                cin >> employees[i].hireMonth;
                cout << "\n Day: ";
                cin >> employees[i].hireDay;
            }
            cout << "\nModify branch bank? (y/n): ";
            cin >> modify;
            if (modify=='y' or modify=='Y'){
                cout << "\nNew branch code: ";
                cin >> employees[i].branchcode;
            }

            cout << "\nEmployee modified.\n";
            return;
        }
    }
    cout << "\nEmployee not found.\n";
}

//Ddisplay one employee by hix index
void showEmployee(int index){
    cout << "\n  - ID: " << employees[index].id ;
    cout << "\n  - Name: " << employees[index].name ;
    cout << "\n  - Last name: " << employees[index].lastname ;
    cout << "\n  - Address: " << employees[index].address ;
    cout << "\n  - Salary:" << employees[index].salary;
    cout << "\n  - Hire date:"  << employees[index].hireDay << "/" << employees[index].hireMonth << "/" << employees[index].hireYear;
    cout << "\n  - Bank branch: " << employees[index].branchcode ;
}

//Display employees sorted (by lastname)

void showEmployeesSorted() {

    if (employeeCount == 0) {
        cout << "No employees.\n";
        return;
    }

    // Create a temporary copy
    Employee temp[100];
    for (int i = 0; i < employeeCount; i++)
        temp[i] = employees[i];

    // Sort by lastname
    sort(temp, temp + employeeCount, [](const Employee &a, const Employee &b) {
        return a.lastname < b.lastname;
    });

    cout << "\n------Employees Sorted by Last Name------\n";
    for (int i = 0; i < employeeCount; i++) {
        cout << "\n Employee " << i+1;
        showEmployee(i);
    }
}


// Show employees by bank branch

void showEmployeesByBranch() {
    cout << "\n--- Show Employees by Branch ---\n";
    string bc;
    cout << "Enter branch code: ";
    cin >> bc;

    bool found = false;
    int empBranch=0;
    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].branchcode == bc) {
            empBranch++;
            cout << "\n Employee " << empBranch;
            showEmployee(i);
            found = true;
        }
    }

    if (!found)
        cout << "No employees found for this branch.\n";
}


//test if an employee ID exists, returns its index if found, -1 if not 

int employeeIdExists(const string &id) {
    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].id == id) {
            return i;
        }
    }
    return -1;
}

//look for the first recruited employee (oldest hire date)
void showFirstRecruitedEmployee() {
    if (employeeCount == 0) {
        cout << "No employees found.\n";
        return;
    }
    
    int firstIndex = 0;  // Assume first employee is the earliest
    
    for (int i = 1; i < employeeCount; i++) {
        if (isEarlierDate(employees[i].hireYear, employees[i].hireMonth, employees[i].hireDay,
                          employees[firstIndex].hireYear, employees[firstIndex].hireMonth, employees[firstIndex].hireDay)) {
            firstIndex = i;
        }
    }
    showEmployee(firstIndex);
}

//last recruited employee (most recent hire date)
void showLastRecruitedEmployee(){
    if (employeeCount == 0) {
        cout << "No employees found.\n";
        return;
    }
    
    int lastIndex = 0;  // Assume first employee is the most recent
    
    for (int i = 1; i < employeeCount; i++) {
        if (isLaterDate(employees[i].hireYear, employees[i].hireMonth, employees[i].hireDay,
                        employees[lastIndex].hireYear, employees[lastIndex].hireMonth, employees[lastIndex].hireDay)) {
            lastIndex = i;
        }
    }
    showEmployee(lastIndex);
}

// test if if date1 is earlier than date2
bool isEarlierDate(int y1, int m1, int d1, int y2, int m2, int d2) {
    if (y1 != y2) return (y1 < y2);
    if (m1 != m2) return (m1 < m2);
    return (d1 <= d2);
}

// test if date1 is later than date2
bool isLaterDate(int y1, int m1, int d1, int y2, int m2, int d2) {
    if (y1 != y2) return (y1 > y2);
    if (m1 != m2) return (m1 > m2);
    return (d1 >= d2);
}