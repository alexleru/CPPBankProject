# Test Cases — Machine-Readable Spec

This file is consumed by `run_tests.py`. Each test case has three parts:
- a level-2 heading `## <TC-Name>: <description>`
- a `### commands` fenced block — lines are fed to the program's stdin
- a `### expected stdout` fenced block — every non-empty line must appear as a
  substring somewhere in the program's stdout

Substring matching (rather than exact match) is used because the menu prompts
share lines with the next field's input.

---

## How to run

1. Build the binary (from the project root):
   ```bash
   # Linux / macOS
   make
   # Windows (MinGW, cmd.exe)
   mingw32-make
   ```
2. Run the harness:
   ```bash
   python run_tests.py
   ```
3. Each case prints `[PASS]` / `[FAIL]` to the console, and a full log is
   written to `logs/test_run_<YYYY-MM-DD_HH-MM-SS>.log`. Exit code is `0` on
   all-pass, `1` on any failure.

> Menu: option 5 exits. Tests therefore end with `5` (and the menu-range
> error message reads "Enter a number between 1 and 5.").
>
> The native age-verification cases (TC-5.*) require the shared library to
> exist at `native/windows/age_verifier.dll` (Windows) or
> `native/linux/libage_verifier.so` (Linux). `make` (or `mingw32-make`)
> builds it automatically.

---

## TC-1.1: Register Valid Customer

### commands
```
1
John
Doe
john.doe@example.com
123-456-7890
123 Main Street
2
5
```

### expected stdout
```
Customer created successfully. ID: CUST001000
Customer ID : CUST001000
Name        : John Doe
Email       : john.doe@example.com
Status      : Active
Total: 1 customer(s).
Goodbye.
```

---

## TC-1.2: Register Customer with Invalid Email

### commands
```
1
Jane
Doe
not-an-email
555-123-4567
Some Address
5
```

### expected stdout
```
Error: Invalid email address.
Goodbye.
```

---

## TC-1.3: Register Customer with Invalid Phone

### commands
```
1
Jane
Doe
jane@example.com
123
Some Address
5
```

### expected stdout
```
Error: Invalid phone (min 10 digits).
Goodbye.
```

---

## TC-1.4: Register Multiple Customers

### commands
```
1
Alice
Johnson
alice@bank.com
123-456-7890
100 First Ave
1
Bob
Wilson
bob@bank.com
(555) 123-4567
200 Second St
1
Carol
Davis
carol@bank.com
555-987-6543
300 Third Blvd
2
5
```

### expected stdout
```
Customer created successfully. ID: CUST001000
Customer created successfully. ID: CUST001001
Customer created successfully. ID: CUST001002
Name        : Alice Johnson
Name        : Bob Wilson
Name        : Carol Davis
Total: 3 customer(s).
Goodbye.
```

---

## TC-1.5: Empty First Name Field

### commands
```
1

Doe
jane@example.com
555-123-4567
Some Address
5
```

### expected stdout
```
Error: First name is required.
Goodbye.
```

---

## TC-1.6: Empty Address Field

### commands
```
1
Jane
Doe
jane@example.com
555-123-4567

5
```

### expected stdout
```
Error: Address is required.
Goodbye.
```

---

## TC-2.1: List Customers — Empty Bank

### commands
```
2
5
```

### expected stdout
```
No customers registered.
Goodbye.
```

---

## TC-2.2: List Customers — After Registration

### commands
```
1
Alice
Johnson
alice@bank.com
123-456-7890
100 First Ave
1
Bob
Wilson
bob@bank.com
555-123-4567
200 Second St
2
5
```

### expected stdout
```
Customer ID : CUST001000
Customer ID : CUST001001
Name        : Alice Johnson
Name        : Bob Wilson
Total: 2 customer(s).
Goodbye.
```

---

## TC-2.3: Customer Status Displayed as Active

### commands
```
1
Alice
Johnson
alice@bank.com
123-456-7890
100 First Ave
2
5
```

### expected stdout
```
Status      : Active
Goodbye.
```

---

## TC-3.1: Invalid Menu Choice (Out of Range)

### commands
```
9
5
```

### expected stdout
```
Enter a number between 1 and 5.
Goodbye.
```

---

## TC-3.2: Non-Numeric Menu Input

### commands
```
abc
5
```

### expected stdout
```
Enter a number between 1 and 5.
Goodbye.
```

---

## TC-3.3: Exit via Option 5

### commands
```
5
```

### expected stdout
```
Goodbye.
```

---

## TC-4.1: Add Callback via performOperation

### commands
```
3
5
```

### expected stdout
```
=== Function Pointer Demo ===
Result: 8
Goodbye.
```

---

## TC-4.2: Multiply Callback via performOperation

### commands
```
3
5
```

### expected stdout
```
=== Function Pointer Demo ===
Result: 15
Goodbye.
```

---

## TC-5.1: Age Verification — Adult Returns TRUE

> Native library loaded via `LoadLibrary` / `dlopen`; birth date entered as
> `day / month / year`. Anyone born `1/1/1990` is well over 21 for the
> foreseeable life of this test.

### commands
```
4
1
1
1990
5
```

### expected stdout
```
=== Age Verification (21+) ===
Loaded native library:
Result: TRUE  -- age is 21 or older.
Goodbye.
```

---

## TC-5.2: Age Verification — Minor Returns FALSE

> Born `1/1/2020`: turns 21 on `2041-01-01`. Stays under 21 for the
> foreseeable life of this test.

### commands
```
4
1
1
2020
5
```

### expected stdout
```
=== Age Verification (21+) ===
Loaded native library:
Result: FALSE -- age is below 21.
Goodbye.
```

---

## TC-5.3: Age Verification — Invalid Date (Feb 30)

> February never has 30 days; the native library returns
> `AGE_VERIFIER_BAD_INPUT (-1)` and the host prints the invalid-date message.

### commands
```
4
30
2
2000
5
```

### expected stdout
```
=== Age Verification (21+) ===
Loaded native library:
Invalid date (not a real calendar day).
Goodbye.
```

---

## TC-5.4: Age Verification — Invalid Date (April 31)

> April only has 30 days; the native library rejects the date.

### commands
```
4
31
4
2020
5
```

### expected stdout
```
=== Age Verification (21+) ===
Loaded native library:
Invalid date (not a real calendar day).
Goodbye.
```
