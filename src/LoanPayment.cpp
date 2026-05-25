#include "../include/LoanPayment.h"
#include "../include/TransactionVisitor.h"
#include "../include/Loan.h"

LoanPayment::LoanPayment(Account* source, Loan* loan_, Money amount)
    : Transaction(source, NULL, amount), loan(loan_) {}

void LoanPayment::apply() {
    Transaction::apply();              // debits source (no dest)
    if (loan) loan->recordPayment(amount);
}

void LoanPayment::accept(TransactionVisitor& v) {
    v.visit(this);
}

Loan* LoanPayment::getLoan() const { return loan; }
