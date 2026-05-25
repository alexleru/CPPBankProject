#include "../include/Withdrawal.h"
#include "../include/TransactionVisitor.h"

Withdrawal::Withdrawal(Account* source, Money amount)
    : Transaction(source, NULL, amount) {}

void Withdrawal::accept(TransactionVisitor& v) {
    v.visit(this);
}
