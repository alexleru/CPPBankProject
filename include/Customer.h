#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Constants.h"
#include "Enums.h"

// C-style struct alias (typedef struct)
typedef struct ContactInfo_ {
    std::string email;
    std::string phone;
    std::string address;
} ContactInfo;

// Two-level inheritance: Entity -> Person -> Customer
class Entity {
protected:
    CustomerId customerId;
public:
    virtual ~Entity() {}
    CustomerId getCustomerId() const { return customerId; }
};

class Person : public Entity {
protected:
    std::string firstName;
    std::string lastName;
public:
    std::string getFirstName() const { return firstName; }
    std::string getLastName()  const { return lastName; }
};

class Customer : public Person {
public:
    typedef Customer Self;   // Self-typedef
    typedef Self*    Ptr;

private:
    ContactInfo    contact;
    CustomerStatus status;

public:
    Customer(const std::string& firstName, const std::string& lastName,
             const ContactInfo& contact, CustomerStatus status = ACTIVE);

    std::string    getEmail()   const;
    std::string    getPhone()   const;
    std::string    getAddress() const;
    CustomerStatus getStatus()  const;

    bool validate(std::string& errorMsg) const;
    void display() const;
};

#endif // CUSTOMER_H
