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
├── docs/
│   ├── SCC_DEMO_LAYOUT.md              # Load-bearing: diagrams + edges
│   ├── SCC_DEMO_PROJECT_PROMPT.md      # Original spec (Russian)
│   ├── README.md                       # This file
│   ├── DOCUMENTATION_INDEX.md          # Doc nav
│   ├── QUICK_REFERENCE.md              # Class taxonomy + menu
│   ├── TEST_CASES.md                   # Scenario test cases
│   ├── TESTING_GUIDE.md                # Scripted stdin sequences
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
`BondCalculator`, `LoggingVisitor`.

## Build & Run

### Using Make (recommended)

```bash
make           # builds ./BankSystem with -std=c++03 -Wall -Wextra (zero warnings)
./BankSystem
```

### Manual

```bash
g++ -std=c++03 -Wall -Wextra -I./include -o BankSystem src/*.cpp
```

### Targets

- `make` / `make all` — build
- `make clean` — remove `build/` and executable

### Cross-platform

Linux (g++), macOS (clang), Windows (MinGW). Standard-library only.

## Interactive menu

```
1. Run new-bank flow             (exercises mega-SCC + SCC B visitor)
2. Generate report               (exercises SCC D pipeline)
3. Calculate bond parameters     (acyclic baseline)
0. Exit
```

### Scenario 1 — new-bank flow

Creates a `Bank`, wires up `AuditLogger`, `NotificationCenter`,
`BranchManager`+`RiskAnalyzer`, registers two `Customer`s, opens
`Account`s, runs `Deposit`/`Withdrawal`/`Transfer`/`LoanPayment` through
both `apply()` and a `LoggingVisitor`, approves a `Loan` via the
manager, broadcasts notifications, dumps the audit log, then
`delete bank;` cascades the entire graph.

### Scenario 2 — report

Builds the 5-class reporting pipeline, runs
`engine->generate(std::cout)`. Tears down via `delete engine;`.

### Scenario 3 — bond calculator

Acyclic standalone module (`BondCalculator.{h,cpp}`). Computes PV/FV
of a bond with random coupon stream.

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
