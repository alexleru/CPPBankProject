# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

The project uses a single hand-written Makefile (no CMake, no test framework). It is cross-platform aware (`OS=Windows_NT` branch picks `.exe` and `cmd /C mkdir`).

```bash
make            # builds ./BankSystem (or BankSystem.exe on Windows MinGW)
make clean      # removes build/ and the executable
./BankSystem    # launches the interactive menu
```

Manual compile (no Make):

```bash
g++ -std=c++03 -I./include -o BankSystem src/*.cpp
```

If you add a new `.cpp` under `src/`, also append it to the explicit `SOURCES` list in the `Makefile` — it does not glob.

## C++03 is a hard constraint

The codebase deliberately targets **C++03** (compiled with `-std=c++03`). Do not introduce C++11+ features when editing. Specifically, do not use:

- `nullptr` (use `NULL`)
- `auto`, range-based `for`
- `using` type aliases (use `typedef`)
- `std::unique_ptr` / `std::shared_ptr` (raw `new`/`delete`, container-owned objects, or `Account*` in `AccountRegistry`)
- scoped enums (`enum class`), `override`, `final`, `noexcept`, lambdas, `std::function`

This constraint is explicit in `MortgageAccount.h` and is the whole reason this codebase exists — it is a study for porting C++03 idioms to Java. If you would naturally reach for a C++11 idiom, prefer the C++03 equivalent that's already used elsewhere in the file (function pointers instead of `std::function`, `typedef` instead of `using`, etc.).

## Architecture

Three roughly independent subsystems share `main.cpp` as the menu dispatcher and `globalBank` as a process-wide singleton.

**1. Account hierarchy (`Account.h` + 4 subclasses).** `Account` is abstract with two pure virtuals: `getAccountType()` and `applyMonthlyProcessing()`. Concrete subclasses: `SavingsAccount`, `CheckingAccount`, `LoanAccount`, and `MortgageAccount : public LoanAccount` (a two-level hierarchy — `Bank::findMortgageAccount` uses `dynamic_cast` to safely downcast). Each subclass owns its monthly-processing rule (interest, fee, EMI). Accounts hold their own `TransactionHistory` (a `typedef`'d `std::vector<Transaction>`).

**2. `Bank` (the aggregate root).** Owns `CustomerList customers` (vector) and `AccountRegistry accountRegistry` (`std::map<std::string, Account*>`). The map owns the heap-allocated accounts and frees them in `~Bank()`. Customer registration, account creation, deposits/withdrawals/transfers, monthly processing, and reports all flow through `Bank`. `Bank::transferBetweenAccounts` uses `try`/`throw;` (bare rethrow) to roll back the source-side credit if the destination side fails — preserve this pattern when editing transfer-like flows.

**3. `BondCalculator` (standalone).** Self-contained valuation module under `BondCalculator.{h,cpp}`. Not part of the account hierarchy. Drives menu option 16. Uses `std::rand` (C++03-safe) seeded via `std::time`.

**Globals.** `Globals.h` declares `extern` counters and config flags (`globalCustomerCounter`, `enableDebugLogging`, …) that are defined exactly once in `Globals.cpp` and initialized via `initializeGlobals()` from `main`. `main.cpp` also declares `Bank globalBank(BANK_NAME);` as a file-scope global. Treat these as the project's "singletons."

**Friend functions for admin/debug.** Headers declare free-function `friend`s like `debugAccountInfo`, `validateAccountBalance`, `forceBalanceUpdate`, `getAccountTransactions`, `forceCloseAccount`. These are intentional back-doors used by menu option 15 ("Admin/Debug Functions") — they bypass normal validation. Don't replace them with public getters/setters; the friend pattern is part of what the codebase is demonstrating.

**Function-pointer strategy hooks.** Three places intentionally take raw function pointers (the C++03 stand-in for `std::function`):

- `Utils::performOperation(int, int, int (*)(int, int))` — demo only.
- `Bank::applyToAllAccounts(double (*rule)(double))` — walks the registry, applies the rule to each active balance.
- `MortgageAccount::applyMortgageRule(MortgageRule)` (typedef `Money (*MortgageRule)(Money)`).

These are wired to menu option 17. They exist to be ported to Java functional interfaces and should stay as raw function pointers in C++.

**Typedef-heavy mortgage module.** `MortgageAccount.h` deliberately aliases primitives (`Money`, `Rate`, `TermInYears`, `PropertyId`), containers (`AmortizationSchedule`, `BalanceTimeline`), iterators, and the function-pointer type. Use these aliases consistently when extending mortgage code — that's the point of the module.

## Testing

There is **no unit-test framework**. Testing is exercising the interactive console. Two paths:

- Manual: `./BankSystem` and follow `docs/TESTING_GUIDE.md` / `docs/TEST_CASES.md` (61 documented cases organized into 7 scripted scenarios).
- Scripted: pipe a sequence of menu choices/answers into stdin. The Testing Guide lists the keystroke sequences for each scenario (e.g. `./BankSystem < test_scenario_a.txt`).

If you change menu numbering or prompts, the scripted scenarios in `docs/TESTING_GUIDE.md` will silently desync — update them in the same change.

## Documentation map

- `docs/README.md` — project overview, full feature list, build steps (duplicates some of this file but in more detail).
- `docs/QUICK_REFERENCE.md` — menu options, validation rules, formulas, configuration constants.
- `docs/TEST_CASES.md` — 61 numbered test cases organized by feature.
- `docs/TESTING_GUIDE.md` — 7 scenario scripts + Python automation harness.
- `CPP_Language_Constructs.md` — inventory of every C++03 construct used (useful when wondering "is X idiomatic here?").
- `CPP_Reports_Comparison.md` — meta-doc comparing this codebase's dialect to another project; not load-bearing for development.
