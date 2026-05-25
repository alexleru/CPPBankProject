#ifndef TRANSACTION_VISITOR_H
#define TRANSACTION_VISITOR_H

// SCC B core. Forward-declares the four concrete transaction types
// so this header can be #included by them in turn.
class Deposit;
class Withdrawal;
class Transfer;
class LoanPayment;

class TransactionVisitor {
public:
    virtual ~TransactionVisitor();

    virtual void visit(Deposit*     d) = 0;
    virtual void visit(Withdrawal*  w) = 0;
    virtual void visit(Transfer*    t) = 0;
    virtual void visit(LoanPayment* l) = 0;
};

#endif
