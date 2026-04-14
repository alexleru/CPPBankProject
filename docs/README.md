# Bank Account Management System in C++

A C++ project implementing a small bank account management system with support for multiple account types and customer management.

## Project Structure

```
CPPBankProject/
├── include/                          # Header files
│   ├── Constants.h                  # Typed constants
│   ├── Enums.h                      # Enumeration types
│   ├── Globals.h                    # Global variables and counters
│   ├── Utils.h                      # Utility class declarations
│   ├── Customer.h                   # Customer class
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
│   ├── Account.cpp                  # Base Account class implementation
│   ├── SavingsAccount.cpp           # Savings Account implementation
│   ├── CheckingAccount.cpp          # Checking Account implementation
│   ├── LoanAccount.cpp              # Loan Account implementation
│   └── Bank.cpp                     # Bank logic implementation
├── docs/                            # Documentation
│   └── README.md                    # This file
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
- **Vectors**: For storing customers (`typedef std::vector<...>`)
- **Maps**: Fast account lookup by ID (`typedef std::map<std::string, Account*>`)
- **Strings**: All data uses `std::string` for cross-platform compatibility
- **Global State**: Shared counters and configuration via `Globals.h`

### Key Classes

#### Account (Abstract Base)
- Pure virtual method: `getAccountType()`
- Virtual `display()` method
- Balance management

#### SavingsAccount
- Stores interest rate (3.5% default)
- Inherits from Account

#### CheckingAccount
- Supports overdraft limit ($500 default)
- Inherits from Account

#### LoanAccount
- Principal-based balance tracking
- Stores interest rate (8% default) and term in months
- Inherits from Account

#### Customer
- Stores personal info (name, email, phone, address)
- Tracks status (ACTIVE / INACTIVE)
- Can hold up to 5 accounts
- Email and phone validation on `validate()`

#### Bank
- Customer management and registration
- Account creation and lookup via `accountRegistry`
- List customers

### Static Methods
```cpp
Utils::generateCustomerId()     // Unique customer ID (CUST + 6 digits)
Utils::generateAccountId()      // Unique account ID (ACC + 6 digits)
Utils::validateEmail()          // Email format validation
Utils::validatePhone()          // Phone format validation
Utils::accountTypeToString()    // Convert AccountType enum to string
```

### Enums (2 total, C++03 plain enums)
```cpp
enum AccountType    { SAVINGS, CHECKING, LOAN }
enum CustomerStatus { ACTIVE, INACTIVE }
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
    src/main.cpp src/Utils.cpp src/Customer.cpp \
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
1. Create Customer
2. Create Account (Savings/Checking/Loan)
3. List Customers
4. Exit

### Core Functionality

#### Customer Management
- Register customers with email and phone validation
- Track customer status (Active, Inactive)
- Support multiple accounts per customer (max 5)

#### Account Creation
- Savings account with configurable interest rate
- Checking account with configurable overdraft limit
- Loan account with configurable principal, rate, and term

## Code Statistics

- **Total Files**: 19 (10 headers + 9 sources)
- **C++ Standard**: C++03

## Design Patterns

- **Polymorphism**: Treating different accounts through base class interface
- **Encapsulation**: Private member variables with public accessors
- **Separation of Concerns**: Utils, Bank, Customer, Account classes with distinct responsibilities

## Usage Example

```cpp
// Create bank
Bank bank("Small Bank System");

// Register customer
std::string customerId, errorMsg;
bank.registerCustomer("John", "Doe", "john@example.com", "123-456-7890", "123 Main St",
                      customerId, errorMsg);

// Create savings account for customer
std::string err;
Account* account = bank.createAccount(customerId, SAVINGS, 1000.0, 0.0, err);

// List all customers
bank.listCustomers();
```

## Build Requirements

- **C++ Standard**: C++03 or later
- **Build System**: GNU Make
- **Compiler**: GCC 4.x+, Clang, or MinGW
- **Platform**: Windows, Linux, macOS

## Notes

- All currency values are stored as `double`
- Constants defined as typed `const` variables in `Constants.h` (not `#define`)
- Global counters managed through `Globals.h` / `Globals.cpp`
- No external dependencies (pure C++ Standard Library)
- Uses `typedef` for type aliases (C++03 compatible, no `using` aliases)

## Learning Objectives

This project demonstrates:
- Object-Oriented Programming (OOP) principles
- Inheritance and polymorphism
- STL containers (vector, map, string)
- C++03 compatible patterns (raw pointers, plain enums, typedef)
- Console I/O
- Input validation

---

**Version**: 1.0.0
**Date**: April 2026
**Standard**: C++03
**Platform**: Cross-Platform (Windows/Linux/macOS)
