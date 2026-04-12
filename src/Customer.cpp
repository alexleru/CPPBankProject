#include "../include/Customer.h"
#include "../include/Utils.h"
#include "../include/Account.h"
#include "../include/Constants.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

// Initialize static counter
int Customer::customerCounter = 0;

// Constructor - creates new customer with validation
Customer::Customer(const std::string& firstName, const std::string& lastName,
                   const std::string& email, const std::string& phone, const std::string& address)
    : customerId(generateCustomerId()),
      firstName(firstName),
      lastName(lastName),
      email(email),
      phoneNumber(phone),
      address(address),
      status(CustomerStatus::ACTIVE),
      registrationDate(std::time(nullptr)) {
}

// Private method to generate unique customer ID
std::string Customer::generateCustomerId() {
    std::ostringstream oss;
    oss << CUSTOMER_ID_PREFIX << std::setfill('0') << std::setw(6) << (++customerCounter);
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
std::shared_ptr<Account> Customer::addAccount(std::shared_ptr<Account> account) {
    if (!canAddAccount()) {
        throw std::runtime_error("Maximum number of accounts reached for this customer");
    }
    
    if (!account) {
        throw std::invalid_argument("Cannot add null account");
    }
    
    accounts.push_back(account);
    return account;
}

// Get account by index
std::shared_ptr<Account> Customer::getAccount(int index) const {
    if (index < 0 || index >= static_cast<int>(accounts.size())) {
        return nullptr;
    }
    return accounts[index];
}

// Get account by ID
std::shared_ptr<Account> Customer::getAccountById(const std::string& accountId) const {
    for (const auto& account : accounts) {
        if (account && account->getAccountId() == accountId) {
            return account;
        }
    }
    return nullptr;
}

// Check if customer can add more accounts
bool Customer::canAddAccount() const {
    return accounts.size() < MAX_ACCOUNTS_PER_CUSTOMER;
}

// Remove account from customer list
bool Customer::removeAccount(const std::string& accountId) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
                          [&accountId](const std::shared_ptr<Account>& acc) {
                              return acc && acc->getAccountId() == accountId;
                          });
    
    if (it != accounts.end()) {
        accounts.erase(it);
        return true;
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
        case CustomerStatus::ACTIVE:
            statusStr = "Active";
            break;
        case CustomerStatus::INACTIVE:
            statusStr = "Inactive";
            break;
        case CustomerStatus::SUSPENDED:
            statusStr = "Suspended";
            break;
        case CustomerStatus::CLOSED:
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
