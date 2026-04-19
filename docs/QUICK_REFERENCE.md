# Bank Account Management System - Quick Reference & Features Matrix

## Recent Changes

| Date       | Commit    | Author          | Summary                                                                 |
|------------|-----------|-----------------|-------------------------------------------------------------------------|
| 2026-04-19 | _working_ | Alexey Leshchuk | Add function-pointer parameter demo: `Utils::add` / `Utils::multiply` / `Utils::performOperation(int,int,int(*)(int,int))` plus `Bank::applyToAllAccounts(double(*)(double))`, exposed through new menu option 17 "Function Pointer Demo". |
| 2026-04-19 | `c4a3f1d` | Alexey Leshchuk | Add "Calculate Bond Parameters" feature as menu option 16 (PV, FV, coupon stream analytics, 200-line `generateBondAnalysisReport`, cross-platform Makefile). |
| 2026-04-12 | `c712366` | Alexey Leshchuk | Update README to reflect C++03 codebase accurately.                      |
| 2026-04-12 | `181d423` | Alexey Leshchuk | Build cleanly without extra flags.                                       |
| 2026-04-12 | `977c94a` | Alexey Leshchuk | Refactor codebase to C++03 with type aliases and global state.           |

## Quick Reference Guide

### Menu Options Map

```
┌─ MAIN MENU (17 Options) ─────────────────────────────┐
│                                                      │
│  1. Register New Customer         (Customer Mgmt)    │
│  2. Open New Account              (Account Creation) │
│  3. Deposit Funds                 (Transactions)     │
│  4. Withdraw Funds                (Transactions)     │
│  5. Transfer Between Accounts     (Transactions)     │
│  6. View Account Statement        (Reporting)        │
│  7. View Customer Portfolio       (Reporting)        │
│  8. Apply Monthly Processing      (Batch Operation)  │
│  9. Calculate Loan EMI            (Calculation)      │
│  10. Search Account by ID         (Lookup)           │
│  11. List All Customers           (Reporting)        │
│  12. List All Accounts            (Reporting)        │
│  13. Generate Bank Report         (Reporting)        │
│  14. Close Account                (Account Mgmt)     │
│  15. Admin/Debug Functions        (Admin)            │
│  16. Calculate Bond Parameters    (Calculation)      │
│  17. Function Pointer Demo        (Language Demo)    │
│                                                      │
│  0. Exit                          (Terminate)        │
│                                                      │
└──────────────────────────────────────────────────────┘
```

### Account Type Selection

| Type # | Name | Minimum Balance | Key Feature | Interest Rate | Fee |
|--------|------|-----------------|-------------|---------------|-----|
| 1 | Savings | $100.00 | Interest earnings | 3.5% p.a. | None |
| 2 | Checking | None | Overdraft support | 0.1% p.a. | $5.00/mo |
| 3 | Loan | N/A | EMI payments | 8% p.a. | None |

### ID Format Reference

| Entity | Format | Example | Range |
|--------|--------|---------|-------|
| Customer | CUST + 6 digits | CUST000001 | CUST000001 - CUST999999 |
| Account | ACC + 6 digits | ACC002000 | ACC002000 - ACC999999 |
| Transaction | TXN + 8 digits | TXN00010000 | TXN00010000 - TXN99999999 |

---

## Features Matrix

### Account Features Comparison

```
Feature                          | Savings | Checking | Loan
─────────────────────────────────┼─────────┼──────────┼──────
Deposit Funds                    |   ✅    |    ✅    |  ❌
Withdraw Funds                   |   ✅*   |    ✅    |  ❌
Minimum Balance Enforcement      |   ✅    |    ❌    |  N/A
Overdraft Allowed                |   ❌    |    ✅    |  ❌
Monthly Interest                 |   ✅    |   ✅     |  ❌
Monthly Fees                     |   ❌    |   ✅     |  ❌
EMI Payments                     |   ❌    |    ❌    |  ✅
Transfer Capability              |   ✅    |    ✅    |  ❌
Transaction History              |   ✅    |    ✅    |  ✅
Account Statement               |   ✅    |    ✅    |  ✅
Repayment Schedule              |   ❌    |    ❌    |  ✅

* Cannot withdraw below $100 minimum
✅ = Supported
❌ = Not Supported
N/A = Not Applicable
```

### Transaction Types Supported

| Type | Initiator | Debit Acct | Credit Acct | Records |
|------|-----------|-----------|------------|---------|
| Deposit | Manual | N/A | Account | 1 |
| Withdrawal | Manual | Account | N/A | 1 |
| Transfer | Manual | Account A | Account B | 2 |
| Interest | System | N/A | Account | 1 |
| Fee | System | Account | N/A | 1 |
| EMI Payment | Manual | Account | Loan Acct | 1 |

---

## Validation Rules Reference

### Email Validation
- **Pattern**: `[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}`
- **Valid Examples**:
  - john.doe@example.com
  - alice+test@domain.co.uk
  - user_name@company.org
- **Invalid Examples**:
  - invalid.email (missing @domain)
  - user@.com (missing domain name)
  - user@domain (missing TLD)

### Phone Number Validation
- **Pattern**: `[\d\s\-\(\)\+]{10,}`
- **Minimum Length**: 10 characters (with formatting)
- **Valid Examples**:
  - 123-456-7890
  - (555) 123-4567
  - 5551234567
  - +1 555-123-4567
  - +7 999-999-9999
- **Invalid Examples**:
  - 555-1234 (only 8 digits)
  - 123 (too short)
  - abcdefghij (non-numeric)

### Numeric Input Validation
- **Amounts**: Must be positive (> 0)
- **Menu Choices**: Must be integer 0-17
- **Invalid Inputs**: Non-numeric characters rejected

---

## Calculation Formulas

### Monthly Interest (Savings Account)
```
Monthly Interest = (Balance × Annual Rate) / 12
Monthly Interest = (Balance × 0.035) / 12

Example: $1,000 balance
Monthly Interest = ($1,000 × 0.035) / 12 = $2.92
```

### EMI (Equated Monthly Installment) Calculation
```
EMI = P × r × (1 + r)^n / ((1 + r)^n - 1)

Where:
  P = Principal Amount
  r = Monthly Interest Rate (Annual Rate / 12)
  n = Number of Months

Example: $10,000 loan at 8% for 12 months
  r = 0.08 / 12 = 0.00667
  EMI = 10000 × 0.00667 × (1.00667)^12 / ((1.00667)^12 - 1)
  EMI ≈ $880.00
```

### Total Loan Cost
```
Total Cost = EMI × Number of Months
Total Interest = Total Cost - Principal

Example: $10,000 loan with $880 EMI for 12 months
  Total Cost = $880 × 12 = $10,560
  Total Interest = $10,560 - $10,000 = $560
```

### Bond Parameters (Menu Option 16)
```
Inputs:
  N            = nominal (face value)
  n            = term in months
  r_fix        = fixed monthly coupon rate (decimal)
  r_rand_i     = random monthly coupon rate per month i,
                 uniformly distributed in [0, r_rand_max],
                 where r_rand_max ≤ 3% (0.03)
  d            = annual discount rate (decimal)
  d_m          = d / 12      (monthly discount rate)

Cash flow of month i:
  C_i = N × (r_fix + r_rand_i)

Present Value (today's price of the bond):
  PV = Σ_{i=1..n} C_i / (1 + d_m)^i  +  N / (1 + d_m)^n

Future Value (all coupons reinvested at d_m to maturity):
  FV = Σ_{i=1..n} C_i × (1 + d_m)^(n-i)  +  N

Macaulay Duration (in years):
  D = [ Σ_{i=1..n} i × (C_i / (1 + d_m)^i) + n × (N / (1 + d_m)^n) ]
      / [ PV × 12 ]

Example: N = 1000, n = 12, r_fix = 0.5%, r_rand_max = 3%, d = 5%
  Monthly coupons fluctuate between $5 and $35.
  PV and FV depend on the random draws; regenerate to resample.
```

### Portfolio Total (Customer)
```
Portfolio Total = Sum of all Account Balances

Example: 3 accounts ($1,200 + $500 + $2,000)
Portfolio Total = $3,700
```

---

## Constraints & Limits

### Account Constraints
```
├─ Savings Account
│  ├─ Minimum Balance: $100.00 (enforced)
│  ├─ Interest Rate: 3.5% per annum
│  ├─ Monthly Interest Processing: Automatic
│  └─ Withdrawal Limit: Balance must stay ≥ $100
│
├─ Checking Account
│  ├─ Overdraft Limit: $500.00
│  ├─ Monthly Fee: $5.00
│  ├─ Interest Rate: 0.1% per annum
│  └─ Withdrawal Limit: Balance + Overdraft
│
└─ Loan Account
   ├─ Maximum Loan Amount: $100,000.00
   ├─ Default Interest Rate: 8% per annum
   ├─ Payment: EMI (calculated)
   └─ Repayment Schedule: Generated automatically
```

### Customer Constraints
```
├─ First Name: Required, non-empty
├─ Last Name: Required, non-empty
├─ Email: Required, must match pattern
├─ Phone: Required, must be 10+ characters
├─ Address: Required, non-empty
├─ Max Accounts per Customer: 5
└─ Status Values: ACTIVE, INACTIVE, SUSPENDED, CLOSED
```

### System Limits
```
├─ Maximum Customers: Unlimited (practical limit: RAM)
├─ Maximum Accounts: Unlimited (max 5 per customer)
├─ Maximum Transactions: Unlimited
├─ Currency Precision: 2 decimal places
├─ Transaction History: Unlimited per account
└─ Bank Name: "National C++ Bank" (configurable)
```

---

## Error Messages & Solutions

### Customer Registration Errors

| Message | Cause | Solution |
|---------|-------|----------|
| "Invalid email format: [email]" | Email doesn't match pattern | Use format: name@domain.com |
| "Invalid phone format: [phone]" | Phone < 10 chars or invalid chars | Use 10+ digit format with dashes |
| "First and last name cannot be empty" | Missing name field | Provide both first and last name |
| "Customer validation failed" | Overall validation issue | Check all fields meet requirements |

### Account Errors

| Message | Cause | Solution |
|---------|-------|----------|
| "Customer not found" | Invalid Customer ID | Use correct CUST ID |
| "Customer has reached maximum accounts" | 5 accounts already exist | Close an account first |
| "Cannot withdraw from inactive account" | Account is closed | Reopen or use different account |
| "Withdrawal would exceed overdraft limit" | Checking: withdrawal > balance + overdraft | Reduce withdrawal amount |

### Transaction Errors

| Message | Cause | Solution |
|---------|-------|----------|
| "Insufficient balance" | Account balance too low | Deposit funds first |
| "Withdrawal failed! Minimum balance would be violated" | Savings: withdrawal < $100 minimum | Reduce withdrawal amount |
| "One or both accounts not found" | Invalid ACC IDs for transfer | Use correct account IDs |
| "Deposit amount must be positive" | Amount ≤ 0 | Enter positive amount |

---

## Constants & Configuration

### File: Constants.h
```cpp
#define BANK_NAME "National C++ Bank"
#define VERSION "1.0.0"
#define MAX_ACCOUNTS_PER_CUSTOMER 5
#define MIN_SAVINGS_BALANCE 100.0
#define OVERDRAFT_LIMIT 500.0
#define DEFAULT_INTEREST_RATE 0.035
#define SAVINGS_INTEREST_RATE 0.035
#define CHECKING_INTEREST_RATE 0.001
#define MONTHLY_CHECKING_FEE 5.0
#define LOAN_INTEREST_RATE 0.08
#define MAX_LOAN_AMOUNT 100000.0
#define CUSTOMER_ID_PREFIX "CUST"
#define ACCOUNT_ID_PREFIX "ACC"
```

### To Modify Constants
1. Edit `include/Constants.h`
2. Update desired `#define` values
3. Rebuild: `make`

---

## Supported Platforms

| Platform | Status | Compiler | Command |
|----------|--------|----------|---------|
| Linux | ✅ Active | GCC 7+ | `g++ -std=c++17` |
| Windows | ✅ Compatible | MinGW/MSVC | `mingw-g++ -std=c++17` |
| macOS | ✅ Compatible | Clang 5+ | `clang++ -std=c++17` |

### Build Instructions by Platform

**Linux**:
```bash
cd /home/user/Documents/CPPproject/CPPBankProject
make
./BankSystem
```

**Windows (MinGW)**:
```cmd
cd C:\CPPBankProject
make
.\BankSystem.exe
```

**macOS**:
```bash
cd ~/CPPBankProject
make
./BankSystem
```

---

## Code Organization Quick Reference

### Include Files (Directory: `include/`)
- `Constants.h` - System constants (#defines)
- `Enums.h` - Enumeration types
- `Utils.h` - Static utility methods
- `Customer.h` - Customer class
- `Account.h` - Base Account abstract class
- `SavingsAccount.h` - Savings account derived
- `CheckingAccount.h` - Checking account derived
- `LoanAccount.h` - Loan account derived
- `Transaction.h` - Transaction class
- `Bank.h` - Bank management class
- `BondCalculator.h` - Bond valuation class (PV / FV / analytics)

### Source Files (Directory: `src/`)
- `Utils.cpp` - Utility implementations (130+ lines)
- `Transaction.cpp` - Transaction processing (75+ lines)
- `Account.cpp` - Base account logic (140+ lines)
- `Customer.cpp` - Customer management (180+ lines)
- `SavingsAccount.cpp` - Savings features (110+ lines)
- `CheckingAccount.cpp` - Checking features (115+ lines)
- `LoanAccount.cpp` - Loan operations (160+ lines)
- `Bank.cpp` - Bank core logic (280+ lines)
- `BondCalculator.cpp` - Bond valuation and analysis (350+ lines;
  `generateBondAnalysisReport` alone is ~200 lines)
- `main.cpp` - UI and entry point (400+ lines)

---

## Static Methods Reference

### Utils Class Methods
```cpp
// Currency & Date Formatting
static std::string formatCurrency(double amount);      // "$1,234.56"
static std::string formatDate(time_t timestamp);       // "YYYY-MM-DD HH:MM:SS"

// ID Generation
static std::string generateCustomerId();               // "CUST000001"
static std::string generateAccountId();                // "ACC002000"

// Validation
static bool validateEmail(const std::string& email);
static bool validatePhone(const std::string& phone);

// Conversion
static std::string accountTypeToString(AccountType);   // "Savings"
static std::string transactionTypeToString(TransactionType);
static std::string transactionStatusToString(TransactionStatus);

// Function pointer demo (menu option 17)
static int  add(int a, int b);                         // returns a + b
static int  multiply(int a, int b);                    // returns a * b
static void performOperation(int x, int y,
                             int (*operation)(int, int));  // dispatches via fn ptr
```

### Account Static Methods
```cpp
static double getInterestForType(AccountType type);    // Returns rate
static std::string typeToString(AccountType type);
```

### LoanAccount Static Methods
```cpp
static double calculateEMI(double principal, double rate, int months);
```

### Bank Instance Methods (function-pointer parameter)
```cpp
// Applies rule(balance) to every active account.
// Rule must have signature: double (*)(double)
void applyToAllAccounts(double (*rule)(double));
```

---

## Function Pointers Quick Reference

### C++ Syntax
```cpp
// Declaration of a parameter that is a function pointer:
//   returnType (*paramName)(arg1Type, arg2Type, ...)

void performOperation(int x, int y, int (*operation)(int, int));
void applyToAllAccounts(double (*rule)(double));

// Calling:
int r = operation(x, y);       // use exactly like a normal call
```

### Where It Is Used in This Project
| Where | Signature | Purpose |
|-------|-----------|---------|
| `Utils::performOperation` | `int (*)(int, int)` | Generic binary-int operation demo |
| `Bank::applyToAllAccounts` | `double (*)(double)` | Apply a rule to every active account |
| `main.cpp: applyOnePercentBonus` | `double (*)(double)` matching rule | Sample bonus rule (+1%) |
| `main.cpp: applyFlatMaintenanceFee` | `double (*)(double)` matching rule | Sample fee rule (-$5, floor 0) |

### Java Modernisation Mapping
| C++ signature | Java 8+ functional interface |
|---------------|------------------------------|
| `int (*)(int, int)` | `java.util.function.IntBinaryOperator` |
| `double (*)(double)` | `java.util.function.DoubleUnaryOperator` |
| Generic `R (*)(A, B)` | `java.util.function.BiFunction<A, B, R>` |
| Generic `R (*)(A)` | `java.util.function.Function<A, R>` |

### Expected Output (menu option 17, empty bank)
```
--- Utils::performOperation (int (*)(int, int)) ---
Result: 8
Result: 15

--- Bank::applyToAllAccounts (double (*)(double)) ---
Rule 1: +1% bonus to every active account
Applying custom rule to all active accounts...
Custom rule applied to 0 account(s).
...
```

---

## Daily Operations Workflow

### Typical Customer Session

```
START
  ↓
[Press Enter] - See Welcome Screen
  ↓
[Menu Option 1] - Register New Customer
  ├─ Enter Name, Email, Phone, Address
  └─ Receive Customer ID
  ↓
[Menu Option 2] - Open Account
  ├─ Choose Account Type
  ├─ Enter Initial Deposit
  └─ Receive Account ID
  ↓
[Menu Option 3-5] - Perform Transactions
  ├─ Options: Deposit, Withdraw, Transfer
  └─ Confirm each operation
  ↓
[Menu Option 6] - View Statement
  ├─ Review account details
  └─ See transaction history
  ↓
[Menu Option 0] - Exit
  └─ System terminates cleanly
END
```

### Bank Administrator Session

```
START
  ↓
[Menu Option 11] - List All Customers
  ├─ Review total customers
  └─ Verify registration data
  ↓
[Menu Option 12] - List All Accounts
  ├─ Check account types
  └─ Verify balances
  ↓
[Menu Option 8] - Monthly Processing
  ├─ Apply interest (Savings)
  ├─ Deduct fees (Checking)
  └─ Process EMI (Loans)
  ↓
[Menu Option 13] - Generate Report
  ├─ Review bank statistics
  └─ Check total assets
  ↓
[Menu Option 0] - Exit
END
```

---

## Performance Characteristics

### Time Complexity
- **Find Account**: O(1) - Map lookup
- **Find Customer**: O(n) - Linear search
- **List All**: O(n) - Iterate all entries
- **Transactions**: O(1) - Direct operations
- **Monthly Processing**: O(n×m) - n customers, m accounts each

### Space Complexity
- **Customers**: O(n) - n customer records
- **Accounts**: O(n×m) - Up to 5m accounts per n customers
- **Transactions**: O(t) - t transactions per account
- **Total**: O(n×m×t) worst case

### Scalability
- Tested with: 50+ customers
- Recommended limit: 1,000+ customers (system-dependent)
- Practical limit: Available RAM

---

## Troubleshooting Quick Reference

| Problem | Likely Cause | Quick Fix |
|---------|-------------|-----------|
| Program won't compile | Missing C++17 support | Use g++ 7.0+ or update compiler |
| Menu not responding | Input format issue | Ensure proper numeric input |
| Validation keeps failing | Email/phone format | Check format against patterns |
| Balance doesn't update | Transaction type incompatible | Verify account type supports operation |
| Can't find account | Wrong account ID | Copy exact ACC ID from account creation |

---

**Quick Reference Version**: 1.2  
**Last Updated**: April 19, 2026 (function-pointer demo, menu option 17 — working tree)  
**For**: Developers & QA Team
