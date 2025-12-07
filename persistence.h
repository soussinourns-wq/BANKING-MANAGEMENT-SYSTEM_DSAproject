#ifndef PERSISTENCE_H
#define PERSISTENCE_H
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#include "customer.h"
#include "employee.h"

vector<string> split(const string& line, char delimiter);
void saveAccounts(const string& filename);
void loadAccounts(const string& filename);
void saveArchivedAccounts(const string& filename);
void loadArchivedAccounts(const string& filename);
void saveEmployees(const string& filename); 
void loadEmployees(const string& filename);
void saveRequestedLoans(const string& filename);
void loadRequestedLoans(const string& filename);
void saveCompletedLoans(const string& filename);
void loadCompletedLoans(const string& filename);
void saveLoanMetadata(const string& filename);
void loadLoanMetadata(const string& filename);

//one call to save all
void saveAll();

//one call to save all
void loadAll();

//ensure the file exists, if not create it
bool ensureFileExists(const string& filename);

#endif 
