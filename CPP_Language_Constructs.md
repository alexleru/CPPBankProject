# C++ Language Constructs Used in CPPBankProject

The codebase is intentionally written to the **C++03** standard (no
`nullptr`, no `using` aliases, no `auto`, no range-based `for`, no smart
pointers, no `enum class`, no lambdas, no `std::function`). That
constraint is enforced by the build flags `-std=c++03 -Wall -Wextra`
(zero warnings required) and is called out in `CLAUDE.md`.

This file is an inventory of every C++03 construct used somewhere in
`include/`, `src/` and `native/`. The shorter `docs/README.md` carries
the same list as part of the project overview; this file goes into
more detail and is meant for grepping when the question is "is X
idiomatic here? where is X already used?".

---

## 1. Preprocessor

- Include guards `#ifndef / #define / #endif` in every header.
- Quoted vs. angle-bracket includes (`#include "Foo.h"` vs.
  `#include <vector>`).
- Object-like macros for constants — `Constants.h` (`BANK_NAME`,
  `OVERDRAFT_LIMIT`, `CLEAR_SCREEN`, …) and `BondCalculator.h`
  (`BOND_MAX_RANDOM_RATE`, `BOND_DEFAULT_DISCOUNT_RATE`).
- Conditional compilation `#ifdef _WIN32 / #else / #endif` — selects
  `cls` vs. `clear` in `Constants.h`; selects `LoadLibrary` vs.
  `dlopen` in `src/AgeVerifier.cpp` and `native/include/age_verifier.h`;
  also `#if defined(_WIN32) / __linux__ / #else #error` to fail-fast
  on unsupported platforms.

## 2. Built-in & Standard Types

`int`, `double`, `bool`, `char`, `unsigned int`, `unsigned long`,
`size_t`, `time_t`, `std::string`, `std::vector<T>`, `std::map<K,V>`,
raw C arrays (`char buffer[80]` in `Utils.cpp`, `double
sensitivityRates[5]` in `BondCalculator.cpp`).

## 3. `typedef` Aliases (the C++03-only alias mechanism)

- **Primitive domain aliases** — `Money`, `AccountId`, `LoanId`,
  `CustomerId` (`Constants.h`).
- **Container alias** — `typedef std::vector<Transaction*>
  TransactionHistory;` (`Account.h`).
- **Iterator aliases** — `typedef TransactionHistory::iterator
  TxHistoryIterator;` and `typedef TransactionHistory::const_iterator
  TxHistoryConstIterator;` (`Account.h`).
- **`typedef struct` C-style alias** — `typedef struct BondCashFlow
  BondCashFlow_t;` (`BondCalculator.h`).
- **`typedef enum` aliases** — `typedef enum AccountType AccountKind;`
  and `typedef enum LoanStatus LoanState;` (`Enums.h`).
- **Function-pointer typedef** — `typedef int (*VerifyFn)(int, int,
  int);` in the anonymous namespace of `src/AgeVerifier.cpp`.
- **"Self typedef" inside a class** — `typedef Account self_type;`
  (`Account.h`), the STL/Boost idiom for exposing the wrapped type.

## 4. Enums (Unscoped, C++03 Style)

`AccountType`, `TransactionType`, `TransactionStatus`, `CustomerStatus`,
`LoanStatus` — all in `Enums.h`.

## 5. Structs

`BondCashFlow` (`BondCalculator.h`) — plain data aggregate (also
exposed via the `typedef struct` idiom).

## 6. Classes & OOP Features

- Abstract base class with pure virtual methods — `TransactionVisitor`
  (`virtual void visit(Deposit*) = 0;` etc.).
- Public single inheritance — `Deposit : public Transaction`,
  `Withdrawal : public Transaction`, `Transfer : public Transaction`,
  `LoanPayment : public Transaction`; concrete `LoggingVisitor : public
  TransactionVisitor`.
- **Multi-level inheritance (3 levels)** — `Deposit / Withdrawal /
  Transfer / LoanPayment → Transaction → TransactionBase`. `TransactionBase`
  is an acyclic root that owns a process-wide `unsigned long
  instanceCounter`.
- Virtual destructor on every polymorphic base — `virtual
  ~Transaction()`, `virtual ~TransactionBase()`, `virtual
  ~TransactionVisitor()`, `virtual ~ReportFilter()` etc.
- Virtual method overriding — `Transaction::apply()` overridden by
  each concrete subclass; `accept(TransactionVisitor&)` implemented
  per concrete subclass.
- Explicit base-class call — `Transaction::apply();` from
  `LoanPayment::apply()` (also forwards to `Loan::recordPayment`).
- Access specifiers — `public`, `protected`, `private` (e.g.
  `protected: Account* source; Account* dest;` on `Transaction`).
- `const` member functions — every getter, e.g. `Bank::getName() const`,
  `Account::getId() const`, `TransactionBase::getInstanceId() const`.
- Static data members — `TransactionBase::instanceCounter`,
  `Utils::customerIdCounter` (defined in `.cpp`).
- Static member methods — `Utils::formatCurrency`,
  `Utils::performOperation`, `TransactionBase::getInstanceCount`.
- Function overloading — `BondCalculator::generateCashFlowSchedule()`
  vs. `(unsigned int seed)`; `Utils::add` / `Utils::multiply` plus
  `Utils::performOperation`.
- Default arguments — `Utils::getValidatedAmount(prompt, bool
  allowZero = false)`; `BondCalculator` ctor has multiple defaults.
- Constructor member-initializer lists in every class
  (`Transaction::Transaction(...) : TransactionBase(), source(...),
  dest(...), amount(...), sequence(0), timestamp(std::time(NULL)) {}`).
- `friend` functions to expose private state for debug/admin —
  `debugDumpAccount` and `forceAccountBalance`, declared friend in
  `Account` and defined as free functions in `Account.cpp`.
- Forward declarations — `class Customer;` in `Account.h`, the four
  concrete subclasses forward-declared in `TransactionVisitor.h`, etc.
- "Utility class" idiom — `Utils` exposes only static methods.

## 7. Memory Management

- `new` / `delete` with raw pointers — throughout `main.cpp`, in
  `Bank::~Bank`, `Customer::~Customer`, `Account::~Account`,
  `BranchManager::~BranchManager`, `ReportEngine::~ReportEngine`,
  `ReportWriter::~ReportWriter`.
- C-style `NULL` rather than `nullptr`.
- Container-managed RAII for `std::vector` members.

## 8. Type-Conversion Casts

- `static_cast<int>(history.size())`,
  `static_cast<unsigned int>(std::time(NULL))`,
  `static_cast<void*>(h)` (in `AgeVerifier`),
  `static_cast<double>(...)` etc.
- `dynamic_cast` — `main.cpp::runBankFlow` walks
  `aliceChk->getHistory()` and downcasts each `Transaction*` to
  `Deposit* / Withdrawal* / Transfer* / LoanPayment*` to recover the
  concrete subclass type (the safe RTTI-based downcast).
- `reinterpret_cast<void*>(p)` — once, in `AgeVerifier.cpp` to convert
  a `FARPROC` returned by `GetProcAddress` to `void*` for storage.

## 9. Exception Handling

- `try` / `catch` / `throw` — `BondCalculator.cpp`, `main.cpp`,
  `Account::debit/credit` for overdraft etc.
- Standard exception types — `std::invalid_argument`,
  `std::runtime_error`, base `std::exception`.
- Catch by `const std::exception&` (slicing-safe).
- **Bare `throw;` re-throw** — `main.cpp::runBondCalc` catches
  `std::invalid_argument` in an inner block, logs it, then re-throws
  via `throw;` to the outer `catch (const std::exception&)`. Preserves
  the dynamic type of the exception object.

## 10. Control Flow

- `if` / `else if` / `else`; `switch / case / default / break`;
  `while`, `for` (classic index form `for (size_t i = 0; …)` and
  iterator form `for (TxHistoryConstIterator it = hist.begin(); …;
  ++it)`); `return`, `continue`.
- Ternary `?:` — e.g. `(allowZero ? amount >= 0 : amount > 0)` in
  `Utils.cpp`, `holder ? "<set>" : "<null>"` in `debugDumpAccount`.
- No range-based `for` (deliberate, C++03).

## 11. Functions

- Free functions — `main`, `pauseScreen`, `clearScreen`,
  `initializeGlobals`, the friend back-doors `debugDumpAccount` and
  `forceAccountBalance`.
- Member functions, static methods.
- Function pointers as parameters — `Utils::performOperation(int, int,
  int (*operation)(int, int))`.
- `main()` returning `int`.
- **Anonymous (unnamed) namespace** — `namespace { ... }` in
  `src/AgeVerifier.cpp` to give the `VerifyFn` typedef and
  `SYMBOL_NAME` constant internal linkage.

## 12. Global State & Linkage

- `extern` global variables declared in `Globals.h`, defined in
  `Globals.cpp` (`globalCustomerCounter`, `enableDebugLogging`,
  `logFilePath`).
- Static data member with file-scope definition —
  `unsigned long TransactionBase::instanceCounter = 0UL;`
  in `TransactionBase.cpp`.
- File-scope `static const` constants — `DEFAULT_LIB_PATH` and
  `SYMBOL_NAME` in `src/AgeVerifier.cpp`.

## 13. Standard Library Usage

- **Streams `<iostream>`** — `std::cout`, `std::cerr`, `std::cin`,
  `std::endl`, stream-state with `cin.clear()`,
  `cin.ignore(numeric_limits<streamsize>::max(), '\n')`.
- **I/O manipulators `<iomanip>`** — `std::fixed`, `std::setprecision`,
  `std::setw`, `std::setfill`, `std::left`.
- **String streams `<sstream>`** — `std::ostringstream` for audit
  entries, `LoadLibrary` error messages, currency formatting.
- **Strings** — `find`, `rfind`, `length`, `empty`, `size`, indexed
  access.
- **Vectors** — `push_back`, `clear`, `reserve`, `empty`, `size`,
  `back`, `begin`/`end`, `erase`, `[i]`.
- **Maps `<map>`** — `find`, `operator[]` (default-constructs value on
  first hit), `const_iterator` iteration over
  `std::map<std::string, unsigned long>` in
  `AuditLogger::dumpEventSummary`.
  Also nested-container values: `std::map<int, std::vector<ScoreCard*> >`
  in `ObligationMatrix::tiers` and as the return type of
  `TierClassifier::batchClassify` — note the `> >` spacing required by
  C++03 (a closing `>>` would tokenize as right-shift).
- **Pairs `<utility>`** — `std::pair<ReportEngine*, ReportEngine*>` as a
  return type in `ReportEngine::partition`, constructed via
  `std::make_pair` in `ReportEngine.cpp`.
- **Math `<cmath>`** — `std::pow`, `std::sqrt` in `BondCalculator`.
- **Algorithm `<algorithm>`** — `std::max` in `BondCalculator`.
- **Time `<ctime>`** — `std::time`, `std::localtime`, `std::strftime`.
- **Random `<cstdlib>`** — `std::rand`, `std::srand`, `RAND_MAX`,
  `std::system(CLEAR_SCREEN)`.
- **Limits `<limits>`** — `std::numeric_limits<std::streamsize>::max()`.
- **Char classification `<cctype>`** — `std::isdigit`.
- **`<stdexcept>` / `<exception>`** for the exception hierarchy.
- **`<iosfwd>`** — used in headers that only forward-declare
  `std::ostream&` (`AuditLogger.h`, `Account.h`).

## 14. Native interop (cross-platform dynamic loading)

The host application links statically with the standard library only;
the `age_verifier` shared library is loaded at runtime through the
platform-specific dynamic-linker API:

- **Windows** — `LoadLibraryA` / `GetProcAddress` / `FreeLibrary` from
  `<windows.h>`. The exported symbol uses `__declspec(dllexport)` /
  `__declspec(dllimport)` in `native/include/age_verifier.h` and is
  guarded by `extern "C"` to avoid name mangling.
- **Linux** — `dlopen` / `dlsym` / `dlclose` from `<dlfcn.h>`; the
  application links with `-ldl`. Same `extern "C"` ABI.

The wrapper class `AgeVerifier` owns the library handle (RAII —
`FreeLibrary` / `dlclose` from the destructor), stores a typed
function pointer (`VerifyFn`), and surfaces the result as an enum.

## 15. Idiomatic Patterns Demonstrated

- **Visitor (double dispatch)** — `TransactionVisitor::visit(Deposit*)`
  paired with `Deposit::accept(TransactionVisitor&)`.
- **Template Method** via virtual functions — `Transaction::apply()`
  overridden per concrete subclass.
- **Strategy** via function pointers — `Utils::performOperation`.
- **Pseudo-singleton** global state — `globalCustomerCounter`,
  `enableDebugLogging` initialised once via `initializeGlobals()`.
- **Polymorphism through base-class pointers in a container** —
  `std::vector<Transaction*>` owned by `Account`,
  `std::vector<Customer*>` owned by `Bank`.
- **Friend-function back-doors** for debug/admin — `debugDumpAccount`,
  `forceAccountBalance`.
- **Mediator / observer mesh** — `AuditLogger` +
  `NotificationCenter` + `BranchManager` + `RiskAnalyzer` are the
  mediators that close the mega-SCC around `Bank` / `Customer` /
  `Loan` / `Account` / `Transaction`.
- **Pipeline with back-callback** — the SCC D reporting pipeline:
  `ReportEngine::generate → ReportWriter::write → ReportSection::render
  → ReportFormatter::format`, then
  `ReportWriter → ReportEngine::onPageReady` closes the cycle.
- **RAII wrapper over a C handle** — `AgeVerifier` holds a `void*`
  library handle and releases it in the destructor.

---

## What's intentionally NOT used

The codebase studies C++03 → Java porting, so the following C++11+
features are deliberately absent and must stay absent:

- `nullptr`, `auto`, range-based `for`.
- `using` type aliases (use `typedef`).
- `std::unique_ptr` / `std::shared_ptr` (use raw `new`/`delete` with
  documented ownership in the relevant header).
- `enum class`, `override`, `final`, `noexcept`.
- Lambdas, `std::function`.
- `constexpr`, uniform-initialization braces `{}` for non-aggregates.
- `std::array`, `std::unordered_map`, `<chrono>`, `<thread>`,
  `<filesystem>`, `<memory>`.

If you find yourself reaching for one of these, the C++03 equivalent
already exists somewhere in the codebase — find it and follow the
pattern.

---

## Summary

The project deliberately exercises the full C++03 surface —
preprocessor, typedefs, enums, structs, single + multi-level public
inheritance, abstract classes with pure virtuals, virtual dispatch,
`dynamic_cast`, `reinterpret_cast`, friend functions, static members,
function pointers, anonymous namespaces, exception handling
(including nested catch + bare re-throw), STL containers / iterators /
streams, and cross-platform native dynamic-loading — without using any
C++11+ features.
