#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>
#include <map>
#include "Customer.h"
#include "Account.h"

// Type aliases for better readability
typedef std::vector<Customer> CustomerList;
typedef std::map<std::string, Account*> AccountRegistry;
typedef std::vector<Account*> AccountList;

// Bank class - manages all customers and accounts
class Bank {
private:
    std::string bankName;
    CustomerList customers;
    AccountRegistry accountRegistry; // Fast account lookup

public:
    // Constructor
    Bank(const std::string& name);

    // Destructor
    ~Bank();

    // Customer management
    bool registerCustomer(const std::string& firstName, const std::string& lastName,
                         const std::string& email, const std::string& phone,
                         const std::string& address);
    Customer* findCustomer(const std::string& customerId);
    const Customer* findCustomer(const std::string& customerId) const;
    void listAllCustomers() const;
    int getCustomerCount() const;

    // Account management
    Account* createAccount(const std::string& customerId,
                                          AccountType type, double initialBalance,
                                          double additionalParam = 0.0);
    Account* findAccount(const std::string& accountId) const;
    void searchAccounts(const std::string& customerId) const;
    AccountList getCustomerAccounts(const std::string& customerId) const;
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

    // Friend functions for external access to private data
    friend void debugBankInfo(const Bank& bank);
    friend CustomerList getAllCustomers(const Bank& bank);
    friend AccountRegistry getAccountRegistry(const Bank& bank);
    friend void forceCloseAccount(Bank& bank, const std::string& accountId);

private:
    // Helper method for finding customer index
    int findCustomerIndex(const std::string& customerId) const;
};

#endif // BANK_H
