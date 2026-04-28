# Bank Customer Management System - Test Cases

## Overview
Test cases for the Bank Customer Management System covering customer
registration, listing, input validation, and the function-pointer demo.

> The contact fields (email, phone, address) are collected from the user
> separately at the prompt but stored together in the `ContactInfo` struct
> alias (`typedef struct ContactInfo_ { ... } ContactInfo`). Newly registered
> customers default to `CustomerStatus = ACTIVE` via the default constructor
> argument.

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

**Input**: Enter `9` at the main menu (valid range 1–4).
**Expected Result**: User is prompted to re-enter; program does not crash.
**Status**: PASS

---

### TC-3.2: Non-Numeric Menu Input
**Priority**: Medium
**Objective**: Verify that text input at numeric prompts is rejected

**Input**: Enter `abc` at the main menu.
**Expected Result**: User is prompted to re-enter; program recovers without crash.
**Status**: PASS

---

### TC-3.3: Exit via Option 4
**Priority**: High
**Objective**: Verify clean program exit

**Input**: Enter `4` at the main menu.
**Expected Result**: Output shows `Goodbye.` and program exits with code 0.
**Status**: PASS

---

## Function Pointer Demo

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

## Summary

| Section | Total Cases | Passing |
|---------|-------------|---------|
| Customer Registration | 6 | 6 |
| List Customers | 3 | 3 |
| Input Validation | 3 | 3 |
| Function Pointer Demo | 2 | 2 |
| **Total** | **14** | **14** |

_Last verified: April 2026 — driven via stdin against `BankSystem.exe`
(g++ `-std=c++03 -Wall`)._
