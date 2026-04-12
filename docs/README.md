# Bank Account Management System in C++

A comprehensive, production-style C++ project implementing a complete bank account management system with support for multiple account types, customer management, and transaction processing.

## 📁 Project Structure

```
CPPBankProject/
├── include/                          # Header files
│   ├── Constants.h                  # #define macros and constants
│   ├── Enums.h                      # All enumeration types
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
│   ├── Utils.cpp                    # Utility function implementations
│   ├── Customer.cpp                 # Customer class implementation
│   ├── Transaction.cpp              # Transaction class implementation
│   ├── Account.cpp                  # Base Account class implementation
│   ├── SavingsAccount.cpp           # Savings Account implementation
│   ├── CheckingAccount.cpp          # Checking Account implementation
│   ├── LoanAccount.cpp              # Loan Account implementation
│   └── Bank.cpp                     # Bank logic implementation
├── build/                           # Build artifacts (created by make)
│   ├── *.o                         # Object files
│   └── BankSystem                  # Executable (also in root)
├── Makefile                         # Build configuration
└── README.md                        # This file
```

## ⚙️ Technical Features

### Object-Oriented Design
- **Abstract Base Class**: `Account` with pure virtual methods
- **Inheritance Hierarchy**: SavingsAccount, CheckingAccount, LoanAccount inherit from Account
- **Polymorphism**: Accounts use virtual methods for type-specific behavior
- **Memory Management**: Smart pointers (`std::shared_ptr`) for automatic memory management

### Data Structures
- **Vectors**: For storing customers and transaction history
- **Maps**: Fast account lookup by ID
- **Strings**: All data uses `std::string` for cross-platform compatibility

### Key Classes

#### Account (Abstract Base)
- Pure virtual methods: `getAccountType()`, `applyMonthlyProcessing()`
- Transaction history tracking
- Balance management and validation

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
- Account creation and lookups
- Transaction processing (deposits, withdrawals, transfers)
- Monthly processing (interest, fees)
- Reporting and analytics

### Static Methods (8 total)
```cpp
Utils::formatCurrency()         // Format: $1,234.56
Utils::formatDate()             // Format: YYYY-MM-DD HH:MM:SS
Utils::generateCustomerId()     // Unique customer ID
Utils::generateAccountId()      // Unique account ID
Utils::validateEmail()          // Email format validation
Utils::validatePhone()          // Phone format validation
LoanAccount::calculateEMI()     // EMI calculation
Account::getInterestForType()   // Type-based interest rate
```

### Enums (5 total)
```cpp
enum class AccountType { SAVINGS, CHECKING, LOAN }
enum class TransactionType { DEPOSIT, WITHDRAWAL, TRANSFER, INTEREST, FEE, EMI_PAYMENT }
enum class TransactionStatus { PENDING, COMPLETED, FAILED, REVERSED }
enum class CustomerStatus { ACTIVE, INACTIVE, SUSPENDED, CLOSED }
enum class LoanStatus { ACTIVE, PAID_OFF, DEFAULTED, PENDING_APPROVAL }
```

## 🚀 Compilation & Build

### Using Make (Recommended)
```bash
cd CPPBankProject
make
./BankSystem
```

### Using g++ (Manual)
```bash
cd CPPBankProject
g++ -std=c++17 -Wall -Wextra -pedantic -I./include \
    -o BankSystem \
    src/main.cpp src/Utils.cpp src/Customer.cpp src/Transaction.cpp \
    src/Account.cpp src/SavingsAccount.cpp src/CheckingAccount.cpp \
    src/LoanAccount.cpp src/Bank.cpp
./BankSystem
```

### Build Targets
- `make` or `make all`: Build the project
- `make clean`: Clean build artifacts

### Cross-Platform Support
- ✅ Linux (GCC)
- ✅ Windows (MinGW/MSVC)
- ✅ macOS (Clang)
- Uses cross-platform headers only: `<iostream>`, `<string>`, `<vector>`, `<map>`, etc.
- Platform detection for console clearing
- Static linking for C++ standard library compatibility

## 🏦 Features & Menus

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

## 📏 Code Statistics

- **Total Lines**: 1500+
- **Number of Files**: 15
- **Bank.cpp**: 280+ lines
- **main.cpp**: 400+ lines
- **Account.cpp**: 120+ lines
- All `.cpp` files: 60-150+ lines each

## 🎯 Exception Handling

- Input validation with try-catch blocks
- Meaningful error messages for invalid operations
- Transaction reversal on failed transfers
- Insufficient balance detection
- Overdraft validation
- Minimum balance enforcement

## 💡 Design Patterns

- **Abstract Factory**: Creating different account types
- **Polymorphism**: Treating different accounts through base class interface
- **Encapsulation**: Private member variables with public accessors
- **RAII**: Smart pointers for automatic memory cleanup
- **Separation of Concerns**: Utils, Bank, Account classes with distinct responsibilities

## 📚 Usage Example

```cpp
// Create bank
Bank bank("National C++ Bank");

// Register customer
bank.registerCustomer("John", "Doe", "john@example.com", "123-456-7890", "123 Main St");

// Create savings account for customer
auto account = bank.createAccount("CUST000001", AccountType::SAVINGS, 1000.0);

// Perform transactions
bank.depositToAccount("ACC002000", 500.0);
bank.withdrawFromAccount("ACC002000", 200.0);

// Apply monthly processing
bank.applyMonthlyProcessing();

// Generate reports
bank.generateBankReport();
```

## 🔧 Build Requirements

- **C++ Standard**: C++17 or later
- **Build System**: GNU Make
- **Compiler**: GCC 7+, Clang 5+, or MSVC 2017+
- **Platform**: Windows, Linux, macOS

## 📝 Notes

- All currency values are stored as `double`
- Timestamps use `time_t` from `<ctime>`
- Transaction history is persistent within program execution
- No external dependencies (pure C++ Standard Library)
- Fully cross-platform compatible

## 🎓 Learning Objectives

This project demonstrates:
- Object-Oriented Programming (OOP) principles
- Inheritance and polymorphism
- Exception handling
- STL containers (vector, map, string)
- File and console I/O
- Date/time handling
- Financial calculations
- State management
- System design patterns

---

**Version**: 1.0.0  
**Date**: April 2026  
**Platform**: Cross-Platform (Windows/Linux/macOS)
