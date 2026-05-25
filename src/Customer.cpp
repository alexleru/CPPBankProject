#include "../include/Customer.h"
#include "../include/Bank.h"
#include "../include/Account.h"
#include "../include/Loan.h"
#include "../include/AuditLogger.h"

Customer::Customer(const CustomerId& id_,
                   const std::string& first,
                   const std::string& last,
                   Bank* o)
    : id(id_), firstName(first), lastName(last), owner(o) {}

Customer::~Customer() {
    for (size_t i = 0; i < accounts.size(); ++i) delete accounts[i];
    for (size_t i = 0; i < loans.size();    ++i) delete loans[i];
}

void Customer::addAccount(Account* a) {
    if (!a) return;
    accounts.push_back(a);
    a->setHolder(this);
    if (owner) {
        a->setBank(owner);
        AuditLogger* log = owner->getAuditLogger();
        if (log) {
            a->setLogger(log);
            log->onAccountCreated(a);
        }
    }
}

void Customer::addLoan(Loan* l) {
    if (!l) return;
    loans.push_back(l);
    if (owner && owner->getAuditLogger())
        owner->getAuditLogger()->onLoanApproved(l);
}

const CustomerId&            Customer::getId()       const { return id; }
std::string                  Customer::getFullName() const { return firstName + " " + lastName; }
Bank*                        Customer::getOwner()    const { return owner; }
const std::vector<Account*>& Customer::getAccounts() const { return accounts; }
const std::vector<Loan*>&    Customer::getLoans()    const { return loans; }
