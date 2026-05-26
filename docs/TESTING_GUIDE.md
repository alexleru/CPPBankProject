# Testing Guide — SCC Demo Build

There is no unit-test framework. Testing is exercising the interactive
console, either by hand or by piping a stdin script. Pass criteria are
in `TEST_CASES.md`; SCC graph expectations are in `SCC_DEMO_LAYOUT.md`.

## Prerequisites

```bash
make clean && make all
# build/  populated, ./BankSystem ready
```

## Scripted runs

### Scenario 1 — new-bank flow (mega-SCC + SCC B visitor)

```bash
printf '1\n\n0\n' | ./BankSystem
```

Stdin breakdown:
- `1` — choose menu option 1
- `\n` — press Enter at the "Press Enter to continue..." pause
- `0` — exit

Expected output highlights:
- Three `Customer registered` broadcasts
- Four `[LOG]` lines (one per transaction type) plus a `LOANPAY` line
- Audit-log dump ending with `Broadcast: Loan approved for Alice Smith`
- `[AUDIT SUMMARY]` block with per-event-type counters (`std::map` demo)
- `-- Transaction breakdown (via dynamic_cast) --` listing each
  concrete subclass for Alice's checking-account history, with each
  line including the `TransactionBase` `instanceId` (`iid=...`) and a
  trailing total instance count
- `-- Friend back-door (debug dump) --` line emitted by
  `debugDumpAccount`
- Final balances table (see TC-1.4)
- Trailing `(Bank dismantled cleanly.)`

### Scenario 2 — report (SCC D pipeline)

```bash
printf '2\n\n0\n' | ./BankSystem
```

Expected output highlights:
- `=== Generating report for 2026-01-01 .. 2026-03-31 ===`
- Two `-- Section: Page N` blocks with `[rendered]` and `style=plain-text`
- `=== Report complete: pages=2 ===`
- Trailing `(Report engine torn down.)`

### Scenario 3 — bond calc

```bash
printf '3\n100\n12\n5\n2\n3\n\n0\n' | ./BankSystem
```

Stdin breakdown:
- `3` — menu option 3
- `100` — nominal
- `12` — term in months
- `5` — fixed monthly coupon %
- `2` — max random monthly part % (will clamp at 3)
- `3` — annual discount %
- `\n` — pause acknowledgement
- `0` — exit

Expected output highlights:
- `PV: $...` and `FV: $...` lines (numeric values vary; uses `std::rand`)

### Scenario 4 — verify age 21+ (native library)

```bash
printf '4\n5\n6\n2000\n\n0\n' | ./BankSystem
```

Stdin breakdown:

- `4` — menu option 4
- `5`, `6`, `2000` — day / month / year of birth (5 June 2000)
- `\n` — pause acknowledgement
- `0` — exit

Expected output highlights:

- `Result: OK -- subject is 21 or older.` (for a date that's clearly
  more than 21 years ago)
- `Result: UNDER -- subject is younger than 21.` for a recent year
- `Result: BAD INPUT -- not a real calendar date.` for `31 / 2 / 2000`
- `Result: LIB ERROR -- ...` if the native library was not built or
  was deleted; covers the `LoadLibrary`/`dlopen` failure path. Run
  `make native` first if you see this.

### Combined scenarios 1 + 2

```bash
printf '1\n\n2\n\n0\n' | ./BankSystem
```

Useful as a regression smoke test for any change that touches mega-SCC
or SCC D code.

## Hand-test sequence

When debugging interactively:

1. `./BankSystem`
2. Pick option `1`. Read the audit log + `[AUDIT SUMMARY]`. Verify the
   four `[LOG]` lines, the loan-approval broadcasts, the
   `dynamic_cast` breakdown and the `debugDumpAccount` block all
   appear.
3. Press Enter, pick option `2`. Verify "pages=2" prints.
4. Press Enter, pick option `3`, enter `100 / 12 / 5 / 2 / 3`. Verify
   PV/FV print.
5. Press Enter, pick option `4`, enter a clearly-old date (e.g.
   `5 / 6 / 1990`). Verify `Result: OK ...`. Repeat with a recent year
   for the `UNDER` path, and an impossible date (`31 / 2 / 2000`) for
   `BAD INPUT`.
6. Press Enter, pick `0`. Confirm clean exit.

## Memory check

If `valgrind` is installed:

```bash
printf '1\n\n2\n\n0\n' | valgrind --leak-check=full --error-exitcode=1 ./BankSystem 2>&1 | tail -30
```

Expected: zero leaks (cascade-delete should cover everything), zero
"definitely lost" bytes. Allocations during scenario 1 are owned by
`Bank` and freed by `delete bank;` at end-of-scenario; allocations
during scenario 2 are owned by `ReportEngine`/`ReportWriter` and freed
by `delete engine;`.

## Dependency-graph ground truth

This is the actual deliverable. Use the `java_cpp_chunkagent` plugin:

```bash
# Pseudo-command — exact CLI depends on the chunker's invocation:
java_cpp_chunkagent --src . --emit-graph dependency_graph.json

# Then validate:
jq '.sccs | map(select(.size > 1)) | length' dependency_graph.json
# expected: 3

jq '.sccs | map(select(.size > 1) | .size) | sort' dependency_graph.json
# expected: [5, 5, 9]
```

If anything other than `[5, 5, 9]` shows up, cross-reference
`SCC_DEMO_LAYOUT.md` for the expected member lists and find what
shifted.

## SCC D isolation check (must run every time SCC D is touched)

```bash
grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer' \
    include/Report*.h src/Report*.cpp
```

**Expected output: nothing.** Any match means SCC D references SCC
A/B/C and will get fused into the mega-SCC.

## Warning-free build check

```bash
make clean && make all 2>&1 | grep -cE 'warning|error'
# expected: 0
```

## Python harness (optional)

For more elaborate test orchestration, the original project shipped a
Python script that drove the binary via subprocess. The current build
has four menu options, so a Python harness is overkill — the shell
`printf | ./BankSystem` pattern is sufficient. If you do want a
harness, a 20-line wrapper around `subprocess.run(['./BankSystem'],
input=..., capture_output=True)` plus regex assertions against stdout
covers all four scenarios.

## Troubleshooting

| Symptom | Likely cause |
|---|---|
| Build error `‘nullptr’ was not declared` | C++11 code slipped in; use `NULL` |
| Build error `‘unique_ptr’ in namespace ‘std’ does not name a type` | smart pointer slipped in; use raw `new`/`delete` |
| Warning under `-Wall` about unused parameter | cast to `(void)param;` or remove the parameter |
| Scenario 1 segfaults on exit | likely double-delete — check ownership table in `QUICK_REFERENCE.md` |
| Chunker reports 2 SCCs instead of 3 | SCC B fused into mega-SCC; check that `accept()` is **not** declared on `Transaction` base |
| Chunker reports 1 mega-SCC | SCC D isolation broken; run the grep one-liner above |
| Chunker reports 4+ SCCs | Cycle edges missing — check forward-decl + method-call pairs in `SCC_DEMO_LAYOUT.md` |
