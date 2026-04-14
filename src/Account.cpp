#include "../include/Account.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>

// Constructor - initializes account with type and balance
Account::Account(AccountType accountType, double initialBalance)
    : type(accountType),
      balance(initialBalance),
      createdDate(std::time(NULL)),
      lastModifiedDate(std::time(NULL)),
      isActive(true) {
    accountId = Utils::generateAccountId();
}

// Destructor
Account::~Account() {
}

// Deposit funds into account
void Account::deposit(double amount, const std::string& description) {
    if (amount <= 0) {
        throw std::invalid_argument("Deposit amount must be positive");
    }
    
    if (!isActive) {
        throw std::runtime_error("Cannot deposit to inactive account");
    }
    
    // Update balance
    balance += amount;
    lastModifiedDate = std::time(nullptr);
    
    // Record transaction
    Transaction transaction(DEPOSIT, amount, description);
    addTransaction(transaction);
    
    std::cout << "Deposit successful! " << Utils::formatCurrency(amount) 
              << " deposited to account " << accountId << std::endl;
}

// Withdraw funds from account (base implementation)
bool Account::withdraw(double amount, const std::string& description) {
    if (amount <= 0) {
        throw std::invalid_argument("Withdrawal amount must be positive");
    }
    
    if (!isActive) {
        throw std::runtime_error("Cannot withdraw from inactive account");
    }
    
    // Check if sufficient balance
    if (balance < amount) {
        std::cerr << "Insufficient balance. Available: " << Utils::formatCurrency(balance) << std::endl;
        return false;
    }
    
    // Update balance
    balance -= amount;
    lastModifiedDate = std::time(nullptr);
    
    // Record transaction
    Transaction transaction(WITHDRAWAL, amount, description);
    addTransaction(transaction);
    
    std::cout << "Withdrawal successful! " << Utils::formatCurrency(amount) 
              << " withdrawn from account " << accountId << std::endl;
    return true;
}

// Print formatted account statement with transaction history
void Account::printStatement() const {
    std::cout << "\n";
    std::cout << "===============================================\n";
    std::cout << "ACCOUNT STATEMENT\n";
    std::cout << "===============================================\n";
    std::cout << "Account ID: " << accountId << std::endl;
    std::cout << "Account Type: " << typeToString(type) << std::endl;
    std::cout << "Current Balance: " << Utils::formatCurrency(balance) << std::endl;
    std::cout << "Account Status: " << (isActive ? "Active" : "Inactive") << std::endl;
    std::cout << "Created Date: " << Utils::formatDate(createdDate) << std::endl;
    std::cout << "Last Modified: " << Utils::formatDate(lastModifiedDate) << std::endl;
    std::cout << "\nTRANSACTION HISTORY:\n";
    std::cout << "===============================================\n";
    
    if (transactionHistory.empty()) {
        std::cout << "No transactions found.\n";
    } else {
        std::cout << std::left << std::setw(15) << "Transaction ID"
                  << std::setw(15) << "Type"
                  << std::setw(12) << "Amount"
                  << std::setw(20) << "Date/Time"
                  << std::setw(12) << "Status"
                  << std::setw(20) << "Description" << std::endl;
        std::cout << "-----------------------------------------------\n";
        
        for (size_t i = 0; i < transactionHistory.size(); ++i) {
            transactionHistory[i].display();
        }
    }
    
    std::cout << "===============================================\n\n";
}

// Getters
std::string Account::getAccountId() const {
    return accountId;
}

double Account::getBalance() const {
    return balance;
}

bool Account::getIsActive() const {
    return isActive;
}

time_t Account::getCreatedDate() const {
    return createdDate;
}

const std::vector<Transaction>& Account::getTransactionHistory() const {
    return transactionHistory;
}

// Setters
void Account::setIsActive(bool active) {
    isActive = active;
    lastModifiedDate = std::time(nullptr);
}

// Static method to get interest rate for account type
double Account::getInterestForType(AccountType type) {
    switch (type) {
        case SAVINGS:
            return SAVINGS_INTEREST_RATE;
        case CHECKING:
            return CHECKING_INTEREST_RATE;
        case LOAN:
            return LOAN_INTEREST_RATE;
        default:
            return 0.0;
    }
}}

// Static method to convert account type to string
std::string Account::typeToString(AccountType type) {
    return Utils::accountTypeToString(type);
}

// Add transaction to history
void Account::addTransaction(const Transaction& transaction) {
    transactionHistory.push_back(transaction);
}

// Display account information
void Account::displayAccountInfo() const {
    std::cout << "\nAccount ID: " << accountId << std::endl;
    std::cout << "Type: " << typeToString(type) << std::endl;
    std::cout << "Balance: " << Utils::formatCurrency(balance) << std::endl;
    std::cout << "Status: " << (isActive ? "Active" : "Inactive") << std::endl;
}

// Format balance for display
std::string Account::formatBalance() const {
    return Utils::formatCurrency(balance);
}
