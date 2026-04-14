#include "../include/LoanAccount.h"
#include <iostream>
#include <iomanip>

LoanAccount::LoanAccount(double principal, double rate, int term)
    : Account(LOAN, 0.0), loanAmount(principal), interestRate(rate), termMonths(term) {}

LoanAccount::~LoanAccount() {}

std::string LoanAccount::getAccountType() const { return "Loan"; }
double LoanAccount::getLoanAmount()        const { return loanAmount; }
double LoanAccount::getInterestRate()      const { return interestRate; }
int    LoanAccount::getTermMonths()        const { return termMonths; }

void LoanAccount::display() const {
    std::cout << "  Account ID : " << accountId << "\n"
              << "  Type       : Loan\n"
              << "  Principal  : $" << std::fixed << std::setprecision(2) << loanAmount << "\n"
              << "  Rate       : " << (interestRate * 100.0) << "% p.a.\n"
              << "  Term       : " << termMonths << " months\n"
              << "  Status     : " << (isActive ? "Active" : "Inactive") << "\n";
}
