#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "Enums.h"
#include "Account.h"

// Forward declaration
class Account;

// Customer class
class Customer {
private:
    std::string customerId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phoneNumber;
    std::string address;
    CustomerStatus status;
    time_t registrationDate;
    std::vector<Account*> accounts;

public:
    // Constructor
    Customer(const std::string& firstName, const std::string& lastName,
             const std::string& email, const std::string& phone, const std::string& address);

    // Destructor
    ~Customer();

    // Getters
    std::string getCustomerId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getEmail() const;
    std::string getPhoneNumber() const;
    std::string getAddress() const;
    CustomerStatus getStatus() const;
    time_t getRegistrationDate() const;
    int getAccountCount() const;

    // Setters
    void setFirstName(const std::string& name);
    void setLastName(const std::string& name);
    void setEmail(const std::string& email);
    void setPhoneNumber(const std::string& phone);
    void setAddress(const std::string& addr);
    void setStatus(CustomerStatus newStatus);

    // Account management
    Account* addAccount(Account* account);
    Account* getAccount(int index) const;
    Account* getAccountById(const std::string& accountId) const;
    bool canAddAccount() const;
    bool removeAccount(const std::string& accountId);

    // Validation and operations
    bool validate() const;
    void display() const;
    void displayPortfolio() const;

private:
    // Helper method to generate customer ID
    static std::string generateCustomerId();
    static int customerCounter;
};

#endif // CUSTOMER_H
