# Bank Customer Management System in C++

A small C++ project implementing a bank customer management system with registration, validation, and listing.

## Project Structure

```
CPPBankProject/
├── include/                          # Header files
│   ├── Constants.h                  # Typed constants
│   ├── Enums.h                      # Enumeration types
│   ├── Globals.h                    # Global variables and counters
│   ├── Utils.h                      # Utility class declarations
│   ├── Customer.h                   # Customer class
│   └── Bank.h                       # Bank management class
├── src/                             # Implementation files
│   ├── main.cpp                     # Entry point with interactive menu
│   ├── Globals.cpp                  # Global variable definitions
│   ├── Utils.cpp                    # Utility function implementations
│   ├── Customer.cpp                 # Customer class implementation
│   └── Bank.cpp                     # Bank logic implementation
├── docs/                            # Documentation
│   └── README.md                    # This file
├── build/                           # Build artifacts (created by make)
│   └── *.o                         # Object files
├── BankSystem(.exe)                 # Compiled executable (.exe on Windows)
└── Makefile                         # Cross-platform build (GNU make / mingw32-make)
```

## Technical Features

### Object-Oriented Design
- **Encapsulation**: Private member variables with public accessors
- **Validation**: Input validation on customer registration
- **Memory Management**: Raw pointers with manual cleanup in destructor

### Data Structures
- **Vectors**: For storing customers (`std::vector<Customer*>`)
- **Strings**: All data uses `std::string` for cross-platform compatibility
- **Global State**: Shared counters and configuration via `Globals.h`

### Key Classes

#### Customer
- Stores personal info (name, email, phone, address)
- Tracks status (`ACTIVE` / `INACTIVE`)
- ID auto-generated via `Utils::generateCustomerId()`
- Email and phone validation on `validate()`

#### Bank
- Owns a `std::vector<Customer*>` (destructor frees all)
- `registerCustomer()` — validates and adds a customer
- `listCustomers()` — prints all registered customers
- `getBankName()` — returns the bank name

#### Utils (static methods only)
- `generateCustomerId()` — generates unique `CUST` + 6-digit ID
- `validateEmail()` — checks `@` and domain with TLD
- `validatePhone()` — checks minimum 10 characters, digits/symbols only
- `add(int, int)` / `multiply(int, int)` — sample arithmetic functions used by the function-pointer demo
- `performOperation(int, int, int (*)(int, int))` — accepts a function pointer and applies it to two integers

### Static Methods
```cpp
Utils::generateCustomerId()        // "CUST001000", "CUST001001", ...
Utils::validateEmail()             // Email format check
Utils::validatePhone()             // Phone format check (min 10 chars)
Utils::add(a, b)                   // Sample callback: a + b
Utils::multiply(a, b)              // Sample callback: a * b
Utils::performOperation(x, y, fn)  // Calls fn(x, y) and prints the result
```

### Function Pointers as Parameters

`Utils::performOperation` demonstrates passing a function pointer as a parameter
(`int (*operation)(int, int)`). Any function that matches this signature can be
supplied as the callback:

```cpp
Utils::performOperation(5, 3, Utils::add);      // Result: 8
Utils::performOperation(5, 3, Utils::multiply); // Result: 15
```

### Enums (1 total, C++03 plain enum)
```cpp
enum CustomerStatus { ACTIVE, INACTIVE }
```

## Compilation & Build

The `Makefile` detects the host OS through the `OS` environment variable and
adjusts the commands accordingly. On Linux/macOS it uses `mkdir -p` / `rm -rf`;
on Windows (where `OS=Windows_NT`) it uses `mkdir` / `rmdir /S /Q` / `del /Q`
and produces `BankSystem.exe`.

### Linux / macOS (GNU make)
```bash
cd CPPBankProject
make
./BankSystem
```

### Windows (MinGW, cmd.exe)
```cmd
cd CPPBankProject
mingw32-make
BankSystem.exe
```

### Using g++ (Manual)
```bash
cd CPPBankProject
g++ -std=c++03 -I./include \
    -o BankSystem \
    src/main.cpp src/Utils.cpp src/Customer.cpp \
    src/Globals.cpp src/Bank.cpp
./BankSystem
```

### Build Targets
- `make` / `mingw32-make`: Build the project
- `make clean` / `mingw32-make clean`: Remove build artifacts and executable

### Cross-Platform Support
- Linux (GCC) — `make`
- Windows (MinGW) — `mingw32-make` (produces `BankSystem.exe`)
- macOS (Clang) — `make`
- Uses standard headers only: `<iostream>`, `<string>`, `<vector>`, etc.

## Features & Menu

### Main Menu Options
```
1. Create Customer
2. List Customers
3. Function Pointer Demo
4. Exit
```

### Core Functionality

#### Customer Management
- Register customers with first name, last name, email, phone, address
- Email and phone validation on registration
- Auto-generated unique customer ID (`CUST001000`, `CUST001001`, ...)
- List all registered customers with their details and status

#### Function Pointer Demo
- Option 3 exercises `Utils::performOperation`, which takes a function pointer
  as a parameter (`int (*operation)(int, int)`)
- Passes `Utils::add` and `Utils::multiply` as callbacks
- Prints `Result: 8` and `Result: 15`

## Code Statistics

- **Total Files**: 11 (6 headers + 5 sources)
- **Total Lines**: ~278
- **C++ Standard**: C++03

## Design Patterns

- **Encapsulation**: Private member variables with public accessors
- **Separation of Concerns**: Utils, Bank, Customer classes with distinct responsibilities
- **Static Utility Class**: `Utils` with private constructor — not instantiable

## Usage Example

```cpp
// Create bank
Bank bank("Small Bank System");

// Register a customer
std::string customerId, errorMsg;
if (bank.registerCustomer("John", "Doe", "john@example.com",
                          "123-456-7890", "123 Main St",
                          customerId, errorMsg))
    std::cout << "ID: " << customerId << "\n"; // CUST001000

// List all customers
bank.listCustomers();
```

## Build Requirements

- **C++ Standard**: C++03 or later
- **Build System**: GNU Make
- **Compiler**: GCC 4.x+, Clang, or MinGW
- **Platform**: Windows, Linux, macOS

## Notes

- Constants defined as typed `const` variables in `Constants.h` (not `#define`)
- Global counter managed through `Globals.h` / `Globals.cpp`
- No external dependencies (pure C++ Standard Library)
- Uses raw pointers with manual memory management (C++03 style)

## Learning Objectives

This project demonstrates:
- Object-Oriented Programming (OOP) principles
- Encapsulation and class design
- STL containers (`vector`, `string`)
- C++03 compatible patterns (raw pointers, plain enums)
- Console I/O and input validation
- Function pointers as parameters (`int (*operation)(int, int)`)
- Cross-platform build with a single Makefile (GNU make / MinGW `mingw32-make`)

---

**Version**: 2.0.0
**Date**: April 2026
**Standard**: C++03
**Platform**: Cross-Platform (Windows/Linux/macOS)
