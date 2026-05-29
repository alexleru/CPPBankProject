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

Money RiskAnalyzer::evaluateApplication(Loan*          application,
                                        Customer*      applicant,
                                        BranchManager* approvingManager,
                                        AuditLogger*   auditTrail) {
    if (!application || !applicant) return 0.0;
    // Base risk from the loan itself.
    Money score = assess(application);
    // Discount if the applicant already holds accounts at the bank.
    if (!applicant->getAccounts().empty()) score *= 0.9;
    // Confirm chain-of-approval: manager owns this analyzer.
    if (approvingManager && approvingManager->getRiskAnalyzer() == this) {
        score *= 0.95;  // approved-by-own-branch discount
    }
    if (auditTrail) {
        auditTrail->log("Application evaluated for " + applicant->getFullName() +
                        " loan=" + application->getId());
    }
    return score;
}

std::vector<Loan*> RiskAnalyzer::flagHighRisk(Customer*      customer,
                                              BranchManager* responsibleManager,
                                              Money          threshold) {
    std::vector<Loan*> highRisk;
    if (!customer) return highRisk;
    const std::vector<Loan*>& loans = customer->getLoans();
    for (size_t i = 0; i < loans.size(); ++i) {
        Loan* l = loans[i];
        if (!l) continue;
        if (l->assessRisk() > threshold) {
            highRisk.push_back(l);
            if (logger) logger->log("High-risk loan flagged: " + l->getId());
            if (responsibleManager && logger &&
                responsibleManager->getRiskAnalyzer() == this) {
                logger->log("Notified responsible branch manager");
            }
        }
    }
    return highRisk;
}
