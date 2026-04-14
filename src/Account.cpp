#include "../include/Account.h"
#include "../include/Utils.h"
#include <iostream>
#include <iomanip>

Account::Account(AccountType t, double initialBalance)
    : accountId(Utils::generateAccountId()), type(t),
      balance(initialBalance), isActive(true) {}

Account::~Account() {}

std::string Account::getAccountId() const { return accountId; }
AccountType Account::getType()      const { return type; }
double      Account::getBalance()   const { return balance; }
bool        Account::getIsActive()  const { return isActive; }

void Account::display() const {
    std::cout << "  Account ID : " << accountId << "\n"
              << "  Type       : " << getAccountType() << "\n"
              << "  Balance    : $" << std::fixed << std::setprecision(2) << balance << "\n"
              << "  Status     : " << (isActive ? "Active" : "Inactive") << "\n";
}
