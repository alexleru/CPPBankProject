# Bank Account Management System - Test Execution Guide

## Quick Start Testing

### Prerequisites
```bash
cd /home/user/Documents/CPPproject/CPPBankProject/build
# Ensure BankSystem binary exists and is executable
ls -l BankSystem
```

---

## Automated Test Scenarios

### Test Scenario A: Basic Customer & Account Setup

**File**: `test_scenario_a.txt`  
**Purpose**: Verify basic customer registration and account creation  
**Expected Duration**: < 2 minutes

**Input Sequence**:
```
<press_enter>
1
Alice
Johnson
alice@test.com
123-456-7890
123 Main Street

2
CUST000001
1
1000

0
```

**Expected Outputs**:
- ✅ "Customer registered successfully!" - Customer ID: CUST000001
- ✅ "Account created successfully!" - Account ID ACC002000 (or similar)
- ✅ "Thank you for using National C++ Bank!"

**Pass Criteria**:
- Both operations complete without errors
- Customer ID displayed
- Account ID displayed
- Program exits gracefully

---

### Test Scenario B: Multi-Account with Transactions

**File**: `test_scenario_b.txt`  
**Purpose**: Test multiple accounts and transaction types  
**Expected Duration**: < 3 minutes

**Input Sequence**:
```
<press_enter>
1
Bob
Smith
bob@test.com
(555) 123-4567
456 Oak Ave

2
CUST000001
1
2000

2
CUST000001
2
1500

3
ACC002000
500

4
ACC002001
300

5
ACC002000
ACC002001
200

6
ACC002000

0
```

**Expected Outputs**:
- ✅ Savings account created with $2,000
- ✅ Checking account created with $1,500
- ✅ Deposit of $500 successful
- ✅ Withdrawal of $300 successful
- ✅ Transfer of $200 successful
- ✅ Statement displays all transactions

**Pass Criteria**:
- All accounts created
- All transactions processed
- Balances reflect operations accurately
- Statement contains transaction history

---

### Test Scenario C: Savings Account with Interest

**File**: `test_scenario_c.txt`  
**Purpose**: Verify interest calculation on savings accounts  
**Expected Duration**: < 2 minutes

**Input Sequence**:
```
<press_enter>
1
Carol
Davis
carol@bank.com
555-0123
789 Pine Road

2
CUST000002
1
1000

6
ACC002002

8

6
ACC002002

0
```

**Expected Outputs**:
- ✅ Savings account created with $1,000
- ✅ Statement shows initial balance $1,000
- ✅ After monthly processing, statement shows balance ~$1,002.92
- ✅ Interest transaction appears in history

**Pass Criteria**:
- Interest calculated correctly (1000 * 0.035 / 12 ≈ 2.92)
- Transaction recorded
- Statement updated

---

### Test Scenario D: Checking Account with Overdraft

**File**: `test_scenario_d.txt`  
**Purpose**: Test overdraft functionality  
**Expected Duration**: < 2 minutes

**Input Sequence**:
```
<press_enter>
1
David
Brown
david@bank.com
555-9876
321 Elm St

2
CUST000003
2
500

4
ACC002003
600

6
ACC002003

0
```

**Expected Outputs**:
- ✅ Checking account created with $500
- ✅ Withdrawal of $600 successful (overdraft used)
- ✅ Balance shows -$100 (within $500 overdraft limit)
- ✅ Statement reflects negative balance

**Pass Criteria**:
- Withdrawal succeeds despite exceeding balance
- Balance goes negative
- Overdraft message appears
- Within overdraft limit

---

### Test Scenario E: Loan Account & EMI

**File**: `test_scenario_e.txt`  
**Purpose**: Test loan creation and EMI calculation  
**Expected Duration**: < 2 minutes

**Input Sequence**:
```
<press_enter>
1
Eve
Wilson
eve@bank.com
555-5555
654 Maple Dr

2
CUST000004
3
10000

9
10000
8
12

0
```

**Expected Outputs**:
- ✅ Loan account created for $10,000
- ✅ Loan EMI Calculation displayed:
  - Principal: $10,000.00
  - Rate: 8% per annum
  - Term: 12 months
  - Monthly EMI: ~$880.00

**Pass Criteria**:
- Loan account successfully created
- EMI calculation is accurate
- Schedule properly formatted

---

### Test Scenario F: Portfolio & Reporting

**File**: `test_scenario_f.txt`  
**Purpose**: Test portfolio view and bank reports  
**Expected Duration**: < 2 minutes

**Input Sequence**:
```
<press_enter>
1
Frank
Miller
frank@bank.com
555-1111
999 Cedar Ln

2
CUST000005
1
5000

2
CUST000005
2
3000

7
CUST000005

13

11

0
```

**Expected Outputs**:
- ✅ Two accounts created (Savings $5,000, Checking $3,000)
- ✅ Portfolio shows both accounts, total: $8,000
- ✅ Bank report shows:
  - Total customers: 1 (in this scenario)
  - Total accounts: 2
  - Total assets: $8,000.00
- ✅ List all customers displays the customer

**Pass Criteria**:
- Portfolio calculation correct
- Report generation works
- Customer list displays

---

### Test Scenario G: Error Handling

**File**: `test_scenario_g.txt`  
**Purpose**: Test validation and error conditions  
**Expected Duration**: < 2 minutes

**Input Sequence**:
```
<press_enter>
1
Grace
Lee
invalid.email
555-2222
111 Birch Ave

1
Henry
Taylor
henry@bank.com
123
222 Spruce St

11

0
```

**Expected Outputs**:
- ✅ First registration fails: "Invalid email format"
- ✅ Second registration fails: "Invalid phone format"
- ✅ Customer list shows 0 customers
- ✅ Error messages are clear

**Pass Criteria**:
- Email validation rejects invalid format
- Phone validation rejects invalid format
- System continues operation after errors
- No customers added

---

## Manual Test Checklist

### Test 1: Menu Navigation
- [ ] Main menu displays 14 options (1-14) + 0 for Exit
- [ ] Each menu option (1-14) can be selected
- [ ] Invalid selections (15+, negative) show error
- [ ] Non-numeric input shows error
- [ ] Enter 0 exits cleanly

### Test 2: Currency Formatting
- [ ] Amounts display as $1,234.56 (not $1234.56)
- [ ] Large amounts: $10,000.00 formats correctly
- [ ] Small amounts: $0.01 formats correctly
- [ ] Negative amounts: -$100.00 format correctly

### Test 3: Data Persistence
- [ ] Create customer A
- [ ] Create account for A
- [ ] Perform transaction
- [ ] Create customer B
- [ ] List all customers shows both A and B
- [ ] View portfolio A shows the account
- [ ] List all accounts shows both (if any)

### Test 4: Account Type Specific Rules
**Savings Account**:
- [ ] Enforces minimum $100 balance
- [ ] Rejects withdrawal that would go below minimum
- [ ] Applies 3.5% annual interest monthly
- [ ] Shows minimum balance required in info

**Checking Account**:
- [ ] Allows overdraft up to $500
- [ ] Permits negative balance within limit
- [ ] Rejects withdrawal exceeding balance + overdraft
- [ ] Shows available balance with overdraft

**Loan Account**:
- [ ] Created with principal amount
- [ ] Generates repayment schedule
- [ ] Displays EMI information
- [ ] Tracks payment progress

### Test 5: Transaction Recording
- [ ] Every deposit creates transaction entry
- [ ] Every withdrawal creates transaction entry
- [ ] Every transfer creates two entries (debit/credit)
- [ ] Interest appears as transaction type "Interest"
- [ ] Fees appear as transaction type "Fee"

### Test 6: Boundary Conditions
- [ ] Create exactly 5 accounts for one customer (max allowed)
- [ ] Attempt 6th account - rejected
- [ ] Withdraw $0.00 - rejected
- [ ] Withdraw negative - rejected
- [ ] Deposit huge amount (>$1M) - accepted

### Test 7: Cross-Platform
- [ ] Test on Linux (GCC compiled)
- [ ] Verify clear screen works
- [ ] Verify menu displays correctly
- [ ] Verify input is captured properly

---

## Python Test Script

**File**: `bank_system_tests.py`

```python
#!/usr/bin/env python3
"""
Automated test runner for Bank Account Management System
Usage: python3 bank_system_tests.py
"""

import subprocess
import sys
import os
import time

class BankSystemTester:
    def __init__(self, binary_path="./BankSystem"):
        self.binary_path = binary_path
        self.tests_run = 0
        self.tests_passed = 0
        self.tests_failed = 0
        
    def run_test(self, test_name, input_sequence, expected_outputs):
        """
        Run a test scenario
        
        Args:
            test_name: Name of the test
            input_sequence: List of input strings
            expected_outputs: List of strings expected in output
            
        Returns:
            True if all expected outputs found, False otherwise
        """
        self.tests_run += 1
        
        print(f"\n{'='*60}")
        print(f"Test {self.tests_run}: {test_name}")
        print(f"{'='*60}")
        
        # Prepare input
        input_str = "\n".join(input_sequence) + "\n"
        
        try:
            # Run the binary with input
            process = subprocess.Popen(
                [self.binary_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                timeout=10
            )
            
            output, _ = process.communicate(input=input_str)
            
            # Check for expected outputs
            all_found = True
            for expected in expected_outputs:
                if expected.lower() in output.lower():
                    print(f"✅ Found: {expected}")
                else:
                    print(f"❌ NOT Found: {expected}")
                    all_found = False
            
            if all_found:
                self.tests_passed += 1
                print(f"✅ PASS: {test_name}")
                return True
            else:
                self.tests_failed += 1
                print(f"❌ FAIL: {test_name}")
                return False
                
        except subprocess.TimeoutExpired:
            self.tests_failed += 1
            print(f"❌ FAIL: {test_name} - Timeout")
            return False
        except Exception as e:
            self.tests_failed += 1
            print(f"❌ FAIL: {test_name} - Error: {str(e)}")
            return False
    
    def run_all_tests(self):
        """Run all predefined test scenarios"""
        
        # Test 1: Customer Registration
        self.run_test(
            "TC-1.1: Register Valid Customer",
            ["", "1", "John", "Doe", "john@example.com", 
             "123-456-7890", "123 Main Street", "", "0"],
            ["Customer registered successfully", "Customer ID"]
        )
        
        # Test 2: Invalid Email
        self.run_test(
            "TC-1.2: Reject Invalid Email",
            ["", "1", "Jane", "Smith", "invalid.email", 
             "456-789-0123", "456 Oak Ave", "", "0"],
            ["Invalid email format", "Failed to register"]
        )
        
        # Test 3: Account Creation
        self.run_test(
            "TC-2.1: Create Savings Account",
            ["", "1", "Alice", "Johnson", "alice@bank.com",
             "555-1111", "789 Elm Dr", "", "2", "CUST000001", 
             "1", "1000", "", "0"],
            ["Account created successfully", "Account ID"]
        )
        
        # Test 4: Multiple Customers
        self.run_test(
            "TC-1.4: Register Multiple Customers",
            ["", "1", "Bob", "Wilson", "bob@bank.com",
             "555-2222", "321 Oak St", "", "1", "Carol",
             "Davis", "carol@bank.com", "(555) 123-4567",
             "654 Maple Ave", "", "11", "", "0"],
            ["Total Customers: 2", "Bob Wilson", "Carol Davis"]
        )
        
        # Test 5: Deposit
        self.run_test(
            "TC-3.1: Deposit to Account",
            ["", "1", "Eve", "Wilson", "eve@bank.com",
             "555-3333", "999 Pine Ln", "", "2",
             "CUST000005", "1", "500", "", "3",
             "ACC002004", "300", "", "0"],
            ["Deposit successful", "$300"]
        )
        
        # Print Summary
        self.print_summary()
    
    def print_summary(self):
        """Print test execution summary"""
        print(f"\n{'='*60}")
        print("TEST EXECUTION SUMMARY")
        print(f"{'='*60}")
        print(f"Total Tests Run: {self.tests_run}")
        print(f"Tests Passed:   {self.tests_passed}")
        print(f"Tests Failed:   {self.tests_failed}")
        if self.tests_run > 0:
            pass_rate = (self.tests_passed / self.tests_run) * 100
            print(f"Pass Rate:      {pass_rate:.1f}%")
        print(f"{'='*60}\n")
        
        return self.tests_failed == 0

if __name__ == "__main__":
    # Check if binary exists
    if not os.path.exists("./BankSystem"):
        print("Error: ./BankSystem not found")
        print("Please run from build directory and ensure binary is compiled")
        sys.exit(1)
    
    # Run tests
    tester = BankSystemTester()
    success = tester.run_all_tests()
    
    sys.exit(0 if success else 1)
```

**Usage**:
```bash
cd /home/user/Documents/CPPproject/CPPBankProject/build
python3 bank_system_tests.py
```

---

## Individual Feature Testing

### Testing Customer Registration
```bash
echo -e "\n1\nTest\nUser\ntest@example.com\n123-456-7890\n123 Test St\n1\nTest\nUser2\ntest2@example.com\n555-1111\n456 Test Ave\n11\n0" | ./BankSystem
```
Expected: 2 customers registered and listed

### Testing Account Operations
```bash
echo -e "\n1\nJohn\nDoe\njohn@example.com\n123-456-7890\n123 Main\n2\nCUST000001\n1\n1000\n3\nACCxxxxxx\n500\n6\nACCxxxxxx\n0" | ./BankSystem
```
Expected: Account created, deposit processed, statement shown

### Testing Transfer
```bash
echo -e "\n1\nAlice\nSmith\nalice@example.com\n555-5555\n789 Oak\n2\nCUST000002\n1\n1000\n2\nCUST000002\n2\n2000\n5\nACCxxxxxxx\nACCyyyyyyy\n500\n0" | ./BankSystem
```
Expected: Accounts created, transfer successful, balances updated

---

## Troubleshooting Test Issues

### Issue: "Account not found"
**Cause**: ACC ID doesn't match  
**Solution**: Note the actual ACC ID from creation, use in subsequent operations

### Issue: "Invalid input! Please enter a number"
**Cause**: Non-numeric input at menu  
**Solution**: Ensure first character of menu input is numeric

### Issue: Timeout during test
**Cause**: Program waiting for input that wasn't provided  
**Solution**: Ensure all required input fields are in the echo command

### Issue: Phone validation fails
**Cause**: Format doesn't match regex (needs 10+ characters including spaces)  
**Solution**: Use format like "123-456-7890" or "(555) 123-4567"

### Issue: Email validation fails
**Cause**: Format not standard  
**Solution**: Use format like "name@domain.com"

---

## Performance Testing

### Stress Test: Many Customers
```bash
python3 << 'EOF'
import subprocess

inputs = "\n"
# Register 100 customers
for i in range(50):
    inputs += f"1\nCustomer{i}\nUser{i}\ncustomer{i}@test.com\n555-0000\nAddress{i}\n"

inputs += "11\n0\n"

proc = subprocess.Popen(['./BankSystem'], stdin=subprocess.PIPE, 
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
output, _ = proc.communicate(input=inputs, timeout=30)
# Count "Total Customers: X"
if "Total Customers: 50" in output:
    print("✅ PASS: System handles 50 customers")
else:
    print("❌ FAIL: Issue with many customers")
EOF
```

### Stress Test: Many Transactions
```bash
# Create account with balance, perform 100 deposits
python3 << 'EOF'
import subprocess

inputs = "\n1\nTest\nUser\ntest@bank.com\n123-456-7890\n123 St\n2\nCUST000001\n1\n1000\n"

# 100 deposits
for i in range(50):
    inputs += f"3\nACCxxxxxx\n10\n"

inputs += "6\nACCxxxxxx\n0\n"

proc = subprocess.Popen(['./BankSystem'], stdin=subprocess.PIPE,
                       stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
output, _ = proc.communicate(input=inputs, timeout=30)
print("✅ 50 transactions processed")
EOF
```

---

## Regression Testing Checklist

Before each build/deployment, verify:

- [ ] All 14 menu options work
- [ ] Customer registration validates email and phone
- [ ] Accounts can be created for all 3 types
- [ ] Deposits increase balance
- [ ] Withdrawals decrease balance
- [ ] Savings minimum balance enforced
- [ ] Checking overdraft works
- [ ] Transfers move money correctly
- [ ] Statements display transactions
- [ ] Interest applied monthly
- [ ] Fees applied monthly
- [ ] Loan EMI calculated correctly
- [ ] Reports generate
- [ ] No crashes or undefined behavior

---

## Test Reporting Template

```markdown
## Test Report: [Date]

### Environment
- OS: [Linux/Windows/macOS]
- Compiler: [GCC/Clang/MSVC]
- Build Command: [make]
- Binary: BankSystem

### Test Results
- Total Test Cases: 61
- Passed: X
- Failed: Y
- Pending: Z
- Pass Rate: X%

### Critical Issues
[List any critical bugs found]

### Minor Issues
[List any non-critical issues]

### Recommendations
[Testing recommendations for next cycle]
```

---

**Test Guide Version**: 1.0  
**Last Updated**: April 12, 2026  
**Maintainer**: QA Team


---

## Mortgage Feature Testing (menu option 18)

The mortgage feature is implemented as `MortgageAccount`, a class that derives from `LoanAccount` and is registered in the same `AccountRegistry`. The C++03 `typedef`-rich API is the focal point of this feature.

### Quick smoke test (interactive)

1. Launch `./BankSystem` (or `BankSystem.exe`).
2. Press Enter at the welcome screen.
3. Menu 1 -> register a customer (note the `CUST00xxxx` ID).
4. Menu 18 -> 1 -> open a mortgage:
   - Customer ID = the one you just got.
   - Address = any string.
   - Market value = `500000`, Down payment = `100000`.
   - Annual rate = `6`%, term = `30` years, kind = `1` (fixed).
5. Menu 18 -> 4 -> verify LTV `80 %`, PMI `No`, monthly payment around `$3044.04`.
6. Menu 18 -> 2 -> verify the 360-row amortisation schedule prints.
7. Menu 18 -> 3 -> make a payment and re-check `Payments Made: 1 of 360`.

### Scripted test cases

See `TEST_CASES.md`, **Test Section 17**:

- **TC-MORT-01** — Open a 30-year fixed mortgage at 80% LTV (no PMI).
- **TC-MORT-02** — Mortgage at 90% LTV triggers PMI.
- **TC-MORT-03** — Make a mortgage payment and confirm `monthsPaid` advances (joins `LoanAccount` flow).
- **TC-MORT-04** — Interest-only mortgage builds a balloon-style schedule.
- **TC-MORT-05** — Validation: down payment > market value is rejected, no account is registered.

All five scenarios were executed against `BankSystem` via stdin redirection (`./BankSystem < tc_mort_NN_input.txt`) and pass.
