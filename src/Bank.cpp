#include "../include/Bank.h"
#include "../include/SavingsAccount.h"
#include "../include/CheckingAccount.h"
#include "../include/LoanAccount.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

// Constructor - initializes bank with name
Bank::Bank(const std::string& name) : bankName(name) {
}

// Destructor
Bank::~Bank() {
    accountRegistry.clear();
}

// Register new customer
bool Bank::registerCustomer(const std::string& firstName, const std::string& lastName,
                           const std::string& email, const std::string& phone,
                           const std::string& address) {
    try {
        // Create new customer
        Customer newCustomer(firstName, lastName, email, phone, address);
        
        // Validate customer data
        if (!newCustomer.validate()) {
            std::cerr << "Customer validation failed\n";
            return false;
        }
        
        // Add to customers list
        customers.push_back(newCustomer);
        
        std::cout << "Customer registered successfully!\n";
        std::cout << "Customer ID: " << newCustomer.getCustomerId() << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error during customer registration: " << e.what() << std::endl;
        return false;
    }
}

// Find customer by ID (non-const version)
Customer* Bank::findCustomer(const std::string& customerId) {
    int index = findCustomerIndex(customerId);
    if (index >= 0) {
        return &customers[index];
    }
    return NULL;
}

// Find customer by ID (const version)
const Customer* Bank::findCustomer(const std::string& customerId) const {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].getCustomerId() == customerId) {
            return &customers[i];
        }
    }
    return NULL;
}

// Private helper to find customer index
int Bank::findCustomerIndex(const std::string& customerId) const {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].getCustomerId() == customerId) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// List all customers in the bank
void Bank::listAllCustomers() const {
    if (customers.empty()) {
        std::cout << "No customers registered yet.\n";
        return;
    }
    
    std::cout << "\n===============================================\n";
    std::cout << "ALL CUSTOMERS\n";
    std::cout << "===============================================\n";
    
    std::cout << std::left << std::setw(12) << "Customer ID"
              << std::setw(20) << "Name"
              << std::setw(25) << "Email"
              << std::setw(15) << "Phone"
              << std::setw(10) << "Accounts" << std::endl;
    std::cout << "-----------------------------------------------\n";
    
    for (size_t i = 0; i < customers.size(); ++i) {
        const Customer& customer = customers[i];
        std::cout << std::left << std::setw(12) << customer.getCustomerId()
                  << std::setw(20) << (customer.getFirstName() + " " + customer.getLastName())
                  << std::setw(25) << customer.getEmail()
                  << std::setw(15) << customer.getPhoneNumber()
                  << std::setw(10) << customer.getAccountCount() << std::endl;
    }
    
    std::cout << "===============================================\n";
    std::cout << "Total Customers: " << customers.size() << std::endl;
}

// Get number of customers
int Bank::getCustomerCount() const {
    return customers.size();
}

// Create new account for customer
Account* Bank::createAccount(const std::string& customerId,
                                            AccountType type, double initialBalance,
                                            double additionalParam) {
    try {
        // Find customer
        Customer* customer = findCustomer(customerId);
        if (!customer) {
            throw std::runtime_error("Customer not found");
        }
        
        // Check if customer can add more accounts
        if (!customer->canAddAccount()) {
            throw std::runtime_error("Customer has reached maximum accounts limit");
        }
        
        // Validate balance
        if (initialBalance < 0) {
            throw std::invalid_argument("Initial balance cannot be negative");
        }
        
        // Create appropriate account type
        Account* newAccount = NULL;
        
        switch (type) {
            case SAVINGS: {
                double rate = (additionalParam > 0) ? additionalParam : SAVINGS_INTEREST_RATE;
                newAccount = new SavingsAccount(initialBalance, rate);
                break;
            }
            case CHECKING: {
                double overdraft = (additionalParam > 0) ? additionalParam : OVERDRAFT_LIMIT;
                newAccount = new CheckingAccount(initialBalance, overdraft);
                break;
            }
            case LOAN: {
                double rate = (additionalParam > 0) ? additionalParam : LOAN_INTEREST_RATE;
                int term = 12; // Default 12 months
                newAccount = new LoanAccount(initialBalance, rate, term);
                break;
            }
            default:
                throw std::invalid_argument("Unknown account type");
        }
        
        // Add account to customer
        customer->addAccount(newAccount);
        
        // Register in bank's account registry
        accountRegistry[newAccount->getAccountId()] = newAccount;
        
        std::cout << "Account created successfully!\n";
        std::cout << "Account ID: " << newAccount->getAccountId() << std::endl;
        std::cout << "Account Type: " << Utils::accountTypeToString(type) << std::endl;
        
        return newAccount;
        
    } catch (const std::exception& e) {
        std::cerr << "Error creating account: " << e.what() << std::endl;
        return NULL;
    }
}

// Find account by ID
Account* Bank::findAccount(const std::string& accountId) const {
    AccountRegistry::const_iterator it = accountRegistry.find(accountId);
    if (it != accountRegistry.end()) {
        return it->second;
    }
    return NULL;
}

// Search and display customer's accounts
void Bank::searchAccounts(const std::string& customerId) const {
    const Customer* customer = findCustomer(customerId);
    if (!customer) {
        std::cerr << "Customer not found\n";
        return;
    }
    
    customer->displayPortfolio();
}

// Get all accounts for a customer
AccountList Bank::getCustomerAccounts(const std::string& customerId) const {
    AccountList result;
    
    const Customer* customer = findCustomer(customerId);
    if (!customer) {
        return result;
    }
    
    for (int i = 0; i < customer->getAccountCount(); ++i) {
        Account* account = customer->getAccount(i);
        if (account) {
            result.push_back(account);
        }
    }
    
    return result;
}

// List all accounts in bank
void Bank::listAllAccounts() const {
    if (accountRegistry.empty()) {
        std::cout << "No accounts exist in the bank.\n";
        return;
    }
    
    std::cout << "\n===============================================\n";
    std::cout << "ALL ACCOUNTS\n";
    std::cout << "===============================================\n";
    
    std::cout << std::left << std::setw(12) << "Account ID"
              << std::setw(12) << "Type"
              << std::setw(15) << "Balance"
              << std::setw(10) << "Status" << std::endl;
    std::cout << "-----------------------------------------------\n";
    
    double totalBalance = 0.0;
    
    for (AccountRegistry::const_iterator it = accountRegistry.begin(); it != accountRegistry.end(); ++it) {
        Account* account = it->second;
        if (account) {
            std::cout << std::left << std::setw(12) << account->getAccountId()
                      << std::setw(12) << Utils::accountTypeToString(account->getAccountType())
                      << std::setw(15) << Utils::formatCurrency(account->getBalance())
                      << std::setw(10) << (account->getIsActive() ? "Active" : "Inactive") << std::endl;
            totalBalance += account->getBalance();
        }
    }
    
    std::cout << "===============================================\n";
    std::cout << "Total Accounts: " << accountRegistry.size() << std::endl;
    std::cout << "Total Balance: " << Utils::formatCurrency(totalBalance) << std::endl;
}

// Deposit to account
bool Bank::depositToAccount(const std::string& accountId, double amount) {
    try {
        Account* account = findAccount(accountId);
        if (!account) {
            throw std::runtime_error("Account not found");
        }
        
        account->deposit(amount, "Deposit via Bank");
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Deposit failed: " << e.what() << std::endl;
        return false;
    }
}

// Withdraw from account
bool Bank::withdrawFromAccount(const std::string& accountId, double amount) {
    try {
        Account* account = findAccount(accountId);
        if (!account) {
            throw std::runtime_error("Account not found");
        }
        
        return account->withdraw(amount, "Withdrawal via Bank");
        
    } catch (const std::exception& e) {
        std::cerr << "Withdrawal failed: " << e.what() << std::endl;
        return false;
    }
}

// Transfer funds between accounts
bool Bank::transferBetweenAccounts(const std::string& fromAccountId,
                                  const std::string& toAccountId, double amount) {
    try {
        Account* fromAccount = findAccount(fromAccountId);
        Account* toAccount = findAccount(toAccountId);
        
        if (!fromAccount || !toAccount) {
            throw std::runtime_error("One or both accounts not found");
        }
        
        // Withdraw from source
        if (!fromAccount->withdraw(amount, "Transfer Out")) {
            return false;
        }
        
        // Deposit to destination
        try {
            toAccount->deposit(amount, "Transfer In");
            std::cout << "Transfer successful from " << fromAccountId << " to " << toAccountId << std::endl;
            return true;
        } catch (const std::exception& e) {
            // Reverse transfer if deposit fails
            fromAccount->deposit(amount, "Transfer Reversal");
            throw;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Transfer failed: " << e.what() << std::endl;
        return false;
    }
}

// Generate report for specific customer
void Bank::generateCustomerReport(const std::string& customerId) const {
    const Customer* customer = findCustomer(customerId);
    if (!customer) {
        std::cerr << "Customer not found\n";
        return;
    }
    
    customer->displayPortfolio();
}

// Generate bank-wide report
void Bank::generateBankReport() const {
    std::cout << "\n";
    std::cout << "===============================================\n";
    std::cout << "BANK REPORT - " << bankName << "\n";
    std::cout << "===============================================\n";
    std::cout << "Report Generated: " << Utils::formatDate(std::time(NULL)) << std::endl;
    std::cout << "Total Customers: " << customers.size() << std::endl;
    std::cout << "Total Accounts: " << accountRegistry.size() << std::endl;
    
    double totalBalance = 0.0;
    for (AccountRegistry::const_iterator it = accountRegistry.begin(); it != accountRegistry.end(); ++it) {
        Account* account = it->second;
        if (account) {
            totalBalance += account->getBalance();
        }
    }
    
    std::cout << "Total Assets Under Management: " << Utils::formatCurrency(totalBalance) << std::endl;
    std::cout << "===============================================\n\n";
}

// Apply a caller-supplied rule to every active account's balance.
// Demonstrates passing a function pointer as a parameter.
//   Parameter syntax: double (*rule)(double)
// Example callers might pass: "add 1% bonus", "apply flat fee", etc.
void Bank::applyToAllAccounts(double (*rule)(double)) {
    if (rule == NULL) {
        std::cerr << "applyToAllAccounts: null rule function pointer\n";
        return;
    }

    std::cout << "\nApplying custom rule to all active accounts...\n";
    int touched = 0;

    for (AccountRegistry::iterator it = accountRegistry.begin();
         it != accountRegistry.end(); ++it) {
        Account* account = it->second;
        if (account && account->getIsActive()) {
            double oldBalance = account->getBalance();
            double newBalance = rule(oldBalance);
            if (newBalance < 0) {
                // Rule produced an invalid balance - skip this account.
                std::cerr << "  Skipping " << account->getAccountId()
                          << " (rule produced negative balance)\n";
                continue;
            }
            // forceBalanceUpdate is a friend of Account declared in Account.h
            forceBalanceUpdate(*account, newBalance);
            ++touched;
        }
    }

    std::cout << "Custom rule applied to " << touched << " account(s).\n";
}

// Apply monthly processing to all accounts
void Bank::applyMonthlyProcessing() {
    std::cout << "\nApplying monthly processing to all accounts...\n";
    
    for (AccountRegistry::iterator it = accountRegistry.begin(); it != accountRegistry.end(); ++it) {
        Account* account = it->second;
        if (account) {
            account->applyMonthlyProcessing();
        }
    }
    
    std::cout << "Monthly processing completed.\n";
}

// Display account statement
void Bank::displayAccountStatement(const std::string& accountId) const {
    Account* account = findAccount(accountId);
    if (!account) {
        std::cerr << "Account not found\n";
        return;
    }
    
    account->printStatement();
}

// Calculate loan EMI
double Bank::calculateLoanEMI(double principal, double rate, int months) const {
    if (months <= 0 || principal <= 0) {
        throw std::invalid_argument("Principal and months must be positive");
    }
    
    return LoanAccount::calculateEMI(principal, rate, months);
}

// Display bank information
void Bank::displayBankInfo() const {
    std::cout << "\n";
    std::cout << "===============================================\n";
    std::cout << "BANK INFORMATION\n";
    std::cout << "Bank Name: " << bankName << std::endl;
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << "Total Customers: " << customers.size() << std::endl;
    std::cout << "Total Accounts: " << accountRegistry.size() << std::endl;
    std::cout << "===============================================\n";
}

// Friend function implementations

// Debug function to access private bank information
void debugBankInfo(const Bank& bank) {
    std::cout << "\n=== DEBUG BANK INFO ===\n";
    std::cout << "Bank Name: " << bank.bankName << std::endl;
    std::cout << "Customer Count: " << bank.customers.size() << std::endl;
    std::cout << "Account Registry Size: " << bank.accountRegistry.size() << std::endl;
    std::cout << "=======================\n";
}

// Friend function to get all customers
CustomerList getAllCustomers(const Bank& bank) {
    return bank.customers;
}

// Friend function to get account registry
AccountRegistry getAccountRegistry(const Bank& bank) {
    return bank.accountRegistry;
}

// Friend function to force close an account (administrative function)
void forceCloseAccount(Bank& bank, const std::string& accountId) {
    AccountRegistry::iterator it = bank.accountRegistry.find(accountId);
    if (it != bank.accountRegistry.end() && it->second) {
        it->second->setIsActive(false);
        std::cout << "Account " << accountId << " has been forcibly closed.\n";
    } else {
        std::cerr << "Account " << accountId << " not found.\n";
    }
}
