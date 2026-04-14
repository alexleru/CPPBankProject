#include "../include/Customer.h"
#include "../include/Utils.h"
#include <iostream>

Customer::Customer(const std::string& fn, const std::string& ln,
                   const std::string& em, const std::string& ph,
                   const std::string& addr)
    : customerId(Utils::generateCustomerId()),
      firstName(fn), lastName(ln), email(em), phone(ph), address(addr),
      status(ACTIVE) {}

std::string    Customer::getCustomerId() const { return customerId; }
std::string    Customer::getFirstName()  const { return firstName; }
std::string    Customer::getLastName()   const { return lastName; }
std::string    Customer::getEmail()      const { return email; }
std::string    Customer::getPhone()      const { return phone; }
std::string    Customer::getAddress()    const { return address; }
CustomerStatus Customer::getStatus()     const { return status; }

bool Customer::validate(std::string& errorMsg) const {
    if (firstName.empty()) { errorMsg = "First name is required."; return false; }
    if (lastName.empty())  { errorMsg = "Last name is required.";  return false; }
    if (!Utils::validateEmail(email))  { errorMsg = "Invalid email address."; return false; }
    if (!Utils::validatePhone(phone))  { errorMsg = "Invalid phone (min 10 digits)."; return false; }
    if (address.empty()) { errorMsg = "Address is required."; return false; }
    return true;
}

void Customer::display() const {
    std::cout << "Customer ID : " << customerId << "\n"
              << "Name        : " << firstName << " " << lastName << "\n"
              << "Email       : " << email << "\n"
              << "Phone       : " << phone << "\n"
              << "Address     : " << address << "\n"
              << "Status      : " << (status == ACTIVE ? "Active" : "Inactive") << "\n";
}
