# Bank Customer Management System - Test Cases

## Overview
Test cases for the Bank Customer Management System covering customer registration, listing, and input validation.

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
**Status**: PENDING

---

### TC-1.6: Empty Address Field
**Priority**: Medium
**Objective**: Verify that blank address is rejected

**Input**: Leave address empty (press Enter).
**Expected Result**: Registration fails with a validation error.
**Status**: PENDING

---

## Test Section 2: List Customers

### TC-2.1: List Customers — Empty Bank
**Priority**: Medium
**Objective**: Verify graceful output when no customers registered

**Input**: Menu option 2 before any customers are added.
**Expected Result**: Output displays "No customers registered." without crashing.
**Status**: PENDING

---

### TC-2.2: List Customers — After Registration
**Priority**: Medium
**Objective**: Verify all registered customers appear in listing

**Setup**: Register at least 2 customers.
**Input**: Menu option 2.
**Expected Result**: All registered customers displayed with their IDs, names, email, phone, address, and status.
**Status**: PENDING

---

### TC-2.3: Customer Status Displayed as "Active"
**Priority**: Low
**Objective**: Verify new customers show status "Active"

**Setup**: Register one customer.
**Input**: Menu option 2.
**Expected Result**: Customer status is displayed as `Active`.
**Status**: PENDING

---

## Test Section 3: Input Validation (Menu)

### TC-3.1: Invalid Menu Choice (Out of Range)
**Priority**: Medium
**Objective**: Verify that out-of-range menu numbers are rejected

**Input**: Enter `9` at the main menu (valid range 1–3).
**Expected Result**: User is prompted to re-enter; program does not crash.
**Status**: PENDING

---

### TC-3.2: Non-Numeric Menu Input
**Priority**: Medium
**Objective**: Verify that text input at numeric prompts is rejected

**Input**: Enter `abc` at the main menu.
**Expected Result**: User is prompted to re-enter; program recovers without crash.
**Status**: PENDING

---

### TC-3.3: Exit via Option 3
**Priority**: High
**Objective**: Verify clean program exit

**Input**: Enter `3` at the main menu.
**Expected Result**: Output shows `Goodbye.` and program exits with code 0.
**Status**: PENDING

---

## Summary

| Section | Total Cases | Passing |
|---------|-------------|---------|
| Customer Registration | 6 | 4 |
| List Customers | 3 | 0 |
| Input Validation | 3 | 0 |
| **Total** | **12** | **4** |
