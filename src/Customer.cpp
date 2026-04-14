#include "../include/Customer.h"
#include "../include/Utils.h"
#include "../include/Account.h"
#include "../include/Constants.h"
#include "../include/Globals.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <stdexcept>

// Initialize static counter
int Customer::customerCounter = 0;

// Destructor - free owned account objects
Customer::~Customer() {
    for (size_t i = 0; i < accounts.size(); ++i) {
        delete accounts[i];
    }
    accounts.clear();
}

// Constructor - creates new customer with validation
Customer::Customer(const std::string& firstName, const std::string& lastName,
                   const std::string& email, const std::string& phone, const std::string& address)
    : customerId(generateCustomerId()),
      firstName(firstName),
      lastName(lastName),
      email(email),
      phoneNumber(phone),
      address(address),
      status(ACTIVE),
      registrationDate(std::time(NULL)) {
    // Increment global statistics
    totalCustomersCreated++;
}

// Private method to generate unique customer ID
std::string Customer::generateCustomerId() {
    std::ostringstream oss;
    oss << CUSTOMER_ID_PREFIX << std::setfill('0') << std::setw(6) << (++globalCustomerCounter);
    return oss.str();
}

// Getters
std::string Customer::getCustomerId() const {
    return customerId;
}

std::string Customer::getFirstName() const {
    return firstName;
}

std::string Customer::getLastName() const {
    return lastName;
}

std::string Customer::getEmail() const {
    return email;
}

std::string Customer::getPhoneNumber() const {
    return phoneNumber;
}

std::string Customer::getAddress() const {
    return address;
}

CustomerStatus Customer::getStatus() const {
    return status;
}

time_t Customer::getRegistrationDate() const {
    return registrationDate;
}

int Customer::getAccountCount() const {
    return accounts.size();
}

// Setters
void Customer::setFirstName(const std::string& name) {
    firstName = name;
}

void Customer::setLastName(const std::string& name) {
    lastName = name;
}

void Customer::setEmail(const std::string& emailAddr) {
    if (!Utils::validateEmail(emailAddr)) {
        throw std::invalid_argument("Invalid email format");
    }
    email = emailAddr;
}

void Customer::setPhoneNumber(const std::string& phone) {
    if (!Utils::validatePhone(phone)) {
        throw std::invalid_argument("Invalid phone format");
    }
    phoneNumber = phone;
}

void Customer::setAddress(const std::string& addr) {
    address = addr;
}

void Customer::setStatus(CustomerStatus newStatus) {
    status = newStatus;
}

// Add account to customer's account list
Account* Customer::addAccount(Account* account) {
    if (!canAddAccount()) {
        throw std::runtime_error("Maximum number of accounts reached for this customer");
    }
    
    if (account == NULL) {
        throw std::invalid_argument("Cannot add null account");
    }
    
    accounts.push_back(account);
    return account;
}

// Get account by index
Account* Customer::getAccount(int index) const {
    if (index < 0 || index >= static_cast<int>(accounts.size())) {
        return NULL;
    }
    return accounts[index];
}

// Get account by ID
Account* Customer::getAccountById(const std::string& accountId) const {
    for (int i = 0; i < static_cast<int>(accounts.size()); ++i) {
        Account* account = accounts[i];
        if (account != NULL && account->getAccountId() == accountId) {
            return account;
        }
    }
    return NULL;
}

// Check if customer can add more accounts
bool Customer::canAddAccount() const {
    return static_cast<int>(accounts.size()) < MAX_ACCOUNTS_PER_CUSTOMER;
}

// Remove account from customer list
bool Customer::removeAccount(const std::string& accountId) {
    for (int i = 0; i < static_cast<int>(accounts.size()); ++i) {
        Account* acc = accounts[i];
        if (acc != NULL && acc->getAccountId() == accountId) {
            accounts.erase(accounts.begin() + i);
            return true;
        }
    }
    return false;
}

// Validate customer information
bool Customer::validate() const {
    // Check required fields
    if (firstName.empty() || lastName.empty()) {
        std::cerr << "First and last name cannot be empty" << std::endl;
        return false;
    }
    
    // Validate email
    if (!Utils::validateEmail(email)) {
        std::cerr << "Invalid email format: " << email << std::endl;
        return false;
    }
    
    // Validate phone
    if (!Utils::validatePhone(phoneNumber)) {
        std::cerr << "Invalid phone format: " << phoneNumber << std::endl;
        return false;
    }
    
    // Check address
    if (address.empty()) {
        std::cerr << "Address cannot be empty" << std::endl;
        return false;
    }
    
    return true;
}

// Display customer information
void Customer::display() const {
    std::cout << "\n=====================================\n";
    std::cout << "CUSTOMER INFORMATION\n";
    std::cout << "=====================================\n";
    std::cout << "Customer ID: " << customerId << std::endl;
    std::cout << "Name: " << firstName << " " << lastName << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Phone: " << phoneNumber << std::endl;
    std::cout << "Address: " << address << std::endl;
    
    std::string statusStr;
    switch (status) {
        case ACTIVE:
            statusStr = "Active";
            break;
        case INACTIVE:
            statusStr = "Inactive";
            break;
        case SUSPENDED:
            statusStr = "Suspended";
            break;
        case CLOSED:
            statusStr = "Closed";
            break;
    }
    
    std::cout << "Status: " << statusStr << std::endl;
    std::cout << "Registration Date: " << Utils::formatDate(registrationDate) << std::endl;
    std::cout << "Number of Accounts: " << accounts.size() << std::endl;
    std::cout << "=====================================\n";
}

// Display customer's portfolio (all accounts)
void Customer::displayPortfolio() const {
    std::cout << "\n=====================================\n";
    std::cout << "CUSTOMER PORTFOLIO\n";
    std::cout << "Customer: " << firstName << " " << lastName << " (" << customerId << ")\n";
    std::cout << "=====================================\n";
    
    if (accounts.empty()) {
        std::cout << "No accounts found.\n";
    } else {
        double totalBalance = 0.0;
        
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i]) {
                std::cout << "\nAccount " << (i + 1) << ":\n";
                accounts[i]->displayAccountInfo();
                totalBalance += accounts[i]->getBalance();
            }
        }
        
        std::cout << "\n-------------------------------------\n";
        std::cout << "Total Portfolio Balance: " << Utils::formatCurrency(totalBalance) << std::endl;
    }
    
    std::cout << "=====================================\n";
}

// Friend function implementations

// Debug function to access private customer information
void debugCustomerInfo(const Customer& customer) {
    std::cout << "\n=== DEBUG CUSTOMER INFO ===\n";
    std::cout << "Customer ID: " << customer.customerId << std::endl;
    std::cout << "Name: " << customer.firstName << " " << customer.lastName << std::endl;
    std::cout << "Email: " << customer.email << std::endl;
    std::cout << "Phone: " << customer.phoneNumber << std::endl;
    std::cout << "Address: " << customer.address << std::endl;
    std::cout << "Status: " << customer.status << std::endl;
    std::cout << "Registration Date: " << customer.registrationDate << std::endl;
    std::cout << "Account Count: " << customer.accounts.size() << std::endl;
    std::cout << "===========================\n";
}

// Friend function to validate customer data
bool validateCustomerData(const Customer& customer) {
    if (customer.firstName.empty() || customer.lastName.empty()) {
        std::cerr << "ERROR: Customer " << customer.customerId << " has empty name fields\n";
        return false;
    }
    if (customer.email.empty() || customer.email.find('@') == std::string::npos) {
        std::cerr << "ERROR: Customer " << customer.customerId << " has invalid email\n";
        return false;
    }
    return true;
}

// Friend function to update customer status (administrative function)
void updateCustomerStatus(Customer& customer, CustomerStatus newStatus) {
    customer.status = newStatus;
    std::cout << "Customer " << customer.customerId << " status updated to: " << newStatus << std::endl;
}

// Friend function to get customer accounts list
AccountList getCustomerAccounts(const Customer& customer) {
    return customer.accounts;
}
