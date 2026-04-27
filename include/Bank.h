#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>
#include <map>
#include "Customer.h"
#include "Account.h"
#include "MortgageAccount.h"

typedef std::vector<Customer> CustomerList;
typedef std::map<std::string, Account*> AccountRegistry;
typedef std::vector<Account*> AccountList;

class Bank {
private:
    std::string bankName;
    CustomerList customers;
    AccountRegistry accountRegistry;

public:
    Bank(const std::string& name);
    ~Bank();

    bool registerCustomer(const std::string& firstName, const std::string& lastName,
                         const std::string& email, const std::string& phone,
                         const std::string& address);
    Customer* findCustomer(const std::string& customerId);
    const Customer* findCustomer(const std::string& customerId) const;
    void listAllCustomers() const;
    int getCustomerCount() const;

    Account* createAccount(const std::string& customerId,
                           AccountType type, double initialBalance,
                           double additionalParam = 0.0);
    Account* findAccount(const std::string& accountId) const;
    void searchAccounts(const std::string& customerId) const;
    AccountList getCustomerAccounts(const std::string& customerId) const;
    void listAllAccounts() const;

    bool depositToAccount(const std::string& accountId, double amount);
    bool withdrawFromAccount(const std::string& accountId, double amount);
    bool transferBetweenAccounts(const std::string& fromAccountId,
                                const std::string& toAccountId, double amount);

    void generateCustomerReport(const std::string& customerId) const;
    void generateBankReport() const;
    void applyMonthlyProcessing();
    void displayAccountStatement(const std::string& accountId) const;

    void applyToAllAccounts(double (*rule)(double));

    double calculateLoanEMI(double principal, double rate, int months) const;

    // Mortgage operations - joins Mortgage with Loan account hierarchy.
    MortgageAccount* createMortgageAccount(const std::string& customerId,
                                           const Property&    property,
                                           Rate               annualRate,
                                           TermInYears        years,
                                           MortgageKind       kind = FIXED_RATE);
    MortgageAccount* findMortgageAccount(const std::string& accountId) const;

    void displayBankInfo() const;

    friend void debugBankInfo(const Bank& bank);
    friend CustomerList getAllCustomers(const Bank& bank);
    friend AccountRegistry getAccountRegistry(const Bank& bank);
    friend void forceCloseAccount(Bank& bank, const std::string& accountId);

private:
    int findCustomerIndex(const std::string& customerId) const;
};

#endif
