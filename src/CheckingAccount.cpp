#include "../include/CheckingAccount.h"
#include <iostream>
#include <iomanip>

CheckingAccount::CheckingAccount(double initialBalance, double overdraft)
    : Account(CHECKING, initialBalance), overdraftLimit(overdraft) {}

CheckingAccount::~CheckingAccount() {}

std::string CheckingAccount::getAccountType() const { return "Checking"; }
double CheckingAccount::getOverdraftLimit()   const { return overdraftLimit; }

void CheckingAccount::display() const {
    Account::display();
    std::cout << "  Overdraft  : $" << std::fixed << std::setprecision(2)
              << overdraftLimit << "\n";
}
