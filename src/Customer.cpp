#include "../include/Customer.h"
#include "../include/Utils.h"
#include <iostream>

Customer::Customer(const std::string& fn, const std::string& ln,
                   const ContactInfo& info, CustomerStatus st) {
    customerId = Utils::generateCustomerId();
    firstName  = fn;
    lastName   = ln;
    contact    = info;
    status     = st;
}

std::string    Customer::getEmail()   const { return contact.email;   }
std::string    Customer::getPhone()   const { return contact.phone;   }
std::string    Customer::getAddress() const { return contact.address; }
CustomerStatus Customer::getStatus()  const { return status;          }

bool Customer::validate(std::string& errorMsg) const {
    if (firstName.empty()) { errorMsg = "First name is required."; return false; }
    if (lastName.empty())  { errorMsg = "Last name is required.";  return false; }
    if (!Utils::validateEmail(contact.email))  { errorMsg = "Invalid email address.";      return false; }
    if (!Utils::validatePhone(contact.phone))  { errorMsg = "Invalid phone (min 10 digits)."; return false; }
    if (contact.address.empty()) { errorMsg = "Address is required."; return false; }
    return true;
}

void Customer::display() const {
    std::cout << "Customer ID : " << customerId << "\n"
              << "Name        : " << firstName << " " << lastName << "\n"
              << "Email       : " << contact.email << "\n"
              << "Phone       : " << contact.phone << "\n"
              << "Address     : " << contact.address << "\n"
              << "Status      : " << (status == ACTIVE ? "Active" : "Inactive") << "\n";
}
