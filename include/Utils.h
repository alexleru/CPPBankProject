#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>
#include "Enums.h"

// Utility class with static helper methods
class Utils {
public:
    // Delete default constructor to prevent instantiation
    Utils();

    // Currency formatting
    static std::string formatCurrency(double amount);

    // Date formatting
    static std::string formatDate(time_t timestamp);

    // ID generation
    static std::string generateCustomerId();
    static std::string generateAccountId();

    // Validation methods
    static bool validateEmail(const std::string& email);
    static bool validatePhone(const std::string& phoneNumber);

    // Input validation helpers
    static double getValidatedAmount(const std::string& prompt);
    static int getValidatedInteger(const std::string& prompt);

    // Utility conversions
    static std::string accountTypeToString(AccountType type);
    static std::string transactionTypeToString(TransactionType type);
    static std::string transactionStatusToString(TransactionStatus status);

private:
    // Private static counter for ID generation
    static int customerIdCounter;
    static int accountIdCounter;
};

#endif // UTILS_H
