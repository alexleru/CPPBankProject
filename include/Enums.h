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

// C++03 "typedef enum" alias idiom — gives the enum a second name without
// `using` (which is C++11). Useful when the original enum tag is a verb
// and the alias reads as a noun.
typedef enum AccountType   AccountKind;
typedef enum LoanStatus    LoanState;

#endif // ENUMS_H
