#ifndef LOANACCOUNT_H
#define LOANACCOUNT_H

#include "Account.h"

class LoanAccount : public Account {
private:
    double loanAmount;
    double interestRate;
    int    termMonths;

public:
    LoanAccount(double principal, double rate = 0.08, int termMonths = 12);
    ~LoanAccount();

    std::string getAccountType() const;
    double getLoanAmount()   const;
    double getInterestRate() const;
    int    getTermMonths()   const;
    void display() const;
};

#endif // LOANACCOUNT_H
