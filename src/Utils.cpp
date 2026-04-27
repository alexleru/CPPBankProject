#include "../include/Utils.h"
#include "../include/Constants.h"
#include "../include/Globals.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <ctime>

int Utils::customerIdCounter = 1000;
int Utils::accountIdCounter = 2000;

std::string Utils::formatCurrency(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    if (amount < 0) { oss << "-"; amount = -amount; }
    oss << "$" << amount;
    return oss.str();
}

std::string Utils::formatDate(time_t timestamp) {
    char buffer[80];
    struct tm* timeinfo = std::localtime(&timestamp);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

std::string Utils::generateCustomerId() {
    std::ostringstream oss;
    oss << CUSTOMER_ID_PREFIX << std::setfill('0') << std::setw(6) << (customerIdCounter++);
    return oss.str();
}

std::string Utils::generateAccountId() {
    std::ostringstream oss;
    oss << ACCOUNT_ID_PREFIX << std::setfill('0') << std::setw(6) << (globalAccountCounter++);
    return oss.str();
}

bool Utils::validateEmail(const std::string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    if (atPos == std::string::npos || dotPos == std::string::npos) return false;
    if (atPos == 0 || dotPos <= atPos + 1 || dotPos == email.length() - 1) return false;
    return true;
}

bool Utils::validatePhone(const std::string& phoneNumber) {
    if (phoneNumber.length() < 10) return false;
    for (size_t i = 0; i < phoneNumber.length(); ++i) {
        char c = phoneNumber[i];
        if (!std::isdigit(c) && c != ' ' && c != '-' && c != '(' && c != ')' && c != '+') return false;
    }
    return true;
}

double Utils::getValidatedAmount(const std::string& prompt, bool allowZero) {
    double amount;
    bool valid = false;
    while (!valid) {
        std::cout << prompt;
        if (std::cin >> amount && (allowZero ? amount >= 0 : amount > 0)) valid = true;
        else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << (allowZero ? "Enter non-negative.\n" : "Enter positive.\n");
        }
    }
    return amount;
}

int Utils::getValidatedInteger(const std::string& prompt) {
    int value;
    bool valid = false;
    while (!valid) {
        std::cout << prompt;
        if (std::cin >> value) valid = true;
        else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Enter an integer.\n";
        }
    }
    return value;
}

std::string Utils::accountTypeToString(AccountType type) {
    switch (type) {
        case SAVINGS:  return "Savings";
        case CHECKING: return "Checking";
        case LOAN:     return "Loan";
        case MORTGAGE: return "Mortgage";
        default:       return "Unknown";
    }
}

std::string Utils::transactionTypeToString(TransactionType type) {
    switch (type) {
        case DEPOSIT:     return "Deposit";
        case WITHDRAWAL:  return "Withdrawal";
        case TRANSFER:    return "Transfer";
        case INTEREST:    return "Interest";
        case FEE:         return "Fee";
        case EMI_PAYMENT: return "EMI Payment";
        default:          return "Unknown";
    }
}

int Utils::add(int a, int b) { return a + b; }
int Utils::multiply(int a, int b) { return a * b; }

void Utils::performOperation(int x, int y, int (*operation)(int, int)) {
    if (operation == NULL) { std::cerr << "performOperation: null function pointer\n"; return; }
    std::cout << "Result: " << operation(x, y) << std::endl;
}

std::string Utils::transactionStatusToString(TransactionStatus status) {
    switch (status) {
        case PENDING:   return "Pending";
        case COMPLETED: return "Completed";
        case FAILED:    return "Failed";
        case REVERSED:  return "Reversed";
        default:        return "Unknown";
    }
}
