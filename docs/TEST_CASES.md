# Test Cases — SCC Demo

The interactive console exposes three menu options. Each is a scripted
scenario; this file lists pass criteria per scenario. The actual stdin
sequences live in `TESTING_GUIDE.md`.

For dependency-graph ground truth (what the chunker should report on
this codebase), see `SCC_DEMO_LAYOUT.md`.

---

## Scenario 1 — New bank flow (menu option 1)

Exercises the mega-SCC end-to-end and runs every `Transaction` subclass
through both `apply()` (mega-SCC edge: `Transaction → Account`) and
`LoggingVisitor` (SCC B edge: `Deposit → TransactionVisitor`).

### TC-1.1: Bank construction prints registration notices

**Steps**: choose option `1`.
**Expected**: lines like
```
[NC] -> Alice Smith: [Demo Bank] Customer registered: Alice Smith
[NC] -> Alice Smith: [Demo Bank] Customer registered: Bob Jones
[NC] -> Bob Jones:   [Demo Bank] Customer registered: Bob Jones
```
Both customers receive every broadcast (subscribers list is global, by
design — see SCC C mediator mesh).

### TC-1.2: All four transaction types appear in the visitor log

**Expected**: four `[LOG] ...` lines, exactly one per concrete subclass:
```
[LOG] DEPOSIT  #1 amount=$1000.00 to=A1001
[LOG] DEPOSIT  #1 amount=$500.00 to=A2001
[LOG] TRANSFER #2 amount=$250.00 from=A1001 to=A1002
[LOG] WITHDRAW #2 amount=$100.00 from=A2001
```
Plus one `[LOG] LOANPAY ... from=A1001` after the loan-payment step.

### TC-1.3: Loan approval flow

**Expected** (in order):
- `[LOG]`-style not emitted for `Loan` directly — instead audit lines `LOAN_APPROVED id=L0001 ...`.
- `Risk assessed for loan L0001` line in the audit dump.
- `[NC] -> ... Loan approved for Alice Smith` broadcast.

### TC-1.4: Final balances are correct

After `Deposit($1000)` to `A1001`, `Transfer($250)` from `A1001→A1002`, and `LoanPayment($200)` from `A1001`:

| Account | Expected balance |
|---|---|
| A1001 (Alice/Chk) | $550.00 |
| A1002 (Alice/Sav) | $250.00 |
| A2001 (Bob/Chk)   | $400.00 |
| Loan L0001 outstanding | $4800.00 |

### TC-1.5: Cascade delete is clean

**Expected**: the trailing line `(Bank dismantled cleanly.)` prints, and
there is no segfault, double-free, or use-after-free. (Use `valgrind
./BankSystem` if you want stronger evidence than "no crash".)

---

## Scenario 2 — Generate report (menu option 2)

Exercises SCC D's full pipeline cycle and verifies the back-callback
`ReportWriter → ReportEngine::onPageReady` closes correctly.

### TC-2.1: Both report sections render

**Expected**:
```
=== Generating report for 2026-01-01 .. 2026-03-31 ===
  -- Section: Page 1
     [rendered]
     style=plain-text
  -- Section: Page 2
     [rendered]
     style=plain-text
=== Report complete: pages=2 ===
```

### TC-2.2: Engine page counter advances via the back-callback

The line `Report complete: pages=2` proves `ReportEngine::onPageReady`
was invoked twice — i.e. the cycle
`Engine → Writer → Section → (callback) → Engine` closed.

### TC-2.3: Cascade delete is clean

**Expected**: trailing `(Report engine torn down.)`. No crash. Order is
`delete engine;` → deletes `filter`, `writer`; `writer` deletes
`formatter`; section objects were stack-allocated inside
`ReportWriter::write` and already destroyed.

---

## Scenario 3 — Calculate bond parameters (menu option 3)

Drives the acyclic baseline `BondCalculator`. Not part of any SCC.

### TC-3.1: PV/FV print for valid inputs

**Inputs** (one per prompt): `100`, `12`, `5`, `2`, `3`.
**Expected**: two lines like `PV: $...` and `FV: $...`. Numbers vary
because the coupon stream uses `std::rand()` seeded by `std::time(NULL)`
— so check the *shape*, not the exact value.

### TC-3.2: Random-rate cap clamps to 3%

Inputting `5` for the max random rate should silently clamp to `3.0`
(per the `if (maxRandPct > 3.0) maxRandPct = 3.0` line in `main.cpp`).
No error, just a cap.

### TC-3.3: Exception path

If `BondCalculator` ctor throws (e.g. negative nominal would, although
`getValidatedAmount` already rejects non-positive input), the catch
block in `runBondCalc` prints `Error: <what>`.

---

## SCC topology test (chunker ground truth)

This is the actual deliverable of the project — verifying it requires
running the `java_cpp_chunkagent` chunker against this tree.

### TC-4.1: Three cyclic SCCs reported

`DependencyGraphService.get_cyclic_sccs()` must return exactly three
SCCs:
- mega-SCC of size 9 (`Account`, `Transaction`, `Bank`, `Customer`,
  `Loan`, `AuditLogger`, `NotificationCenter`, `BranchManager`,
  `RiskAnalyzer`).
- SCC of size 5 (`TransactionVisitor`, `Deposit`, `Withdrawal`,
  `Transfer`, `LoanPayment`).
- SCC of size 5 (`ReportEngine`, `ReportFilter`, `ReportSection`,
  `ReportFormatter`, `ReportWriter`).

### TC-4.2: SCC D isolation invariant

```bash
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer' \
    include/Report*.h src/Report*.cpp
```

**Expected**: no output. If anything matches, SCC D will fuse into the
mega-SCC and TC-4.1 fails.

### TC-4.3: Acyclic baseline is acyclic

`Utils`, `Globals`, `Constants`, `Enums`, `BondCalculator`, and
`LoggingVisitor` must appear in `dependency_graph.json` outside any
SCC (or in a singleton SCC of size 1, depending on chunker
representation).

---

## Build test

### TC-5.1: Clean build with zero warnings

```bash
make clean && make all 2>&1 | grep -cE 'warning|error'
# expected: 0
```

### TC-5.2: Binary runs and exits cleanly via menu option 0

```bash
printf '0\n' | ./BankSystem
# expected: exit code 0, trailing "Goodbye." line
```
