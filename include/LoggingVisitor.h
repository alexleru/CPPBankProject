#ifndef LOGGING_VISITOR_H
#define LOGGING_VISITOR_H

#include <iosfwd>
#include "TransactionVisitor.h"

// Concrete TransactionVisitor that writes each operation to a stream.
// Lives outside SCC B's tight 5-class cycle (it depends *on* the cycle
// but is not depended on by any cycle member).
class LoggingVisitor : public TransactionVisitor {
private:
    std::ostream& out;
public:
    LoggingVisitor(std::ostream& stream);

    virtual void visit(Deposit*     d);
    virtual void visit(Withdrawal*  w);
    virtual void visit(Transfer*    t);
    virtual void visit(LoanPayment* l);
};

#endif
