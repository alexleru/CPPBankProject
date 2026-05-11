# Documentation Index - Bank Customer Management System

## Documentation Files

### 1. README.md
**Purpose**: Project overview and general information
**Contents**:
- Project structure (host application + native age-verification library)
- Technical features and OOP design
- Catalog of C++03 syntactic constructs showcased (typedefs, struct/enum
  aliases, container & iterator aliases, function-pointer typedef,
  self-typedef, two-level inheritance, default arguments)
- Key classes and the `Entity` → `Person` → `Customer` inheritance chain
- Compilation instructions
- Feature list and 5-option menu (function-pointer demo + native age check)
- Cross-platform build (GNU make / `mingw32-make`)
- Usage examples

**When to use**: First-time setup, understanding architecture

---

### 2. QUICK_REFERENCE.md
**Purpose**: Quick lookup and reference information
**Contents**:
- Menu options (5 options)
- Customer field requirements (fields are bundled in the `ContactInfo` struct)
- Validation rules (email, phone)
- Constants and typedefs from `Constants.h` (`CustomerId`)
- Static methods reference + `BinaryIntOp` function-pointer typedef
- Native age-verification ABI + host wrapper summary
- Code organization (typedef catalog per header, + native library tree)
- Inheritance chain diagram (`Entity` → `Person` → `Customer`)
- Troubleshooting

**When to use**: Quick lookups, configuration changes, troubleshooting

---

### 3. NATIVE_LIBRARY.md
**Purpose**: Native age-verification library — architecture and build
**Contents**:
- Layout of `native/` (shared header, shared source, per-OS build scripts)
- Public C ABI (`verify_age_21` return codes)
- Build steps via top-level Makefile or standalone scripts
- How `_WIN32` / `__linux__` selects the binary at compile time
- Guidance for adding further native features

**When to use**: Building or extending the native library, debugging
`LoadLibrary` / `dlopen` failures

---

### 4. TEST_CASES.md
**Purpose**: Test case documentation
**Contents**: Test cases organized by feature — customer registration,
listing, validation, function-pointer demo, **and the native
age-verification library (TC-5.*)**

**When to use**: Quality assurance, regression testing

---

### 5. TESTING_GUIDE.md
**Purpose**: Practical testing guidance
**Contents**: Test scenarios with input sequences, manual test checklist,
including Scenario F (native age verification) and a "library not loaded"
troubleshooting note

**When to use**: Running tests, debugging

---

### 6. TC_SPEC.md
**Purpose**: Machine-readable test spec consumed by `run_tests.py`
**Contents**: Stdin commands + expected substrings for every automated
case (TC-1.*, TC-2.*, TC-3.*, TC-4.*, TC-5.*)

**When to use**: Adding or editing automated cases for `python run_tests.py`

---

## Quick Navigation

### "How do I...?"
- ...compile the project? → README.md (Compilation & Build)
- ...run the program? → README.md
- ...create a customer? → QUICK_REFERENCE.md (Menu Options)
- ...build/extend the native library? → NATIVE_LIBRARY.md
- ...run the test suite? → TESTING_GUIDE.md ("Running the Automated Suite")
- ...test a feature? → TEST_CASES.md
- ...look up a constant? → QUICK_REFERENCE.md (Constants)

### "What is...?"
- ...the customer ID format? → QUICK_REFERENCE.md (Customer ID Format)
- ...the validation rules? → QUICK_REFERENCE.md (Validation Rules)
- ...an enum value? → QUICK_REFERENCE.md (Enums)
- ...the inheritance chain? → QUICK_REFERENCE.md (Inheritance Chain)
- ...the `BinaryIntOp` typedef? → QUICK_REFERENCE.md (Static Methods Reference)
- ...the native ABI? → QUICK_REFERENCE.md (Native Age Verification) or NATIVE_LIBRARY.md

### "Test Case for..."
- ...customer registration → TEST_CASES.md (Section 1)
- ...listing customers → TEST_CASES.md (Section 2)
- ...input validation → TEST_CASES.md (Section 3)
- ...function pointer demo → TEST_CASES.md (Section 4)
- ...native age verification → TEST_CASES.md (Section 5)

---

## By User Role

| Role | Documents | Purpose |
|------|-----------|---------|
| Developer | README, QUICK_REFERENCE, NATIVE_LIBRARY | Architecture, building, native ABI |
| QA Tester | TEST_CASES, TESTING_GUIDE, TC_SPEC | Test execution and automation |
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
3. `NATIVE_LIBRARY.md` — How the dynamic loading works and how to extend it
4. `README.md` — Build instructions

### For QA:
1. `TESTING_GUIDE.md` — Manual scenarios + checklist
2. `TEST_CASES.md` — Numbered cases per feature
3. `TC_SPEC.md` — Spec used by `run_tests.py`

---

**Last Updated**: May 2026
