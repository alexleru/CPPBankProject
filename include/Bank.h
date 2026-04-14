#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>
#include <map>
#include "Customer.h"
#include "Account.h"
#include "Enums.h"

class Bank {
private:
    std::string bankName;
    std::vector<Customer*> customers;
    std::map<std::string, Account*> accountRegistry;

    Customer* findCustomer(const std::string& customerId);

public:
    Bank(const std::string& name);
    ~Bank();

    bool registerCustomer(const std::string& firstName, const std::string& lastName,
                          const std::string& email, const std::string& phone,
                          const std::string& address,
                          std::string& outCustomerId, std::string& errorMsg);

    Account* createAccount(const std::string& customerId, AccountType type,
                           double initialBalance, double additionalParam,
                           std::string& errorMsg);

    void listCustomers() const;
    std::string getBankName() const;
};

#endif // BANK_H
