#include "../include/Bank.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include "../include/Globals.h"
#include "../include/BondCalculator.h"
#include "../include/MortgageAccount.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <stdexcept>

void initializeApplication() {
    initializeGlobals();
}

Bank globalBank(BANK_NAME);

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMainMenu() {
    std::cout << "\n\n";
    std::cout << "========================================\n";
    std::cout << "   " << BANK_NAME << "\n";
    std::cout << "========================================\n";
    std::cout << "1.  Register New Customer\n";
    std::cout << "2.  Open New Account (Savings/Checking/Loan)\n";
    std::cout << "3.  Deposit Funds\n";
    std::cout << "4.  Withdraw Funds\n";
    std::cout << "5.  Transfer Between Accounts\n";
    std::cout << "6.  View Account Statement\n";
    std::cout << "7.  View Customer Portfolio\n";
    std::cout << "8.  Apply Monthly Processing (interest + fees)\n";
    std::cout << "9.  Calculate Loan EMI\n";
    std::cout << "10. Search Account by ID\n";
    std::cout << "11. List All Customers\n";
    std::cout << "12. List All Accounts\n";
    std::cout << "13. Generate Bank Report\n";
    std::cout << "14. Close Account\n";
    std::cout << "15. Admin/Debug Functions\n";
    std::cout << "16. Calculate Bond Parameters\n";
    std::cout << "17. Function Pointer Demo\n";
    std::cout << "18. Open Mortgage Account / Mortgage Operations\n";
    std::cout << "0.  Exit\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
}

void registerCustomer() {
    clearScreen();
    std::cout << "=== REGISTER NEW CUSTOMER ===\n\n";
    std::string firstName, lastName, email, phone, address;
    std::cout << "Enter first name: ";
    std::cin.ignore();
    std::getline(std::cin, firstName);
    std::cout << "Enter last name: ";
    std::getline(std::cin, lastName);
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    std::cout << "Enter phone number: ";
    std::getline(std::cin, phone);
    std::cout << "Enter address: ";
    std::getline(std::cin, address);
    if (globalBank.registerCustomer(firstName, lastName, email, phone, address)) {
        std::cout << "\nCustomer registered successfully!\n";
    } else {
        std::cerr << "\nFailed to register customer.\n";
    }
    pauseScreen();
}

void openNewAccount() {
    clearScreen();
    std::cout << "=== OPEN NEW ACCOUNT ===\n\n";
    std::string customerId;
    std::cout << "Enter customer ID: ";
    std::cin >> customerId;
    if (!globalBank.findCustomer(customerId)) {
        std::cerr << "Customer not found!\n";
        pauseScreen();
        return;
    }
    std::cout << "\nAccount Types:\n";
    std::cout << "1. Savings Account\n";
    std::cout << "2. Checking Account\n";
    std::cout << "3. Loan Account\n";
    std::cout << "Enter account type: ";
    int typeChoice;
    std::cin >> typeChoice;
    AccountType type;
    switch (typeChoice) {
        case 1: type = SAVINGS; break;
        case 2: type = CHECKING; break;
        case 3: type = LOAN; break;
        default:
            std::cerr << "Invalid account type!\n";
            pauseScreen();
            return;
    }
    double initialBalance = Utils::getValidatedAmount("Enter initial balance/amount: ", true);
    Account* newAccount = globalBank.createAccount(customerId, type, initialBalance);
    if (newAccount) {
        std::cout << "Account created successfully!\n";
    }
    pauseScreen();
}

void depositFunds() {
    clearScreen();
    std::cout << "=== DEPOSIT FUNDS ===\n\n";
    std::string accountId;
    std::cout << "Enter account ID: ";
    std::cin >> accountId;
    double amount = Utils::getValidatedAmount("Enter deposit amount: ");
    if (globalBank.depositToAccount(accountId, amount)) {
        std::cout << "\nDeposit successful!\n";
    } else {
        std::cerr << "\nDeposit failed!\n";
    }
    pauseScreen();
}

void withdrawFunds() {
    clearScreen();
    std::cout << "=== WITHDRAW FUNDS ===\n\n";
    std::string accountId;
    std::cout << "Enter account ID: ";
    std::cin >> accountId;
    double amount = Utils::getValidatedAmount("Enter withdrawal amount: ");
    if (globalBank.withdrawFromAccount(accountId, amount)) {
        std::cout << "\nWithdrawal successful!\n";
    } else {
        std::cerr << "\nWithdrawal failed!\n";
    }
    pauseScreen();
}

void transferFunds() {
    clearScreen();
    std::cout << "=== TRANSFER BETWEEN ACCOUNTS ===\n\n";
    std::string fromAccountId, toAccountId;
    std::cout << "Enter source account ID: ";
    std::cin >> fromAccountId;
    std::cout << "Enter destination account ID: ";
    std::cin >> toAccountId;
    double amount = Utils::getValidatedAmount("Enter transfer amount: ");
    if (globalBank.transferBetweenAccounts(fromAccountId, toAccountId, amount)) {
        std::cout << "\nTransfer successful!\n";
    } else {
        std::cerr << "\nTransfer failed!\n";
    }
    pauseScreen();
}

void viewStatement() {
    clearScreen();
    std::cout << "=== VIEW ACCOUNT STATEMENT ===\n\n";
    std::string accountId;
    std::cout << "Enter account ID: ";
    std::cin >> accountId;
    globalBank.displayAccountStatement(accountId);
    pauseScreen();
}

void viewPortfolio() {
    clearScreen();
    std::cout << "=== VIEW CUSTOMER PORTFOLIO ===\n\n";
    std::string customerId;
    std::cout << "Enter customer ID: ";
    std::cin >> customerId;
    globalBank.generateCustomerReport(customerId);
    pauseScreen();
}

void applyMonthlyProcessing() {
    clearScreen();
    std::cout << "=== APPLY MONTHLY PROCESSING ===\n\n";
    globalBank.applyMonthlyProcessing();
    pauseScreen();
}

void calculateLoanEMI() {
    clearScreen();
    std::cout << "=== CALCULATE LOAN EMI ===\n\n";
    double principal = Utils::getValidatedAmount("Enter principal amount: ");
    double rate = Utils::getValidatedAmount("Enter annual interest rate (as percentage): ");
    int months = Utils::getValidatedInteger("Enter loan term (in months): ");
    rate = rate / 100.0;
    try {
        double emi = globalBank.calculateLoanEMI(principal, rate, months);
        std::cout << "\n===============================================\n";
        std::cout << "LOAN EMI CALCULATION\n";
        std::cout << "===============================================\n";
        std::cout << "Principal: " << Utils::formatCurrency(principal) << std::endl;
        std::cout << "Interest Rate: " << (rate * 100.0) << "% per annum\n";
        std::cout << "Loan Term: " << months << " months\n";
        std::cout << "Monthly EMI: " << Utils::formatCurrency(emi) << std::endl;
        std::cout << "Total Repayment: " << Utils::formatCurrency(emi * months) << std::endl;
        std::cout << "Total Interest: " << Utils::formatCurrency((emi * months) - principal) << std::endl;
        std::cout << "===============================================\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    pauseScreen();
}

void searchAccountById() {
    clearScreen();
    std::cout << "=== SEARCH ACCOUNT BY ID ===\n\n";
    std::string accountId;
    std::cout << "Enter account ID: ";
    std::cin >> accountId;
    Account* account = globalBank.findAccount(accountId);
    if (account) {
        std::cout << "\nAccount found:\n";
        account->displayAccountInfo();
    } else {
        std::cerr << "\nAccount not found!\n";
    }
    pauseScreen();
}

void listAllCustomers() {
    clearScreen();
    std::cout << "=== ALL CUSTOMERS ===\n\n";
    globalBank.listAllCustomers();
    pauseScreen();
}

void listAllAccounts() {
    clearScreen();
    std::cout << "=== ALL ACCOUNTS ===\n\n";
    globalBank.listAllAccounts();
    pauseScreen();
}

void generateBankReport() {
    clearScreen();
    std::cout << "=== BANK REPORT ===\n\n";
    globalBank.generateBankReport();
    pauseScreen();
}

void closeAccount() {
    clearScreen();
    std::cout << "=== CLOSE ACCOUNT ===\n\n";
    std::string accountId;
    std::cout << "Enter account ID to close: ";
    std::cin >> accountId;
    Account* account = globalBank.findAccount(accountId);
    if (account) {
        account->setIsActive(false);
        std::cout << "Account closed successfully!\n";
    } else {
        std::cerr << "Account not found!\n";
    }
    pauseScreen();
}

void adminDebugFunctions() {
    clearScreen();
    std::cout << "=== ADMIN/DEBUG FUNCTIONS ===\n\n";
    std::cout << "1. Print Global Statistics\n";
    std::cout << "2. Debug Bank Information\n";
    std::cout << "3. Debug Customer Information\n";
    std::cout << "4. Debug Account Information\n";
    std::cout << "5. Force Close Account\n";
    std::cout << "6. Validate All Accounts\n";
    std::cout << "0. Back to Main Menu\n\n";
    int choice;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    switch (choice) {
        case 1: printGlobalStatistics(); break;
        case 2: debugBankInfo(globalBank); break;
        case 3: {
            std::string customerId;
            std::cout << "Enter customer ID: ";
            std::cin >> customerId;
            const Customer* customer = globalBank.findCustomer(customerId);
            if (customer) debugCustomerInfo(*customer);
            else std::cout << "Customer not found.\n";
            break;
        }
        case 4: {
            std::string accountId;
            std::cout << "Enter account ID: ";
            std::cin >> accountId;
            Account* account = globalBank.findAccount(accountId);
            if (account) debugAccountInfo(*account);
            else std::cout << "Account not found.\n";
            break;
        }
        case 5: {
            std::string accountId;
            std::cout << "Enter account ID to force close: ";
            std::cin >> accountId;
            forceCloseAccount(globalBank, accountId);
            break;
        }
        case 6: {
            std::cout << "Account validation feature would iterate through all accounts.\n";
            break;
        }
        case 0: return;
        default: std::cout << "Invalid choice.\n";
    }
    pauseScreen();
}

void calculateBondParameters() {
    clearScreen();
    std::cout << "=== CALCULATE BOND PARAMETERS ===\n\n";
    double nominal = Utils::getValidatedAmount("Enter bond nominal: ");
    int term = Utils::getValidatedInteger("Enter bond term (months): ");
    double fixedRatePct = Utils::getValidatedAmount("Enter fixed monthly coupon rate (%): ");
    double maxRandomPct = Utils::getValidatedAmount("Enter max random monthly part (%, 0..3): ");
    double discountRatePct = Utils::getValidatedAmount("Enter annual discount rate (%): ");
    if (maxRandomPct > 3.0) maxRandomPct = 3.0;
    double fixedRate = fixedRatePct / 100.0;
    double maxRandomRate = maxRandomPct / 100.0;
    double discountRate = discountRatePct / 100.0;
    try {
        BondCalculator calc(nominal, term, fixedRate, maxRandomRate, discountRate);
        calc.generateCashFlowSchedule();
        std::cout << "PV: " << Utils::formatCurrency(calc.calculatePresentValue()) << "\n";
        std::cout << "FV: " << Utils::formatCurrency(calc.calculateFutureValue()) << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    pauseScreen();
}

double applyOnePercentBonus(double balance) { return balance * 1.01; }
double applyFlatMaintenanceFee(double balance) {
    double r = balance - 5.0;
    return (r < 0) ? 0.0 : r;
}

void functionPointerDemo() {
    clearScreen();
    std::cout << "=== FUNCTION POINTER DEMO ===\n\n";
    Utils::performOperation(5, 3, Utils::add);
    Utils::performOperation(5, 3, Utils::multiply);
    globalBank.applyToAllAccounts(applyOnePercentBonus);
    globalBank.applyToAllAccounts(applyFlatMaintenanceFee);
    pauseScreen();
}

// =====================================================================
// Mortgage menu - new feature joined with the loan account hierarchy
// =====================================================================
void mortgageMenu() {
    clearScreen();
    std::cout << "=== MORTGAGE OPERATIONS ===\n\n";
    std::cout << "1. Open new mortgage account\n";
    std::cout << "2. Show mortgage amortization schedule\n";
    std::cout << "3. Make mortgage payment\n";
    std::cout << "4. Show mortgage details (LTV, PMI, monthly payment)\n";
    std::cout << "0. Back to main menu\n\n";

    int choice = -1;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    if (choice == 0) return;

    if (choice == 1) {
        std::string customerId;
        std::cout << "Enter customer ID: ";
        std::cin >> customerId;
        if (!globalBank.findCustomer(customerId)) {
            std::cerr << "Customer not found!\n";
            pauseScreen();
            return;
        }
        Property prop;
        prop.id = static_cast<PropertyId>(std::time(NULL));
        std::cout << "Enter property address: ";
        std::cin.ignore();
        std::getline(std::cin, prop.address);
        prop.marketValue = Utils::getValidatedAmount("Enter property market value: ");
        prop.downPayment = Utils::getValidatedAmount("Enter down payment: ", true);
        Rate rate     = Utils::getValidatedAmount("Enter annual rate (percent): ") / 100.0;
        TermInYears y = Utils::getValidatedInteger("Enter term in years: ");
        std::cout << "Mortgage kind:\n  1) Fixed rate\n  2) ARM\n  3) Interest-only\nChoice: ";
        int k = 1;
        std::cin >> k;
        MortgageKind kind = FIXED_RATE;
        if (k == 2) kind = ADJUSTABLE_RATE;
        else if (k == 3) kind = INTEREST_ONLY;
        try {
            MortgageAccount* m = globalBank.createMortgageAccount(customerId, prop, rate, y, kind);
            if (m) m->displayAccountInfo();
        } catch (const std::exception& e) {
            std::cerr << "Mortgage creation failed: " << e.what() << "\n";
        }
    }
    else if (choice == 2) {
        std::string accountId;
        std::cout << "Enter mortgage account ID: ";
        std::cin >> accountId;
        MortgageAccount* m = globalBank.findMortgageAccount(accountId);
        if (!m) std::cerr << "Mortgage account not found!\n";
        else m->displayAmortizationSchedule();
    }
    else if (choice == 3) {
        std::string accountId;
        std::cout << "Enter mortgage account ID: ";
        std::cin >> accountId;
        MortgageAccount* m = globalBank.findMortgageAccount(accountId);
        if (!m) std::cerr << "Mortgage account not found!\n";
        else {
            Money expected = m->calculateTotalMonthlyPayment();
            std::cout << "Expected total monthly payment: " << Utils::formatCurrency(expected) << "\n";
            Money amount = Utils::getValidatedAmount("Enter payment amount: ");
            try {
                m->makeMortgagePayment(amount);
            } catch (const std::exception& e) {
                std::cerr << "Payment failed: " << e.what() << "\n";
            }
        }
    }
    else if (choice == 4) {
        std::string accountId;
        std::cout << "Enter mortgage account ID: ";
        std::cin >> accountId;
        MortgageAccount* m = globalBank.findMortgageAccount(accountId);
        if (!m) std::cerr << "Mortgage account not found!\n";
        else m->displayAccountInfo();
    }
    else {
        std::cout << "Invalid choice.\n";
    }
    pauseScreen();
}

void displayWelcome() {
    clearScreen();
    std::cout << "\n";
    std::cout << "=========================================================\n";
    std::cout << "          WELCOME TO " << BANK_NAME << "\n";
    std::cout << "=========================================================\n";
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << "Platform: " << PLATFORM_NAME << std::endl;
    std::cout << "=========================================================\n\n";
    pauseScreen();
}

int main() {
    initializeApplication();
    displayWelcome();
    int choice = -1;
    bool running = true;
    while (running) {
        displayMainMenu();
        if (std::cin >> choice) {
            switch (choice) {
                case 1:  registerCustomer(); break;
                case 2:  openNewAccount(); break;
                case 3:  depositFunds(); break;
                case 4:  withdrawFunds(); break;
                case 5:  transferFunds(); break;
                case 6:  viewStatement(); break;
                case 7:  viewPortfolio(); break;
                case 8:  applyMonthlyProcessing(); break;
                case 9:  calculateLoanEMI(); break;
                case 10: searchAccountById(); break;
                case 11: listAllCustomers(); break;
                case 12: listAllAccounts(); break;
                case 13: generateBankReport(); break;
                case 14: closeAccount(); break;
                case 15: adminDebugFunctions(); break;
                case 16: calculateBondParameters(); break;
                case 17: functionPointerDemo(); break;
                case 18: mortgageMenu(); break;
                case 0:
                    running = false;
                    std::cout << "\nGoodbye!\n";
                    break;
                default:
                    std::cerr << "Invalid choice!\n";
                    pauseScreen();
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input!\n";
            pauseScreen();
        }
    }
    return 0;
}
