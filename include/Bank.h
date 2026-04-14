#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>
#include "Customer.h"

class Bank {
private:
    std::string bankName;
    std::vector<Customer*> customers;

public:
    Bank(const std::string& name);
    ~Bank();

    bool registerCustomer(const std::string& firstName, const std::string& lastName,
                          const std::string& email, const std::string& phone,
                          const std::string& address,
                          std::string& outCustomerId, std::string& errorMsg);

    void listCustomers() const;
    std::string getBankName() const;
};

#endif // BANK_H
