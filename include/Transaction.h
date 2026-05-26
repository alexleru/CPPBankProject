#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <ctime>
#include "Constants.h"
#include "TransactionBase.h"

// Forward declaration of the SCC A partner.
class Account;

// SCC A base. Concrete: holds Account* source/dest and implements apply()
// by calling Account::debit / Account::credit. This closes the SCC A
// cycle (Account ↔ Transaction).
//
// Inherits from TransactionBase (acyclic) to give the project a real
// 3-level inheritance chain (Deposit -> Transaction -> TransactionBase)
// without dragging any new file into a cycle.
//
// NOTE: accept(TransactionVisitor&) is intentionally NOT declared here.
// Declaring it would add a Transaction → TransactionVisitor edge and
// collapse SCC B (Visitor cycle) into the mega-SCC. Visitor dispatch
// lives only on the concrete subclasses Deposit/Withdrawal/Transfer/
// LoanPayment.
class Transaction : public TransactionBase {
protected:
    Account*    source;
    Account*    dest;
    Money       amount;
    int         sequence;
    std::time_t timestamp;

public:
    Transaction(Account* src, Account* dst, Money amt);
    virtual ~Transaction();

    virtual void apply();

    Account*    getSource()    const;
    Account*    getDest()      const;
    Money       getAmount()    const;
    int         getSequence()  const;
    void        setSequence(int s);
    std::time_t getTimestamp() const;
};

#endif
