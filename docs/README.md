# Bank Account Management System in C++

A comprehensive, production-style C++ project implementing a complete bank account management system with support for multiple account types, customer management, and transaction processing.

## Project Structure

```
CPPBankProject/
├── include/                          # Header files
│   ├── Constants.h                  # #define macros and constants
│   ├── Enums.h                      # All enumeration types
│   ├── Globals.h                    # Global variables and counters
│   ├── Utils.h                      # Utility class declarations
│   ├── Customer.h                   # Customer class
│   ├── Transaction.h                # Transaction class
│   ├── Account.h                    # Abstract base Account class
│   ├── SavingsAccount.h             # Savings Account derived class
│   ├── CheckingAccount.h            # Checking Account derived class
│   ├── LoanAccount.h                # Loan Account derived class
│   └── Bank.h                       # Bank management class
├── src/                             # Implementation files
│   ├── main.cpp                     # Entry point with interactive menu
│   ├── Globals.cpp                  # Global variable definitions
│   ├── Utils.cpp                    # Utility function implementations
│   ├── Customer.cpp                 # Customer class implementation
│   ├── Transaction.cpp              # Transaction class implementation
│   ├── Account.cpp                  # Base Account class implementation
│   ├── SavingsAccount.cpp           # Savings Account implementation
│   ├── CheckingAccount.cpp          # Checking Account implementation
│   ├── LoanAccount.cpp              # Loan Account implementation
│   └── Bank.cpp                     # Bank logic implementation
├── docs/                            # Documentation
│   ├── README.md                    # This file
│   ├── DOCUMENTATION_INDEX.md       # Index of all documentation
│   ├── QUICK_REFERENCE.md           # Quick reference card
│   ├── START_HERE.md                # Getting started guide
│   ├── TEST_CASES.md                # Test case descriptions
│   └── TESTING_GUIDE.md             # Testing instructions
├── build/                           # Build artifacts (created by make)
│   └── *.o                         # Object files
├── BankSystem                       # Compiled executable
└── Makefile                         # Build configuration
```

## Technical Features

### Object-Oriented Design
- **Abstract Base Class**: `Account` with pure virtual methods
- **Inheritance Hierarchy**: SavingsAccount, CheckingAccount, LoanAccount inherit from Account
- **Polymorphism**: Accounts use virtual methods for type-specific behavior
- **Memory Management**: Raw pointers with manual cleanup in destructors

### Data Structures
- **Vectors**: For storing customers and transaction history (`typedef std::vector<...>`)
- **Maps**: Fast account lookup by ID (`typedef std::map<std::string, Account*>`)
- **Strings**: All data uses `std::string` for cross-platform compatibility
- **Global State**: Shared counters and configuration via `Globals.h`

### Key Classes

#### Account (Abstract Base)
- Pure virtual methods: `getAccountType()`, `applyMonthlyProcessing()`
- Transaction history tracking
- Balance management and validation
- Friend functions for privileged external access

#### SavingsAccount
- Enforces minimum balance requirement ($100)
- Applies monthly interest (3.5% default)
- Prevents withdrawal if minimum balance would be violated

#### CheckingAccount
- Supports overdraft limit ($500 default)
- Monthly maintenance fee ($5)
- Allows negative balance within overdraft limit
- Transaction counting

#### LoanAccount
- Principal-based balance tracking
- Automatic EMI (Equated Monthly Installment) calculation
- Repayment schedule generation
- Loan status tracking

#### Bank
- Customer management and registration
- Account creation and lookups via `AccountRegistry`
- Transaction processing (deposits, withdrawals, transfers)
- Monthly processing (interest, fees)
- Reporting and analytics
- `applyToAllAccounts(double (*rule)(double))` — applies a caller-supplied
  function pointer to every active account's balance (bonus, fee, custom rule).
  Demonstrates the C++ strategy-pattern-via-function-pointer idiom; maps to
  `java.util.function.DoubleUnaryOperator` in the Java port.

### Static Methods (11 total)
```cpp
Utils::formatCurrency()         // Format: $1234.56
Utils::formatDate()             // Format: YYYY-MM-DD HH:MM:SS
Utils::generateCustomerId()     // Unique customer ID
Utils::generateAccountId()      // Unique account ID
Utils::validateEmail()          // Email format validation
Utils::validatePhone()          // Phone format validation
Utils::add(int, int)            // Function-pointer demo helper
Utils::multiply(int, int)       // Function-pointer demo helper
Utils::performOperation(int, int, int (*)(int, int))  // Takes a function pointer
LoanAccount::calculateEMI()     // EMI calculation
Account::getInterestForType()   // Type-based interest rate
```

### Enums (5 total, C++03 plain enums)
```cpp
enum AccountType     { SAVINGS, CHECKING, LOAN }
enum TransactionType { DEPOSIT, WITHDRAWAL, TRANSFER, INTEREST, FEE, EMI_PAYMENT }
enum TransactionStatus { PENDING, COMPLETED, FAILED, REVERSED }
enum CustomerStatus  { ACTIVE, INACTIVE, SUSPENDED, CLOSED }
enum LoanStatus      { LOAN_ACTIVE, PAID_OFF, DEFAULTED, PENDING_APPROVAL }
```

## Compilation & Build

### Using Make (Recommended)
```bash
cd CPPBankProject
make
./BankSystem
```

### Using g++ (Manual)
```bash
cd CPPBankProject
g++ -std=c++03 -I./include \
    -o BankSystem \
    src/main.cpp src/Utils.cpp src/Customer.cpp src/Transaction.cpp \
    src/Account.cpp src/SavingsAccount.cpp src/CheckingAccount.cpp \
    src/LoanAccount.cpp src/Globals.cpp src/Bank.cpp
./BankSystem
```

### Build Targets
- `make` or `make all`: Build the project
- `make clean`: Clean build artifacts

### Cross-Platform Support
- Linux (GCC)
- Windows (MinGW)
- macOS (Clang)
- Uses cross-platform standard headers only: `<iostream>`, `<string>`, `<vector>`, `<map>`, etc.

## Features & Menus

### Main Menu Options
1. Register New Customer
2. Open New Account (Savings/Checking/Loan)
3. Deposit Funds
4. Withdraw Funds
5. Transfer Between Accounts
6. View Account Statement
7. View Customer Portfolio
8. Apply Monthly Processing (interest + fees)
9. Calculate Loan EMI
10. Search Account by ID
11. List All Customers
12. List All Accounts
13. Generate Bank Report
14. Close Account
15. Admin/Debug Functions
16. Calculate Bond Parameters
17. Function Pointer Demo
0. Exit

### Core Functionality

#### Customer Management
- Register customers with email and phone validation
- Track customer status (Active, Inactive, Suspended, Closed)
- Support multiple accounts per customer (max 5)

#### Account Operations
- Deposit with automatic transaction recording
- Withdraw with validation and exception handling
- Transfer between accounts with automatic reversal on failure
- Account statements with formatted transaction history

#### Interest & Fees
- Automatic interest calculation for savings accounts
- Monthly maintenance fee for checking accounts
- Monthly processing applies all charges uniformly

#### Loan Management
- EMI calculation using standard financial formula
- Automatic repayment schedule generation
- Payment tracking and loan status updates
- Detailed schedule display with principal/interest breakdown

#### Function Pointer Demo (menu option 17)
- Illustrates passing a function as a parameter in C++
- `Utils::performOperation(int, int, int (*)(int, int))` dispatches to
  `Utils::add` or `Utils::multiply` (mirrors the canonical C++ example:
  `Result: 8`, `Result: 15`)
- `Bank::applyToAllAccounts(double (*rule)(double))` walks every active
  account and applies a caller-supplied rule (e.g. `applyOnePercentBonus`,
  `applyFlatMaintenanceFee` defined in `main.cpp`)
- Included to document the C++ idiom that will be modernised to Java
  functional interfaces (`IntBinaryOperator`, `DoubleUnaryOperator`,
  `BiFunction`)

## Code Statistics

- **Total Lines**: 1500+
- **Number of Files**: 20 (10 headers + 10 sources)
- **Bank.cpp**: 280+ lines
- **main.cpp**: 400+ lines
- **Account.cpp**: 120+ lines
- All `.cpp` files: 60-150+ lines each

## Exception Handling

- Input validation with try-catch blocks
- Meaningful error messages for invalid operations
- Transaction reversal on failed transfers
- Insufficient balance detection
- Overdraft validation
- Minimum balance enforcement

## Design Patterns

- **Abstract Factory**: Creating different account types
- **Polymorphism**: Treating different accounts through base class interface
- **Encapsulation**: Private member variables with public accessors
- **Separation of Concerns**: Utils, Bank, Account classes with distinct responsibilities
- **Friend Functions**: Controlled privileged access to private members

## Usage Example

```cpp
// Create bank
Bank bank("National C++ Bank");

// Register customer
bank.registerCustomer("John", "Doe", "john@example.com", "123-456-7890", "123 Main St");

// Create savings account for customer
Account* account = bank.createAccount("CUST001000", SAVINGS, 1000.0);

// Perform transactions
bank.depositToAccount("ACC002000", 500.0);
bank.withdrawFromAccount("ACC002000", 200.0);

// Apply monthly processing
bank.applyMonthlyProcessing();

// Generate reports
bank.generateBankReport();
```

## Build Requirements

- **C++ Standard**: C++03 or later
- **Build System**: GNU Make
- **Compiler**: GCC 4.x+, Clang, or MinGW
- **Platform**: Windows, Linux, macOS

## Notes

- All currency values are stored as `double`
- Timestamps use `time_t` from `<ctime>`
- Transaction history is persistent within program execution
- No external dependencies (pure C++ Standard Library)
- Uses `typedef` for type aliases (C++03 compatible, no `using` aliases)
- Global state managed through `Globals.h` / `Globals.cpp`

## Learning Objectives

This project demonstrates:
- Object-Oriented Programming (OOP) principles
- Inheritance and polymorphism
- Exception handling
- STL containers (vector, map, string)
- C++03 compatible patterns (raw pointers, plain enums, typedef)
- Console I/O
- Date/time handling
- Financial calculations
- State management
- Function pointers as parameters (strategy pattern in C++)

---

**Version**: 1.0.0  
**Date**: April 2026  
**Standard**: C++03  
**Platform**: Cross-Platform (Windows/Linux/macOS)
