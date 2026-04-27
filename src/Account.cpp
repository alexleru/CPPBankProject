#include "../include/Account.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include "../include/Globals.h"
#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>

Account::Account(AccountType accountType, double initialBalance)
    : type(accountType),
      balance(initialBalance),
      createdDate(std::time(NULL)),
      lastModifiedDate(std::time(NULL)),
      isActive(true) {
    accountId = Utils::generateAccountId();
    totalAccountsCreated++;
}

Account::~Account() {}

void Account::deposit(double amount, const std::string& description) {
    if (amount <= 0) throw std::invalid_argument("Deposit amount must be positive");
    if (!isActive) throw std::runtime_error("Cannot deposit to inactive account");
    balance += amount;
    lastModifiedDate = std::time(NULL);
    Transaction transaction(DEPOSIT, amount, description);
    addTransaction(transaction);
    std::cout << "Deposit successful! " << Utils::formatCurrency(amount)
              << " deposited to account " << accountId << std::endl;
}

bool Account::withdraw(double amount, const std::string& description) {
    if (amount <= 0) throw std::invalid_argument("Withdrawal amount must be positive");
    if (!isActive) throw std::runtime_error("Cannot withdraw from inactive account");
    if (balance < amount) {
        std::cerr << "Insufficient balance. Available: " << Utils::formatCurrency(balance) << std::endl;
        return false;
    }
    balance -= amount;
    lastModifiedDate = std::time(NULL);
    Transaction transaction(WITHDRAWAL, amount, description);
    addTransaction(transaction);
    std::cout << "Withdrawal successful! " << Utils::formatCurrency(amount)
              << " withdrawn from account " << accountId << std::endl;
    return true;
}

void Account::printStatement() const {
    std::cout << "\n===============================================\n";
    std::cout << "ACCOUNT STATEMENT\n";
    std::cout << "===============================================\n";
    std::cout << "Account ID: " << accountId << std::endl;
    std::cout << "Account Type: " << typeToString(type) << std::endl;
    std::cout << "Current Balance: " << Utils::formatCurrency(balance) << std::endl;
    std::cout << "Account Status: " << (isActive ? "Active" : "Inactive") << std::endl;
    std::cout << "Created Date: " << Utils::formatDate(createdDate) << std::endl;
    std::cout << "Last Modified: " << Utils::formatDate(lastModifiedDate) << std::endl;
    std::cout << "\nTRANSACTION HISTORY:\n===============================================\n";
    if (transactionHistory.empty()) {
        std::cout << "No transactions found.\n";
    } else {
        for (size_t i = 0; i < transactionHistory.size(); ++i) {
            transactionHistory[i].display();
        }
    }
    std::cout << "===============================================\n\n";
}

std::string Account::getAccountId() const { return accountId; }
double      Account::getBalance() const { return balance; }
bool        Account::getIsActive() const { return isActive; }
time_t      Account::getCreatedDate() const { return createdDate; }
const TransactionHistory& Account::getTransactionHistory() const { return transactionHistory; }

void Account::setIsActive(bool active) {
    isActive = active;
    lastModifiedDate = std::time(NULL);
}

double Account::getInterestForType(AccountType type) {
    switch (type) {
        case SAVINGS:  return SAVINGS_INTEREST_RATE;
        case CHECKING: return CHECKING_INTEREST_RATE;
        case LOAN:     return LOAN_INTEREST_RATE;
        case MORTGAGE: return LOAN_INTEREST_RATE;
        default:       return 0.0;
    }
}

std::string Account::typeToString(AccountType type) {
    return Utils::accountTypeToString(type);
}

void Account::addTransaction(const Transaction& transaction) {
    transactionHistory.push_back(transaction);
}

void Account::displayAccountInfo() const {
    std::cout << "\nAccount ID: " << accountId << std::endl;
    std::cout << "Type: " << typeToString(type) << std::endl;
    std::cout << "Balance: " << Utils::formatCurrency(balance) << std::endl;
    std::cout << "Status: " << (isActive ? "Active" : "Inactive") << std::endl;
}

std::string Account::formatBalance() const { return Utils::formatCurrency(balance); }

void debugAccountInfo(const Account& account) {
    std::cout << "\n=== DEBUG ACCOUNT INFO ===\n";
    std::cout << "Account ID: " << account.accountId << std::endl;
    std::cout << "Type: " << account.typeToString(account.type) << std::endl;
    std::cout << "Balance: " << account.balance << std::endl;
    std::cout << "Is Active: " << (account.isActive ? "Yes" : "No") << std::endl;
    std::cout << "Transaction Count: " << account.transactionHistory.size() << std::endl;
    std::cout << "===========================\n";
}

bool validateAccountBalance(const Account& account) {
    if (account.balance < 0) {
        std::cerr << "ERROR: Account " << account.accountId << " negative balance" << std::endl;
        return false;
    }
    return true;
}

void forceBalanceUpdate(Account& account, double newBalance) {
    if (newBalance >= 0) {
        account.balance = newBalance;
        account.lastModifiedDate = std::time(NULL);
        std::cout << "Account " << account.accountId << " balance set to: "
                  << Utils::formatCurrency(newBalance) << std::endl;
    }
}

TransactionHistory getAccountTransactions(const Account& account) {
    return account.transactionHistory;
}
