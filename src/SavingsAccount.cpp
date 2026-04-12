#include "../include/SavingsAccount.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include <iostream>
#include <stdexcept>

// Constructor - initializes savings account with interest rate
SavingsAccount::SavingsAccount(double initialBalance, double rate)
    : Account(AccountType::SAVINGS, initialBalance),
      interestRate(rate),
      minimumBalance(MIN_SAVINGS_BALANCE),
      monthsSinceInterestApplied(0) {
}

// Pure virtual implementation - returns account type
AccountType SavingsAccount::getAccountType() const {
    return AccountType::SAVINGS;
}

// Monthly processing - applies interest to savings account
void SavingsAccount::applyMonthlyProcessing() {
    if (isActive) {
        applyInterest();
        monthsSinceInterestApplied = 0;
    }
}

// Apply interest to account balance
void SavingsAccount::applyInterest() {
    if (!isActive) {
        return;
    }
    
    // Calculate interest amount
    double interestAmount = balance * interestRate / 12.0; // Monthly interest
    
    // Only apply if positive
    if (interestAmount > 0) {
        balance += interestAmount;
        lastModifiedDate = std::time(nullptr);
        
        // Record interest transaction
        Transaction transaction(TransactionType::INTEREST, interestAmount, "Monthly Interest");
        transaction.setStatus(TransactionStatus::COMPLETED);
        addTransaction(transaction);
        
        std::cout << "Interest applied: " << Utils::formatCurrency(interestAmount) 
                  << " to account " << accountId << std::endl;
    }
    
    monthsSinceInterestApplied++;
}

// Override withdraw to check minimum balance
bool SavingsAccount::withdraw(double amount, const std::string& description) {
    if (amount <= 0) {
        throw std::invalid_argument("Withdrawal amount must be positive");
    }
    
    if (!isActive) {
        throw std::runtime_error("Cannot withdraw from inactive account");
    }
    
    // Check if withdrawal would violate minimum balance
    if ((balance - amount) < minimumBalance) {
        std::cerr << "Withdrawal failed! Minimum balance of " 
                  << Utils::formatCurrency(minimumBalance) << " must be maintained.\n";
        std::cerr << "Available for withdrawal: " 
                  << Utils::formatCurrency(balance - minimumBalance) << std::endl;
        return false;
    }
    
    // Check if sufficient balance
    if (balance < amount) {
        std::cerr << "Insufficient balance. Available: " << Utils::formatCurrency(balance) << std::endl;
        return false;
    }
    
    // Perform withdrawal
    balance -= amount;
    lastModifiedDate = std::time(nullptr);
    
    // Record transaction
    Transaction transaction(TransactionType::WITHDRAWAL, amount, description);
    addTransaction(transaction);
    
    std::cout << "Withdrawal successful! " << Utils::formatCurrency(amount) 
              << " withdrawn from account " << accountId << std::endl;
    return true;
}

// Set interest rate
void SavingsAccount::setInterestRate(double rate) {
    if (rate < 0) {
        throw std::invalid_argument("Interest rate cannot be negative");
    }
    interestRate = rate;
}

// Get current interest rate
double SavingsAccount::getInterestRate() const {
    return interestRate;
}

// Get minimum balance requirement
double SavingsAccount::getMinimumBalance() const {
    return minimumBalance;
}

// Check if balance meets minimum requirement
bool SavingsAccount::checkMinimumBalance() const {
    return balance >= minimumBalance;
}

// Display savings account specific information
void SavingsAccount::displayAccountInfo() const {
    Account::displayAccountInfo();
    std::cout << "Interest Rate: " << (interestRate * 100.0) << "% per annum" << std::endl;
    std::cout << "Minimum Balance Required: " << Utils::formatCurrency(minimumBalance) << std::endl;
    std::cout << "Balance Status: " << (checkMinimumBalance() ? "OK" : "Below Minimum") << std::endl;
}
