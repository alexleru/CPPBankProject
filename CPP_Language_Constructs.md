# C++ Language Constructs Used in CPPBankProject

The codebase is intentionally written to the **C++03** standard (no `nullptr`, no `using` aliases, no `auto`, no range-based `for`, no smart pointers). That constraint is explicitly called out in `MortgageAccount.h`.

---

## 1. Preprocessor

- Include guards `#ifndef / #define / #endif` in every header.
- Quoted vs. angle-bracket includes (`#include "Account.h"` vs. `#include <vector>`).
- Object-like macros for constants — `Constants.h` (`BANK_NAME`, `MAX_ACCOUNTS_PER_CUSTOMER`, `OVERDRAFT_LIMIT`, …) and `BondCalculator.h` (`BOND_MAX_RANDOM_RATE`, `BOND_DEFAULT_DISCOUNT_RATE`).
- Conditional compilation `#ifdef _WIN32 / #else / #endif` (`Constants.h`, `main.cpp::clearScreen`).

## 2. Built-in & Standard Types

`int`, `double`, `bool`, `char`, `unsigned int`, `unsigned long`, `size_t`, `time_t`, `std::string`, `std::vector<T>`, `std::map<K,V>`, raw C arrays (`char buffer[80]` in `Utils.cpp`, `double sensitivityRates[5]` in `BondCalculator.cpp`).

## 3. `typedef` Aliases (the C++03-only alias mechanism)

- **Domain primitives** — `Money`, `Percentage`, `Rate`, `TermInMonths`, `TermInYears`, `MonthIndex`, `PropertyId`, `PropertyAddress`, `AccountIdRef` (`MortgageAccount.h`).
- **Container aliases** — `TransactionHistory` (`Account.h`), `CustomerList`, `AccountRegistry`, `AccountList` (`Bank.h`), `AmortizationSchedule`, `BalanceTimeline` (`MortgageAccount.h`).
- **Iterator aliases** — `ScheduleIterator`, `ScheduleConstIterator`, `BalanceTimelineIt`.
- **Struct typedef pattern** — `typedef struct PropertyInfo PropertyInfo_t;` then `typedef PropertyInfo_t Property;`.
- **Enum typedef** — `typedef enum MortgageType MortgageKind;`.
- **Function-pointer typedef** — `typedef Money (*MortgageRule)(Money);`.
- **"Self typedef" inside a class** — `typedef MortgageAccount self_type;` in `MortgageAccount`.

## 4. Enums (Unscoped, C++03 Style)

`AccountType`, `TransactionType`, `TransactionStatus`, `CustomerStatus`, `LoanStatus` (`Enums.h`), and the feature-local `MortgageType` (`MortgageAccount.h`).

## 5. Structs

`EMIPayment` (`LoanAccount.h`), `PropertyInfo`, `MortgageInstallment` (`MortgageAccount.h`), `BondCashFlow` (`BondCalculator.h`) — all plain data aggregates.

## 6. Classes & OOP Features

- Abstract base class with pure virtual methods — `Account` (`virtual AccountType getAccountType() const = 0;`, `virtual void applyMonthlyProcessing() = 0;`).
- Public single inheritance — `SavingsAccount`, `CheckingAccount`, `LoanAccount` derive from `Account`.
- Multi-level inheritance — `MortgageAccount : public LoanAccount` (which itself derives from `Account`).
- Virtual destructor on the polymorphic base (`virtual ~Account()`).
- Virtual method overriding (`withdraw`, `displayAccountInfo`, `applyMonthlyProcessing`).
- Explicit base-class call from override — `LoanAccount::applyMonthlyProcessing()`, `Account::displayAccountInfo()`.
- Access specifiers `public` / `protected` / `private`.
- `const` member functions (e.g., all getters).
- Static data members and static methods — `Customer::customerCounter`, `Transaction::transactionCounter`, `Utils::customerIdCounter`, `Utils::add/multiply/performOperation`, `LoanAccount::calculateEMI`, `MortgageAccount::computeLoanPrincipal`.
- Function overloading — `Bank::findCustomer` non-const + const, `BondCalculator::generateCashFlowSchedule()` and `(unsigned int seed)`.
- Default arguments — `withdraw(amount, description = "Withdrawal")`, `LoanAccount(double, double rate = 0.08, int termMonths = 12)`, `BondCalculator(..., double maxRandomRate = BOND_MAX_RANDOM_RATE, …)`.
- Constructor member-initializer lists in every class (e.g. `Account::Account(...) : type(..), balance(..), createdDate(std::time(NULL)), ...`).
- `friend` functions to expose private state for debug/admin — `debugAccountInfo`, `validateAccountBalance`, `forceBalanceUpdate`, `getAccountTransactions`, `debugCustomerInfo`, `validateCustomerData`, `updateCustomerStatus`, `getCustomerAccounts`, `debugBankInfo`, `getAllCustomers`, `getAccountRegistry`, `forceCloseAccount`.
- Forward declaration — `class Account;` in `Customer.h`.
- "Utility class" idiom — `Utils` exposes only static methods.

## 7. Memory Management

- `new` / `delete` with raw pointers (e.g. `new SavingsAccount(...)`, `delete accounts[i]` in `Customer::~Customer`).
- C-style `NULL` rather than `nullptr` (consistent with C++03).
- Container-managed RAII for `std::vector` / `std::map` members.

## 8. Type-Conversion Casts

- `static_cast<int>(i)`, `static_cast<size_t>(termMonths)`, `static_cast<double>(...)`, `static_cast<unsigned int>(std::time(NULL))`, `static_cast<Rate>(MONTHS_PER_YEAR)`, `static_cast<PropertyId>(...)`.
- `dynamic_cast<MortgageAccount*>(a)` in `Bank::findMortgageAccount` for safe downcasting.

## 9. Exception Handling

- `try` / `catch` / `throw` blocks throughout `Bank.cpp`, `Account.cpp`, `BondCalculator.cpp`, `LoanAccount.cpp`, `MortgageAccount.cpp`, `main.cpp`.
- Standard exception types — `std::invalid_argument`, `std::runtime_error`, base `std::exception`.
- Catch by `const std::exception&`.
- Bare `throw;` to re-throw (transfer reversal in `Bank::transferBetweenAccounts`).

## 10. Control Flow

- `if` / `else if` / `else`; `switch / case / default / break`; `while`, `for` (classic index and iterator forms); `return`, `continue`.
- Ternary `?:` (e.g. `(allowZero ? amount >= 0 : amount > 0)`, `pmiRequired ? "Yes" : "No"`).
- No range-based `for` (deliberate, C++03).

## 11. Functions

- Free functions, member functions, static methods.
- Function pointers as parameters — `void performOperation(int, int, int (*operation)(int, int))`, `void Bank::applyToAllAccounts(double (*rule)(double))`, `Money MortgageAccount::applyMortgageRule(MortgageRule rule)`.
- `main()` returning `int`.

## 12. Global State & Linkage

- `extern` global variables declared in `Globals.h`, defined in `Globals.cpp` (`globalCustomerCounter`, `enableDebugLogging`, `logFilePath`, …).
- Global object instance — `Bank globalBank(BANK_NAME);` in `main.cpp`.
- File-scope `static const` constants — `PMI_LTV_THRESHOLD`, `PMI_ANNUAL_RATE`, `DEFAULT_PROPERTY_TAX`, `MONTHS_PER_YEAR` in `MortgageAccount.cpp`.

## 13. Standard Library Usage

- **Streams `<iostream>`** — `std::cout`, `std::cerr`, `std::cin`, `std::endl`, stream-state with `cin.clear()`, `cin.ignore(numeric_limits<streamsize>::max(), '\n')`.
- **I/O manipulators `<iomanip>`** — `std::fixed`, `std::setprecision`, `std::setw`, `std::setfill`, `std::left`.
- **String streams `<sstream>`** — `std::ostringstream` for ID generation and currency formatting.
- **Strings** — `std::getline`, `find`, `rfind`, `length`, `empty`, `size`, `npos`, indexed access.
- **Vectors** — `push_back`, `clear`, `reserve`, `empty`, `size`, `back`, `begin`/`end`, `erase`, `[i]`.
- **Maps** — `find`, `end`, iterator pair (`it->first`/`it->second`), `[key]` insertion, `const_iterator`.
- **Math `<cmath>`** — `std::pow`, `std::sqrt`, `std::abs`.
- **Algorithm `<algorithm>`** — `std::max`.
- **Time `<ctime>`** — `std::time`, `std::localtime`, `std::strftime`.
- **Random `<cstdlib>`** — `std::rand`, `std::srand`, `RAND_MAX`.
- **Limits `<limits>`** — `std::numeric_limits<std::streamsize>::max()`.
- **Char classification** — `std::isdigit`.
- **`<stdexcept>` / `<exception>`** for the exception hierarchy.
- **`system("cls" / "clear")`** from `<cstdlib>`.

## 14. Idiomatic Patterns Demonstrated

- **Template Method** via virtual functions (`Account::applyMonthlyProcessing` overridden per subclass).
- **Strategy** via function pointers (`Bank::applyToAllAccounts`, `MortgageAccount::applyMortgageRule`, `Utils::performOperation`).
- **Pseudo-singleton** global object (`globalBank`).
- **Polymorphism** through `Account*` stored in a `std::map<std::string, Account*>` registry.
- **Friend-function back-doors** used for debug/admin operations rather than getters/setters.

---

## Summary

The project deliberately exercises the full C++03 surface — preprocessor, typedefs, enums, structs, single + multi-level public inheritance, abstract classes with pure virtuals, virtual dispatch, `dynamic_cast`, friend functions, static members, function pointers, exception handling, and STL containers/iterators/streams — without using any C++11+ features.
