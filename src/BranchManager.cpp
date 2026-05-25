#include "../include/BranchManager.h"
#include "../include/Bank.h"
#include "../include/Customer.h"
#include "../include/Loan.h"
#include "../include/RiskAnalyzer.h"
#include "../include/AuditLogger.h"
#include "../include/NotificationCenter.h"

BranchManager::BranchManager(Bank* bank_, RiskAnalyzer* analyzer_)
    : bank(bank_), analyzer(analyzer_) {}

BranchManager::~BranchManager() { delete analyzer; }

void BranchManager::assignCustomer(Customer* c) {
    if (c) assigned.push_back(c);
}

void BranchManager::approveLoan(Loan* l, AuditLogger* logger) {
    if (!l) return;
    if (analyzer) analyzer->assess(l);
    l->approve(logger);
    if (bank && bank->getNotificationCenter()) {
        const std::string borrowerName =
            l->getBorrower() ? l->getBorrower()->getFullName() : std::string("?");
        bank->getNotificationCenter()->broadcast(bank,
            "Loan approved for " + borrowerName);
    }
}

void BranchManager::escalate(Loan* l, AuditLogger* logger) {
    if (!l || !analyzer) return;
    analyzer->assess(l);
    if (l->getBorrower()) analyzer->flagCustomer(l->getBorrower(), this);
    if (logger) logger->log("Loan escalated: " + l->getId());
}

RiskAnalyzer* BranchManager::getRiskAnalyzer() const { return analyzer; }
Bank*         BranchManager::getBank()         const { return bank;     }
