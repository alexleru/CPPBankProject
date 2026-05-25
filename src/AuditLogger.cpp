#include "../include/AuditLogger.h"
#include "../include/Account.h"
#include "../include/Loan.h"
#include "../include/Customer.h"
#include "../include/NotificationCenter.h"
#include "../include/Bank.h"
#include "../include/Utils.h"
#include <sstream>
#include <ostream>

AuditLogger::AuditLogger() : bank(NULL) {}

void AuditLogger::setBank(Bank* b) { bank = b; }

void AuditLogger::onAccountCreated(Account* a) {
    if (!a) return;
    std::ostringstream s;
    s << "[AUDIT]";
    if (bank) s << " bank=" << bank->getName();
    s << " ACCOUNT_CREATED id=" << a->getId()
      << " balance=" << Utils::formatCurrency(a->getBalance());
    entries.push_back(s.str());
}

void AuditLogger::onLoanApproved(Loan* l) {
    if (!l) return;
    std::ostringstream s;
    s << "[AUDIT] LOAN_APPROVED id=" << l->getId()
      << " principal=" << Utils::formatCurrency(l->getPrincipal());
    if (l->getBorrower()) s << " borrower=" << l->getBorrower()->getFullName();
    entries.push_back(s.str());
}

void AuditLogger::onCustomerRegistered(Customer* c) {
    if (!c) return;
    std::ostringstream s;
    s << "[AUDIT] CUSTOMER_REGISTERED name=" << c->getFullName()
      << " id=" << c->getId();
    entries.push_back(s.str());
}

void AuditLogger::notify(NotificationCenter* nc, const std::string& msg) {
    if (!nc) return;
    if (bank) nc->broadcast(bank, msg);
    entries.push_back(std::string("[AUDIT] NOTIFY ") + msg);
}

void AuditLogger::log(const std::string& msg) {
    entries.push_back(std::string("[AUDIT] ") + msg);
}

const std::vector<std::string>& AuditLogger::getEntries() const { return entries; }

void AuditLogger::dump(std::ostream& out) const {
    for (size_t i = 0; i < entries.size(); ++i) out << entries[i] << "\n";
}
