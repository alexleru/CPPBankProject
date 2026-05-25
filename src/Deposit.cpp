#include "../include/Deposit.h"
#include "../include/TransactionVisitor.h"

Deposit::Deposit(Account* dest, Money amount)
    : Transaction(NULL, dest, amount) {}

void Deposit::accept(TransactionVisitor& v) {
    v.visit(this);
}
