#include "../include/Loan.h"
#include "../include/Customer.h"
#include "../include/Account.h"
#include "../include/RiskAnalyzer.h"
#include "../include/AuditLogger.h"

Loan::Loan(const LoanId& id_, Customer* borrower_, Account* repay_,
           RiskAnalyzer* analyzer_, Money principal_)
    : id(id_), borrower(borrower_), repaymentAccount(repay_),
      analyzer(analyzer_), principal(principal_),
      outstanding(principal_), status(PENDING_APPROVAL) {}

void Loan::approve(AuditLogger* logger) {
    status = LOAN_ACTIVE;
    if (logger) logger->onLoanApproved(this);
}

Money Loan::assessRisk() {
    if (analyzer) return analyzer->assess(this);
    return 0.0;
}

void Loan::recordPayment(Money amount) {
    outstanding -= amount;
    if (outstanding <= 0) {
        outstanding = 0;
        status = PAID_OFF;
    }
}

const LoanId& Loan::getId()               const { return id; }
Customer*     Loan::getBorrower()         const { return borrower; }
Account*      Loan::getRepaymentAccount() const { return repaymentAccount; }
Money         Loan::getPrincipal()        const { return principal; }
Money         Loan::getOutstanding()      const { return outstanding; }
LoanStatus    Loan::getStatus()           const { return status; }
