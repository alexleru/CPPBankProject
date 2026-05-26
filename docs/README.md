# C++ SCC Demo Project (formerly Bank Account Management System)

A small C++03 codebase engineered to expose a specific dependency-graph
topology for the `java_cpp_chunkagent` plugin (a C++ → Java/Spring
converter). The bank domain is incidental — the **strongly-connected-
component layout** is the deliverable.

For the authoritative SCC description (diagrams, edges, isolation
invariant) see `SCC_DEMO_LAYOUT.md`.

## Project Structure

```
CPPBankProject/
├── include/                            # Headers
│   ├── Constants.h, Enums.h, Globals.h, Utils.h   # Acyclic baseline
│   ├── BondCalculator.h                            # Acyclic baseline
│   ├── TransactionBase.h                           # Acyclic parent of Transaction
│   ├── AgeVerifier.h                               # Acyclic wrapper for native lib
│   │
│   ├── Account.h, Transaction.h                    # SCC A (size 2)
│   │
│   ├── TransactionVisitor.h                        # SCC B (size 5)
│   ├── Deposit.h, Withdrawal.h, Transfer.h, LoanPayment.h
│   ├── LoggingVisitor.h                            # concrete visitor (acyclic)
│   │
│   ├── Bank.h, Customer.h, Loan.h                  # SCC C (size 7)
│   ├── AuditLogger.h, NotificationCenter.h
│   ├── BranchManager.h, RiskAnalyzer.h
│   │
│   ├── ReportEngine.h, ReportFilter.h              # SCC D (size 5, isolated)
│   ├── ReportSection.h, ReportFormatter.h, ReportWriter.h
│
├── src/                                # One .cpp per header (+ main.cpp)
├── native/                             # Cross-platform native library
│   ├── include/age_verifier.h          # C ABI (extern "C")
│   ├── src/age_verifier.cpp            # Shared implementation
│   ├── linux/                          # libage_verifier.so + build.sh
│   └── windows/                        # age_verifier.dll  + build.bat
├── docs/
│   ├── SCC_DEMO_LAYOUT.md              # Load-bearing: diagrams + edges
│   ├── SCC_DEMO_PROJECT_PROMPT.md      # Original spec (Russian)
│   ├── README.md                       # This file
│   ├── DOCUMENTATION_INDEX.md          # Doc nav
│   ├── QUICK_REFERENCE.md              # Class taxonomy + menu
│   ├── TEST_CASES.md                   # Scenario test cases
│   ├── TESTING_GUIDE.md                # Scripted stdin sequences
│   ├── NATIVE_LIBRARY.md               # Native age_verifier lib (menu option 4)
│   └── START_HERE.md                   # Doc orientation
├── Makefile
└── BankSystem                          # built executable
```

## Expected SCC topology

| SCC | Size | Description |
|---|---|---|
| mega-SCC (A ∪ C) | 9 | `Account` ↔ `Transaction` + mediator/observer mesh around `Bank`. Worst-case Tier-C/D in chunker terms. |
| SCC B | 5 | `TransactionVisitor` + 4 concrete transaction types. Visitor double-dispatch, Tier-B algorithmic. |
| SCC D | 5 | `ReportEngine`/`Filter`/`Section`/`Formatter`/`Writer`. Pipeline with back-callback. Tier-B, fully isolated from A/B/C. |

Acyclic baseline (Tier-A): `Utils`, `Globals`, `Constants`, `Enums`,
`BondCalculator`, `LoggingVisitor`, `AgeVerifier`, `TransactionBase`.

## Build & Run

### Using Make (recommended)

```bash
make           # builds the native age_verifier library AND ./BankSystem
               # (-std=c++03 -Wall -Wextra, zero warnings)
./BankSystem
```

### Manual

The native library is a separate shared object loaded at runtime, so it has
to be built before the application — and the application needs `-ldl` on
Linux to resolve `dlopen`/`dlsym`:

```bash
# 1. Build the native library
g++ -std=c++03 -O2 -Wall -fPIC -shared \
    -o native/linux/libage_verifier.so native/src/age_verifier.cpp

# 2. Build the application
g++ -std=c++03 -Wall -Wextra -I./include -o BankSystem src/*.cpp -ldl
```

### Targets

- `make` / `make all` — build native library + executable
- `make native` — build only the native library for the current OS
- `make clean` — remove `build/` and executable; keep the built native lib
- `make clean-all` — also delete the native library binaries

### Cross-platform

Linux (g++), Windows (MinGW). The native `age_verifier` library has
`_WIN32` and `__linux__` branches; other platforms are not supported.

## Interactive menu

```
1. Run new-bank flow             (exercises mega-SCC + SCC B visitor)
2. Generate report               (exercises SCC D pipeline)
3. Calculate bond parameters     (acyclic baseline)
4. Verify age 21+                (native library demo)
0. Exit
```

### Scenario 1 — new-bank flow

Creates a `Bank`, wires up `AuditLogger`, `NotificationCenter`,
`BranchManager`+`RiskAnalyzer`, registers two `Customer`s, opens
`Account`s, runs `Deposit`/`Withdrawal`/`Transfer`/`LoanPayment` through
both `apply()` and a `LoggingVisitor`, approves a `Loan` via the
manager, broadcasts notifications, dumps the audit log + event-type
summary, walks the transaction history through `dynamic_cast` to recover
each concrete subclass and its `TransactionBase` instance ID, calls the
`debugDumpAccount` friend back-door, then `delete bank;` cascades the
entire graph.

### Scenario 2 — report

Builds the 5-class reporting pipeline, runs
`engine->generate(std::cout)`. Tears down via `delete engine;`.

### Scenario 3 — bond calculator

Acyclic standalone module (`BondCalculator.{h,cpp}`). Computes PV/FV
of a bond with random coupon stream. The handler also demonstrates a
nested `try` + bare `throw;` re-throw pattern.

### Scenario 4 — verify age 21+

Acyclic `AgeVerifier` wrapper dynamically loads the native
`age_verifier` library via `dlopen`/`LoadLibrary`, looks up the C-ABI
`verify_age_21` symbol, calls it with three numeric inputs, and prints
`OK` / `UNDER` / `BAD INPUT` / `LIB ERROR`. See
[`NATIVE_LIBRARY.md`](NATIVE_LIBRARY.md) for the ABI and build details.

## Ownership / lifetime contract

`delete bank;` and `delete engine;` cascade through the whole object
graph. Don't introduce smart pointers or change ownership without
updating both the relevant header comments and `SCC_DEMO_LAYOUT.md`.

| Owner | Owns |
|---|---|
| `Bank` | customers, managers, `AuditLogger`, `NotificationCenter` |
| `Customer` | accounts, loans |
| `Account` | transactions |
| `BranchManager` | `RiskAnalyzer` |
| `ReportEngine` | filter, writer |
| `ReportWriter` | formatter |

Every other pointer is non-owning.

## C++03 constraints (hard)

No `nullptr` / `auto` / `using` aliases / smart pointers / `enum class` /
`override` / `final` / `noexcept` / lambdas / `std::function`. Use the
C++03 equivalents already present in the code.

## C++ language constructs exercised

The codebase is engineered to cover the full C++03 surface area so the
chunker / converter has every common construct to translate. Each item
below appears at least once in `include/` or `src/`.

### Preprocessor
- Include guards (`#ifndef` / `#define` / `#endif`) — every header.
- `#include` with quoted form (`"Foo.h"`) and angle-bracket form (`<vector>`).
- Object-like macros (`#define`) — `Constants.h` (`BANK_NAME`, `OVERDRAFT_LIMIT`, …), `BondCalculator.h` (`BOND_MAX_RANDOM_RATE`, `BOND_DEFAULT_DISCOUNT_RATE`).
- Conditional compilation (`#ifdef` / `#else` / `#endif`) — `Constants.h` (`_WIN32`).

### Types
- Built-in: `int`, `double`, `bool`, `char`, `unsigned int`, `unsigned long`, `size_t`, `time_t`.
- `std::string`, `std::vector`, `std::map`.
- Raw C arrays — `char buffer[80]` (`Utils.cpp`), `double sensitivityRates[5]` (`BondCalculator.cpp`).

### Type aliasing (C++03 `typedef`)
- Primitive alias — `typedef double Money;`, `typedef std::string AccountId;` (`Constants.h`).
- Container alias — `typedef std::vector<Transaction*> TransactionHistory;` (`Account.h`).
- Iterator alias — `typedef TransactionHistory::iterator TxHistoryIterator;` (`Account.h`).
- `typedef struct` — `typedef struct BondCashFlow BondCashFlow_t;` (`BondCalculator.h`).
- `typedef enum` — `typedef enum AccountType AccountKind;` (`Enums.h`).
- Function-pointer typedef — `typedef int (*VerifyFn)(int,int,int);` (`AgeVerifier.cpp`).
- Self typedef inside a class — `typedef Account self_type;` (`Account.h`).

### Enums
- Unscoped C++03 enums — `AccountType`, `TransactionType`, `TransactionStatus`, `CustomerStatus`, `LoanStatus` (`Enums.h`).

### Structs
- Plain data aggregates — `struct BondCashFlow { … };` (`BondCalculator.h`).

### Classes & OOP
- Abstract base class with pure virtuals — `TransactionVisitor` (`= 0` on every `visit`).
- Public single inheritance — `Deposit : public Transaction`, etc.
- Multi-level inheritance — `Deposit → Transaction → TransactionBase` (3 levels).
- Virtual destructor — `virtual ~Transaction()`, `virtual ~TransactionBase()`.
- Virtual method overriding — `Transaction::apply` overridden by `Deposit`/`Withdrawal`/`Transfer`/`LoanPayment`.
- Explicit base-class call — `Transaction::apply();` in `LoanPayment::apply()`.
- Access specifiers — `public`, `protected`, `private` (all three across the codebase).
- `const` member functions — every getter, e.g. `Bank::getName() const`.
- Static data members — `Utils::customerIdCounter`, `TransactionBase::instanceCounter`.
- Static member methods — `Utils::formatCurrency`, `TransactionBase::getInstanceCount`.
- Function overloading — `BondCalculator::generateCashFlowSchedule()` / `(unsigned int)`; `Utils::add`/`multiply` + `performOperation`.
- Default arguments — `Utils::getValidatedAmount(prompt, bool allowZero = false)`, `BondCalculator` ctor.
- Constructor member-initializer lists — every class constructor.
- `friend` functions — `debugDumpAccount` / `forceAccountBalance` declared friend in `Account`.
- Forward declarations — `class Customer;` in `Account.h`, `Bank.h`, etc.
- Utility class idiom — `Utils` exposes only static methods.

### Memory management
- `new` / `delete` — throughout `main.cpp`, destructors of `Bank`, `Customer`, `Account`.
- Raw pointers — `Customer*`, `Account*`, `Transaction*` throughout.
- `NULL` (C++03 macro) — never `nullptr`.
- RAII via STL containers — `std::vector<Transaction*>` owned by `Account`, deleted in dtor.

### Type-conversion casts
- `static_cast` — `static_cast<int>(history.size())`, `static_cast<unsigned int>(std::time(NULL))`, etc.
- `dynamic_cast` — `main.cpp::runBankFlow` downcasts `Transaction*` to each concrete subclass.

### Exception handling
- `try` / `catch` / `throw` — `BondCalculator.cpp`, `main.cpp`.
- Standard exception types — `std::invalid_argument`, `std::runtime_error`, base `std::exception`.
- Catch by `const std::exception&`.
- Bare `throw;` re-throw — `main.cpp::runBondCalc` nested catch.

### Control flow
- `if` / `else if` / `else`.
- `switch` / `case` / `default` / `break` — menu loop, `AgeVerifier` result mapping.
- `while` loop — input validation in `Utils.cpp`.
- `for` loop — both index-based (`for (size_t i = 0; …)`) and iterator-based (`for (TxHistoryIterator it = …; …)`).
- `return`, `continue`, ternary `?:`.

### Functions
- Free functions — `main`, `pauseScreen`, `clearScreen`, `initializeGlobals`, friend back-doors.
- Member functions.
- Static methods.
- Function pointers as parameters — `Utils::performOperation(int, int, int(*)(int,int))`.
- `main()` entry point.

### Global state & linkage
- `extern` global variables — `globalCustomerCounter`, `enableDebugLogging`, `logFilePath` (`Globals.h`).
- Global object instances — globals defined in `Globals.cpp`.
- File-scope `static const` constants — `DEFAULT_LIB_PATH` in `AgeVerifier.cpp`.

### Standard library
- I/O streams `<iostream>` — `std::cout`, `std::cerr`, `std::cin`, `std::endl`, stream-state (`cin.clear()`, `cin.ignore`).
- I/O manipulators `<iomanip>` — `std::fixed`, `std::setprecision`, `std::setw`, `std::setfill`, `std::left`.
- String streams `<sstream>` — `std::ostringstream` in `AuditLogger`, `Utils`.
- Strings `<string>` — `find`, `rfind`, `length`, `empty`, `size`, indexed access.
- Vector ops `<vector>` — `push_back`, `clear`, `reserve`, `begin`/`end`, `size`, `back`, `[i]`.
- Map ops `<map>` — `find`, `operator[]`, `const_iterator` iteration in `AuditLogger::dumpEventSummary`.
- Math `<cmath>` — `std::pow`, `std::sqrt` (`BondCalculator`).
- Algorithms `<algorithm>` — `std::max` (`BondCalculator`).
- Time `<ctime>` — `std::time`, `std::localtime`, `std::strftime`.
- Random `<cstdlib>` — `std::rand`, `std::srand`, `RAND_MAX`.
- Limits `<limits>` — `std::numeric_limits<std::streamsize>::max()`.
- Character classification `<cctype>` — `std::isdigit`.
- Exception headers `<stdexcept>` / `<exception>`.
- `system()` calls — `clearScreen()` in `main.cpp`.

### Idiomatic patterns
- Template Method — `Transaction::apply()` overridden per concrete subclass.
- Strategy via function pointers — `Utils::performOperation`.
- Pseudo-singleton global object — `globalBank` style globals in `Globals.cpp`.
- Polymorphism through base-class pointers in a container — `std::vector<Transaction*>`, `std::vector<Customer*>`, etc.
- Friend-function back-doors — `debugDumpAccount`, `forceAccountBalance` in `Account.cpp`.

## Verification

```bash
make clean && make all                              # zero warnings
printf '1\n\n2\n\n0\n' | ./BankSystem               # exercises mega-SCC + SCC D

# SCC D isolation invariant — must produce zero output
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer' \
    include/Report*.h src/Report*.cpp
```

## History

The codebase originally implemented a polymorphic bank account hierarchy
(`Account` abstract, `SavingsAccount`/`CheckingAccount`/`LoanAccount`/
`MortgageAccount` subclasses, 17-option menu). That layout was replaced
wholesale to fit the SCC-demo spec. The replacement preserves
`BondCalculator`, `Utils`, `Globals`, `Constants`, and `Enums` as
acyclic baseline.

---

**Standard**: C++03 · **Build**: `make` · **Run**: `./BankSystem`
