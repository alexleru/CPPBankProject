#include "../include/Utils.h"
#include "../include/Constants.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <regex>

// Initialize static counters
int Utils::customerIdCounter = 1000;
int Utils::accountIdCounter = 2000;

// Format currency value to string with $ and thousand separators
std::string Utils::formatCurrency(double amount) {
    // Create string stream with fixed precision and thousands separator
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    // Handle negative amounts
    bool isNegative = amount < 0;
    double absAmount = std::abs(amount);
    
    // Build the formatted string manually with thousand separators
    long long cents = static_cast<long long>(std::round(absAmount * 100));
    long long dollars = cents / 100;
    long long centsRemainder = cents % 100;
    
    std::string dollarStr = std::to_string(dollars);
    std::string result;
    
    // Add thousand separators
    int count = 0;
    for (int i = dollarStr.length() - 1; i >= 0; --i) {
        if (count == 3) {
            result = ',' + result;
            count = 0;
        }
        result = dollarStr[i] + result;
        count++;
    }
    
    // Format final output
    oss.str("");
    oss << "$" << (isNegative ? "-" : "") << result << "." 
        << std::setfill('0') << std::setw(2) << centsRemainder;
    
    return oss.str();
}

// Format timestamp to YYYY-MM-DD format
std::string Utils::formatDate(time_t timestamp) {
    struct tm* timeinfo = std::localtime(&timestamp);
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return oss.str();
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
    oss << ACCOUNT_ID_PREFIX << std::setfill('0') << std::setw(6) << (accountIdCounter++);
    return oss.str();
}

// Validate email format using basic regex
bool Utils::validateEmail(const std::string& email) {
    // Simple email validation pattern
    const std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, emailPattern);
}

// Validate phone number format (basic: digits, spaces, dashes, parentheses, plus allowed)
bool Utils::validatePhone(const std::string& phoneNumber) {
    // Allow digits, spaces, dashes, parentheses, and plus sign
    const std::regex phonePattern(R"(^[\d\s\-\(\)\+]{10,}$)");
    return std::regex_match(phoneNumber, phonePattern);
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
        case AccountType::SAVINGS:
            return "Savings";
        case AccountType::CHECKING:
            return "Checking";
        case AccountType::LOAN:
            return "Loan";
        default:
            return "Unknown";
    }
}

// Convert TransactionType enum to string
std::string Utils::transactionTypeToString(TransactionType type) {
    switch (type) {
        case TransactionType::DEPOSIT:
            return "Deposit";
        case TransactionType::WITHDRAWAL:
            return "Withdrawal";
        case TransactionType::TRANSFER:
            return "Transfer";
        case TransactionType::INTEREST:
            return "Interest";
        case TransactionType::FEE:
            return "Fee";
        case TransactionType::EMI_PAYMENT:
            return "EMI Payment";
        default:
            return "Unknown";
    }
}

// Convert TransactionStatus enum to string
std::string Utils::transactionStatusToString(TransactionStatus status) {
    switch (status) {
        case TransactionStatus::PENDING:
            return "Pending";
        case TransactionStatus::COMPLETED:
            return "Completed";
        case TransactionStatus::FAILED:
            return "Failed";
        case TransactionStatus::REVERSED:
            return "Reversed";
        default:
            return "Unknown";
    }
}
