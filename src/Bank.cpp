#include "../include/Bank.h"
#include "../include/SavingsAccount.h"
#include "../include/CheckingAccount.h"
#include "../include/LoanAccount.h"
#include "../include/MortgageAccount.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

Bank::Bank(const std::string& name) : bankName(name) {}
Bank::~Bank() { accountRegistry.clear(); }

bool Bank::registerCustomer(const std::string& firstName, const std::string& lastName,
                           const std::string& email, const std::string& phone,
                           const std::string& address) {
    try {
        Customer newCustomer(firstName, lastName, email, phone, address);
        if (!newCustomer.validate()) {
            std::cerr << "Customer validation failed\n";
            return false;
        }
        customers.push_back(newCustomer);
        std::cout << "Customer registered successfully!\n";
        std::cout << "Customer ID: " << newCustomer.getCustomerId() << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

Customer* Bank::findCustomer(const std::string& customerId) {
    int index = findCustomerIndex(customerId);
    if (index >= 0) return &customers[index];
    return NULL;
}

const Customer* Bank::findCustomer(const std::string& customerId) const {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].getCustomerId() == customerId) return &customers[i];
    }
    return NULL;
}

int Bank::findCustomerIndex(const std::string& customerId) const {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].getCustomerId() == customerId) return static_cast<int>(i);
    }
    return -1;
}

void Bank::listAllCustomers() const {
    if (customers.empty()) { std::cout << "No customers registered yet.\n"; return; }
    std::cout << "\n===============================================\n";
    std::cout << "ALL CUSTOMERS\n";
    std::cout << "===============================================\n";
    for (size_t i = 0; i < customers.size(); ++i) {
        const Customer& c = customers[i];
        std::cout << c.getCustomerId() << " " << c.getFirstName() << " " << c.getLastName()
                  << " accounts=" << c.getAccountCount() << std::endl;
    }
    std::cout << "Total Customers: " << customers.size() << std::endl;
}

int Bank::getCustomerCount() const { return customers.size(); }

Account* Bank::createAccount(const std::string& customerId, AccountType type,
                             double initialBalance, double additionalParam) {
    try {
        Customer* customer = findCustomer(customerId);
        if (!customer) throw std::runtime_error("Customer not found");
        if (!customer->canAddAccount()) throw std::runtime_error("Max accounts reached");
        if (initialBalance < 0) throw std::invalid_argument("Initial balance cannot be negative");
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
                int term = 12;
                newAccount = new LoanAccount(initialBalance, rate, term);
                break;
            }
            case MORTGAGE: {
                Rate rate = (additionalParam > 0) ? additionalParam : LOAN_INTEREST_RATE;
                Property prop;
                prop.id = 0;
                prop.address = "Unspecified Property";
                prop.marketValue = initialBalance;
                prop.downPayment = initialBalance * 0.20;
                TermInYears years = 30;
                newAccount = new MortgageAccount(prop, rate, years, FIXED_RATE);
                break;
            }
            default:
                throw std::invalid_argument("Unknown account type");
        }
        customer->addAccount(newAccount);
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

Account* Bank::findAccount(const std::string& accountId) const {
    AccountRegistry::const_iterator it = accountRegistry.find(accountId);
    if (it != accountRegistry.end()) return it->second;
    return NULL;
}

void Bank::searchAccounts(const std::string& customerId) const {
    const Customer* c = findCustomer(customerId);
    if (!c) { std::cerr << "Customer not found\n"; return; }
    c->displayPortfolio();
}

AccountList Bank::getCustomerAccounts(const std::string& customerId) const {
    AccountList result;
    const Customer* c = findCustomer(customerId);
    if (!c) return result;
    for (int i = 0; i < c->getAccountCount(); ++i) {
        Account* a = c->getAccount(i);
        if (a) result.push_back(a);
    }
    return result;
}

void Bank::listAllAccounts() const {
    if (accountRegistry.empty()) { std::cout << "No accounts.\n"; return; }
    std::cout << "\n=== ALL ACCOUNTS ===\n";
    double totalBalance = 0.0;
    for (AccountRegistry::const_iterator it = accountRegistry.begin();
         it != accountRegistry.end(); ++it) {
        Account* a = it->second;
        if (a) {
            std::cout << a->getAccountId() << " | " << Utils::accountTypeToString(a->getAccountType())
                      << " | " << Utils::formatCurrency(a->getBalance())
                      << " | " << (a->getIsActive() ? "Active" : "Inactive") << "\n";
            totalBalance += a->getBalance();
        }
    }
    std::cout << "Total Accounts: " << accountRegistry.size() << "  Total Balance: "
              << Utils::formatCurrency(totalBalance) << std::endl;
}

bool Bank::depositToAccount(const std::string& accountId, double amount) {
    try {
        Account* a = findAccount(accountId);
        if (!a) throw std::runtime_error("Account not found");
        a->deposit(amount, "Deposit via Bank");
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Deposit failed: " << e.what() << std::endl;
        return false;
    }
}

bool Bank::withdrawFromAccount(const std::string& accountId, double amount) {
    try {
        Account* a = findAccount(accountId);
        if (!a) throw std::runtime_error("Account not found");
        return a->withdraw(amount, "Withdrawal via Bank");
    } catch (const std::exception& e) {
        std::cerr << "Withdrawal failed: " << e.what() << std::endl;
        return false;
    }
}

bool Bank::transferBetweenAccounts(const std::string& fromAccountId,
                                  const std::string& toAccountId, double amount) {
    try {
        Account* from = findAccount(fromAccountId);
        Account* to   = findAccount(toAccountId);
        if (!from || !to) throw std::runtime_error("Account(s) not found");
        if (!from->withdraw(amount, "Transfer Out")) return false;
        try {
            to->deposit(amount, "Transfer In");
            std::cout << "Transfer successful from " << fromAccountId << " to " << toAccountId << "\n";
            return true;
        } catch (const std::exception& e) {
            from->deposit(amount, "Transfer Reversal");
            throw;
        }
    } catch (const std::exception& e) {
        std::cerr << "Transfer failed: " << e.what() << std::endl;
        return false;
    }
}

void Bank::generateCustomerReport(const std::string& customerId) const {
    const Customer* c = findCustomer(customerId);
    if (!c) { std::cerr << "Customer not found\n"; return; }
    c->displayPortfolio();
}

void Bank::generateBankReport() const {
    std::cout << "\n=== BANK REPORT - " << bankName << " ===\n";
    std::cout << "Total Customers: " << customers.size() << std::endl;
    std::cout << "Total Accounts: " << accountRegistry.size() << std::endl;
    double totalBalance = 0.0;
    for (AccountRegistry::const_iterator it = accountRegistry.begin();
         it != accountRegistry.end(); ++it) {
        if (it->second) totalBalance += it->second->getBalance();
    }
    std::cout << "Total Assets: " << Utils::formatCurrency(totalBalance) << std::endl;
}

void Bank::applyToAllAccounts(double (*rule)(double)) {
    if (rule == NULL) { std::cerr << "applyToAllAccounts: null rule\n"; return; }
    int touched = 0;
    for (AccountRegistry::iterator it = accountRegistry.begin(); it != accountRegistry.end(); ++it) {
        Account* a = it->second;
        if (a && a->getIsActive()) {
            double oldB = a->getBalance();
            double newB = rule(oldB);
            if (newB < 0) continue;
            forceBalanceUpdate(*a, newB);
            ++touched;
        }
    }
    std::cout << "Custom rule applied to " << touched << " account(s).\n";
}

void Bank::applyMonthlyProcessing() {
    std::cout << "\nApplying monthly processing...\n";
    for (AccountRegistry::iterator it = accountRegistry.begin(); it != accountRegistry.end(); ++it) {
        if (it->second) it->second->applyMonthlyProcessing();
    }
    std::cout << "Monthly processing completed.\n";
}

void Bank::displayAccountStatement(const std::string& accountId) const {
    Account* a = findAccount(accountId);
    if (!a) { std::cerr << "Account not found\n"; return; }
    a->printStatement();
}

double Bank::calculateLoanEMI(double principal, double rate, int months) const {
    if (months <= 0 || principal <= 0) throw std::invalid_argument("Invalid input");
    return LoanAccount::calculateEMI(principal, rate, months);
}

// =====================================================================
// Mortgage operations - joins Mortgage feature with Loan account hierarchy
// =====================================================================
MortgageAccount* Bank::createMortgageAccount(const std::string& customerId,
                                             const Property&    property,
                                             Rate               annualRate,
                                             TermInYears        years,
                                             MortgageKind       kind) {
    try {
        Customer* customer = findCustomer(customerId);
        if (!customer) throw std::runtime_error("Customer not found");
        if (!customer->canAddAccount())
            throw std::runtime_error("Customer has reached maximum accounts limit");
        MortgageAccount* m = new MortgageAccount(property, annualRate, years, kind);
        customer->addAccount(m);
        accountRegistry[m->getAccountId()] = m;
        std::cout << "Mortgage account created successfully!\n";
        std::cout << "Account ID: " << m->getAccountId() << std::endl;
        std::cout << "Property  : " << property.address << std::endl;
        std::cout << "Principal : " << Utils::formatCurrency(
                MortgageAccount::computeLoanPrincipal(property)) << std::endl;
        return m;
    } catch (const std::exception& e) {
        std::cerr << "Error creating mortgage account: " << e.what() << std::endl;
        return NULL;
    }
}

MortgageAccount* Bank::findMortgageAccount(const std::string& accountId) const {
    Account* a = findAccount(accountId);
    if (!a) return NULL;
    return dynamic_cast<MortgageAccount*>(a);
}

void Bank::displayBankInfo() const {
    std::cout << "\n=== BANK INFO ===\n";
    std::cout << "Bank Name: " << bankName << std::endl;
    std::cout << "Total Customers: " << customers.size() << std::endl;
    std::cout << "Total Accounts: " << accountRegistry.size() << std::endl;
}

void debugBankInfo(const Bank& bank) {
    std::cout << "\n=== DEBUG BANK INFO ===\n";
    std::cout << "Bank Name: " << bank.bankName << std::endl;
    std::cout << "Customer Count: " << bank.customers.size() << std::endl;
    std::cout << "Account Registry Size: " << bank.accountRegistry.size() << std::endl;
}

CustomerList getAllCustomers(const Bank& bank) { return bank.customers; }
AccountRegistry getAccountRegistry(const Bank& bank) { return bank.accountRegistry; }

void forceCloseAccount(Bank& bank, const std::string& accountId) {
    AccountRegistry::iterator it = bank.accountRegistry.find(accountId);
    if (it != bank.accountRegistry.end() && it->second) {
        it->second->setIsActive(false);
        std::cout << "Account " << accountId << " closed.\n";
    } else {
        std::cerr << "Account " << accountId << " not found.\n";
    }
}
