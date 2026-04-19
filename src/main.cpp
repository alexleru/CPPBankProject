#include "../include/Bank.h"
#include "../include/Globals.h"
#include "../include/Constants.h"
#include "../include/Utils.h"
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

static void demoFunctionPointers() {
    std::cout << "\n=== Function Pointer Demo ===\n";
    // Passing the address of 'add'
    Utils::performOperation(5, 3, Utils::add);      // Output: Result: 8
    // Passing the address of 'multiply'
    Utils::performOperation(5, 3, Utils::multiply); // Output: Result: 15
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

int main() {
    initializeGlobals();
    Bank bank(BANK_NAME);

    std::cout << "=== " << BANK_NAME << " ===\n";

    while (true) {
        std::cout << "\n1. Create Customer\n"
                  << "2. List Customers\n"
                  << "3. Function Pointer Demo\n"
                  << "4. Exit\n";
        int choice = readInt("Select (1-4): ", 1, 4);
        switch (choice) {
            case 1: createCustomer(bank); break;
            case 2: bank.listCustomers(); break;
            case 3: demoFunctionPointers(); break;
            case 4: std::cout << "Goodbye.\n"; return 0;
        }
    }
}
