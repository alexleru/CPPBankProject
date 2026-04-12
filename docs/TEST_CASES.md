# Bank Account Management System - Test Cases

## Overview
Comprehensive test cases for the Bank Account Management System covering all major features and edge cases.

---

## Test Section 1: Customer Registration

### Test Case 1.1: Register Valid Customer
**ID**: TC-1.1  
**Priority**: High  
**Objective**: Verify valid customer registration

| Field | Value |
|-------|-------|
| First Name | John |
| Last Name | Doe |
| Email | john.doe@example.com |
| Phone | 123-456-7890 |
| Address | 123 Main Street |

**Expected Result**: Customer registered successfully with unique Customer ID  
**Status**: ✅ PASS

---

### Test Case 1.2: Register Customer with Invalid Email
**ID**: TC-1.2  
**Priority**: High  
**Objective**: Verify email validation

| Field | Value |
|-------|-------|
| First Name | Jane |
| Last Name | Smith |
| Email | invalid.email@invalid |
| Phone | 456-789-0123 |
| Address | 456 Oak Avenue |

**Expected Result**: Registration fails with "Invalid email format" error  
**Status**: ✅ PASS

---

### Test Case 1.3: Register Customer with Invalid Phone
**ID**: TC-1.3  
**Priority**: High  
**Objective**: Verify phone validation

| Field | Value |
|-------|-------|
| First Name | Bob |
| Last Name | Wilson |
| Email | bob.wilson@example.com |
| Phone | 123 |
| Address | 789 Pine Road |

**Expected Result**: Registration fails with "Invalid phone format" error  
**Status**: ✅ PASS

---

### Test Case 1.4: Register Multiple Customers
**ID**: TC-1.4  
**Priority**: High  
**Objective**: Verify system handles multiple customers

| Customer | Email | Phone |
|----------|-------|-------|
| Alice Johnson | alice@bank.com | 123-456-7890 |
| Bob Wilson | bob@bank.com | (555) 123-4567 |
| Carol Davis | carol@bank.com | 555-987-6543 |

**Expected Result**: All customers registered with unique IDs (CUST000001, CUST000002, CUST000003, etc.)  
**Status**: ✅ PASS

---

### Test Case 1.5: List All Customers
**ID**: TC-1.5  
**Priority**: Medium  
**Objective**: Verify customer listing displays all registered customers

**Input**: Menu Option 11  
**Expected Result**: Displays formatted table with all customers, total count, and details  
**Status**: ✅ PASS

---

## Test Section 2: Account Creation

### Test Case 2.1: Create Savings Account
**ID**: TC-2.1  
**Priority**: High  
**Objective**: Verify successful savings account creation

| Parameter | Value |
|-----------|-------|
| Customer ID | CUST000001 |
| Account Type | Savings (1) |
| Initial Balance | $1,000.00 |
| Interest Rate | 3.5% (default) |

**Expected Result**: Account created with ID ACC002000, balance $1,000.00, minimum balance enforcement enabled  
**Status**: ✅ PASS

---

### Test Case 2.2: Create Checking Account
**ID**: TC-2.2  
**Priority**: High  
**Objective**: Verify successful checking account creation

| Parameter | Value |
|-----------|-------|
| Customer ID | CUST000001 |
| Account Type | Checking (2) |
| Initial Balance | $500.00 |
| Overdraft Limit | $500.00 (default) |

**Expected Result**: Account created with overdraft capability, monthly fee structure enabled  
**Status**: ⏳ PENDING

---

### Test Case 2.3: Create Loan Account
**ID**: TC-2.3  
**Priority**: High  
**Objective**: Verify loan account creation with EMI calculation

| Parameter | Value |
|-----------|-------|
| Customer ID | CUST000002 |
| Account Type | Loan (3) |
| Principal Amount | $10,000.00 |
| Interest Rate | 8% |
| Term | 12 months |

**Expected Result**: Loan account created, EMI calculated (~$880/month), repayment schedule generated  
**Status**: ⏳ PENDING

---

### Test Case 2.4: Maximum Accounts per Customer
**ID**: TC-2.4  
**Priority**: Medium  
**Objective**: Verify MAX_ACCOUNTS_PER_CUSTOMER constraint (max 5)

**Input**: Create 6 accounts for single customer  
**Expected Result**: First 5 succeed, 6th fails with "Maximum number of accounts reached" error  
**Status**: ⏳ PENDING

---

### Test Case 2.5: Create Account with Zero Balance
**ID**: TC-2.5  
**Priority**: Low  
**Objective**: Verify system handles zero initial balance

| Parameter | Value |
|-----------|-------|
| Customer ID | CUST000001 |
| Account Type | Checking |
| Initial Balance | $0.00 |

**Expected Result**: Account created successfully with $0.00 balance  
**Status**: ⏳ PENDING

---

## Test Section 3: Deposit Operations

### Test Case 3.1: Deposit to Savings Account
**ID**: TC-3.1  
**Priority**: High  
**Objective**: Verify successful deposit operation

| Parameter | Value |
|-----------|-------|
| Account ID | ACC002000 |
| Initial Balance | $1,000.00 |
| Deposit Amount | $500.00 |

**Expected Result**: New balance $1,500.00, transaction recorded with type "Deposit"  
**Status**: ⏳ PENDING

---

### Test Case 3.2: Deposit Multiple Times
**ID**: TC-3.2  
**Priority**: High  
**Objective**: Verify multiple deposits update balance correctly

| Deposit # | Amount | Expected Balance |
|-----------|--------|------------------|
| 1 | $100.00 | $1,100.00 |
| 2 | $250.00 | $1,350.00 |
| 3 | $75.50 | $1,425.50 |

**Expected Result**: All transactions recorded in history, balance correct  
**Status**: ⏳ PENDING

---

### Test Case 3.3: Deposit Negative Amount
**ID**: TC-3.3  
**Priority**: High  
**Objective**: Verify validation of negative deposits

**Input**: Attempt to deposit -$100  
**Expected Result**: Error "Deposit amount must be positive"  
**Status**: ⏳ PENDING

---

### Test Case 3.4: Deposit Zero Amount
**ID**: TC-3.4  
**Priority**: Medium  
**Objective**: Verify handling of zero deposit

**Input**: Attempt to deposit $0.00  
**Expected Result**: Error "Deposit amount must be positive"  
**Status**: ⏳ PENDING

---

## Test Section 4: Withdrawal Operations

### Test Case 4.1: Withdraw from Savings Account with Sufficient Balance
**ID**: TC-4.1  
**Priority**: High  
**Objective**: Verify successful withdrawal

| Parameter | Value |
|-----------|-------|
| Account ID | (Savings Account) |
| Current Balance | $1,500.00 |
| Minimum Balance | $100.00 |
| Withdrawal Amount | $800.00 |

**Expected Result**: New balance $700.00, transaction recorded, withdrawal successful  
**Status**: ⏳ PENDING

---

### Test Case 4.2: Withdraw from Savings Account Violating Minimum Balance
**ID**: TC-4.2  
**Priority**: High  
**Objective**: Verify minimum balance enforcement on savings account

| Parameter | Value |
|-----------|-------|
| Current Balance | $500.00 |
| Minimum Balance Required | $100.00 |
| Attempted Withdrawal | $450.00 |

**Expected Result**: Withdrawal rejected, balance remains $500.00, message shows available for withdrawal is $400.00  
**Status**: ⏳ PENDING

---

### Test Case 4.3: Withdraw from Checking Account with Overdraft
**ID**: TC-4.3  
**Priority**: High  
**Objective**: Verify checking account overdraft capability

| Parameter | Value |
|-----------|-------|
| Balance | $200.00 |
| Overdraft Limit | $500.00 |
| Withdrawal Amount | $600.00 |

**Expected Result**: Withdrawal approved, balance becomes -$400.00, overdraft message displayed  
**Status**: ⏳ PENDING

---

### Test Case 4.4: Withdraw from Checking Account Exceeding Overdraft
**ID**: TC-4.4  
**Priority**: High  
**Objective**: Verify overdraft limit enforcement

| Parameter | Value |
|-----------|-------|
| Balance | $200.00 |
| Overdraft Limit | $500.00 |
| Withdrawal Amount | $750.00 |

**Expected Result**: Withdrawal rejected, available with overdraft is $700.00  
**Status**: ⏳ PENDING

---

### Test Case 4.5: Withdraw Negative Amount
**ID**: TC-4.5  
**Priority**: High  
**Objective**: Verify validation of negative withdrawals

**Input**: Attempt to withdraw -$100.00  
**Expected Result**: Error "Withdrawal amount must be positive"  
**Status**: ⏳ PENDING

---

### Test Case 4.6: Withdraw from Inactive Account
**ID**: TC-4.6  
**Priority**: Medium  
**Objective**: Verify cannot withdraw from closed account

**Input**: Close account, then attempt withdrawal  
**Expected Result**: Error "Cannot withdraw from inactive account"  
**Status**: ⏳ PENDING

---

## Test Section 5: Transfer Operations

### Test Case 5.1: Transfer Between Own Accounts
**ID**: TC-5.1  
**Priority**: High  
**Objective**: Verify inter-account transfer

| Parameter | Value |
|-----------|-------|
| From Account (Checking) | Balance: $1,000.00 |
| To Account (Savings) | Balance: $500.00 |
| Transfer Amount | $300.00 |

**Expected Result**: From account: $700.00, To account: $800.00, both have transfer transactions  
**Status**: ⏳ PENDING

---

### Test Case 5.2: Transfer with Insufficient Funds
**ID**: TC-5.2  
**Priority**: High  
**Objective**: Verify transfer validation

| Parameter | Value |
|-----------|-------|
| From Account Balance | $200.00 |
| Transfer Amount | $500.00 |

**Expected Result**: Transfer failed, both account balances unchanged  
**Status**: ⏳ PENDING

---

### Test Case 5.3: Partial Transfer Reversal on Failure
**ID**: TC-5.3  
**Priority**: High  
**Objective**: Verify automatic reversal if transfer fails

**Scenario**: Transfer initiates successfully from source but fails at destination (hypothetical)  
**Expected Result**: Source account is refunded, maintaining consistency  
**Status**: ⏳ PENDING

---

### Test Case 5.4: Transfer from Invalid Account
**ID**: TC-5.4  
**Priority**: Medium  
**Objective**: Verify error handling for non-existent accounts

**Input**: Transfer from ACC999999 (non-existent)  
**Expected Result**: Error "One or both accounts not found"  
**Status**: ⏳ PENDING

---

## Test Section 6: Account Statements & History

### Test Case 6.1: View Complete Account Statement
**ID**: TC-6.1  
**Priority**: High  
**Objective**: Verify statement shows all transactions with proper formatting

**Setup**: Account with multiple transactions (deposits, withdrawals, transfers)  
**Expected Result**: Statement displays:
  - Account ID
  - Account Type
  - Current Balance
  - Account Status
  - Created Date
  - Formatted transaction table with headers
  - All transactions with ID, type, amount, date/time, status, description

**Status**: ⏳ PENDING

---

### Test Case 6.2: View Statement for Account with No Transactions
**ID**: TC-6.2  
**Priority**: Medium  
**Objective**: Verify empty transaction history handling

**Input**: View statement for newly created account  
**Expected Result**: "No transactions found." message displayed  
**Status**: ⏳ PENDING

---

### Test Case 6.3: View Customer Portfolio
**ID**: TC-6.3  
**Priority**: High  
**Objective**: Verify portfolio shows all customer's accounts with total balance

**Setup**: Customer with 3 accounts (values: $1,200, $500, $2,000)  
**Expected Result**: All 3 accounts displayed with individual balances, total portfolio: $3,700.00  
**Status**: ⏳ PENDING

---

## Test Section 7: Savings Account Features

### Test Case 7.1: Apply Monthly Interest
**ID**: TC-7.1  
**Priority**: High  
**Objective**: Verify monthly interest calculation and application

| Parameter | Value |
|-----------|-------|
| Initial Balance | $1,000.00 |
| Interest Rate | 3.5% per annum |
| Monthly Interest | $2.92 |
| After Processing | $1,002.92 |

**Expected Result**: Interest transaction recorded, balance updated, transaction type "Interest"  
**Status**: ⏳ PENDING

---

### Test Case 7.2: Multiple Interest Applications
**ID**: TC-7.2  
**Priority**: Medium  
**Objective**: Verify compound interest calculation

**Setup**: Account with $1,000, apply interest processing 3 times  
**Expected Result**: 
  - After Month 1: ~$1,002.92
  - After Month 2: ~$1,005.85
  - After Month 3: ~$1,008.79

**Status**: ⏳ PENDING

---

### Test Case 7.3: Interest Not Applied to Closed Account
**ID**: TC-7.3  
**Priority**: Medium  
**Objective**: Verify inactive accounts don't earn interest

**Setup**: Close savings account, then apply monthly processing  
**Expected Result**: No interest transaction recorded  
**Status**: ⏳ PENDING

---

## Test Section 8: Checking Account Features

### Test Case 8.1: Apply Monthly Fee
**ID**: TC-8.1  
**Priority**: High  
**Objective**: Verify monthly maintenance fee deduction

| Parameter | Value |
|-----------|-------|
| Initial Balance | $1,000.00 |
| Monthly Fee | $5.00 |
| After Processing | $995.00 |

**Expected Result**: Fee transaction recorded, transaction type "Fee"  
**Status**: ⏳ PENDING

---

### Test Case 8.2: Monthly Fee with Insufficient Balance
**ID**: TC-8.2  
**Priority**: High  
**Objective**: Verify fee application even with low balance

| Parameter | Value |
|-----------|-------|
| Initial Balance | $2.00 |
| Monthly Fee | $5.00 |
| After Processing | -$3.00 |

**Expected Result**: Fee applied (overdraft used), balance goes negative  
**Status**: ⏳ PENDING

---

### Test Case 8.3: Transaction Counting
**ID**: TC-8.3  
**Priority**: Low  
**Objective**: Verify monthly transaction counter

**Setup**: Perform 5 transactions, apply monthly processing, then perform 3 more  
**Expected Result**: Counter resets after processing, new count is 3  
**Status**: ⏳ PENDING

---

## Test Section 9: Loan Account Features

### Test Case 9.1: Loan EMI Calculation
**ID**: TC-9.1  
**Priority**: High  
**Objective**: Verify EMI calculation using standard formula

| Parameter | Value |
|-----------|-------|
| Principal | $10,000.00 |
| Rate | 8% per annum |
| Term | 12 months |
| Expected EMI | ~$880.00 |

**Expected Result**: EMI calculated correctly, repayment schedule generated with 12 entries  
**Status**: ⏳ PENDING

---

### Test Case 9.2: Loan Repayment Schedule Display
**ID**: TC-9.2  
**Priority**: High  
**Objective**: Verify complete repayment schedule with principal/interest breakdown

**Setup**: Loan account with 12-month term  
**Expected Result**: Schedule shows:
  - Month number
  - EMI amount
  - Principal component
  - Interest component
  - Remaining balance

**Status**: ⏳ PENDING

---

### Test Case 9.3: Make EMI Payment
**ID**: TC-9.3  
**Priority**: High  
**Objective**: Verify loan payment processing

| Parameter | Value |
|-----------|-------|
| Expected EMI | $880.00 |
| Payment Amount | $880.00 |
| Expected Message | Payment 1 of 12 |

**Expected Result**: Transaction recorded, payment counter incremented  
**Status**: ⏳ PENDING

---

### Test Case 9.4: EMI Payment with Incorrect Amount
**ID**: TC-9.4  
**Priority**: Medium  
**Objective**: Verify warning for non-standard payment

| Parameter | Value |
|-----------|-------|
| Expected EMI | $880.00 |
| Actual Payment | $900.00 |

**Expected Result**: Warning displayed, payment still processed  
**Status**: ⏳ PENDING

---

### Test Case 9.5: Complete Loan Payoff
**ID**: TC-9.5  
**Priority**: High  
**Objective**: Verify loan status changes to PAID_OFF after all payments

**Setup**: Make 12 EMI payments on 12-month loan  
**Expected Result**: Loan status changes to "Paid Off", remaining balance: $0.00  
**Status**: ⏳ PENDING

---

### Test Case 9.6: Calculate EMI with Zero Interest
**ID**: TC-9.6  
**Priority**: Low  
**Objective**: Verify EMI calculation with 0% interest

| Parameter | Value |
|-----------|-------|
| Principal | $10,000.00 |
| Rate | 0% |
| Term | 12 months |
| Expected EMI | $833.33 |

**Expected Result**: EMI = Principal / Term  
**Status**: ⏳ PENDING

---

## Test Section 10: Monthly Processing

### Test Case 10.1: Apply Monthly Processing to All Accounts
**ID**: TC-10.1  
**Priority**: High  
**Objective**: Verify batch monthly processing

**Setup**:
- 2 Savings accounts (should earn interest)
- 2 Checking accounts (should pay fees)
- 1 Loan account (should process EMI)

**Expected Result**: All interest, fees, and EMI updates applied consistently  
**Status**: ⏳ PENDING

---

### Test Case 10.2: Monthly Processing Report
**ID**: TC-10.2  
**Priority**: Medium  
**Objective**: Verify confirmation of processing completion

**Input**: Option 8 (Apply Monthly Processing)  
**Expected Result**: Message "Monthly processing completed."  
**Status**: ⏳ PENDING

---

## Test Section 11: Search & Lookup Operations

### Test Case 11.1: Search Account by ID
**ID**: TC-11.1  
**Priority**: High  
**Objective**: Verify account lookup functionality

**Input**: Search for ACC002000  
**Expected Result**: Account information displayed (type, balance, status, creation date)  
**Status**: ⏳ PENDING

---

### Test Case 11.2: Search Non-existent Account
**ID**: TC-11.2  
**Priority**: Medium  
**Objective**: Verify error handling for invalid account ID

**Input**: Search for ACC999999  
**Expected Result**: "Account not found!" message  
**Status**: ⏳ PENDING

---

### Test Case 11.3: Search Customer Accounts
**ID**: TC-11.3  
**Priority**: High  
**Objective**: Verify searching all accounts for specific customer

**Input**: Customer ID CUST000001 with 3 accounts  
**Expected Result**: All 3 customer accounts displayed in portfolio view  
**Status**: ⏳ PENDING

---

## Test Section 12: Bank Reports

### Test Case 12.1: Generate Bank Report
**ID**: TC-12.1  
**Priority**: Medium  
**Objective**: Verify bank-wide report generation

**Setup**:
- 5 customers registered
- 8 total accounts
- Total assets: $25,000.00

**Expected Result**: Report shows:
  - Report generation timestamp
  - Total customers: 5
  - Total accounts: 8
  - Total assets: $25,000.00

**Status**: ⏳ PENDING

---

### Test Case 12.2: Bank Report with No Customers
**ID**: TC-12.2  
**Priority**: Low  
**Objective**: Verify report on empty bank

**Input**: Generate report at start (no data)  
**Expected Result**: Shows 0 customers, 0 accounts, $0.00 assets  
**Status**: ⏳ PENDING

---

## Test Section 13: Account Closure

### Test Case 13.1: Close Active Account
**ID**: TC-13.1  
**Priority**: High  
**Objective**: Verify account closure

**Input**: Close account ACC002000  
**Expected Result**: Account status changes to inactive, cannot perform transactions  
**Status**: ⏳ PENDING

---

### Test Case 13.2: Attempt Withdrawal from Closed Account
**ID**: TC-13.2  
**Priority**: High  
**Objective**: Verify closed accounts reject operations

**Setup**: Close account, then attempt withdrawal  
**Expected Result**: Error "Cannot withdraw from inactive account"  
**Status**: ⏳ PENDING

---

## Test Section 14: Input Validation & Error Handling

### Test Case 14.1: Invalid Menu Selection
**ID**: TC-14.1  
**Priority**: Medium  
**Objective**: Verify invalid menu choice handling

**Input**: Enter "99" at main menu  
**Expected Result**: "Invalid choice! Please try again." message, return to menu  
**Status**: ⏳ PENDING

---

### Test Case 14.2: Non-numeric Input at Menu
**ID**: TC-14.2  
**Priority**: Medium  
**Objective**: Verify non-numeric input handling

**Input**: Enter "abc" at main menu  
**Expected Result**: "Invalid input! Please enter a number." message  
**Status**: ⏳ PENDING

---

### Test Case 14.3: Empty Input Fields
**ID**: TC-14.3  
**Priority**: Medium  
**Objective**: Verify empty field handling during customer registration

**Input**: Leave first name empty  
**Expected Result**: Validation fails, "First and last name cannot be empty"  
**Status**: ⏳ PENDING

---

## Test Section 15: Data Persistence & Consistency

### Test Case 15.1: Transaction Consistency After Multiple Operations
**ID**: TC-15.1  
**Priority**: High  
**Objective**: Verify all transactions are recorded consistently

**Setup**: Perform: Deposit $100 → Withdraw $50 → Transfer $25 → Interest application  
**Expected Result**: All 4 transactions visible in statement, balance correct: $100 - $50 - $25 + interest  
**Status**: ⏳ PENDING

---

### Test Case 15.2: Customer-Account Relationship Integrity
**ID**: TC-15.2  
**Priority**: High  
**Objective**: Verify account ownership tracking

**Setup**: Create multiple accounts for multiple customers  
**Expected Result**: Each account linked to correct customer in portfolio view  
**Status**: ⏳ PENDING

---

## Test Section 16: Edge Cases & Boundary Conditions

### Test Case 16.1: Very Large Currency Amounts
**ID**: TC-16.1  
**Priority**: Low  
**Objective**: Verify system handles large amounts

**Input**: Deposit/Withdraw $999,999.99  
**Expected Result**: Operations succeed, display formatted correctly as "$999,999.99"  
**Status**: ⏳ PENDING

---

### Test Case 16.2: Very Small Currency Amounts
**ID**: TC-16.2  
**Priority**: Low  
**Objective**: Verify precision with cents

**Input**: Deposit $0.01, then $0.01 multiple times  
**Expected Result**: All transactions recorded, balance accumulates correctly  
**Status**: ⏳ PENDING

---

### Test Case 16.3: Negative Balance Persistence
**ID**: TC-16.3  
**Priority**: Medium  
**Objective**: Verify negative balance handling in checking account

**Setup**: Create $-500 balance via overdraft, then deposit $200  
**Expected Result**: New balance: $-300, overdraft still active  
**Status**: ⏳ PENDING

---

## Summary Statistics

| Category | Total Cases | Passed | Failed | Pending | Pass Rate |
|----------|------------|--------|--------|---------|-----------|
| Customer Registration | 5 | 2 | 0 | 3 | 40% |
| Account Creation | 5 | 0 | 0 | 5 | 0% |
| Deposit Operations | 4 | 0 | 0 | 4 | 0% |
| Withdrawal Operations | 6 | 0 | 0 | 6 | 0% |
| Transfer Operations | 4 | 0 | 0 | 4 | 0% |
| Statements & History | 3 | 0 | 0 | 3 | 0% |
| Savings Features | 3 | 0 | 0 | 3 | 0% |
| Checking Features | 3 | 0 | 0 | 3 | 0% |
| Loan Features | 6 | 0 | 0 | 6 | 0% |
| Monthly Processing | 2 | 0 | 0 | 2 | 0% |
| Search & Lookup | 3 | 0 | 0 | 3 | 0% |
| Bank Reports | 2 | 0 | 0 | 2 | 0% |
| Account Closure | 2 | 0 | 0 | 2 | 0% |
| Input Validation | 3 | 0 | 0 | 3 | 0% |
| Data Persistence | 2 | 0 | 0 | 2 | 0% |
| Edge Cases | 3 | 0 | 0 | 3 | 0% |
| **TOTAL** | **61** | **2** | **0** | **59** | **3.3%** |

---

## Testing Instructions

### How to Run Tests:
1. Open `/home/user/Documents/CPPproject/CPPBankProject/build/`
2. Run: `./BankSystem`
3. For automated tests, use input redirection or Python subprocess
4. Follow test cases sequentially or by category

### Input Format Example:
```bash
echo -e "1\nJohn\nDoe\njohn@example.com\n123-456-7890\n123 Main St\n0" | ./BankSystem
```

### Expected Output Pattern:
- Menu displays correctly
- User input prompts appear
- Results displayed in formatted tables
- Error messages are clear and actionable

---

## Notes for Testers

1. **Currency Formatting**: All amounts should display as "$X,XXX.XX"
2. **Date Formatting**: All timestamps display as "YYYY-MM-DD HH:MM:SS"
3. **Account IDs**: Format is "ACCxxxxxx" where x is a digit
4. **Customer IDs**: Format is "CUSTxxxxxx"
5. **Validation**: Email and phone validation is strict; refer to regex patterns
6. **Cross-Platform**: Test on both Windows and Linux for compatibility

---

## Test Execution Timeline

- **Phase 1**: Customer Registration Tests (TC-1.1 to TC-1.5)
- **Phase 2**: Account Creation Tests (TC-2.1 to TC-2.5)
- **Phase 3**: Transaction Operations (TC-3.x to TC-5.x)
- **Phase 4**: Account Features (TC-7.x to TC-9.x)
- **Phase 5**: System Operations (TC-10.x to TC-16.x)

---

**Document Version**: 1.0  
**Last Updated**: April 12, 2026  
**Test Framework**: Manual + Automated Script Testing
