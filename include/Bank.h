#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Customer.h"
#include "Account.h"

// Bank class - manages all customers and accounts
class Bank {
private:
    std::string bankName;
    std::vector<Customer> customers;
    std::map<std::string, std::shared_ptr<Account>> accountRegistry; // Fast account lookup

public:
    // Constructor
    Bank(const std::string& name);

    // Destructor
    ~Bank() = default;

    // Customer management
    bool registerCustomer(const std::string& firstName, const std::string& lastName,
                         const std::string& email, const std::string& phone,
                         const std::string& address);
    Customer* findCustomer(const std::string& customerId);
    const Customer* findCustomer(const std::string& customerId) const;
    void listAllCustomers() const;
    int getCustomerCount() const;

    // Account management
    std::shared_ptr<Account> createAccount(const std::string& customerId,
                                          AccountType type, double initialBalance,
                                          double additionalParam = 0.0);
    std::shared_ptr<Account> findAccount(const std::string& accountId) const;
    void searchAccounts(const std::string& customerId) const;
    std::vector<std::shared_ptr<Account>> getCustomerAccounts(const std::string& customerId) const;
    void listAllAccounts() const;

    // Basic operations
    bool depositToAccount(const std::string& accountId, double amount);
    bool withdrawFromAccount(const std::string& accountId, double amount);
    bool transferBetweenAccounts(const std::string& fromAccountId,
                                const std::string& toAccountId, double amount);

    // Reporting and processing
    void generateCustomerReport(const std::string& customerId) const;
    void generateBankReport() const;
    void applyMonthlyProcessing();
    void displayAccountStatement(const std::string& accountId) const;

    // Loan operations
    double calculateLoanEMI(double principal, double rate, int months) const;

    // Display
    void displayBankInfo() const;

private:
    // Helper method for finding customer index
    int findCustomerIndex(const std::string& customerId) const;
};

#endif // BANK_H
