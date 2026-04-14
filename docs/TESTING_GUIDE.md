# Bank Account Management System - Testing Guide

## Prerequisites

Build the project first:
```bash
cd CPPBankProject
make
```

Confirm the binary exists:
```bash
ls -l BankSystem
```

---

## Running the Program

```bash
./BankSystem
```

The program presents a 4-option menu:
```
1. Create Customer
2. Create Account
3. List Customers
4. Exit
```

---

## Test Scenario A: Basic Customer & Account Setup

**Purpose**: Verify customer registration and account creation work end-to-end.

**Input sequence** (one value per line):
```
1
John
Doe
john.doe@example.com
123-456-7890
123 Main Street
2
CUST000001
1
1000
3
4
```

**Expected outputs**:
- After option 1: `Customer created successfully. ID: CUST000001`
- After option 2: `Account created successfully.` with Account ID and type `Savings`
- After option 3: Customer list shows John Doe with ID CUST000001
- After option 4: `Goodbye.`

**Pass criteria**: All operations complete without errors; IDs are displayed.

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
3
4
```

**Expected output after option 3**: Both Alice Johnson and Bob Smith appear in the list with IDs CUST000001 and CUST000002.

---

## Test Scenario C: All Three Account Types

**Purpose**: Verify creation of Savings, Checking, and Loan accounts.

**Setup**: First register a customer (see Scenario A).

**Then create three accounts**:
```
2
CUST000001
1
500
2
CUST000001
2
300
2
CUST000001
3
10000
4
```

**Expected**: Each account creation reports a unique Account ID and the correct type string (`Savings`, `Checking`, `Loan`).

---

## Test Scenario D: Validation Rejection

**Purpose**: Verify that invalid email and phone are rejected.

**Input sequence**:
```
1
Jane
Doe
not-an-email
555-123-4567
Some Address
```

**Expected**: Registration fails with an error message. Program stays on the main menu.

Repeat with invalid phone:
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

## Manual Test Checklist

### Customer Registration
- [ ] Valid customer registers and receives a `CUST` ID
- [ ] Invalid email is rejected with an error message
- [ ] Invalid phone (< 10 chars) is rejected with an error message
- [ ] Empty first or last name is rejected

### Account Creation
- [ ] Savings account created (type displayed as "Savings")
- [ ] Checking account created (type displayed as "Checking")
- [ ] Loan account created (type displayed as "Loan")
- [ ] Account creation with unknown customer ID fails gracefully
- [ ] 6th account for a customer is rejected

### List Customers
- [ ] Empty bank produces no crash
- [ ] Registered customers all appear in listing

### Menu Navigation
- [ ] Out-of-range number prompts re-entry
- [ ] Non-numeric input prompts re-entry
- [ ] Option 4 exits cleanly

---

## Troubleshooting

| Problem | Likely Cause | Fix |
|---------|-------------|-----|
| `BankSystem: not found` | Not compiled | Run `make` |
| Compile error | Compiler not found | Install `g++` |
| Validation keeps failing | Typo in email/phone | Check format: `name@domain.com`, `123-456-7890` |
| Customer not found | Wrong customer ID | Copy exact ID from creation output |

---

*Last Updated: April 2026*
