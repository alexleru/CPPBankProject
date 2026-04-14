#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include "Enums.h"

class Account {
protected:
    std::string accountId;
    AccountType type;
    double      balance;
    bool        isActive;

public:
    Account(AccountType type, double initialBalance);
    virtual ~Account();

    std::string getAccountId() const;
    AccountType getType()      const;
    double      getBalance()   const;
    bool        getIsActive()  const;

    virtual std::string getAccountType() const = 0;
    virtual void display() const;
};

#endif // ACCOUNT_H
