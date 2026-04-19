#include "../include/Utils.h"
#include "../include/Constants.h"
#include "../include/Globals.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <ctime>

// Initialize static counters
int Utils::customerIdCounter = 1000;
int Utils::accountIdCounter = 2000;

// Format currency value to string with $ and thousand separators
std::string Utils::formatCurrency(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    // Handle negative amounts
    if (amount < 0) {
        oss << "-";
        amount = -amount;
    }
    
    // Simple currency formatting without thousand separators for C++03
    oss << "$" << amount;
    
    return oss.str();
}

// Format timestamp to YYYY-MM-DD format
std::string Utils::formatDate(time_t timestamp) {
    char buffer[80];
    struct tm* timeinfo = std::localtime(&timestamp);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

// Generate unique customer ID
std::string Utils::generateCustomerId() {
    std::ostringstream oss;
    oss << CUSTOMER_ID_PREFIX << std::setfill('0') << std::setw(6) << (customerIdCounter++);
    return oss.str();
}

// Generate unique account ID
std::string Utils::generateAccountId() {
    std::ostringstream oss;
    oss << ACCOUNT_ID_PREFIX << std::setfill('0') << std::setw(6) << (globalAccountCounter++);
    return oss.str();
}

// Validate email format using basic string checks
bool Utils::validateEmail(const std::string& email) {
    // Simple email validation - check for @ and .
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    
    if (atPos == std::string::npos || dotPos == std::string::npos) {
        return false;
    }
    
    if (atPos == 0 || dotPos <= atPos + 1 || dotPos == email.length() - 1) {
        return false;
    }
    
    return true;
}

// Validate phone number format (basic: digits, spaces, dashes, parentheses, plus allowed)
bool Utils::validatePhone(const std::string& phoneNumber) {
    // Simple phone validation - check length and allowed characters
    if (phoneNumber.length() < 10) {
        return false;
    }
    
    for (size_t i = 0; i < phoneNumber.length(); ++i) {
        char c = phoneNumber[i];
        if (!std::isdigit(c) && c != ' ' && c != '-' && c != '(' && c != ')' && c != '+') {
            return false;
        }
    }
    
    return true;
}

// Get validated amount from user input
double Utils::getValidatedAmount(const std::string& prompt) {
    double amount;
    bool valid = false;
    
    while (!valid) {
        std::cout << prompt;
        if (std::cin >> amount && amount > 0) {
            valid = true;
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid amount. Please enter a positive number.\n";
        }
    }
    
    return amount;
}

// Get validated integer from user input
int Utils::getValidatedInteger(const std::string& prompt) {
    int value;
    bool valid = false;
    
    while (!valid) {
        std::cout << prompt;
        if (std::cin >> value) {
            valid = true;
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter an integer.\n";
        }
    }
    
    return value;
}

// Convert AccountType enum to string
std::string Utils::accountTypeToString(AccountType type) {
    switch (type) {
        case SAVINGS:
            return "Savings";
        case CHECKING:
            return "Checking";
        case LOAN:
            return "Loan";
        default:
            return "Unknown";
    }
}

// Convert TransactionType enum to string
std::string Utils::transactionTypeToString(TransactionType type) {
    switch (type) {
        case DEPOSIT:
            return "Deposit";
        case WITHDRAWAL:
            return "Withdrawal";
        case TRANSFER:
            return "Transfer";
        case INTEREST:
            return "Interest";
        case FEE:
            return "Fee";
        case EMI_PAYMENT:
            return "EMI Payment";
        default:
            return "Unknown";
    }
}

// ---------------------------------------------------------------------
// Function pointer demo
// ---------------------------------------------------------------------
// Sample functions to be passed as a function pointer parameter.
int Utils::add(int a, int b) {
    return a + b;
}

int Utils::multiply(int a, int b) {
    return a * b;
}

// Function that takes a function pointer as a parameter.
// Parameter syntax: int (*operation)(int, int)
void Utils::performOperation(int x, int y, int (*operation)(int, int)) {
    if (operation == NULL) {
        std::cerr << "performOperation: null function pointer\n";
        return;
    }
    std::cout << "Result: " << operation(x, y) << std::endl;
}

// Convert TransactionStatus enum to string
std::string Utils::transactionStatusToString(TransactionStatus status) {
    switch (status) {
        case PENDING:
            return "Pending";
        case COMPLETED:
            return "Completed";
        case FAILED:
            return "Failed";
        case REVERSED:
            return "Reversed";
        default:
            return "Unknown";
    }
}
