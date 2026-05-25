#ifndef LOAN_PAYMENT_H
#define LOAN_PAYMENT_H

#include "Transaction.h"

class TransactionVisitor;
class Loan;  // SCC C: payment reduces this loan's outstanding balance

class LoanPayment : public Transaction {
private:
    Loan* loan;  // non-owning
public:
    LoanPayment(Account* source, Loan* loan, Money amount);

    virtual void apply();                       // debits source, then loan->recordPayment
    virtual void accept(TransactionVisitor& v); // SCC B dispatch
    Loan* getLoan() const;
};

#endif
