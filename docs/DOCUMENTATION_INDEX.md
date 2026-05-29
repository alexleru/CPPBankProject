# Documentation Index

All documentation for the SCC-demo build of this project.

## Files

| File | Purpose | When to use |
|---|---|---|
| `SCC_DEMO_LAYOUT.md` | **Load-bearing**: 4-SCC diagram, edge inventory, expected `dependency_graph.json`, SCC D + SCC E isolation invariants. | First read for anyone validating the chunker output. Update whenever edges change. |
| `SCC_DEMO_PROJECT_PROMPT.md` | Original Russian-language spec that drove the SCC layout. | Historical reference; useful when reasoning about why a structural decision was made. |
| `CPP_BANK_PROJECT_COMPLEXITY_ENHANCEMENT.md` | Spec for the complexity-enhancement layer: extra cross-partner methods on SCC D + RiskAnalyzer, plus the 4-node credit-scoring SCC E (`ScoreCard`/`ObligationMatrix`/`WeightingEngine`/`TierClassifier`). | When reasoning about why SCC E exists and why SCC D / RiskAnalyzer carry the extra method surface. |
| `README.md` | Project overview, build, menu options, history, full C++03 construct inventory. | First read for anyone using the project. |
| `QUICK_REFERENCE.md` | Class taxonomy, ownership, type aliases, C++03 rules, isolation check. | Lookup during editing. |
| `START_HERE.md` | One-page orientation, "what to read in what order". | Newcomer onboarding. |
| `TEST_CASES.md` | Pass criteria per scenario. | Validating a build by hand. |
| `TESTING_GUIDE.md` | Scripted stdin sequences for the 4 menu options. | Running scripted smoke tests. |
| `NATIVE_LIBRARY.md` | C ABI, build, and runtime-loading story for the cross-platform `age_verifier` shared library (menu option 4). | Only when touching `native/` or `AgeVerifier`. |

## SCC topology (summary)

| SCC | Size | Tier (chunker) | Members |
|---|---|---|---|
| mega (A ∪ C) | 9 | C/D | `Account`, `Transaction`, `Bank`, `Customer`, `Loan`, `AuditLogger`, `NotificationCenter`, `BranchManager`, `RiskAnalyzer` |
| visitor (B) | 5 | B | `TransactionVisitor`, `Deposit`, `Withdrawal`, `Transfer`, `LoanPayment` |
| report (D) | 5 | B | `ReportEngine`, `ReportFilter`, `ReportSection`, `ReportFormatter`, `ReportWriter` |
| credit (E) | 4 | B | `ScoreCard`, `ObligationMatrix`, `WeightingEngine`, `TierClassifier` |

Acyclic baseline (Tier-A): `Constants`, `Enums`, `Globals`, `Utils`, `BondCalculator`, `LoggingVisitor`, `TransactionBase`, `AgeVerifier`.

## Cross-reference

| Question | Doc |
|---|---|
| What does SCC X contain? | `SCC_DEMO_LAYOUT.md` |
| How do I build? | `README.md` (Build section) |
| What's the ownership rule for class X? | `QUICK_REFERENCE.md` (taxonomy table) |
| What does menu option N do? | `README.md` (Menu) or `QUICK_REFERENCE.md` (Menu) |
| How do I script a smoke test? | `TESTING_GUIDE.md` |
| What's the pass criterion for scenario X? | `TEST_CASES.md` |
| Can I use `nullptr`? | `CLAUDE.md` (C++03 constraints) — no, use `NULL` |
| Did I just break SCC D isolation? | `QUICK_REFERENCE.md` (grep one-liner) |
| Did I just break SCC E isolation? | `QUICK_REFERENCE.md` (grep one-liner) |
| Why does SCC E exist with non-banking class names? | `CPP_BANK_PROJECT_COMPLEXITY_ENHANCEMENT.md` (stub-demo rationale) |

## Validation checklist

Before shipping a change, confirm:

- [ ] `make clean && make all` finishes with zero warnings (builds native lib + executable).
- [ ] `./BankSystem` runs the new-bank flow without segfault; cascade-delete prints "Bank dismantled cleanly". Audit summary + `dynamic_cast` breakdown + friend back-door dump all appear.
- [ ] `./BankSystem` runs the report flow; pipeline completes; engine torn down cleanly.
- [ ] `./BankSystem` runs menu option 4 against the native lib (`OK` for an old date, `LIB ERROR` if the lib was removed).
- [ ] SCC D isolation grep produces zero output.
- [ ] SCC E isolation grep produces zero output.
- [ ] If edges changed, `SCC_DEMO_LAYOUT.md` was updated in the same commit.

## Project metadata

- **Standard**: C++03
- **Build flags**: `-std=c++03 -Wall -Wextra -I./include`
- **Executable**: `./BankSystem`
- **Status**: SCC-demo build, post-rewrite from prior account-hierarchy demo
