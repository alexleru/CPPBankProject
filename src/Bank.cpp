#include "../include/Bank.h"
#include <iostream>

Bank::Bank(const std::string& name) : bankName(name) {}

Bank::~Bank() {
    for (size_t i = 0; i < customers.size(); ++i)
        delete customers[i];
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
