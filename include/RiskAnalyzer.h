#ifndef RISK_ANALYZER_H
#define RISK_ANALYZER_H

#include "Constants.h"
#include <vector>

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

    // Evaluates credit risk for a loan application requiring full approval chain context.
    // SCC: takes FOUR SCC-C partners simultaneously — Loan, Customer, BranchManager, AuditLogger.
    Money evaluateApplication(
        Loan*          application,
        Customer*      applicant,
        BranchManager* approvingManager,
        AuditLogger*   auditTrail);

    // Aggregates portfolio risk across all loans of a customer.
    // SCC: returns std::vector<Loan*> — collection of SCC partner type.
    std::vector<Loan*> flagHighRisk(
        Customer*      customer,
        BranchManager* responsibleManager,
        Money          threshold);
};

#endif
