#ifndef ENUMS_H
#define ENUMS_H

// Account type enumeration
namespace AccountTypeEnum {
    enum AccountType {
        SAVINGS,
        CHECKING,
        LOAN
    };
}
typedef AccountTypeEnum::AccountType AccountType;

// Transaction type enumeration
namespace TransactionTypeEnum {
    enum TransactionType {
        DEPOSIT,
        WITHDRAWAL,
        TRANSFER,
        INTEREST,
        FEE,
        EMI_PAYMENT
    };
}
typedef TransactionTypeEnum::TransactionType TransactionType;

// Transaction status enumeration
namespace TransactionStatusEnum {
    enum TransactionStatus {
        PENDING,
        COMPLETED,
        FAILED,
        REVERSED
    };
}
typedef TransactionStatusEnum::TransactionStatus TransactionStatus;

// Customer status enumeration
namespace CustomerStatusEnum {
    enum CustomerStatus {
        ACTIVE,
        INACTIVE,
        SUSPENDED,
        CLOSED
    };
}
typedef CustomerStatusEnum::CustomerStatus CustomerStatus;

// Loan status enumeration
namespace LoanStatusEnum {
    enum LoanStatus {
        ACTIVE,
        PAID_OFF,
        DEFAULTED,
        PENDING_APPROVAL
    };
}
typedef LoanStatusEnum::LoanStatus LoanStatus;

#endif // ENUMS_H
