#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include "Enums.h"
#include "Transaction.h"

// Type aliases for better readability
typedef std::vector<Transaction> TransactionHistory;

// Abstract base class for all account types
class Account {
protected:
    std::string accountId;
    AccountType type;
    double balance;
    TransactionHistory transactionHistory;
    time_t createdDate;
    time_t lastModifiedDate;
    bool isActive;

public:
    // Constructor
    Account(AccountType accountType, double initialBalance);

    // Destructor
    virtual ~Account();

    // Pure virtual methods
    virtual AccountType getAccountType() const = 0;
    virtual void applyMonthlyProcessing() = 0;

    // Virtual methods
    virtual void deposit(double amount, const std::string& description = "Deposit");
    virtual bool withdraw(double amount, const std::string& description = "Withdrawal");
    virtual void printStatement() const;

    // Getters
    std::string getAccountId() const;
    double getBalance() const;
    bool getIsActive() const;
    time_t getCreatedDate() const;
    const TransactionHistory& getTransactionHistory() const;

    // Setters
    void setIsActive(bool active);

    // Static methods
    static double getInterestForType(AccountType type);
    static std::string typeToString(AccountType type);

    // Transaction management
    void addTransaction(const Transaction& transaction);

    // Display account information
    virtual void displayAccountInfo() const;

    // Friend functions for external access to private data
    friend void debugAccountInfo(const Account& account);
    friend bool validateAccountBalance(const Account& account);
    friend void forceBalanceUpdate(Account& account, double newBalance);
    friend TransactionHistory getAccountTransactions(const Account& account);

protected:
    // Protected method to format balance display
    std::string formatBalance() const;
};

#endif // ACCOUNT_H
