# Bank Customer Management System - Testing Guide

## Prerequisites

Build the project first (this also builds the native age-verification
library into `native/windows/` or `native/linux/`):
```bash
# Linux / macOS
cd CPPBankProject
make
```

```cmd
:: Windows (cmd.exe with MinGW)
cd CPPBankProject
mingw32-make
```

Confirm the binary and native library exist:
```bash
ls -l BankSystem native/linux/libage_verifier.so       # Linux / macOS
```
```cmd
dir BankSystem.exe native\windows\age_verifier.dll     :: Windows
```

---

## Running the Program

```bash
./BankSystem             # Linux / macOS
BankSystem.exe           :: Windows
```

The program presents a 5-option menu:
```
1. Create Customer
2. List Customers
3. Function Pointer Demo
4. Verify Age (21+, native library)
5. Exit
```

---

## Test Scenario A: Basic Customer Registration

**Purpose**: Verify customer registration and listing work end-to-end.

**Input sequence** (one value per line):
```
1
John
Doe
john.doe@example.com
123-456-7890
123 Main Street
2
5
```

**Expected outputs**:
- After option 1: `Customer created successfully. ID: CUST001000`
- After option 2: Customer list shows John Doe with ID `CUST001000`
- After option 5: `Goodbye.`

**Pass criteria**: Customer registers and appears in the list; program exits cleanly.

---

## Test Scenario B: Multiple Customers

**Purpose**: Verify sequential customer IDs and correct listing.

**Input sequence**:
```
1
Alice
Johnson
alice@bank.com
123-456-7890
100 First Ave
1
Bob
Smith
bob@bank.com
555-123-4567
200 Second St
2
5
```

**Expected output after option 2**: Both Alice Johnson (`CUST001000`) and Bob Smith (`CUST001001`) appear in the list.

---

## Test Scenario C: Validation Rejection

**Purpose**: Verify that invalid email and phone are rejected.

**Invalid email input sequence**:
```
1
Jane
Doe
not-an-email
555-123-4567
Some Address
```
**Expected**: Registration fails with an error message about invalid email. Program returns to the main menu.

**Invalid phone input sequence**:
```
1
Jane
Doe
jane@example.com
123
Some Address
```
**Expected**: Registration fails with an error message about phone format.

---

## Test Scenario D: Function Pointer Demo

**Purpose**: Verify that the function-pointer demo invokes both callbacks correctly.

**Input sequence**:
```
3
5
```

**Expected output after option 3**:
```
=== Function Pointer Demo ===
Result: 8
Result: 15
```

**Pass criteria**: Both results print in order; program returns to the menu and exits on `5`.

---

## Test Scenario E: Empty Fields

**Purpose**: Verify that missing required fields are rejected.

**Input sequence** (press Enter for first name):
```
1

Doe
jane@example.com
555-123-4567
Some Address
```
**Expected**: Registration fails with "First name is required."

---

## Test Scenario F: Native Age Verification (21+)

**Purpose**: Verify the application loads the platform-appropriate native
library at runtime (`age_verifier.dll` / `libage_verifier.so`), forwards a
birth date through the C ABI, and reports the result.

**Pre-condition**: `make` (or `mingw32-make`) has been run, so the shared
library exists at:
- `native/windows/age_verifier.dll` (Windows), or
- `native/linux/libage_verifier.so` (Linux).

### F.1 Adult — expect TRUE
Input sequence:
```
4
1
1
1990
5
```
**Expected**:
- `Loaded native library:` appears (DLL/`so` loaded via `LoadLibrary` /
  `dlopen` with no error).
- `Result: TRUE  -- age is 21 or older.`
- `Goodbye.` after option `5`.

### F.2 Minor — expect FALSE
Input sequence:
```
4
1
1
2020
5
```
**Expected**: `Result: FALSE -- age is below 21.`

### F.3 Invalid date — expect rejection
Two equivalent inputs (Feb 30 and April 31):
```
4
30
2
2000
5
```
```
4
31
4
2020
5
```
**Expected (both)**: `Invalid date (not a real calendar day).`

### F.4 Day-precision boundary (manual)
Compute `today − 21 years` and `today − 21 years + 1 day` mentally, then
enter each:
- Exactly-21-today → `Result: TRUE`
- One day short of 21 → `Result: FALSE`

This case is not automated because the expected output is date-relative;
see TC-5.5 in [TEST_CASES.md](TEST_CASES.md).

### What "library not loaded" looks like
If you delete or rename the shared library before running, option 4 prints:
```
Native library unavailable: LoadLibrary('native\windows\age_verifier.dll') failed (error 126).
Expected at: native\windows\age_verifier.dll
```
(or the analogous `dlopen` message on Linux). The app stays alive and
returns to the menu.

---

## Manual Test Checklist

### Customer Registration
- [ ] Valid customer registers and receives a `CUST` ID (`CustomerId` typedef of `std::string`)
- [ ] `ContactInfo` fields (email, phone, address) are stored together
- [ ] New customers default to `CustomerStatus::ACTIVE` (default ctor arg)
- [ ] Invalid email is rejected with an error message
- [ ] Invalid phone (< 10 chars) is rejected with an error message
- [ ] Empty first or last name is rejected
- [ ] Empty address is rejected

### List Customers
- [ ] Empty bank displays "No customers registered." without crashing
- [ ] All registered customers appear in listing
- [ ] Each customer shows ID, full name, email, phone, address, and status

### Menu Navigation
- [ ] Out-of-range number prompts re-entry (valid range is 1–5)
- [ ] Non-numeric input prompts re-entry
- [ ] Option 3 runs the Function Pointer Demo and returns to menu
- [ ] Option 4 runs the Native Age Verification and returns to menu
- [ ] Option 5 exits cleanly with "Goodbye."

### Function Pointer Demo
- [ ] `Utils::performOperation(5, 3, Utils::add)` prints `Result: 8`
- [ ] `Utils::performOperation(5, 3, Utils::multiply)` prints `Result: 15`
- [ ] Both callbacks are passed via the `BinaryIntOp` typedef parameter

### Native Age Verification
- [ ] Platform-correct binary is auto-selected at compile time (`_WIN32` /
      `__linux__`); the loaded path appears in stdout
- [ ] Adult birth date returns `Result: TRUE`
- [ ] Birth date < 21 years ago returns `Result: FALSE`
- [ ] Impossible dates (Feb 30, April 31, ...) return `Invalid date`
- [ ] Day-precision: exactly-21-today is TRUE; one day short is FALSE
- [ ] Missing/renamed library produces a diagnostic but does not crash

---

## Running the Automated Suite

The full set of automated cases (including TC-5.* native-library tests) is
driven by:
```bash
python run_tests.py
```
Spec lives at [TC_SPEC.md](TC_SPEC.md); per-run logs are written to
`logs/test_run_<timestamp>.log`.

---

## Troubleshooting

| Problem | Likely Cause | Fix |
|---------|-------------|-----|
| `BankSystem: not found` | Not compiled | Run `make` (or `mingw32-make` on Windows) |
| Compile error | Compiler not found | Install `g++` (or MinGW on Windows) |
| `rm`/`mkdir` fails on Windows cmd | Using Unix `make` with Unix commands | Use `mingw32-make`; the Makefile switches to Windows commands automatically |
| Validation keeps failing | Typo in email/phone | Check format: `name@domain.com`, `123-456-7890` |
| List shows nothing | No customers added | Use option 1 first |
| Option 4 says "Native library unavailable" | DLL/`.so` missing or moved | Re-run `make native` (or run `native/windows/build.bat` / `native/linux/build.sh`) from the project root |
| `dlopen: ... cannot open shared object file` on Linux | Running outside the project root | Run `./BankSystem` from the project root so the relative path `native/linux/libage_verifier.so` resolves |
| Link error `undefined reference to dlopen` on Linux | `-ldl` not passed | The top-level Makefile adds it automatically; if you build by hand, append `-ldl` |

---

*Last Updated: May 2026*
