#include "../include/Globals.h"
#include <iostream>
#include <string>

// Initialize global counters
int globalCustomerCounter = 0;
int globalTransactionCounter = 10000;
int globalAccountCounter = 1000;

// Initialize global statistics
int totalCustomersCreated = 0;
int totalAccountsCreated = 0;
int totalTransactionsProcessed = 0;

// Initialize global configuration
bool enableDebugLogging = false;
bool enableTransactionValidation = true;
std::string logFilePath = "bank_log.txt";

// Initialize all global variables
void initializeGlobals() {
    globalCustomerCounter = 0;
    globalTransactionCounter = 10000;
    globalAccountCounter = 1000;
    totalCustomersCreated = 0;
    totalAccountsCreated = 0;
    totalTransactionsProcessed = 0;
    enableDebugLogging = false;
    enableTransactionValidation = true;
    logFilePath = "bank_log.txt";
}

// Reset global counters (useful for testing)
void resetGlobalCounters() {
    globalCustomerCounter = 0;
    globalTransactionCounter = 10000;
    globalAccountCounter = 1000;
    totalCustomersCreated = 0;
    totalAccountsCreated = 0;
    totalTransactionsProcessed = 0;
}

// Print global statistics
void printGlobalStatistics() {
    std::cout << "\n=== GLOBAL STATISTICS ===\n";
    std::cout << "Total Customers Created: " << totalCustomersCreated << std::endl;
    std::cout << "Total Accounts Created: " << totalAccountsCreated << std::endl;
    std::cout << "Total Transactions Processed: " << totalTransactionsProcessed << std::endl;
    std::cout << "Current Customer Counter: " << globalCustomerCounter << std::endl;
    std::cout << "Current Transaction Counter: " << globalTransactionCounter << std::endl;
    std::cout << "Current Account Counter: " << globalAccountCounter << std::endl;
    std::cout << "Debug Logging: " << (enableDebugLogging ? "Enabled" : "Disabled") << std::endl;
    std::cout << "Transaction Validation: " << (enableTransactionValidation ? "Enabled" : "Disabled") << std::endl;
    std::cout << "========================\n";
}