#include "../include/Bank.h"
#include "../include/Globals.h"
#include "../include/Constants.h"
#include <iostream>
#include <string>
#include <limits>

static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

static double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 0) {
            clearInput();
            return value;
        }
        std::cout << "Invalid input. Enter a non-negative number.\n";
        clearInput();
    }
}

static int readInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            clearInput();
            return value;
        }
        std::cout << "Enter a number between " << min << " and " << max << ".\n";
        clearInput();
    }
}

static void createCustomer(Bank& bank) {
    std::cout << "\n=== Create Customer ===\n";
    std::string fn = readLine("First name : ");
    std::string ln = readLine("Last name  : ");
    std::string em = readLine("Email      : ");
    std::string ph = readLine("Phone      : ");
    std::string ad = readLine("Address    : ");

    std::string customerId, errorMsg;
    if (bank.registerCustomer(fn, ln, em, ph, ad, customerId, errorMsg))
        std::cout << "Customer created successfully. ID: " << customerId << "\n";
    else
        std::cout << "Error: " << errorMsg << "\n";
}

static void createAccount(Bank& bank) {
    std::cout << "\n=== Create Account ===\n";
    std::string customerId = readLine("Customer ID: ");
    std::cout << "Account type:\n"
              << "  1. Savings\n"
              << "  2. Checking\n"
              << "  3. Loan\n";
    int choice = readInt("Select (1-3): ", 1, 3);
    double balance = readDouble("Initial balance: $");

    AccountType type;
    switch (choice) {
        case 1: type = SAVINGS;  break;
        case 2: type = CHECKING; break;
        default: type = LOAN;   break;
    }

    std::string errorMsg;
    Account* account = bank.createAccount(customerId, type, balance, 0.0, errorMsg);
    if (account)
        std::cout << "Account created successfully.\n"
                  << "  Account ID : " << account->getAccountId() << "\n"
                  << "  Type       : " << account->getAccountType() << "\n";
    else
        std::cout << "Error: " << errorMsg << "\n";
}

int main() {
    initializeGlobals();
    Bank bank(BANK_NAME);

    std::cout << "=== " << BANK_NAME << " ===\n";

    while (true) {
        std::cout << "\n1. Create Customer\n"
                  << "2. Create Account\n"
                  << "3. List Customers\n"
                  << "4. Exit\n";
        int choice = readInt("Select (1-4): ", 1, 4);
        switch (choice) {
            case 1: createCustomer(bank); break;
            case 2: createAccount(bank);  break;
            case 3: bank.listCustomers(); break;
            case 4: std::cout << "Goodbye.\n"; return 0;
        }
    }
}
