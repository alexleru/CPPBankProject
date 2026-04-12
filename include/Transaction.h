#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include "Enums.h"

// Transaction class to represent a single transaction
class Transaction {
private:
    std::string transactionId;
    TransactionType type;
    double amount;
    time_t timestamp;
    TransactionStatus status;
    std::string description;

public:
    // Constructor
    Transaction(TransactionType type, double amount, const std::string& description);

    // Getters
    std::string getTransactionId() const;
    TransactionType getType() const;
    double getAmount() const;
    time_t getTimestamp() const;
    TransactionStatus getStatus() const;
    std::string getDescription() const;

    // Setters
    void setStatus(TransactionStatus newStatus);
    void setDescription(const std::string& newDescription);

    // Display
    void display() const;

private:
    // Private method for generating transaction ID
    static std::string generateTransactionId();
    static int transactionCounter;
};

#endif // TRANSACTION_H
