# Bank Customer Management System - Testing Guide

## Prerequisites

Build the project first:
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

Confirm the binary exists:
```bash
ls -l BankSystem         # Linux / macOS
dir BankSystem.exe       :: Windows
```

---

## Running the Program

```bash
./BankSystem             # Linux / macOS
BankSystem.exe           :: Windows
```

The program presents a 4-option menu:
```
1. Create Customer
2. List Customers
3. Function Pointer Demo
4. Exit
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
4
```

**Expected outputs**:
- After option 1: `Customer created successfully. ID: CUST001000`
- After option 2: Customer list shows John Doe with ID `CUST001000`
- After option 4: `Goodbye.`

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
4
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
4
```

**Expected output after option 3**:
```
=== Function Pointer Demo ===
Result: 8
Result: 15
```

**Pass criteria**: Both results print in order; program returns to the menu and exits on `4`.

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
- [ ] Out-of-range number prompts re-entry (valid range is 1–4)
- [ ] Non-numeric input prompts re-entry
- [ ] Option 3 runs the Function Pointer Demo and returns to menu
- [ ] Option 4 exits cleanly with "Goodbye."

### Function Pointer Demo
- [ ] `Utils::performOperation(5, 3, Utils::add)` prints `Result: 8`
- [ ] `Utils::performOperation(5, 3, Utils::multiply)` prints `Result: 15`
- [ ] Both callbacks are passed via the `BinaryIntOp` typedef parameter

---

## Troubleshooting

| Problem | Likely Cause | Fix |
|---------|-------------|-----|
| `BankSystem: not found` | Not compiled | Run `make` (or `mingw32-make` on Windows) |
| Compile error | Compiler not found | Install `g++` (or MinGW on Windows) |
| `rm`/`mkdir` fails on Windows cmd | Using Unix `make` with Unix commands | Use `mingw32-make`; the Makefile switches to Windows commands automatically |
| Validation keeps failing | Typo in email/phone | Check format: `name@domain.com`, `123-456-7890` |
| List shows nothing | No customers added | Use option 1 first |

---

*Last Updated: April 2026*
