#include "../include/Transfer.h"
#include "../include/TransactionVisitor.h"

Transfer::Transfer(Account* source, Account* dest, Money amount)
    : Transaction(source, dest, amount) {}

void Transfer::accept(TransactionVisitor& v) {
    v.visit(this);
}
