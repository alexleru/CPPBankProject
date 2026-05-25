#ifndef BRANCH_MANAGER_H
#define BRANCH_MANAGER_H

#include <vector>

class Bank;
class Customer;
class Loan;
class RiskAnalyzer;
class AuditLogger;
class NotificationCenter;

// Owns its RiskAnalyzer. Bank owns this BranchManager.
// 'assigned' holds non-owning Customer* refs assigned to this branch.
class BranchManager {
private:
    Bank*                   bank;       // non-owning
    std::vector<Customer*>  assigned;   // non-owning
    RiskAnalyzer*           analyzer;   // owning

public:
    BranchManager(Bank* bank, RiskAnalyzer* analyzer);
    ~BranchManager();

    void assignCustomer(Customer* c);
    void approveLoan(Loan* l, AuditLogger* logger);
    void escalate(Loan* l, AuditLogger* logger);

    RiskAnalyzer* getRiskAnalyzer() const;
    Bank*         getBank()         const;
};

#endif
