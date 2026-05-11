# Bank Customer Management System in C++

A small C++ project implementing a bank customer management system with
registration, validation, and listing. The codebase intentionally exercises
a wide spread of classic C++03 syntactic constructs (typedefs, struct/enum
aliases, container & iterator aliases, function-pointer typedef, self-typedef,
two-level inheritance, default arguments).

## Project Structure

```
CPPBankProject/
├── include/                          # Header files
│   ├── Constants.h                  # Typed constants + CustomerId typedef
│   ├── Enums.h                      # typedef enum CustomerStatus_ { ... } CustomerStatus
│   ├── Globals.h                    # CustomerCounter typedef + global counter
│   ├── Utils.h                      # Static utility class + BinaryIntOp typedef
│   ├── Customer.h                   # Entity -> Person -> Customer + ContactInfo struct
│   ├── Bank.h                       # CustomerList / CustomerIter aliases
│   └── AgeVerifier.h                # Dynamic-loading wrapper for the native lib
├── src/                             # Implementation files
│   ├── main.cpp                     # Entry point with interactive menu
│   ├── Globals.cpp                  # Global variable definitions
│   ├── Utils.cpp                    # Utility function implementations
│   ├── Customer.cpp                 # Customer class implementation
│   ├── Bank.cpp                     # Bank logic implementation
│   └── AgeVerifier.cpp              # LoadLibrary/dlopen + symbol resolution
├── native/                          # Cross-platform native library
│   ├── include/age_verifier.h       # Public C ABI (extern "C")
│   ├── src/age_verifier.cpp         # Shared implementation
│   ├── windows/                     # age_verifier.dll lives here at runtime
│   │   └── build.bat                # Standalone Windows build script
│   └── linux/                       # libage_verifier.so lives here at runtime
│       └── build.sh                 # Standalone Linux build script
├── docs/                            # Documentation
│   ├── README.md                    # This file
│   ├── NATIVE_LIBRARY.md            # Native library architecture & build
│   ├── QUICK_REFERENCE.md           # Menu / ABI / validation cheatsheet
│   ├── TEST_CASES.md                # Human-readable test cases
│   ├── TESTING_GUIDE.md             # Manual scenarios + checklist
│   └── TC_SPEC.md                   # Machine-readable spec for run_tests.py
├── build/                           # Build artifacts (created by make)
│   └── *.o                         # Object files
├── BankSystem(.exe)                 # Compiled executable (.exe on Windows)
├── run_tests.py                     # Automated harness (reads docs/TC_SPEC.md)
└── Makefile                         # Cross-platform build (GNU make / mingw32-make)
```

## C++03 Language Constructs Showcased

| Construct                              | Where it lives                                              |
|----------------------------------------|-------------------------------------------------------------|
| Semantic primitive typedef             | `Constants.h` (`CustomerId`), `Globals.h` (`CustomerCounter`) |
| Struct alias (`typedef struct`)        | `Customer.h` (`ContactInfo`)                                |
| Enum alias (`typedef enum`)            | `Enums.h` (`CustomerStatus`)                                |
| Container alias                        | `Bank.h` (`CustomerList = std::vector<Customer::Ptr>`)      |
| Iterator alias                         | `Bank.h` (`CustomerIter = CustomerList::const_iterator`)    |
| Function-pointer typedef               | `Utils.h` (`BinaryIntOp = int (*)(int, int)`)               |
| Self-typedef inside a class            | `Customer.h` (`typedef Customer Self; typedef Self* Ptr;`)  |
| Two-level inheritance                  | `Entity` → `Person` → `Customer`                            |
| Default arguments                      | `initializeGlobals(CustomerCounter = 1000)`, `Bank::registerCustomer(..., CustomerStatus = ACTIVE)`, `Customer` ctor |
| Function-pointer parameter via typedef | `Utils::performOperation(int, int, BinaryIntOp)`            |

## Technical Features

### Object-Oriented Design
- **Encapsulation**: Private member variables with public accessors
- **Inheritance**: Two-level chain `Entity` → `Person` → `Customer` with a virtual destructor on the base
- **Validation**: Input validation on customer registration
- **Memory Management**: Raw pointers with manual cleanup in destructor

### Data Structures
- **Vectors via alias**: `typedef std::vector<Customer::Ptr> CustomerList;`
- **Iterator via alias**: `typedef CustomerList::const_iterator CustomerIter;`
- **Strings**: All textual data uses `std::string` for cross-platform compatibility
- **Global State**: Shared counter (`CustomerCounter globalCustomerCounter`) via `Globals.h`

### Key Classes

#### Entity (base)
- Holds the `CustomerId customerId` member, virtual destructor, `getCustomerId()` accessor

#### Person : public Entity
- Adds `firstName` / `lastName` and accessors

#### Customer : public Person
- Declares `typedef Customer Self;` and `typedef Self* Ptr;` (self-typedef)
- Stores a `ContactInfo` struct (email, phone, address) and `CustomerStatus`
- Constructor takes a default `CustomerStatus status = ACTIVE`
- `validate()` checks required fields, email and phone format
- `display()` prints the full record

#### Bank
- Owns a `CustomerList customers` (destructor frees all entries via `CustomerIter`)
- `registerCustomer()` — validates and adds a customer, default `status = ACTIVE`
- `listCustomers()` — iterates with `CustomerIter` and prints each customer
- `getBankName()` — returns the bank name

#### Utils (static methods only)
- `generateCustomerId()` returns a `CustomerId` (semantic typedef of `std::string`)
- `validateEmail()` / `validatePhone()` — format checks
- `add(int, int)` / `multiply(int, int)` — sample callbacks of type `BinaryIntOp`
- `performOperation(int, int, BinaryIntOp)` — accepts a callback through the typedef

### Static Methods
```cpp
Utils::generateCustomerId()        // "CUST001000", "CUST001001", ...
Utils::validateEmail()             // Email format check
Utils::validatePhone()             // Phone format check (min 10 chars)
Utils::add(a, b)                   // Sample callback: a + b
Utils::multiply(a, b)              // Sample callback: a * b
Utils::performOperation(x, y, fn)  // Calls fn(x, y) and prints the result
```

### Function-Pointer Typedef and Parameter

`Utils.h` defines the alias

```cpp
typedef int (*BinaryIntOp)(int, int);
```

`Utils::performOperation` then takes that alias as its parameter type:

```cpp
static void performOperation(int x, int y, BinaryIntOp operation);
```

Any function that matches `int(int, int)` can be supplied as the callback:

```cpp
Utils::performOperation(5, 3, Utils::add);      // Result: 8
Utils::performOperation(5, 3, Utils::multiply); // Result: 15
```

### Enum Alias (C-style)
```cpp
typedef enum CustomerStatus_ {
    ACTIVE,
    INACTIVE
} CustomerStatus;
```

### Struct Alias (C-style)
```cpp
typedef struct ContactInfo_ {
    std::string email;
    std::string phone;
    std::string address;
} ContactInfo;
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
4. Verify Age (21+, native library)
5. Exit
```

### Core Functionality

#### Customer Management
- Register customers with first name, last name, and a `ContactInfo` struct
  (email, phone, address). The `Bank::registerCustomer` signature uses a
  default argument for status (`CustomerStatus status = ACTIVE`).
- Email and phone validation on registration
- Auto-generated unique customer ID (`CUST001000`, `CUST001001`, ...)
- List all registered customers with their details and status

#### Function Pointer Demo
- Option 3 exercises `Utils::performOperation`, which receives a callback
  through the `BinaryIntOp` typedef
- Passes `Utils::add` and `Utils::multiply` as callbacks
- Prints `Result: 8` and `Result: 15`

#### Native Age Verification (21+)
- Option 4 loads a small cross-platform shared library at runtime
  (`native/windows/age_verifier.dll` on Windows, `native/linux/libage_verifier.so`
  on Linux) via `LoadLibrary` / `dlopen`
- The host wrapper [`AgeVerifier`](../include/AgeVerifier.h) picks the
  binary at compile time using `_WIN32` / `__linux__`
- Reads numeric `day / month / year`, compares against the system date,
  prints `Result: TRUE` (≥ 21), `Result: FALSE` (< 21), or
  `Invalid date` for impossible calendar combinations
- See [NATIVE_LIBRARY.md](NATIVE_LIBRARY.md) for the ABI, build steps,
  and extension guidance

## Code Statistics

- **C++ Standard**: C++03
- Application: 6 headers + 6 sources under `include/` and `src/`
- Native library: 1 header + 1 source under `native/` plus per-OS build scripts

## Design Patterns

- **Encapsulation**: Private member variables with public accessors
- **Inheritance**: Two-level chain (`Entity` → `Person` → `Customer`)
- **Separation of Concerns**: `Utils`, `Bank`, `Customer` classes with distinct responsibilities
- **Static Utility Class**: `Utils` with private constructor — not instantiable

## Usage Example

```cpp
// Create bank
Bank bank("Small Bank System");

// Build the contact info struct (typedef struct ContactInfo_ ... ContactInfo)
ContactInfo info;
info.email   = "john@example.com";
info.phone   = "123-456-7890";
info.address = "123 Main St";

// Register a customer (CustomerStatus defaults to ACTIVE)
CustomerId  customerId;
std::string errorMsg;
if (bank.registerCustomer("John", "Doe", info, customerId, errorMsg))
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
- `CustomerId` is a semantic `typedef` of `std::string` (not a distinct type)
- Global counter managed through `Globals.h` / `Globals.cpp` and uses the
  `CustomerCounter` typedef
- No external dependencies (pure C++ Standard Library)
- Uses raw pointers with manual memory management (C++03 style)

## Learning Objectives

This project demonstrates:
- Object-Oriented Programming (OOP) principles
- Encapsulation and class design
- Two-level inheritance with a virtual destructor
- STL containers (`vector`, `string`) and STL iterators
- C++03 compatible patterns (raw pointers, plain enums, `typedef`)
- Semantic primitive, struct, enum, container, iterator, function-pointer,
  and self-typedefs
- Function pointers as parameters declared via a typedef (`BinaryIntOp`)
- Default function arguments
- Console I/O and input validation
- Cross-platform build with a single Makefile (GNU make / MinGW `mingw32-make`)

---

**Version**: 2.1.0
**Date**: April 2026
**Standard**: C++03
**Platform**: Cross-Platform (Windows/Linux/macOS)
