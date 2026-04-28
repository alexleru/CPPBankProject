#include "../include/Bank.h"
#include <iostream>

Bank::Bank(const std::string& name) : bankName(name) {}

Bank::~Bank() {
    for (CustomerIter it = customers.begin(); it != customers.end(); ++it)
        delete *it;
}

bool Bank::registerCustomer(const std::string& fn, const std::string& ln,
                             const ContactInfo& info,
                             CustomerId& outId, std::string& errorMsg,
                             CustomerStatus status) {
    Customer::Ptr c = new Customer(fn, ln, info, status);
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
    int idx = 0;
    for (CustomerIter it = customers.begin(); it != customers.end(); ++it) {
        std::cout << "\n--- Customer " << (++idx) << " ---\n";
        (*it)->display();
    }
    std::cout << "\nTotal: " << customers.size() << " customer(s).\n";
}

std::string Bank::getBankName() const { return bankName; }
