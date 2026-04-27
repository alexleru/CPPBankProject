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
    static double getValidatedAmount(const std::string& prompt, bool allowZero = false);
    static int getValidatedInteger(const std::string& prompt);

    // Utility conversions
    static std::string accountTypeToString(AccountType type);
    static std::string transactionTypeToString(TransactionType type);
    static std::string transactionStatusToString(TransactionStatus status);

    // ---------------------------------------------------------------------
    // Function pointer demo
    // ---------------------------------------------------------------------
    // These helpers demonstrate passing a function pointer as a parameter.
    // The callable has the signature: int (*)(int, int).
    // In Java-modernised code this maps cleanly to:
    //     java.util.function.IntBinaryOperator
    // or  java.util.function.BiFunction<Integer,Integer,Integer>
    // ---------------------------------------------------------------------
    static int add(int a, int b);
    static int multiply(int a, int b);

    // Takes two operands and a function pointer describing the operation.
    // Prints the result to std::cout.
    //   Syntax of the parameter: int (*operation)(int, int)
    static void performOperation(int x, int y, int (*operation)(int, int));

private:
    // Private static counter for ID generation
    static int customerIdCounter;
    static int accountIdCounter;
};

#endif // UTILS_H
