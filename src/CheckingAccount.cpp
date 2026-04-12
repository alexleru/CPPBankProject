#include "../include/CheckingAccount.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include <iostream>
#include <stdexcept>

// Constructor - initializes checking account with overdraft limit
CheckingAccount::CheckingAccount(double initialBalance, double overdraft)
    : Account(AccountType::CHECKING, initialBalance),
      overdraftLimit(overdraft),
      monthlyFee(MONTHLY_CHECKING_FEE),
      monthlyTransactionCount(0) {
}

// Pure virtual implementation - returns account type
AccountType CheckingAccount::getAccountType() const {
    return AccountType::CHECKING;
}

// Monthly processing - applies monthly fee
void CheckingAccount::applyMonthlyProcessing() {
    if (isActive) {
        applyMonthlyFee();
        resetMonthlyTransactionCount();
    }
}

// Apply monthly maintenance fee
void CheckingAccount::applyMonthlyFee() {
    if (!isActive) {
        return;
    }
    
    if (monthlyFee > 0) {
        // Only deduct fee if balance can cover it
        if (balance >= monthlyFee) {
            balance -= monthlyFee;
        } else {
            // Deduct partial fee (overdraft)
            balance -= monthlyFee;
        }
        
        lastModifiedDate = std::time(nullptr);
        
        // Record fee transaction
        Transaction transaction(TransactionType::FEE, monthlyFee, "Monthly Service Fee");
        transaction.setStatus(TransactionStatus::COMPLETED);
        addTransaction(transaction);
        
        std::cout << "Monthly fee applied: " << Utils::formatCurrency(monthlyFee) 
                  << " to account " << accountId << std::endl;
    }
}

// Override withdraw to allow overdraft
bool CheckingAccount::withdraw(double amount, const std::string& description) {
    if (amount <= 0) {
        throw std::invalid_argument("Withdrawal amount must be positive");
    }
    
    if (!isActive) {
        throw std::runtime_error("Cannot withdraw from inactive account");
    }
    
    // Check if withdrawal would exceed overdraft limit
    if (!canWithdraw(amount)) {
        std::cerr << "Withdrawal would exceed overdraft limit.\n";
        std::cerr << "Available balance (with overdraft): " 
                  << Utils::formatCurrency(getAvailableBalance()) << std::endl;
        return false;
    }
    
    // Perform withdrawal (may result in negative balance if overdraft used)
    balance -= amount;
    lastModifiedDate = std::time(nullptr);
    monthlyTransactionCount++;
    
    // Record transaction
    Transaction transaction(TransactionType::WITHDRAWAL, amount, description);
    addTransaction(transaction);
    
    std::cout << "Withdrawal successful! " << Utils::formatCurrency(amount) 
              << " withdrawn from account " << accountId << std::endl;
    
    if (balance < 0) {
        std::cout << "Note: Overdraft is being used. Balance: " 
                  << Utils::formatCurrency(balance) << std::endl;
    }
    
    return true;
}

// Check if balance permits withdrawal considering overdraft
bool CheckingAccount::canWithdraw(double amount) const {
    // Can withdraw if balance + overdraft limit >= amount
    return (getAvailableBalance() >= amount);
}

// Reset monthly transaction counter
void CheckingAccount::resetMonthlyTransactionCount() {
    monthlyTransactionCount = 0;
}

// Get monthly transaction count
int CheckingAccount::getMonthlyTransactionCount() const {
    return monthlyTransactionCount;
}

// Get overdraft limit
double CheckingAccount::getOverdraftLimit() const {
    return overdraftLimit;
}

// Get available balance including overdraft
double CheckingAccount::getAvailableBalance() const {
    return balance + overdraftLimit;
}

// Display checking account specific information
void CheckingAccount::displayAccountInfo() const {
    Account::displayAccountInfo();
    std::cout << "Overdraft Limit: " << Utils::formatCurrency(overdraftLimit) << std::endl;
    std::cout << "Available Balance (with overdraft): " << Utils::formatCurrency(getAvailableBalance()) << std::endl;
    std::cout << "Monthly Fee: " << Utils::formatCurrency(monthlyFee) << std::endl;
    std::cout << "Monthly Transactions: " << monthlyTransactionCount << std::endl;
    
    if (balance < 0) {
        std::cout << "Overdraft Used: " << Utils::formatCurrency(-balance) << std::endl;
    }
}
