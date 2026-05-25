#include "../include/RiskAnalyzer.h"
#include "../include/Bank.h"
#include "../include/AuditLogger.h"
#include "../include/Loan.h"
#include "../include/Customer.h"
#include "../include/BranchManager.h"

RiskAnalyzer::RiskAnalyzer(Bank* bank_, AuditLogger* logger_)
    : bank(bank_), logger(logger_) {}

Money RiskAnalyzer::assess(Loan* l) {
    if (!l) return 0.0;
    Money score = l->getPrincipal() * 0.05;
    if (l->getBorrower() && l->getBorrower()->getAccounts().empty()) {
        score *= 2.0;  // unproven borrower
    }
    if (logger) logger->log("Risk assessed for loan " + l->getId());
    return score;
}

void RiskAnalyzer::flagCustomer(Customer* c, BranchManager* m) {
    if (!c) return;
    if (logger) logger->log("Customer flagged: " + c->getFullName());
    if (m && m->getRiskAnalyzer() == this && logger) {
        logger->log("Flag via own branch manager confirmed");
    }
}
