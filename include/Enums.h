#ifndef ENUMS_H
#define ENUMS_H

// Account type enumeration
enum class AccountType {
    SAVINGS,
    CHECKING,
    LOAN
};

// Transaction type enumeration
enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER,
    INTEREST,
    FEE,
    EMI_PAYMENT
};

// Transaction status enumeration
enum class TransactionStatus {
    PENDING,
    COMPLETED,
    FAILED,
    REVERSED
};

// Customer status enumeration
enum class CustomerStatus {
    ACTIVE,
    INACTIVE,
    SUSPENDED,
    CLOSED
};

// Loan status enumeration
enum class LoanStatus {
    ACTIVE,
    PAID_OFF,
    DEFAULTED,
    PENDING_APPROVAL
};

#endif // ENUMS_H
