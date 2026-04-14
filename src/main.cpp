#include "../include/Bank.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include "../include/Globals.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>

// Initialize global variables
void initializeApplication() {
    initializeGlobals();
}

// Global bank instance
Bank globalBank(BANK_NAME);

// Function to clear console screen (cross-platform)
void clearScreen() {
    // Use a more compatible clear method
    #ifdef _WIN32
        system("cls");
    #else
        // Linux/Unix: Use system clear command for better compatibility
        system("clear");
    #endif
}

// Function to pause and wait for user input
void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Function to display main menu
void displayMainMenu() {
    // Add some spacing instead of clearing screen
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
    std::cout << "0.  Exit\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
}

// Register new customer
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

// Open new account
void openNewAccount() {
    clearScreen();
    std::cout << "=== OPEN NEW ACCOUNT ===\n\n";
    
    std::string customerId;
    std::cout << "Enter customer ID: ";
    std::cin >> customerId;
    
    // Verify customer exists
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
        case 1:
            type = SAVINGS;
            break;
        case 2:
            type = CHECKING;
            break;
        case 3:
            type = LOAN;
            break;
        default:
            std::cerr << "Invalid account type!\n";
            pauseScreen();
            return;
    }
    
    double initialBalance = Utils::getValidatedAmount("Enter initial balance/amount: ");
    
    Account* newAccount = globalBank.createAccount(customerId, type, initialBalance);
    
    if (newAccount) {
        std::cout << "Account created successfully!\n";
    }
    
    pauseScreen();
}

// Deposit funds
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

// Withdraw funds
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

// Transfer between accounts
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

// View account statement
void viewStatement() {
    clearScreen();
    std::cout << "=== VIEW ACCOUNT STATEMENT ===\n\n";
    
    std::string accountId;
    std::cout << "Enter account ID: ";
    std::cin >> accountId;
    
    globalBank.displayAccountStatement(accountId);
    
    pauseScreen();
}

// View customer portfolio
void viewPortfolio() {
    clearScreen();
    std::cout << "=== VIEW CUSTOMER PORTFOLIO ===\n\n";
    
    std::string customerId;
    std::cout << "Enter customer ID: ";
    std::cin >> customerId;
    
    globalBank.generateCustomerReport(customerId);
    
    pauseScreen();
}

// Apply monthly processing
void applyMonthlyProcessing() {
    clearScreen();
    std::cout << "=== APPLY MONTHLY PROCESSING ===\n\n";
    
    globalBank.applyMonthlyProcessing();
    
    pauseScreen();
}

// Calculate loan EMI
void calculateLoanEMI() {
    clearScreen();
    std::cout << "=== CALCULATE LOAN EMI ===\n\n";
    
    double principal = Utils::getValidatedAmount("Enter principal amount: ");
    double rate = Utils::getValidatedAmount("Enter annual interest rate (as percentage): ");
    int months = Utils::getValidatedInteger("Enter loan term (in months): ");
    
    // Convert percentage to decimal
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

// Search account by ID
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

// List all customers
void listAllCustomers() {
    clearScreen();
    std::cout << "=== ALL CUSTOMERS ===\n\n";
    
    globalBank.listAllCustomers();
    
    pauseScreen();
}

// List all accounts
void listAllAccounts() {
    clearScreen();
    std::cout << "=== ALL ACCOUNTS ===\n\n";
    
    globalBank.listAllAccounts();
    
    pauseScreen();
}

// Generate bank report
void generateBankReport() {
    clearScreen();
    std::cout << "=== BANK REPORT ===\n\n";
    
    globalBank.generateBankReport();
    
    pauseScreen();
}

// Close account
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

// Admin/Debug functions menu
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
        case 1:
            printGlobalStatistics();
            break;
        case 2:
            debugBankInfo(globalBank);
            break;
        case 3: {
            std::string customerId;
            std::cout << "Enter customer ID: ";
            std::cin >> customerId;
            const Customer* customer = globalBank.findCustomer(customerId);
            if (customer) {
                debugCustomerInfo(*customer);
            } else {
                std::cout << "Customer not found.\n";
            }
            break;
        }
        case 4: {
            std::string accountId;
            std::cout << "Enter account ID: ";
            std::cin >> accountId;
            Account* account = globalBank.findAccount(accountId);
            if (account) {
                debugAccountInfo(*account);
            } else {
                std::cout << "Account not found.\n";
            }
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
            // Validate all accounts using friend function
            AccountList accounts = globalBank.getCustomerAccounts(""); // This won't work as expected
            std::cout << "Account validation feature would iterate through all accounts.\n";
            break;
        }
        case 0:
            return;
        default:
            std::cout << "Invalid choice.\n";
    }
    
    pauseScreen();
}

// Display welcome message
void displayWelcome() {
    clearScreen();
    std::cout << "\n";
    std::cout << "=========================================================\n";
    std::cout << "          WELCOME TO " << BANK_NAME << "\n";
    std::cout << "=========================================================\n";
    std::cout << "Version: " << VERSION << std::endl;
    std::cout << "Platform: " << PLATFORM_NAME << std::endl;
    std::cout << "\nThis is a comprehensive bank account management system\n";
    std::cout << "with support for Savings, Checking, and Loan accounts.\n";
    std::cout << "=========================================================\n\n";
    
    pauseScreen();
}

// Main function - entry point
int main() {
    // Initialize global variables
    initializeApplication();
    
    // Display welcome screen
    displayWelcome();
    
    // Main program loop
    int choice = -1;
    bool running = true;
    
    while (running) {
        displayMainMenu();
        
        if (std::cin >> choice) {
            switch (choice) {
                case 1:
                    registerCustomer();
                    break;
                case 2:
                    openNewAccount();
                    break;
                case 3:
                    depositFunds();
                    break;
                case 4:
                    withdrawFunds();
                    break;
                case 5:
                    transferFunds();
                    break;
                case 6:
                    viewStatement();
                    break;
                case 7:
                    viewPortfolio();
                    break;
                case 8:
                    applyMonthlyProcessing();
                    break;
                case 9:
                    calculateLoanEMI();
                    break;
                case 10:
                    searchAccountById();
                    break;
                case 11:
                    listAllCustomers();
                    break;
                case 12:
                    listAllAccounts();
                    break;
                case 13:
                    generateBankReport();
                    break;
                case 14:
                    closeAccount();
                    break;
                case 15:
                    adminDebugFunctions();
                    break;
                case 0:
                    running = false;
                    std::cout << "\nThank you for using " << BANK_NAME << "!\n";
                    std::cout << "Goodbye!\n\n";
                    break;
                default:
                    std::cerr << "Invalid choice! Please try again.\n";
                    pauseScreen();
            }
        } else {
            // Handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input! Please enter a number.\n";
            pauseScreen();
        }
    }
    
    return 0;
}
