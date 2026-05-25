#ifndef TRANSFER_H
#define TRANSFER_H

#include "Transaction.h"

class TransactionVisitor;

class Transfer : public Transaction {
public:
    Transfer(Account* source, Account* dest, Money amount);
    virtual void accept(TransactionVisitor& v);
};

#endif
