# Bank Customer Management System - Quick Reference

## Menu Options

```
┌─ MAIN MENU ──────────────────────────────────────────┐
│                                                      │
│  1. Create Customer          (Customer Management)   │
│  2. List Customers           (Reporting)             │
│  3. Function Pointer Demo    (Callback example)      │
│  4. Exit                     (Terminate)             │
│                                                      │
└──────────────────────────────────────────────────────┘
```

## Customer ID Format

| Entity | Format | Starting Value | Example |
|--------|--------|---------------|---------|
| Customer | CUST + 6 digits | CUST001000 | CUST001001 |

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

## Constants & Typedefs (`include/Constants.h`)

```cpp
// Semantic primitive typedef
typedef std::string CustomerId;

const std::string BANK_NAME          = "Small Bank System";
const std::string CUSTOMER_ID_PREFIX = "CUST";
```

To modify: edit `include/Constants.h` and rebuild with `make`.

---

## Customer Fields

| Field | Required | Validation |
|-------|----------|------------|
| First name | Yes | Non-empty |
| Last name | Yes | Non-empty |
| Email | Yes | Must contain `@` and TLD |
| Phone | Yes | Min 10 chars, digits/symbols |
| Address | Yes | Non-empty |
| Status | Auto | Defaulted to `ACTIVE` (default ctor argument) |

Email, phone, and address are bundled in the `ContactInfo` struct alias.

---

## Static Methods Reference

### Utils Class
```cpp
static CustomerId  generateCustomerId();            // "CUST001000"
static bool        validateEmail(const std::string&);
static bool        validatePhone(const std::string&);

// Function-pointer demo helpers
static int         add(int a, int b);               // a + b
static int         multiply(int a, int b);          // a * b

// Receives a callback declared via the BinaryIntOp typedef
static void        performOperation(int x, int y, BinaryIntOp operation);
```

**Function-pointer typedef:**
```cpp
typedef int (*BinaryIntOp)(int, int);
```

**Function-pointer usage:**
```cpp
Utils::performOperation(5, 3, Utils::add);      // Result: 8
Utils::performOperation(5, 3, Utils::multiply); // Result: 15
```

---

## Code Organization

### Include Files (`include/`)
- `Constants.h` — Typed `const` system constants + `CustomerId` typedef
- `Enums.h` — `typedef enum CustomerStatus_ { ... } CustomerStatus`
- `Globals.h` — `CustomerCounter` typedef + global counter; `initializeGlobals` has a default argument
- `Utils.h` — Static utility methods + `BinaryIntOp` function-pointer typedef
- `Customer.h` — `ContactInfo` struct alias + `Entity` → `Person` → `Customer` chain + `Self`/`Ptr` self-typedefs
- `Bank.h` — `CustomerList` (container alias) + `CustomerIter` (iterator alias) + default `CustomerStatus` parameter

### Source Files (`src/`)
- `Globals.cpp` — Global variable definitions
- `Utils.cpp` — Utility implementations
- `Customer.cpp` — Customer management
- `Bank.cpp` — Bank core logic (uses `CustomerIter` to walk the list)
- `main.cpp` — UI and entry point (4-option menu, includes function-pointer demo)

---

## Enums

```cpp
typedef enum CustomerStatus_ {
    ACTIVE,
    INACTIVE
} CustomerStatus;
```

---

## Inheritance Chain

```
Entity                      // virtual ~Entity(); CustomerId customerId
  └── Person                // firstName, lastName
        └── Customer        // ContactInfo contact; CustomerStatus status
                            // typedef Customer Self; typedef Self* Ptr;
```

---

## Build Reference

| Platform | Compiler | Standard | Make command | Executable |
|----------|----------|----------|--------------|------------|
| Linux    | GCC 4.x+ | C++03    | `make`           | `./BankSystem`     |
| macOS    | Clang    | C++03    | `make`           | `./BankSystem`     |
| Windows  | MinGW    | C++03    | `mingw32-make`   | `BankSystem.exe`   |

```bash
# Linux / macOS
make               # build
make clean         # remove build artifacts
./BankSystem       # run
```

```cmd
:: Windows (cmd.exe with MinGW in PATH)
mingw32-make         :: build
mingw32-make clean   :: remove build artifacts
BankSystem.exe       :: run
```

The Makefile auto-detects Windows via the `OS=Windows_NT` env var and swaps
`mkdir`/`rm` commands for `mkdir`/`rmdir /S /Q`/`del /Q`.

---

## Troubleshooting

| Problem | Likely Cause | Fix |
|---------|-------------|-----|
| Won't compile | Compiler too old | Use GCC 4.x+ or Clang |
| Validation keeps failing | Email/phone format | Check against patterns above |
| No customers in list | None registered yet | Use option 1 to create a customer first |

---

**Version**: 2.1
**Last Updated**: April 2026
