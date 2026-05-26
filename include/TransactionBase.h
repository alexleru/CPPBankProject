#ifndef TRANSACTION_BASE_H
#define TRANSACTION_BASE_H

// Acyclic root above Transaction. References nothing in any SCC, so the
// new inheritance edge (Transaction : public TransactionBase) leaves SCC A,
// SCC B, SCC C and SCC D unchanged while introducing a 3-level
// inheritance chain Deposit / Withdrawal / Transfer / LoanPayment
//                       -> Transaction -> TransactionBase.
//
// Also demonstrates `unsigned long` usage via a process-wide instance
// counter that gives every Transaction a unique ID independent of its
// per-account sequence number.
class TransactionBase {
protected:
    unsigned long instanceId;

    static unsigned long instanceCounter;

public:
    TransactionBase();
    virtual ~TransactionBase();

    unsigned long getInstanceId() const;

    static unsigned long getInstanceCount();
};

#endif
