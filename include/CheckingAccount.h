#ifndef CHECKINGACCOUNT_H
#define CHECKINGACCOUNT_H

#include "Account.h"

// CheckingAccount class - inherits from Account
class CheckingAccount : public Account {
private:
    double overdraftLimit;
    double monthlyFee;
    int monthlyTransactionCount;

public:
    // Constructor
    CheckingAccount(double initialBalance, double overdraft = 500.0);

    // Destructor
    ~CheckingAccount() override = default;

    // Pure virtual implementations
    AccountType getAccountType() const override;
    void applyMonthlyProcessing() override;

    // Specific methods for CheckingAccount
    bool withdraw(double amount, const std::string& description = "Withdrawal") override;
    void applyMonthlyFee();
    void resetMonthlyTransactionCount();
    int getMonthlyTransactionCount() const;
    double getOverdraftLimit() const;
    double getAvailableBalance() const;

    // Display
    void displayAccountInfo() const override;

private:
    // Checks if withdrawal would exceed overdraft
    bool canWithdraw(double amount) const;
};

#endif // CHECKINGACCOUNT_H
