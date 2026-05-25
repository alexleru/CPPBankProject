#ifndef LOAN_H
#define LOAN_H

#include "Constants.h"
#include "Enums.h"

class Customer;
class Account;
class RiskAnalyzer;
class AuditLogger;

// Non-owning pointers throughout. Borrower owns this Loan via its
// vector<Loan*>; repaymentAccount, analyzer, and AuditLogger callbacks
// are observed only.
class Loan {
private:
    LoanId        id;
    Customer*     borrower;
    Account*      repaymentAccount;
    RiskAnalyzer* analyzer;
    Money         principal;
    Money         outstanding;
    LoanStatus    status;

public:
    Loan(const LoanId& id, Customer* borrower, Account* repayAcc,
         RiskAnalyzer* analyzer, Money principal);

    void   approve(AuditLogger* logger);
    Money  assessRisk();                 // delegates to analyzer->assess(this)
    void   recordPayment(Money amount);  // reduces outstanding; may close

    const LoanId& getId()               const;
    Customer*     getBorrower()         const;
    Account*      getRepaymentAccount() const;
    Money         getPrincipal()        const;
    Money         getOutstanding()      const;
    LoanStatus    getStatus()           const;
};

#endif
