#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>
#include "Constants.h"

// Forward declarations
class Transaction;   // SCC A partner: owned in history vector
class Customer;      // SCC C: non-owning back-pointer (forms mega-SCC)
class Bank;          // SCC C: non-owning back-pointer (forms mega-SCC)
class AuditLogger;   // SCC C: non-owning observer (forms mega-SCC)

// Owns its transactions (deletes them in dtor).
// Back-pointers to Customer/Bank/AuditLogger are non-owning.
class Account {
private:
    AccountId                  accountId;
    Money                      balance;
    std::vector<Transaction*>  history;   // owning

    Customer*                  holder;    // non-owning (SCC C link)
    Bank*                      bank;      // non-owning (SCC C link)
    AuditLogger*               logger;    // non-owning (SCC C link)

public:
    Account(const AccountId& id, Money initialBalance,
            Customer* holder, Bank* bank, AuditLogger* logger);
    ~Account();

    // SCC A reciprocity: records transaction & numbers it.
    void addTransaction(Transaction* t);
    // Called by Transaction::apply() in SCC A.
    void debit(Money amt);
    void credit(Money amt);

    Money              getBalance()  const;
    const AccountId&   getId()       const;
    Customer*          getHolder()   const;
    Bank*              getBank()     const;
    AuditLogger*       getLogger()   const;

    const std::vector<Transaction*>& getHistory() const;

    // Setters used during late wiring (Customer creates Account, then Bank
    // assigns logger after construction).
    void setHolder(Customer* c);
    void setBank(Bank* b);
    void setLogger(AuditLogger* l);
};

#endif
