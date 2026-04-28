#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>
#include "Customer.h"

// Container alias + iterator alias
typedef std::vector<Customer::Ptr>   CustomerList;
typedef CustomerList::const_iterator CustomerIter;

class Bank {
private:
    std::string  bankName;
    CustomerList customers;

public:
    Bank(const std::string& name);
    ~Bank();

    // Default argument on the trailing CustomerStatus parameter
    bool registerCustomer(const std::string& firstName, const std::string& lastName,
                          const ContactInfo& contact,
                          CustomerId& outCustomerId, std::string& errorMsg,
                          CustomerStatus status = ACTIVE);

    void listCustomers() const;
    std::string getBankName() const;
};

#endif // BANK_H
