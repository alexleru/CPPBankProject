#include "../include/Utils.h"
#include "../include/Globals.h"
#include "../include/Constants.h"
#include <sstream>
#include <iomanip>
#include <iostream>

Utils::Utils() {}

// Sample functions to be passed
int Utils::add(int a, int b) { return a + b; }
int Utils::multiply(int a, int b) { return a * b; }

// Function that takes a function pointer as a parameter
// Syntax: int (*operation)(int, int)
void Utils::performOperation(int x, int y, int (*operation)(int, int)) {
    std::cout << "Result: " << operation(x, y) << std::endl;
}

std::string Utils::generateCustomerId() {
    std::ostringstream oss;
    oss << CUSTOMER_ID_PREFIX << std::setfill('0') << std::setw(6) << globalCustomerCounter++;
    return oss.str();
}

bool Utils::validateEmail(const std::string& email) {
    size_t at  = email.find('@');
    size_t dot = email.rfind('.');
    if (at == std::string::npos || at == 0) return false;
    if (dot == std::string::npos || dot <= at + 1 || dot == email.length() - 1) return false;
    return true;
}

bool Utils::validatePhone(const std::string& phone) {
    if (phone.length() < 10) return false;
    for (size_t i = 0; i < phone.length(); ++i) {
        char c = phone[i];
        if (!std::isdigit(c) && c != ' ' && c != '-' && c != '(' && c != ')' && c != '+')
            return false;
    }
    return true;
}
