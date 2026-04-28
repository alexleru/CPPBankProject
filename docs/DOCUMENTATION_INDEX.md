# Documentation Index - Bank Customer Management System

## Documentation Files

### 1. README.md
**Purpose**: Project overview and general information
**Contents**:
- Project structure (11 files: 6 headers + 5 sources, ~344 lines)
- Technical features and OOP design
- Catalog of C++03 syntactic constructs showcased (typedefs, struct/enum
  aliases, container & iterator aliases, function-pointer typedef,
  self-typedef, two-level inheritance, default arguments)
- Key classes and the `Entity` → `Person` → `Customer` inheritance chain
- Compilation instructions
- Feature list and 4-option menu (includes function-pointer demo)
- Cross-platform build (GNU make / `mingw32-make`)
- Usage examples

**When to use**: First-time setup, understanding architecture

---

### 2. QUICK_REFERENCE.md
**Purpose**: Quick lookup and reference information
**Contents**:
- Menu options (4 options)
- Customer field requirements (fields are bundled in the `ContactInfo` struct)
- Validation rules (email, phone)
- Constants and typedefs from `Constants.h` (`CustomerId`)
- Static methods reference + `BinaryIntOp` function-pointer typedef
- Code organization (typedef catalog per header)
- Inheritance chain diagram (`Entity` → `Person` → `Customer`)
- Troubleshooting

**When to use**: Quick lookups, configuration changes, troubleshooting

---

### 3. TEST_CASES.md
**Purpose**: Test case documentation
**Contents**: Test cases organized by feature (customer registration, listing, validation)

**When to use**: Quality assurance, regression testing

---

### 4. TESTING_GUIDE.md
**Purpose**: Practical testing guidance
**Contents**: Test scenarios with input sequences, manual test checklist

**When to use**: Running tests, debugging

---

## Quick Navigation

### "How do I...?"
- ...compile the project? → README.md (Compilation & Build)
- ...run the program? → README.md
- ...create a customer? → QUICK_REFERENCE.md (Menu Options)
- ...test a feature? → TEST_CASES.md
- ...look up a constant? → QUICK_REFERENCE.md (Constants)

### "What is...?"
- ...the customer ID format? → QUICK_REFERENCE.md (Customer ID Format)
- ...the validation rules? → QUICK_REFERENCE.md (Validation Rules)
- ...an enum value? → QUICK_REFERENCE.md (Enums)
- ...the inheritance chain? → QUICK_REFERENCE.md (Inheritance Chain)
- ...the `BinaryIntOp` typedef? → QUICK_REFERENCE.md (Static Methods Reference)

### "Test Case for..."
- ...customer registration → TEST_CASES.md (Section 1)
- ...listing customers → TEST_CASES.md (Section 2)
- ...input validation → TEST_CASES.md (Section 3)

---

## By User Role

| Role | Documents | Purpose |
|------|-----------|---------|
| Developer | README, QUICK_REFERENCE | Architecture, building |
| QA Tester | TEST_CASES, TESTING_GUIDE | Test execution |
| Maintainer | All docs | Comprehensive reference |

---

## Recommended Reading Order

### For First-Time Users:
1. `README.md` — Overview
2. `QUICK_REFERENCE.md` — Menu and features
3. `TESTING_GUIDE.md` — Try Scenario A

### For Developers:
1. `README.md` — Architecture section
2. `QUICK_REFERENCE.md` — Code organization and constants
3. `README.md` — Build instructions

---

**Last Updated**: April 2026
