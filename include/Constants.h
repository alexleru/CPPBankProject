#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Primitive aliases used across SCC A/C
typedef double      Money;
typedef std::string AccountId;
typedef std::string LoanId;
typedef std::string CustomerId;

// Bank Information
#define BANK_NAME "National C++ Bank"
#define VERSION "1.0.0"

// Account Constraints
#define MAX_ACCOUNTS_PER_CUSTOMER 5
#define MIN_SAVINGS_BALANCE 100.0
#define OVERDRAFT_LIMIT 500.0
#define DEFAULT_INTEREST_RATE 0.035
#define MAX_LOAN_AMOUNT 100000.0

// Processing Constants
#define SAVINGS_INTEREST_RATE 0.035
#define CHECKING_INTEREST_RATE 0.001
#define MONTHLY_CHECKING_FEE 5.0
#define LOAN_INTEREST_RATE 0.08

// ID Generation
#define CUSTOMER_ID_PREFIX "CUST"
#define ACCOUNT_ID_PREFIX "ACC"

// Platform-specific
#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
    #define PLATFORM_NAME "Windows"
#else
    #define CLEAR_SCREEN "clear"
    #define PLATFORM_NAME "Linux/Unix"
#endif

#endif // CONSTANTS_H
