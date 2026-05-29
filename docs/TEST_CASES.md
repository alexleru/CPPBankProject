# Test Cases — SCC Demo

The interactive console exposes four menu options. Each is a scripted
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

### TC-1.5: Audit-event summary

**Expected**: after the regular audit dump, a block of the form

```
[AUDIT SUMMARY] 4 event type(s):
  - ACCOUNT_CREATED : 3
  - CUSTOMER_REGISTERED : 2
  - LOAN_APPROVED : 2
  - LOG : 4
```

prints. Exact counts depend on the scenario script; the
`AuditLogger::notify` path is not hit by this scenario, so `NOTIFY`
does not appear here. Exercises `std::map<std::string, unsigned
long>::find` / `operator[]` / `const_iterator` iteration in
`AuditLogger::dumpEventSummary`.

### TC-1.6: `dynamic_cast` breakdown of transaction history

**Expected**: a `-- Transaction breakdown (via dynamic_cast) --`
heading followed by one line per `Transaction*` in Alice's
checking-account history, identifying the concrete subclass
(`Deposit` / `Withdraw` / `Transfer` / `LoanPay`) and printing the
per-account `sequence` plus the process-wide `TransactionBase`
`instanceId` (`iid=...`). The block ends with
`(Total Transaction instances ever created: N)`.

### TC-1.7: Friend-function back-door

**Expected**: a `[DEBUG Account] id=A1001 balance=... txCount=N
holder=<set> bank=<set> logger=<set>` line emitted by
`debugDumpAccount(*aliceChk, std::cout)`. Proves the friend
declaration grants free-function access to private members of
`Account`.

### TC-1.8: Cascade delete is clean

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

If `BondCalculator` ctor throws `std::invalid_argument`, the nested
inner catch logs `[BondCalc] invalid input: <what>` and re-throws via
bare `throw;`; the outer catch then prints `Error: <what>`. Tests
both the nested-catch pattern and bare-re-throw semantics.

---

## Scenario 4 — Verify age 21+ (menu option 4)

Drives the acyclic `AgeVerifier` wrapper, which dynamically loads the
native `age_verifier` library via `dlopen` / `LoadLibrary` and calls
the C-ABI `verify_age_21` symbol. Not part of any SCC.

### TC-4.1: OK path (age ≥ 21)

**Inputs**: `5`, `6`, `1990` (day / month / year).
**Expected**: `Result: OK -- subject is 21 or older.`.

### TC-4.2: UNDER path (age < 21)

**Inputs**: a date within the last 21 years, e.g. `5 / 6 / 2020`.
**Expected**: `Result: UNDER -- subject is younger than 21.`.

### TC-4.3: BAD INPUT path (invalid date)

**Inputs**: `31`, `2`, `2000` (31 February).
**Expected**: `Result: BAD INPUT -- not a real calendar date.`.

### TC-4.4: LIB ERROR path (native library missing)

**Setup**: `make clean-all` (or delete
`native/linux/libage_verifier.so` / `native\windows\age_verifier.dll`),
then run option 4.
**Expected**: `Result: LIB ERROR -- LoadLibrary(...)` /
`Result: LIB ERROR -- dlopen(...)` with the wrapper's error message.
Run `make native` to restore.

---

## SCC topology test (chunker ground truth)

This is the actual deliverable of the project — verifying it requires
running the `java_cpp_chunkagent` chunker against this tree.

### TC-5.1: Four cyclic SCCs reported

`DependencyGraphService.get_cyclic_sccs()` must return exactly four
SCCs:

- mega-SCC of size 9 (`Account`, `Transaction`, `Bank`, `Customer`,
  `Loan`, `AuditLogger`, `NotificationCenter`, `BranchManager`,
  `RiskAnalyzer`).
- SCC of size 5 (`TransactionVisitor`, `Deposit`, `Withdrawal`,
  `Transfer`, `LoanPayment`).
- SCC of size 5 (`ReportEngine`, `ReportFilter`, `ReportSection`,
  `ReportFormatter`, `ReportWriter`).
- SCC of size 4 (`ScoreCard`, `ObligationMatrix`, `WeightingEngine`,
  `TierClassifier`) — the Tier-B stub-demo cycle.

### TC-5.2: SCC D isolation invariant

```bash
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|ScoreCard|ObligationMatrix|WeightingEngine|TierClassifier' \
    include/Report*.h src/Report*.cpp
```

**Expected**: no output. If anything matches, SCC D will fuse into the
mega-SCC and TC-5.1 fails.

### TC-5.3: SCC E isolation invariant

```bash
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|Report' \
    include/ScoreCard.h include/ObligationMatrix.h include/WeightingEngine.h include/TierClassifier.h \
    src/ScoreCard.cpp src/ObligationMatrix.cpp src/WeightingEngine.cpp src/TierClassifier.cpp
```

**Expected**: no output. If anything matches, SCC E will fuse into the
mega-SCC and the 4-node Tier-B stub demo collapses.

### TC-5.4: Acyclic baseline is acyclic

`Utils`, `Globals`, `Constants`, `Enums`, `BondCalculator`,
`LoggingVisitor`, `TransactionBase`, and `AgeVerifier` must appear in
`dependency_graph.json` outside any SCC (or in a singleton SCC of
size 1, depending on chunker representation).

---

## Build test

### TC-6.1: Clean build with zero warnings

```bash
make clean && make all 2>&1 | grep -cE 'warning|error'
# expected: 0
```

### TC-6.2: Binary runs and exits cleanly via menu option 0

```bash
printf '0\n' | ./BankSystem
# expected: exit code 0, trailing "Goodbye." line
```
