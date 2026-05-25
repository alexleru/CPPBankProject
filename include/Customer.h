#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <vector>
#include <string>
#include "Constants.h"

class Bank;     // SCC C peer (non-owning back-pointer)
class Account;  // SCC A — back-references close the mega-SCC
class Loan;     // SCC C peer

// Owns its Account* and Loan*. Holds a non-owning Bank* back-pointer
// which closes part of the mega-SCC.
class Customer {
private:
    CustomerId             id;
    std::string            firstName;
    std::string            lastName;
    std::vector<Account*>  accounts;  // owning
    std::vector<Loan*>     loans;     // owning
    Bank*                  owner;     // non-owning

public:
    Customer(const CustomerId& id,
             const std::string& firstName,
             const std::string& lastName,
             Bank* owner);
    ~Customer();

    void addAccount(Account* a);
    void addLoan(Loan* l);

    const CustomerId&            getId()        const;
    std::string                  getFullName()  const;
    Bank*                        getOwner()     const;
    const std::vector<Account*>& getAccounts()  const;
    const std::vector<Loan*>&    getLoans()     const;
};

#endif
