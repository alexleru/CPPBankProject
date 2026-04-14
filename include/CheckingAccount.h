#ifndef CHECKINGACCOUNT_H
#define CHECKINGACCOUNT_H

#include "Account.h"

class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    CheckingAccount(double initialBalance, double overdraft = 500.0);
    ~CheckingAccount();

    std::string getAccountType() const;
    double getOverdraftLimit() const;
    void display() const;
};

#endif // CHECKINGACCOUNT_H
