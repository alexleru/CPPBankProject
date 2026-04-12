#include "../include/Transaction.h"
#include "../include/Utils.h"
#include <iostream>
#include <iomanip>

// Initialize static counter
int Transaction::transactionCounter = 10000;

// Constructor - creates a new transaction with unique ID
Transaction::Transaction(TransactionType type, double amount, const std::string& description)
    : transactionId(generateTransactionId()),
      type(type),
      amount(amount),
      timestamp(std::time(nullptr)),
      status(TransactionStatus::COMPLETED),
      description(description) {
}

// Private method to generate unique transaction ID
std::string Transaction::generateTransactionId() {
    std::ostringstream oss;
    oss << "TXN" << std::setfill('0') << std::setw(8) << (transactionCounter++);
    return oss.str();
}

// Getters
std::string Transaction::getTransactionId() const {
    return transactionId;
}

TransactionType Transaction::getType() const {
    return type;
}

double Transaction::getAmount() const {
    return amount;
}

time_t Transaction::getTimestamp() const {
    return timestamp;
}

TransactionStatus Transaction::getStatus() const {
    return status;
}

std::string Transaction::getDescription() const {
    return description;
}

// Setters
void Transaction::setStatus(TransactionStatus newStatus) {
    status = newStatus;
}

void Transaction::setDescription(const std::string& newDescription) {
    description = newDescription;
}

// Display transaction details in formatted output
void Transaction::display() const {
    std::cout << std::left << std::setw(15) << transactionId
              << std::setw(15) << Utils::transactionTypeToString(type)
              << std::setw(12) << Utils::formatCurrency(amount)
              << std::setw(20) << Utils::formatDate(timestamp)
              << std::setw(12) << Utils::transactionStatusToString(status)
              << std::setw(20) << description << std::endl;
}
