#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

// Global counters for ID generation
extern int globalCustomerCounter;
extern int globalTransactionCounter;
extern int globalAccountCounter;

// Global statistics
extern int totalCustomersCreated;
extern int totalAccountsCreated;
extern int totalTransactionsProcessed;

// Global configuration
extern bool enableDebugLogging;
extern bool enableTransactionValidation;
extern std::string logFilePath;

// Global utility functions
void initializeGlobals();
void resetGlobalCounters();
void printGlobalStatistics();

#endif // GLOBALS_H