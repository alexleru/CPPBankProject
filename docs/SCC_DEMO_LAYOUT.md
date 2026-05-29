# SCC structure of the demo project

## Overview

This project is a deliberate fixture for the `java_cpp_chunkagent`
converter plugin. Its `#include` + method-call graph is engineered to
contain **exactly four cyclic SCCs**, plus a small set of acyclic
baseline classes (Tier-A in chunker terminology).

| SCC | Size | Pattern                                    | Chunker tier |
|-----|------|--------------------------------------------|--------------|
| mega-SCC (A merged with C)  | 9 | Account ↔ Transaction + mediator/observer mesh           | C/D (worst-case)  |
| SCC B                       | 5 | Visitor / double dispatch                                 | B (algorithmic)   |
| SCC D                       | 5 | Reporting pipeline with back-callback                     | B (algorithmic)   |
| SCC E                       | 4 | Credit-scoring (ScoreCard / ObligationMatrix / WeightingEngine / TierClassifier) — stored back-pointers in all four nodes | B (stub-demo) |

Acyclic baseline (Tier A):
`Utils`, `Globals`, `Constants`, `Enums`, `BondCalculator`,
`LoggingVisitor`, `AgeVerifier`, `TransactionBase`.

`LoggingVisitor` depends on SCC B but is not itself part of the cycle
(no SCC B member references it), so it stays Tier-A.

`AgeVerifier` is a thin RAII wrapper over the cross-platform native
`age_verifier` library (loaded at runtime via `LoadLibrary` / `dlopen`).
It references no SCC class.

`TransactionBase` is an acyclic parent of `Transaction`. The
`Transaction → TransactionBase` inheritance edge is one-way and
`TransactionBase` itself references nothing, so it never joins any
SCC; its sole purpose is to give the project a 3-level inheritance
chain (`Deposit/Withdrawal/Transfer/LoanPayment → Transaction →
TransactionBase`) and to expose a process-wide `unsigned long`
`instanceCounter`.

---

## Mega-SCC (size 9): Account/Transaction + Bank/Customer/Loan/AuditLogger/NotificationCenter/BranchManager/RiskAnalyzer

### Members

```
                                    +----------+
                          +-------- |   Bank   | --------+
                          |         +----------+         |
                          v              ^               v
                  +-------------+        |       +---------------------+
                  | BranchMgr   |        |       | NotificationCenter  |
                  +------+------+        |       +----------+----------+
                         |               |                  |
                         v               |                  v
                  +-------------+        |             +----------+
                  | RiskAnalyzer| <------+-----------> |AuditLogger|
                  +------+------+        |             +----+-----+
                         |               |                  ^
                         v               v                  |
                  +-------------+   +----------+            |
                  |   Loan      |<->| Customer | -----------+
                  +------+------+   +----+-----+
                         |               |
                         |               v
                         |          +----------+      +-------------+
                         +--------> | Account  |<---->| Transaction |
                                    +----------+      +-------------+
                                       (SCC A core)
```

### Edge types

`Account ↔ Transaction` is the SCC A 2-cycle:
- `Account` owns `std::vector<Transaction*> history`.
- `Transaction` holds `Account* source` / `Account* dest`.
- `Transaction::apply()` calls `source->debit()` / `dest->credit()`.
- `Account::addTransaction()` records and numbers the transaction.

`Transaction` also inherits from the acyclic `TransactionBase`. The
inheritance edge is one-way and `TransactionBase` references nothing,
so it does not affect SCC membership — it only adds a third level to
the inheritance chain.

The SCC A pair merges into the SCC C mesh because:

- `Account::holder` → `Customer` (back-pointer).
- `Account::bank`   → `Bank` (back-pointer).
- `Account::logger` → `AuditLogger` (back-pointer).
- All three create non-owning edges that close the mega-SCC.

### Method-level call density

Every class makes ≥ 2 calls into peer classes inside this SCC. Examples:

- `Bank::registerCustomer(Customer*)` → `AuditLogger::onCustomerRegistered`, `NotificationCenter::broadcast`.
- `Customer::addAccount(Account*)` → `Account::setBank/setLogger/setHolder`, `AuditLogger::onAccountCreated`.
- `Loan::assessRisk()` → `RiskAnalyzer::assess`; `Loan::approve()` → `AuditLogger::onLoanApproved`.
- `AuditLogger::notify(NC*, str)` → `NotificationCenter::broadcast`; logs into all 3 event types.
- `NotificationCenter::broadcast(Bank*, str)` → `Customer::getFullName`, `AuditLogger::log`, `Bank::getName`.
- `BranchManager::approveLoan(Loan*, AuditLogger*)` → `RiskAnalyzer::assess`, `Loan::approve`, `NotificationCenter::broadcast`.
- `RiskAnalyzer::assess(Loan*)` → `Loan::getPrincipal/getBorrower`, `Customer::getAccounts`; `flagCustomer` → `BranchManager::getRiskAnalyzer`.

### Ownership

| Owner            | Owns                                                                          |
|------------------|-------------------------------------------------------------------------------|
| `Bank`           | `vector<Customer*>`, `vector<BranchManager*>`, `AuditLogger*`, `NotificationCenter*` |
| `Customer`       | `vector<Account*>`, `vector<Loan*>`                                           |
| `Account`        | `vector<Transaction*>`                                                        |
| `BranchManager`  | `RiskAnalyzer*`                                                               |
| everyone else    | nothing (back-pointers only)                                                  |

`delete bank;` cascades through the entire object graph.

---

## SCC B (size 5): Visitor pattern

### Members

```
              +---------------------+
              | TransactionVisitor  |
              +----------+----------+
                  ^  ^  ^  ^
                  |  |  |  |
        +---------+  |  |  +-------+
        |            |  |          |
   +---------+ +-----------+ +----------+ +-------------+
   | Deposit | | Withdrawal| | Transfer | | LoanPayment |
   +---------+ +-----------+ +----------+ +-------------+
        \           \           /             /
         \           \         /             /
          +-----------+---+---+-------------+
                          v
                  (inherits Transaction, mega-SCC)
                  one-way inheritance — does NOT merge
```

### Cycle edges

- `TransactionVisitor.h` forward-declares all four concrete types and
  declares four pure virtual `visit(X*) = 0` methods → edge to each
  subclass.
- Each concrete header forward-declares `class TransactionVisitor;`
  and declares `virtual void accept(TransactionVisitor&)` → edge to
  the visitor.
- Result: 4 mutual 2-cycles through `TransactionVisitor`, all 5 nodes
  strongly connected.

### Why SCC B does NOT merge into the mega-SCC

`Transaction` (in the mega-SCC) does **not** declare `accept()` and does
**not** reference `TransactionVisitor`. Therefore the only edge from
the mega-SCC to SCC B is the four inheritance edges
`Deposit/Withdrawal/Transfer/LoanPayment → Transaction`, which are
**unidirectional**. No path back from `Transaction` to any SCC B
member exists.

(LoanPayment.cpp does `#include "Loan.h"` so that `apply()` can call
`Loan::recordPayment(amount)` — but `Loan` has no edge back to
`LoanPayment`, so this stays a one-way edge from SCC B into the
mega-SCC.)

### Members and roles

| Class                | Role                                                          |
|----------------------|---------------------------------------------------------------|
| `TransactionVisitor` | Pure virtual visitor base, four `visit(X*) = 0` methods       |
| `Deposit`            | `Transaction` (source=NULL, dest=acct, credits dest)          |
| `Withdrawal`         | `Transaction` (source=acct, dest=NULL, debits source)         |
| `Transfer`           | `Transaction` (debits source, credits dest)                   |
| `LoanPayment`        | `Transaction` + back-pointer to `Loan`, also records payment  |

`LoggingVisitor` (concrete subclass writing to `std::ostream`) is *not*
part of SCC B — it depends on the cycle but nothing in the cycle
depends on it.

---

## SCC D (size 5): Reporting pipeline

### Members

```
                +-------------+
                | ReportEngine| <-----------+
                +-+---------+-+             |
                  |         |               |
                  v         v               |
          +--------+   +--------+           |
          | Filter |   | Writer | ----------+  (onPageReady callback)
          +---+----+   +---+----+
              |            |
              v            v
          +---------+   +-----------+
          | Section | <-| Formatter |
          +----+----+   +-----+-----+
               ^              |
               +--------------+
            (Formatter holds current Section*)
```

### Cycle edges

- `ReportEngine` holds `ReportFilter*`, `ReportWriter*`; declares `onPageReady(ReportSection*)`.
- `ReportFilter` holds `ReportEngine*`; `apply(ReportEngine*)` calls `engine->getDateRange()`; `getActiveRules(ReportSection*) const` reads `section->getTitle()`.
- `ReportSection` holds `ReportFilter*`, `ReportFormatter*`; `render()` calls `filter->getActiveRules(this)` + `formatter->format(this)`.
- `ReportFormatter` holds `ReportSection*`; `format(ReportSection*)` calls `section->getTitle()`.
- `ReportWriter` holds `ReportEngine*`, `ReportFormatter*`; `write(ReportEngine*, ostream&)` creates a `ReportSection`, renders it, then closes the cycle with `engine->onPageReady(section)`.

All five nodes are reachable from every other node — single SCC.

### Isolation invariant

**Hard rule**: no SCC D header forward-declares or `#include`s any
class from SCC A/B/C/E, and no `.cpp` of an SCC D class references any
SCC A/B/C/E type by name. Verified by `grep`:

```bash
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|ScoreCard|ObligationMatrix|WeightingEngine|TierClassifier' \
    include/Report*.h src/Report*.cpp
# expected: no matches
```

---

## SCC E (size 4): Credit-scoring subsystem

### Members

```
              +------------+
              | ScoreCard  |<---------+
              +-----+------+          |
                ^   |   ^             |
                |   |   |             |
   +------------+   |   +-------------+--------+
   |               v                  ^        |
   |    +-------------------+         |        |
   |    | ObligationMatrix  |<--------+        |
   |    +---------+---------+                  |
   |              ^                            |
   |              |                            |
   |    +---------+-----------+                |
   |    |  WeightingEngine    |<---------------+
   |    +---------+-----------+                |
   |              ^                            |
   |              |                            |
   |    +---------+-----------+                |
   +--->|  TierClassifier     |----------------+
        +---------------------+
```

### Cycle edges (stored back-pointers in all four nodes)

- `ScoreCard` holds `ObligationMatrix*`, `WeightingEngine*`, `TierClassifier*` — back-pointers established by `registerWith()` / `assignClassifier()`.
- `ObligationMatrix` holds `WeightingEngine*`, `TierClassifier*` and `std::map<int, std::vector<ScoreCard*> >` entries.
- `WeightingEngine` holds `ObligationMatrix*`, `TierClassifier*`.
- `TierClassifier` holds `ObligationMatrix*`, `WeightingEngine*` (set by `bind()`).

Cross-partner method signatures include `recalibrate(WeightingEngine*, ScoreCard*)`, `batchClassify(ObligationMatrix*, WeightingEngine*) → std::map<int, std::vector<ScoreCard*> >`, and `reweightMatrix(ObligationMatrix*, double) → std::vector<ScoreCard*>` — see `CPP_BANK_PROJECT_COMPLEXITY_ENHANCEMENT.md` for the full inventory.

### Why SCC E exists separately from the mega-SCC

The credit-scoring subsystem has **no edges** to any class in the
mega-SCC, SCC B, or SCC D. The vocabulary is deliberately non-banking
(`ScoreCard`, `ObligationMatrix`, `WeightingEngine`, `TierClassifier`)
to make LLM-guessed Java types diverge from the real ones when stubs
are absent. The chunker classifies SCC E as Tier-B (4+ nodes,
algorithmic) and exercises the weighted-greedy FAS + stub-generation
path.

### Isolation invariant

**Hard rule**: no SCC E header or `.cpp` references any class from
the mega-SCC / SCC B / SCC D. Verified by `grep`:

```bash
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|Report' \
    include/ScoreCard.h include/ObligationMatrix.h include/WeightingEngine.h include/TierClassifier.h \
    src/ScoreCard.cpp src/ObligationMatrix.cpp src/WeightingEngine.cpp src/TierClassifier.cpp
# expected: no matches
```

If anything matches, the chunker will fuse SCC E into the mega-SCC and
the Tier-B stub demo collapses.

---

## Acyclic baseline (Tier-A)

| File                          | Purpose                                                     |
|-------------------------------|-------------------------------------------------------------|
| `Constants.h`                 | Money/AccountId/LoanId/CustomerId typedefs + constants      |
| `Enums.h`                     | AccountType, TransactionType, TransactionStatus, etc.       |
| `Globals.{h,cpp}`             | extern counters + `initializeGlobals()`                     |
| `Utils.{h,cpp}`               | static helpers (currency format, validated I/O, function-ptr demo) |
| `BondCalculator.{h,cpp}`      | self-contained bond valuation (menu option 3)               |
| `LoggingVisitor.{h,cpp}`      | concrete `TransactionVisitor` — depends on SCC B but is not in it |
| `TransactionBase.{h,cpp}`     | acyclic parent of `Transaction`; process-wide `instanceCounter` |
| `AgeVerifier.{h,cpp}`         | RAII wrapper over `native/age_verifier` shared lib (menu option 4) |

---

## Expected `dependency_graph.json` shape

`DependencyGraphService.get_cyclic_sccs()` should return **exactly 4**
strongly-connected components:

| SCC ID | Approx. size | Members (sorted)                                                                                                            |
|--------|--------------|------------------------------------------------------------------------------------------------------------------------------|
| mega   | 9            | Account, AuditLogger, Bank, BranchManager, Customer, Loan, NotificationCenter, RiskAnalyzer, Transaction                     |
| visitor| 5            | Deposit, LoanPayment, Transfer, TransactionVisitor, Withdrawal                                                              |
| report | 5            | ReportEngine, ReportFilter, ReportFormatter, ReportSection, ReportWriter                                                    |
| credit | 4            | ObligationMatrix, ScoreCard, TierClassifier, WeightingEngine                                                                |

**Critical check**: no edge exists between any reporting-pipeline class
and any other SCC, and no edge exists between any credit-scoring class
and any other SCC. If the chunker reports SCC D or SCC E members fused
into the mega-SCC, an isolation invariant has been violated and must be
fixed before reporting the demo as correct.

---

## Verification

```bash
make clean && make all                                # zero warnings under -Wall -Wextra -std=c++03
./BankSystem                                          # 4-option interactive menu
printf '1\n\n2\n\n0\n' | ./BankSystem                 # exercises mega-SCC and SCC D non-interactively
```
