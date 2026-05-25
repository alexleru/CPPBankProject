#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "Transaction.h"

class TransactionVisitor;  // SCC B cycle: defined in TransactionVisitor.h

class Deposit : public Transaction {
public:
    Deposit(Account* dest, Money amount);
    virtual void accept(TransactionVisitor& v);
};

#endif
