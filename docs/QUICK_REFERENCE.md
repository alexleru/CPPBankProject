# Bank Account Management System - Quick Reference

## Menu Options

```
┌─ MAIN MENU ──────────────────────────────────────────┐
│                                                      │
│  1. Create Customer          (Customer Management)   │
│  2. Create Account           (Account Creation)      │
│  3. List Customers           (Reporting)             │
│  4. Exit                     (Terminate)             │
│                                                      │
└──────────────────────────────────────────────────────┘
```

## Account Types

| # | Type | Default Rate | Overdraft | Notes |
|---|------|-------------|-----------|-------|
| 1 | Savings | 3.5% p.a. | No | `interestRate` stored |
| 2 | Checking | — | $500 default | `overdraftLimit` stored |
| 3 | Loan | 8% p.a. | No | `loanAmount`, `termMonths` stored |

## ID Format Reference

| Entity | Format | Example |
|--------|--------|---------|
| Customer | CUST + 6 digits | CUST000001 |
| Account | ACC + 6 digits | ACC000001 |

---

## Validation Rules

### Email Validation (`Utils::validateEmail`)
- Must contain `@` and a domain with TLD
- **Valid**: `john.doe@example.com`, `user@domain.co.uk`
- **Invalid**: `invalid.email`, `user@.com`, `user@domain`

### Phone Validation (`Utils::validatePhone`)
- Minimum 10 characters (digits, spaces, dashes, parentheses, `+` allowed)
- **Valid**: `123-456-7890`, `(555) 123-4567`, `+1 555-123-4567`
- **Invalid**: `555-1234` (too short), `abcdefghij` (non-numeric)

---

## Constants (`include/Constants.h`)

```cpp
const std::string BANK_NAME                = "Small Bank System";
const int         MAX_ACCOUNTS_PER_CUSTOMER = 5;
const double      SAVINGS_INTEREST_RATE     = 0.035;
const double      OVERDRAFT_LIMIT           = 500.0;
const double      LOAN_INTEREST_RATE        = 0.08;
const std::string CUSTOMER_ID_PREFIX        = "CUST";
const std::string ACCOUNT_ID_PREFIX         = "ACC";
```

To modify: edit `include/Constants.h` and rebuild with `make`.

---

## Customer Constraints

- First name, last name, email, phone, address: all required
- Email must match validation pattern
- Phone must be 10+ characters
- Max 5 accounts per customer
- Status values: `ACTIVE`, `INACTIVE`

---

## Static Methods Reference

### Utils Class
```cpp
static std::string generateCustomerId();                   // "CUST000001"
static std::string generateAccountId();                    // "ACC000001"
static bool        validateEmail(const std::string&);
static bool        validatePhone(const std::string&);
static std::string accountTypeToString(AccountType type);  // "Savings"
```

---

## Code Organization

### Include Files (`include/`)
- `Constants.h` — Typed `const` system constants
- `Enums.h` — `AccountType`, `CustomerStatus` enums
- `Globals.h` — Global counters (`globalCustomerCounter`, `globalAccountCounter`)
- `Utils.h` — Static utility methods
- `Customer.h` — Customer class
- `Account.h` — Abstract base Account class
- `SavingsAccount.h` — Savings account (interestRate)
- `CheckingAccount.h` — Checking account (overdraftLimit)
- `LoanAccount.h` — Loan account (loanAmount, interestRate, termMonths)
- `Bank.h` — Bank management class

### Source Files (`src/`)
- `Globals.cpp` — Global variable definitions
- `Utils.cpp` — Utility implementations
- `Customer.cpp` — Customer management
- `Account.cpp` — Base account logic
- `SavingsAccount.cpp` — Savings account
- `CheckingAccount.cpp` — Checking account
- `LoanAccount.cpp` — Loan account
- `Bank.cpp` — Bank core logic
- `main.cpp` — UI and entry point (4-option menu)

---

## Enums

```cpp
enum AccountType    { SAVINGS, CHECKING, LOAN }
enum CustomerStatus { ACTIVE, INACTIVE }
```

---

## Build Reference

| Platform | Compiler | Standard | Command |
|----------|----------|----------|---------|
| Linux | GCC 4.x+ | C++03 | `g++ -std=c++03` |
| Windows | MinGW | C++03 | `mingw-g++ -std=c++03` |
| macOS | Clang | C++03 | `clang++ -std=c++03` |

```bash
make          # build
make clean    # remove build artifacts
./BankSystem  # run
```

---

## Troubleshooting

| Problem | Likely Cause | Fix |
|---------|-------------|-----|
| Won't compile | Compiler too old | Use GCC 4.x+ or Clang |
| Validation keeps failing | Email/phone format | Check against patterns above |
| Customer ID not found | Wrong ID format | Copy exact CUST ID from creation output |
| Max accounts error | Customer already has 5 accounts | Use a different customer |

---

**Version**: 1.1
**Last Updated**: April 2026
