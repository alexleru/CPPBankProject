#include "../include/Bank.h"
#include "../include/SavingsAccount.h"
#include "../include/CheckingAccount.h"
#include "../include/LoanAccount.h"
#include "../include/Constants.h"
#include <iostream>
#include <iomanip>

Bank::Bank(const std::string& name) : bankName(name) {}

Bank::~Bank() {
    for (size_t i = 0; i < customers.size(); ++i)
        delete customers[i];
}

Customer* Bank::findCustomer(const std::string& customerId) {
    for (size_t i = 0; i < customers.size(); ++i)
        if (customers[i]->getCustomerId() == customerId)
            return customers[i];
    return NULL;
}

bool Bank::registerCustomer(const std::string& fn, const std::string& ln,
                             const std::string& email, const std::string& phone,
                             const std::string& address,
                             std::string& outId, std::string& errorMsg) {
    Customer* c = new Customer(fn, ln, email, phone, address);
    if (!c->validate(errorMsg)) {
        delete c;
        return false;
    }
    customers.push_back(c);
    outId = c->getCustomerId();
    return true;
}

Account* Bank::createAccount(const std::string& customerId, AccountType type,
                              double initialBalance, double additionalParam,
                              std::string& errorMsg) {
    Customer* customer = findCustomer(customerId);
    if (!customer) { errorMsg = "Customer not found."; return NULL; }
    if (!customer->canAddAccount()) {
        errorMsg = "Customer has reached the maximum number of accounts (" +
                   std::string("5") + ").";
        return NULL;
    }
    if (initialBalance < 0) { errorMsg = "Initial balance cannot be negative."; return NULL; }

    Account* account = NULL;
    switch (type) {
        case SAVINGS:
            account = new SavingsAccount(initialBalance,
                additionalParam > 0 ? additionalParam : SAVINGS_INTEREST_RATE);
            break;
        case CHECKING:
            account = new CheckingAccount(initialBalance,
                additionalParam > 0 ? additionalParam : OVERDRAFT_LIMIT);
            break;
        case LOAN:
            account = new LoanAccount(initialBalance,
                additionalParam > 0 ? additionalParam : LOAN_INTEREST_RATE, 12);
            break;
        default:
            errorMsg = "Unknown account type.";
            return NULL;
    }

    customer->addAccount(account);
    accountRegistry[account->getAccountId()] = account;
    return account;
}

void Bank::listCustomers() const {
    if (customers.empty()) {
        std::cout << "No customers registered.\n";
        return;
    }
    for (size_t i = 0; i < customers.size(); ++i) {
        std::cout << "\n--- Customer " << (i + 1) << " ---\n";
        customers[i]->display();
    }
    std::cout << "\nTotal: " << customers.size() << " customer(s).\n";
}

std::string Bank::getBankName() const { return bankName; }
