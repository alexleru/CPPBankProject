#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

#include "Transaction.h"

class TransactionVisitor;

class Withdrawal : public Transaction {
public:
    Withdrawal(Account* source, Money amount);
    virtual void accept(TransactionVisitor& v);
};

#endif
