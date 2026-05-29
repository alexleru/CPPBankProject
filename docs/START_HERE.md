# START HERE

This codebase is a fixture for the `java_cpp_chunkagent` plugin. Its
`#include`/method-call graph is engineered to contain exactly **four
cyclic SCCs**. The bank-domain code is just the carrier; the topology
is the deliverable.

## In 60 seconds

```bash
make                                          # builds native lib + ./BankSystem with -std=c++03 -Wall -Wextra
./BankSystem                                  # 4-option menu
printf '1\n\n2\n\n0\n' | ./BankSystem         # scripted exercise of menu 1 + 2
```

## Read order

1. `SCC_DEMO_LAYOUT.md` — **load-bearing**. Four-SCC diagram, edge inventory, isolation invariants for SCC D and SCC E. Read this first if you only read one doc.
2. `README.md` — project structure, build, what each menu option does.
3. `QUICK_REFERENCE.md` — class taxonomy table, ownership, C++03 rules.
4. `CPP_BANK_PROJECT_COMPLEXITY_ENHANCEMENT.md` — why SCC E exists and what extra cross-partner methods SCC D / RiskAnalyzer carry (stub-demo rationale).
5. `TESTING_GUIDE.md` — scripted stdin sequences for the 4 scenarios.
6. `TEST_CASES.md` — pass criteria per scenario.
7. `NATIVE_LIBRARY.md` — only relevant when touching menu option 4 / the `native/` tree.

## By role

| Role | Read these |
|---|---|
| New contributor | `SCC_DEMO_LAYOUT.md` → `README.md` → `QUICK_REFERENCE.md` |
| Running tests | `TESTING_GUIDE.md` + `TEST_CASES.md` |
| Validating chunker output against ground truth | `SCC_DEMO_LAYOUT.md` (expected `dependency_graph.json` section) |
| Modifying code | `../CLAUDE.md` (C++03 rules, ownership map) |

## Hard rules before editing

- **C++03 only**. No `nullptr`, no `auto`, no smart pointers, no `enum class`, no lambdas, no `std::function`.
- **Build must stay warning-free** under `-std=c++03 -Wall -Wextra`.
- **Don't put `accept(TransactionVisitor&)` on the `Transaction` base** — that collapses SCC B into the mega-SCC and breaks the demo.
- **SCC D headers/.cpp may not name any class from mega-SCC / SCC B / SCC E**. Check with the grep one-liner in `QUICK_REFERENCE.md`.
- **SCC E headers/.cpp may not name any class from mega-SCC / SCC B / SCC D**. Check with the grep one-liner in `QUICK_REFERENCE.md`. If `TierClassifier` loses its stored `ObligationMatrix*` / `WeightingEngine*` back-pointers, the 4-node SCC may collapse to 3 nodes.
- **Update `SCC_DEMO_LAYOUT.md`** whenever you change edges; it's the ground truth used by chunker validation.

## What's where

```
include/   31 headers (4 cyclic SCCs + acyclic baseline incl. TransactionBase / AgeVerifier)
src/       29 .cpp + main.cpp
native/    cross-platform age_verifier shared library (loaded at runtime)
docs/      this file + others
Makefile   explicit SOURCES list (no glob); add new .cpp here
```

## Quick health check

```bash
make clean && make all
# expected: builds clean with zero warnings/errors

grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|ScoreCard|ObligationMatrix|WeightingEngine|TierClassifier' \
    include/Report*.h src/Report*.cpp
# expected: no output (SCC D isolation invariant)

grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|Report' \
    include/ScoreCard.h include/ObligationMatrix.h include/WeightingEngine.h include/TierClassifier.h \
    src/ScoreCard.cpp src/ObligationMatrix.cpp src/WeightingEngine.cpp src/TierClassifier.cpp
# expected: no output (SCC E isolation invariant)
```
