#ifndef SAVINGSACCOUNT_H
#define SAVINGSACCOUNT_H

#include "Account.h"

// SavingsAccount class - inherits from Account
class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;
    int monthsSinceInterestApplied;

public:
    // Constructor
    SavingsAccount(double initialBalance, double rate = 0.035);

    // Destructor
    ~SavingsAccount();

    // Pure virtual implementations
    AccountType getAccountType() const;
    void applyMonthlyProcessing();

    // Specific methods for SavingsAccount
    void applyInterest();
    bool withdraw(double amount, const std::string& description = "Withdrawal");
    void setInterestRate(double rate);
    double getInterestRate() const;
    double getMinimumBalance() const;

    // Display
    void displayAccountInfo() const;

private:
    // Validates minimum balance constraint
    bool checkMinimumBalance() const;
};

#endif // SAVINGSACCOUNT_H
