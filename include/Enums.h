#ifndef ENUMS_H
#define ENUMS_H

// Account type enumeration
enum AccountType {
    SAVINGS,
    CHECKING,
    LOAN,
    MORTGAGE
};

// Transaction type enumeration
enum TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER,
    INTEREST,
    FEE,
    EMI_PAYMENT
};

// Transaction status enumeration
enum TransactionStatus {
    PENDING,
    COMPLETED,
    FAILED,
    REVERSED
};

// Customer status enumeration
enum CustomerStatus {
    ACTIVE,
    INACTIVE,
    SUSPENDED,
    CLOSED
};

// Loan status enumeration
enum LoanStatus {
    LOAN_ACTIVE,
    PAID_OFF,
    DEFAULTED,
    PENDING_APPROVAL
};

#endif // ENUMS_H
