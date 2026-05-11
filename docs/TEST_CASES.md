# Bank Customer Management System - Test Cases

## Overview
Test cases for the Bank Customer Management System covering customer
registration, listing, input validation, the function-pointer demo, and the
native age-verification library loaded at runtime via `LoadLibrary` /
`dlopen`.

> The contact fields (email, phone, address) are collected from the user
> separately at the prompt but stored together in the `ContactInfo` struct
> alias (`typedef struct ContactInfo_ { ... } ContactInfo`). Newly registered
> customers default to `CustomerStatus = ACTIVE` via the default constructor
> argument.

> The native library (TC-5.*) ships as `native/windows/age_verifier.dll`
> on Windows and `native/linux/libage_verifier.so` on Linux. The host
> wrapper [`AgeVerifier`](../include/AgeVerifier.h) picks the correct
> binary at compile time via `_WIN32` / `__linux__`.

---

## Test Section 1: Customer Registration

### TC-1.1: Register Valid Customer
**Priority**: High
**Objective**: Verify valid customer registration

| Field | Value |
|-------|-------|
| First Name | John |
| Last Name | Doe |
| Email | john.doe@example.com |
| Phone | 123-456-7890 |
| Address | 123 Main Street |

**Expected Result**: Customer registered successfully. Output includes unique Customer ID (e.g. `CUST001000`).
**Status**: PASS

---

### TC-1.2: Register Customer with Invalid Email
**Priority**: High
**Objective**: Verify email validation rejects bad input

| Field | Value |
|-------|-------|
| Email | invalid.email@invalid |

**Expected Result**: Registration fails with an error message about invalid email format.
**Status**: PASS

---

### TC-1.3: Register Customer with Invalid Phone
**Priority**: High
**Objective**: Verify phone validation rejects too-short input

| Field | Value |
|-------|-------|
| Phone | 123 |

**Expected Result**: Registration fails with an error message about invalid phone format.
**Status**: PASS

---

### TC-1.4: Register Multiple Customers
**Priority**: High
**Objective**: Verify sequential customer IDs for multiple registrations

| Customer | Email | Phone |
|----------|-------|-------|
| Alice Johnson | alice@bank.com | 123-456-7890 |
| Bob Wilson | bob@bank.com | (555) 123-4567 |
| Carol Davis | carol@bank.com | 555-987-6543 |

**Expected Result**: Each customer gets a unique, incrementing ID (CUST001000, CUST001001, CUST001002).
**Status**: PASS

---

### TC-1.5: Empty Name Fields
**Priority**: Medium
**Objective**: Verify that blank first or last name is rejected

**Input**: Leave first name or last name empty (press Enter).
**Expected Result**: Registration fails with a validation error.
**Status**: PASS

---

### TC-1.6: Empty Address Field
**Priority**: Medium
**Objective**: Verify that blank address is rejected

**Input**: Leave address empty (press Enter).
**Expected Result**: Registration fails with a validation error.
**Status**: PASS

---

## Test Section 2: List Customers

### TC-2.1: List Customers — Empty Bank
**Priority**: Medium
**Objective**: Verify graceful output when no customers registered

**Input**: Menu option 2 before any customers are added.
**Expected Result**: Output displays "No customers registered." without crashing.
**Status**: PASS

---

### TC-2.2: List Customers — After Registration
**Priority**: Medium
**Objective**: Verify all registered customers appear in listing

**Setup**: Register at least 2 customers.
**Input**: Menu option 2.
**Expected Result**: All registered customers displayed with their IDs, names, email, phone, address, and status.
**Status**: PASS

---

### TC-2.3: Customer Status Displayed as "Active"
**Priority**: Low
**Objective**: Verify new customers show status "Active"

**Setup**: Register one customer.
**Input**: Menu option 2.
**Expected Result**: Customer status is displayed as `Active`.
**Status**: PASS

---

## Test Section 3: Input Validation (Menu)

### TC-3.1: Invalid Menu Choice (Out of Range)
**Priority**: Medium
**Objective**: Verify that out-of-range menu numbers are rejected

**Input**: Enter `9` at the main menu (valid range 1–5).
**Expected Result**: Prompt repeats with `Enter a number between 1 and 5.`; program does not crash.
**Status**: PASS

---

### TC-3.2: Non-Numeric Menu Input
**Priority**: Medium
**Objective**: Verify that text input at numeric prompts is rejected

**Input**: Enter `abc` at the main menu.
**Expected Result**: Prompt repeats with `Enter a number between 1 and 5.`; program recovers without crash.
**Status**: PASS

---

### TC-3.3: Exit via Option 5
**Priority**: High
**Objective**: Verify clean program exit

**Input**: Enter `5` at the main menu.
**Expected Result**: Output shows `Goodbye.` and program exits with code 0.
**Status**: PASS

---

## Test Section 4: Function Pointer Demo

### TC-4.1: Add callback via performOperation
**Priority**: Medium
**Objective**: Verify `Utils::performOperation` (parameter declared with the
`BinaryIntOp` typedef) invokes `Utils::add` correctly.

**Input**: Enter `3` at the main menu.
**Expected Result**: Output contains `Result: 8` (from `Utils::performOperation(5, 3, Utils::add)`).
**Status**: PASS

### TC-4.2: Multiply callback via performOperation
**Priority**: Medium
**Objective**: Verify `Utils::performOperation` (parameter declared with the
`BinaryIntOp` typedef) invokes `Utils::multiply` correctly.

**Input**: Enter `3` at the main menu.
**Expected Result**: Output contains `Result: 15` (from `Utils::performOperation(5, 3, Utils::multiply)`).
**Status**: PASS

---

## Test Section 5: Native Age Verification Library

> All TC-5.* cases exercise the cross-platform native library
> (`native/windows/age_verifier.dll` / `native/linux/libage_verifier.so`)
> dynamically loaded at runtime. The host wrapper resolves the
> `verify_age_21` symbol via `GetProcAddress` (Windows) or `dlsym` (Linux).
> Successful library load is asserted by the substring
> `Loaded native library:` appearing in stdout.

### TC-5.1: Adult Birth Date Returns TRUE
**Priority**: High
**Objective**: Verify that a clearly-adult birth date returns the
`AGE_OK (1)` path through the wrapper and prints `Result: TRUE`.

| Field | Value |
|-------|-------|
| Day   | 1 |
| Month | 1 |
| Year  | 1990 |

**Expected Result**: Output contains `Loaded native library:` and
`Result: TRUE  -- age is 21 or older.`
**Status**: PASS

---

### TC-5.2: Minor Birth Date Returns FALSE
**Priority**: High
**Objective**: Verify that a birth date producing age < 21 returns
`AGE_UNDER (0)` and prints `Result: FALSE`.

| Field | Value |
|-------|-------|
| Day   | 1 |
| Month | 1 |
| Year  | 2020 |

**Expected Result**: Output contains `Result: FALSE -- age is below 21.`
**Status**: PASS

---

### TC-5.3: Invalid Calendar Date (Feb 30) Rejected
**Priority**: Medium
**Objective**: Verify the native library returns
`AGE_VERIFIER_BAD_INPUT (-1)` for an impossible calendar day and the host
prints the invalid-date message.

| Field | Value |
|-------|-------|
| Day   | 30 |
| Month | 2 |
| Year  | 2000 |

**Expected Result**: Output contains `Invalid date (not a real calendar day).`
**Status**: PASS

---

### TC-5.4: Invalid Calendar Date (April 31) Rejected
**Priority**: Medium
**Objective**: Verify rejection of a 31st of a 30-day month.

| Field | Value |
|-------|-------|
| Day   | 31 |
| Month | 4 |
| Year  | 2020 |

**Expected Result**: Output contains `Invalid date (not a real calendar day).`
**Status**: PASS

---

### TC-5.5: Day-Precision Boundary (manual / interactive)
**Priority**: Medium
**Objective**: Confirm that the age calculation is day-precise, not just
year-precise: someone whose 21st birthday is **today** is accepted, and
someone whose 21st birthday is **tomorrow** is rejected.

**Why this is a manual case**: the expected output depends on the current
system date and cannot be encoded as a static expectation in
[TC_SPEC.md](TC_SPEC.md). Verify by hand with `today` and `today + 1`
inputs (subtracting 21 from the year).

**Expected Result**:
- "Today − 21 years" → `Result: TRUE`
- "Today − 21 years + 1 day" → `Result: FALSE`

**Status**: PASS (verified 2026-05-11)

---

## Summary

| Section | Total Cases | Automated | Manual |
|---------|-------------|-----------|--------|
| Customer Registration | 6 | 6 | 0 |
| List Customers | 3 | 3 | 0 |
| Input Validation | 3 | 3 | 0 |
| Function Pointer Demo | 2 | 2 | 0 |
| Native Age Verification | 5 | 4 | 1 |
| **Total** | **19** | **18** | **1** |

The 18 automated cases run via `python run_tests.py`, which reads
[TC_SPEC.md](TC_SPEC.md). TC-5.5 is a manual boundary check because its
expectation is date-relative.

_Last verified: May 2026 — driven via stdin against `BankSystem.exe`
(g++ `-std=c++03 -Wall`) with `native/windows/age_verifier.dll` loaded
dynamically via `LoadLibrary`._
