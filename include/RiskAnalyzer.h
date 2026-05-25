#ifndef RISK_ANALYZER_H
#define RISK_ANALYZER_H

#include "Constants.h"

class Bank;
class AuditLogger;
class Loan;
class Customer;
class BranchManager;

// All pointers non-owning. BranchManager owns this RiskAnalyzer.
class RiskAnalyzer {
private:
    Bank*        bank;
    AuditLogger* logger;

public:
    RiskAnalyzer(Bank* bank, AuditLogger* logger);

    Money assess(Loan* l);
    void  flagCustomer(Customer* c, BranchManager* m);
};

#endif
