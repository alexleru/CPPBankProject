# Bank Account Management System - Test Cases

## Overview
Test cases for the Bank Account Management System covering customer registration, account creation, and input validation.

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

**Expected Result**: Customer registered successfully. Output includes unique Customer ID (e.g. `CUST000001`).
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

**Expected Result**: Each customer gets a unique, incrementing ID (CUST000001, CUST000002, CUST000003).
**Status**: PASS

---

### TC-1.5: Empty Name Fields
**Priority**: Medium
**Objective**: Verify that blank first or last name is rejected

**Input**: Leave first name or last name empty.
**Expected Result**: Registration fails with a validation error.
**Status**: PENDING

---

## Test Section 2: Account Creation

### TC-2.1: Create Savings Account
**Priority**: High
**Objective**: Verify savings account creation

| Parameter | Value |
|-----------|-------|
| Customer ID | CUST000001 |
| Account Type | 1 (Savings) |
| Initial Balance | $1,000.00 |

**Expected Result**: Account created. Output shows Account ID (e.g. `ACC000001`) and type "Savings".
**Status**: PASS

---

### TC-2.2: Create Checking Account
**Priority**: High
**Objective**: Verify checking account creation

| Parameter | Value |
|-----------|-------|
| Customer ID | CUST000001 |
| Account Type | 2 (Checking) |
| Initial Balance | $500.00 |

**Expected Result**: Account created. Output shows Account ID and type "Checking".
**Status**: PENDING

---

### TC-2.3: Create Loan Account
**Priority**: High
**Objective**: Verify loan account creation

| Parameter | Value |
|-----------|-------|
| Customer ID | CUST000002 |
| Account Type | 3 (Loan) |
| Initial Balance | $10,000.00 |

**Expected Result**: Account created. Output shows Account ID and type "Loan".
**Status**: PENDING

---

### TC-2.4: Maximum Accounts per Customer
**Priority**: Medium
**Objective**: Verify `MAX_ACCOUNTS_PER_CUSTOMER` (5) is enforced

**Input**: Attempt to create a 6th account for a single customer.
**Expected Result**: Creation fails with an error message; no 6th account is added.
**Status**: PENDING

---

### TC-2.5: Create Account for Non-Existent Customer
**Priority**: High
**Objective**: Verify that an invalid customer ID is rejected

**Input**: Customer ID that does not exist (e.g. `CUST999999`).
**Expected Result**: Account creation fails with "Customer not found" error.
**Status**: PENDING

---

## Test Section 3: List Customers

### TC-3.1: List Customers — Empty Bank
**Priority**: Medium
**Objective**: Verify graceful output when no customers registered

**Input**: Menu option 3 before any customers are added.
**Expected Result**: Output indicates no customers are registered (no crash).
**Status**: PENDING

---

### TC-3.2: List Customers — After Registration
**Priority**: Medium
**Objective**: Verify all registered customers appear in listing

**Setup**: Register at least 2 customers.
**Input**: Menu option 3.
**Expected Result**: All registered customers displayed with their IDs, names, and status.
**Status**: PENDING

---

## Test Section 4: Input Validation (Menu)

### TC-4.1: Invalid Menu Choice (Out of Range)
**Priority**: Medium
**Objective**: Verify that out-of-range menu numbers are rejected

**Input**: Enter `9` at the main menu (valid range 1–4).
**Expected Result**: User is prompted to re-enter; program does not crash.
**Status**: PENDING

---

### TC-4.2: Non-Numeric Menu Input
**Priority**: Medium
**Objective**: Verify that text input at numeric prompts is rejected

**Input**: Enter `abc` at the main menu.
**Expected Result**: User is prompted to re-enter; program recovers without crash.
**Status**: PENDING

---

### TC-4.3: Negative Amount at Account Creation
**Priority**: Medium
**Objective**: Verify that negative initial balance is rejected

**Input**: Enter `-500` when prompted for initial balance.
**Expected Result**: User is prompted to re-enter a non-negative number.
**Status**: PENDING

---

## Summary

| Section | Total Cases | Passing |
|---------|-------------|---------|
| Customer Registration | 5 | 4 |
| Account Creation | 5 | 1 |
| List Customers | 2 | 0 |
| Input Validation | 3 | 0 |
| **Total** | **15** | **5** |
