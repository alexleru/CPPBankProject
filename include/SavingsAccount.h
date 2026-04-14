#ifndef SAVINGSACCOUNT_H
#define SAVINGSACCOUNT_H

#include "Account.h"

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(double initialBalance, double rate = 0.035);
    ~SavingsAccount();

    std::string getAccountType() const;
    double getInterestRate() const;
    void display() const;
};

#endif // SAVINGSACCOUNT_H
