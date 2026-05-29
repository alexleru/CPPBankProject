# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this project is

A deliberate fixture for the `java_cpp_chunkagent` plugin (C++ → Java/Spring converter). The dependency graph is engineered so that a chunker analysing `#include`s and method-call edges finds **exactly four cyclic strongly-connected components**, of curated sizes and tiers. The bank-domain code is incidental — the SCC topology is the point.

See `docs/SCC_DEMO_LAYOUT.md` for the authoritative description of the SCCs, edges, and isolation invariants. See `docs/SCC_DEMO_PROJECT_PROMPT.md` for the original spec that drove the layout. The complexity-enhancement layer (extra cross-partner methods on SCC D + RiskAnalyzer, plus the new credit-scoring SCC) is described in `docs/CPP_BANK_PROJECT_COMPLEXITY_ENHANCEMENT.md`.

## Build & Run

Single hand-written cross-platform Makefile (no CMake, no test framework).

```bash
make            # builds the native age_verifier library AND ./BankSystem (BankSystem.exe on Windows MinGW)
make native     # builds only the platform-appropriate native library
make clean      # removes build/ and the executable; keeps the native lib
make clean-all  # also deletes the built native lib under native/{linux,windows}/
./BankSystem    # 4-option interactive menu
```

Manual compile (note the native lib is built separately and `-ldl` is required on Linux):

```bash
g++ -std=c++03 -O2 -Wall -fPIC -shared -o native/linux/libage_verifier.so native/src/age_verifier.cpp
g++ -std=c++03 -Wall -Wextra -I./include -o BankSystem src/*.cpp -ldl
```

If you add a new `.cpp` under `src/`, append it to the explicit `SOURCES` list in the `Makefile` — it does not glob.

## C++03 is a hard constraint

Compiled with `-std=c++03 -Wall -Wextra`, zero warnings. Do not introduce C++11+ features:

- `nullptr` → use `NULL`
- `auto`, range-based `for` → use explicit iterators
- `using` type aliases → use `typedef`
- `std::unique_ptr` / `std::shared_ptr` → raw `new`/`delete`, owners documented in headers
- `enum class`, `override`, `final`, `noexcept`, lambdas, `std::function` → forbidden

The whole point of this codebase is studying C++03 → Java porting, so the dialect is load-bearing.

## Architecture: five labelled SCCs (four cyclic at the topology level)

| SCC | Members | Size | Files |
|---|---|---|---|
| **A** (Account ↔ Transaction) | `Account`, `Transaction` | 2 | merges into mega-SCC via Account back-pointers |
| **C** (mediator/observer mesh) | `Bank`, `Customer`, `Loan`, `AuditLogger`, `NotificationCenter`, `BranchManager`, `RiskAnalyzer` | 7 | merges with A → mega-SCC of 9 |
| **B** (Visitor) | `TransactionVisitor`, `Deposit`, `Withdrawal`, `Transfer`, `LoanPayment` | 5 | separate SCC; concrete `LoggingVisitor` is acyclic |
| **D** (Reporting pipeline) | `ReportEngine`, `ReportFilter`, `ReportSection`, `ReportFormatter`, `ReportWriter` | 5 | **fully isolated** from A/B/C/E |
| **E** (Credit scoring) | `ScoreCard`, `ObligationMatrix`, `WeightingEngine`, `TierClassifier` | 4 | **fully isolated** from A/B/C/D — Tier-B, used to exercise the chunker's stub mechanism |

**Acyclic baseline** (Tier-A in chunker terms): `Utils`, `Globals`, `Constants`, `Enums`, `BondCalculator`, `LoggingVisitor`, `AgeVerifier`, `TransactionBase`.

`AgeVerifier` (`include/AgeVerifier.h`, `src/AgeVerifier.cpp`) is a thin RAII wrapper around the cross-platform `age_verifier` native library under `native/` (loaded at runtime via `LoadLibrary` / `dlopen`). It references no SCC A/B/C/D class and must stay that way — keep the wrapper isolated so it never gets pulled into the mega-SCC.

`TransactionBase` (`include/TransactionBase.h`, `src/TransactionBase.cpp`) is an acyclic parent of `Transaction` that owns a process-wide `unsigned long instanceCounter` and gives every transaction a unique `instanceId`. Its sole purpose is to give the project a real 3-level inheritance chain `Deposit/Withdrawal/Transfer/LoanPayment → Transaction → TransactionBase` without introducing a new cycle. It references nothing in SCC A/B/C/D/E — keep it that way.

### Why SCC B does NOT merge into the mega-SCC

`Transaction` (mega-SCC) does **not** declare `accept(TransactionVisitor&)` and does **not** reference `TransactionVisitor`. Only the four concrete subclasses do. The inheritance edge `Deposit → Transaction` (etc.) is one-way. If you ever move `accept()` up to the base, SCC B collapses into the mega-SCC and the demo breaks.

### Why SCC D must stay isolated

No header or `.cpp` under SCC D may name any class from SCC A/B/C/E. Verify with:

```bash
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|ScoreCard|ObligationMatrix|WeightingEngine|TierClassifier' \
    include/Report*.h src/Report*.cpp
# expected: no output
```

If anything matches, the isolation invariant is broken and the chunker will fuse SCC D into the mega-SCC.

### Why SCC E (credit scoring) must stay isolated

`ScoreCard`, `ObligationMatrix`, `WeightingEngine`, `TierClassifier` form a 4-node cycle via stored back-pointers. They must not reference any class in mega-SCC / B / D, or the chunker will fuse them into the mega-SCC and the 4-node Tier-B SCC (which exercises the stub-generation path) is lost. Verify with:

```bash
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|Report' \
    include/ScoreCard.h include/ObligationMatrix.h include/WeightingEngine.h include/TierClassifier.h \
    src/ScoreCard.cpp src/ObligationMatrix.cpp src/WeightingEngine.cpp src/TierClassifier.cpp
# expected: no output
```

## Ownership map

`delete bank;` in `main.cpp` must cascade through the entire object graph without leaks or double-frees. Preserve this when editing:

| Owner | Owns |
|---|---|
| `Bank` | `vector<Customer*>`, `vector<BranchManager*>`, `AuditLogger*`, `NotificationCenter*` |
| `Customer` | `vector<Account*>`, `vector<Loan*>` |
| `Account` | `vector<Transaction*>` |
| `BranchManager` | `RiskAnalyzer*` |
| `ReportEngine` | `ReportFilter*`, `ReportWriter*` |
| `ReportWriter` | `ReportFormatter*` |

Everything else is a non-owning back-pointer.

## Globals

`Globals.h` declares extern counters and config flags (`globalCustomerCounter`, `enableDebugLogging`, …) defined once in `Globals.cpp` and initialised via `initializeGlobals()` from `main`. They are present but lightly used by the new SCC scenarios.

## `main.cpp` menu

Four options, all exit with `0`:

1. **New bank flow** — exercises the mega-SCC end-to-end: creates `Bank` + mediators, registers customers, opens accounts, runs `Deposit`/`Withdrawal`/`Transfer`/`LoanPayment` through both `apply()` and `LoggingVisitor`, approves a `Loan` via `BranchManager`, broadcasts via `NotificationCenter`, dumps the audit log, then `delete bank;` (cascade).
2. **Generate report** — exercises SCC D: builds the engine/filter/formatter/writer pipeline, calls `engine->generate(std::cout)`, cascade-deletes via `delete engine;`.
3. **Calculate bond parameters** — drives `BondCalculator` (acyclic baseline).
4. **Verify age 21+** — constructs an `AgeVerifier`, which dynamically loads the native library and calls the C-ABI `verify_age_21` symbol. Prints `OK` / `UNDER` / `BAD INPUT` / `LIB ERROR`. See `docs/NATIVE_LIBRARY.md`.

## Testing

There is no unit-test framework. Testing is scripting stdin into the interactive console. See `docs/TESTING_GUIDE.md` for the 4 scripted scenarios (one per menu option). The dependency-graph topology itself is also a test target — see `docs/SCC_DEMO_LAYOUT.md` for the expected shape.

## Documentation map

- `docs/SCC_DEMO_LAYOUT.md` — **load-bearing**: 3-SCC diagram, edge inventory, isolation invariant for SCC D.
- `docs/SCC_DEMO_PROJECT_PROMPT.md` — original Russian spec.
- `docs/README.md` — project overview and build steps.
- `docs/QUICK_REFERENCE.md` — class taxonomy + menu options.
- `docs/TEST_CASES.md` — test cases organised per menu scenario.
- `docs/TESTING_GUIDE.md` — scripted stdin sequences for each scenario.
- `CPP_Language_Constructs.md` — inventory of every C++03 construct used (helpful when wondering "is X idiomatic here?").
- `CPP_Reports_Comparison.md` — meta-doc; not load-bearing.
- `docs/NATIVE_LIBRARY.md` — cross-platform `age_verifier` library (ABI, build, menu option 4).
