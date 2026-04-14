#include "../include/SavingsAccount.h"
#include <iostream>
#include <iomanip>

SavingsAccount::SavingsAccount(double initialBalance, double rate)
    : Account(SAVINGS, initialBalance), interestRate(rate) {}

SavingsAccount::~SavingsAccount() {}

std::string SavingsAccount::getAccountType() const { return "Savings"; }
double SavingsAccount::getInterestRate()     const { return interestRate; }

void SavingsAccount::display() const {
    Account::display();
    std::cout << "  Interest   : " << std::fixed << std::setprecision(1)
              << (interestRate * 100.0) << "% p.a.\n";
}
