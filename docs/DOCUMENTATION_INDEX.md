# Documentation Index

All documentation for the SCC-demo build of this project.

## Files

| File | Purpose | When to use |
|---|---|---|
| `SCC_DEMO_LAYOUT.md` | **Load-bearing**: 3-SCC diagram, edge inventory, expected `dependency_graph.json`, SCC D isolation invariant. | First read for anyone validating the chunker output. Update whenever edges change. |
| `SCC_DEMO_PROJECT_PROMPT.md` | Original Russian-language spec that drove the SCC layout. | Historical reference; useful when reasoning about why a structural decision was made. |
| `README.md` | Project overview, build, menu options, history. | First read for anyone using the project. |
| `QUICK_REFERENCE.md` | Class taxonomy, ownership, type aliases, C++03 rules, isolation check. | Lookup during editing. |
| `START_HERE.md` | One-page orientation, "what to read in what order". | Newcomer onboarding. |
| `TEST_CASES.md` | Pass criteria per scenario. | Validating a build by hand. |
| `TESTING_GUIDE.md` | Scripted stdin sequences for the 3 menu options. | Running scripted smoke tests. |

## SCC topology (summary)

| SCC | Size | Tier (chunker) | Members |
|---|---|---|---|
| mega (A ∪ C) | 9 | C/D | `Account`, `Transaction`, `Bank`, `Customer`, `Loan`, `AuditLogger`, `NotificationCenter`, `BranchManager`, `RiskAnalyzer` |
| visitor (B) | 5 | B | `TransactionVisitor`, `Deposit`, `Withdrawal`, `Transfer`, `LoanPayment` |
| report (D) | 5 | B | `ReportEngine`, `ReportFilter`, `ReportSection`, `ReportFormatter`, `ReportWriter` |

Acyclic baseline (Tier-A): `Constants`, `Enums`, `Globals`, `Utils`, `BondCalculator`, `LoggingVisitor`.

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

## Validation checklist

Before shipping a change, confirm:

- [ ] `make clean && make all` finishes with zero warnings.
- [ ] `./BankSystem` runs the new-bank flow without segfault; cascade-delete prints "Bank dismantled cleanly".
- [ ] `./BankSystem` runs the report flow; pipeline completes; engine torn down cleanly.
- [ ] SCC D isolation grep produces zero output.
- [ ] If edges changed, `SCC_DEMO_LAYOUT.md` was updated in the same commit.

## Project metadata

- **Standard**: C++03
- **Build flags**: `-std=c++03 -Wall -Wextra -I./include`
- **Executable**: `./BankSystem`
- **Status**: SCC-demo build, post-rewrite from prior account-hierarchy demo
