# START HERE

This codebase is a fixture for the `java_cpp_chunkagent` plugin. Its
`#include`/method-call graph is engineered to contain exactly **three
cyclic SCCs**. The bank-domain code is just the carrier; the topology
is the deliverable.

## In 60 seconds

```bash
make                                          # builds with -std=c++03 -Wall -Wextra
./BankSystem                                  # 3-option menu
printf '1\n\n2\n\n0\n' | ./BankSystem         # scripted exercise of menu 1 + 2
```

## Read order

1. `SCC_DEMO_LAYOUT.md` — **load-bearing**. Three-SCC diagram, edge inventory, isolation invariant for SCC D. Read this first if you only read one doc.
2. `README.md` — project structure, build, what each menu option does.
3. `QUICK_REFERENCE.md` — class taxonomy table, ownership, C++03 rules.
4. `TESTING_GUIDE.md` — scripted stdin sequences for the 3 scenarios.
5. `TEST_CASES.md` — pass criteria per scenario.

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
- **SCC D headers/.cpp may not name any SCC A/B/C class**. Check with the grep one-liner in `QUICK_REFERENCE.md`.
- **Update `SCC_DEMO_LAYOUT.md`** whenever you change edges; it's the ground truth used by chunker validation.

## What's where

```
include/   24 headers (4 SCC clusters + acyclic baseline)
src/       24 .cpp + main.cpp
docs/      this file + 6 others
Makefile   explicit SOURCES list (no glob); add new .cpp here
```

## Quick health check

```bash
make clean && make all
# expected: builds clean with zero warnings/errors

grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer' \
    include/Report*.h src/Report*.cpp
# expected: no output (SCC D isolation invariant)
```
