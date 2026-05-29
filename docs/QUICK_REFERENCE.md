# Quick Reference — SCC Demo Build

## Menu

```
1. Run new-bank flow             (mega-SCC + SCC B visitor demo)
2. Generate report               (SCC D pipeline demo)
3. Calculate bond parameters     (acyclic baseline)
4. Verify age 21+                (native library demo)
0. Exit
```

## Class taxonomy

### SCC A — `Account ↔ Transaction` (size 2)

| Class | Role |
|---|---|
| `Account` | Owns `vector<Transaction*>`. Back-pointers to `Customer`, `Bank`, `AuditLogger` (non-owning). Methods: `debit()`, `credit()`, `addTransaction()`. Friend functions: `debugDumpAccount`, `forceAccountBalance`. |
| `Transaction` | Abstract-ish base, derives from `TransactionBase`. Holds `Account* source` and `Account* dest`. `apply()` calls `source->debit()` + `dest->credit()`. `accept(TransactionVisitor&)` is **not** declared here (would collapse SCC B). |
| `TransactionBase` *(acyclic)* | Acyclic root of the chain. Owns a process-wide `unsigned long instanceCounter`; gives every `Transaction` a unique `instanceId`. Not part of any SCC. |

### SCC B — Visitor (size 5)

| Class | Role |
|---|---|
| `TransactionVisitor` | Pure-virtual base. 4 `visit(X*) = 0` methods, one per concrete subclass. |
| `Deposit` | `Transaction` (dest=acct, source=NULL). `accept()` dispatches via visitor. |
| `Withdrawal` | `Transaction` (source=acct, dest=NULL). |
| `Transfer` | `Transaction` (debits source, credits dest). |
| `LoanPayment` | `Transaction` + non-owning `Loan*`. Overrides `apply()` to also call `loan->recordPayment(amount)`. |

`LoggingVisitor` is a concrete `TransactionVisitor` subclass that writes to a `std::ostream&`. It is **not** part of SCC B (no cycle member references it).

### SCC C / mega-SCC (size 7 + A's 2 = 9)

| Class | Owns | Refs (non-owning) |
|---|---|---|
| `Bank` | `vector<Customer*>`, `vector<BranchManager*>`, `AuditLogger*`, `NotificationCenter*` | — |
| `Customer` | `vector<Account*>`, `vector<Loan*>` | `Bank*` owner |
| `Loan` | — | `Customer* borrower`, `Account* repaymentAccount`, `RiskAnalyzer*` |
| `AuditLogger` | — | `Bank*` |
| `NotificationCenter` | — | `vector<Customer*>`, `AuditLogger*` |
| `BranchManager` | `RiskAnalyzer*` | `Bank*`, `vector<Customer*>` |
| `RiskAnalyzer` | — | `Bank*`, `AuditLogger*` |

### SCC D — Reporting pipeline (size 5, isolated)

| Class | Owns | Refs (non-owning) |
|---|---|---|
| `ReportEngine` | `ReportFilter*`, `ReportWriter*` | — |
| `ReportFilter` | — | `ReportEngine*` |
| `ReportSection` | — | `ReportFilter*`, `ReportFormatter*` (created locally by `ReportWriter::write`) |
| `ReportFormatter` | — | `ReportSection*` (the most recently formatted) |
| `ReportWriter` | `ReportFormatter*` | `ReportEngine*` |

Additional cross-partner methods added by the complexity-enhancement layer:
`ReportEngine::consolidate(ReportEngine*, ReportFilter*, ReportWriter*)`,
`ReportEngine::partition(ReportSection*, ReportFormatter*) → std::pair<ReportEngine*, ReportEngine*>`,
`ReportSection::applyFilter(ReportFilter*, ReportFormatter*) const → std::vector<ReportSection*>`,
`ReportSection::mergeWith(ReportSection*, ReportFormatter*)`,
`ReportWriter::batchWrite(const std::vector<ReportSection*>&, ReportFilter*, ReportEngine*)`.

**Isolation invariant**: zero references between SCC D and any class in SCC A/B/C/E.

### SCC E — Credit scoring (size 4, isolated, Tier-B stub demo)

| Class | Owns | Refs (non-owning) |
|---|---|---|
| `ScoreCard` | — | `ObligationMatrix*`, `WeightingEngine*`, `TierClassifier*` |
| `ObligationMatrix` | — | `std::map<int, std::vector<ScoreCard*> >`, `WeightingEngine*`, `TierClassifier*` |
| `WeightingEngine` | — | `ObligationMatrix*`, `TierClassifier*` |
| `TierClassifier` | — | `ObligationMatrix*`, `WeightingEngine*` (set by `bind()`) |

Cross-partner methods include
`ScoreCard::recalibrate(ObligationMatrix*, WeightingEngine*)`,
`ScoreCard::currentTier(TierClassifier*, ObligationMatrix*) const`,
`ObligationMatrix::computeObligations(ScoreCard*, WeightingEngine*) const`,
`ObligationMatrix::propagate(ScoreCard*, WeightingEngine*)`,
`WeightingEngine::reweightMatrix(ObligationMatrix*, double) → std::vector<ScoreCard*>`,
`WeightingEngine::calibrate(ObligationMatrix*, ScoreCard*)`,
`TierClassifier::batchClassify(ObligationMatrix*, WeightingEngine*) → std::map<int, std::vector<ScoreCard*> >`,
`TierClassifier::recalibrate(WeightingEngine*, ScoreCard*)`.

**Isolation invariant**: zero references between SCC E and any class in SCC A/B/C/D.
Stored back-pointers in all four nodes guarantee the 4-node cycle is visible
in the include graph — not only via method parameters. This makes SCC E a
Tier-B candidate for the chunker's stub-generation path.

### Acyclic baseline (Tier-A)

`Constants`, `Enums`, `Globals`, `Utils`, `BondCalculator`,
`LoggingVisitor`, `TransactionBase`, `AgeVerifier`.

`AgeVerifier` is the host-side wrapper for the native `age_verifier`
library under `native/` (loaded at runtime via `dlopen` /
`LoadLibrary`); see [`NATIVE_LIBRARY.md`](NATIVE_LIBRARY.md).

## Type aliases

```cpp
// Constants.h — primitive domain aliases
typedef double      Money;
typedef std::string AccountId;
typedef std::string LoanId;
typedef std::string CustomerId;

// Account.h — container + iterator + self typedef
typedef std::vector<Transaction*>           TransactionHistory;
typedef TransactionHistory::iterator        TxHistoryIterator;
typedef TransactionHistory::const_iterator  TxHistoryConstIterator;
class Account {
    typedef Account self_type;   // STL-style "self typedef"
    ...
};

// BondCalculator.h — "typedef struct" C-style alias
typedef struct BondCashFlow BondCashFlow_t;

// Enums.h — "typedef enum" aliases
typedef enum AccountType AccountKind;
typedef enum LoanStatus  LoanState;

// AgeVerifier.cpp — function-pointer typedef
typedef int (*VerifyFn)(int, int, int);
```

## Enums (`Enums.h`)

```cpp
enum AccountType     { SAVINGS, CHECKING, LOAN, MORTGAGE }      // legacy; unused by new SCC code
enum TransactionType { DEPOSIT, WITHDRAWAL, TRANSFER, INTEREST, FEE, EMI_PAYMENT }
enum TransactionStatus { PENDING, COMPLETED, FAILED, REVERSED }
enum CustomerStatus  { ACTIVE, INACTIVE, SUSPENDED, CLOSED }
enum LoanStatus      { LOAN_ACTIVE, PAID_OFF, DEFAULTED, PENDING_APPROVAL }
```

## Build flags

```
-std=c++03 -Wall -Wextra -I./include
```

Zero warnings required.

## Ownership / cascade-delete

```
delete bank;       // cascade through Customer → Account → Transaction
                   // and through BranchManager → RiskAnalyzer
                   // and AuditLogger + NotificationCenter
delete engine;     // cascade through Filter, Writer → Formatter
```

Back-pointers are all non-owning; never `delete` through them.

## C++03 rules (hard constraint)

| Forbidden | Use instead |
|---|---|
| `nullptr` | `NULL` |
| `auto`, range-based `for` | explicit `std::vector<T>::iterator` |
| `using X = ...` | `typedef ... X;` |
| `std::unique_ptr` / `std::shared_ptr` | raw `new`/`delete` with documented owner |
| `enum class` | plain `enum` |
| `override`, `final`, `noexcept` | omit (rely on `virtual`) |
| lambdas, `std::function` | function pointers (`int (*)(int,int)`) |

## ID generation

Customer/account IDs in the new scenarios are hand-rolled strings (e.g. `"C0001"`, `"A1001"`, `"L0001"`). The legacy `Utils::generateCustomerId()` / `generateAccountId()` helpers still exist and produce `CUST000xxx` / `ACC000xxx` patterns but aren't called by the new menu.

## SCC isolation checks

```bash
# SCC D must not reference mega-SCC / SCC B / SCC E
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|ScoreCard|ObligationMatrix|WeightingEngine|TierClassifier' \
    include/Report*.h src/Report*.cpp
# must print nothing

# SCC E must not reference mega-SCC / SCC B / SCC D
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|Report' \
    include/ScoreCard.h include/ObligationMatrix.h include/WeightingEngine.h include/TierClassifier.h \
    src/ScoreCard.cpp src/ObligationMatrix.cpp src/WeightingEngine.cpp src/TierClassifier.cpp
# must print nothing
```

If anything matches, SCC D or SCC E will fuse into the mega-SCC and the demo is broken.
