#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Enums.h"

class Customer {
private:
    std::string customerId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    std::string address;
    CustomerStatus status;

public:
    Customer(const std::string& firstName, const std::string& lastName,
             const std::string& email, const std::string& phone,
             const std::string& address);

    std::string getCustomerId() const;
    std::string getFirstName()  const;
    std::string getLastName()   const;
    std::string getEmail()      const;
    std::string getPhone()      const;
    std::string getAddress()    const;
    CustomerStatus getStatus()  const;

    bool validate(std::string& errorMsg) const;
    void display() const;
};

#endif // CUSTOMER_H
