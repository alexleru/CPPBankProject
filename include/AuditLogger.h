#ifndef AUDIT_LOGGER_H
#define AUDIT_LOGGER_H

#include <vector>
#include <string>
#include <iosfwd>

class Account;
class Loan;
class Customer;
class NotificationCenter;
class Bank;

// Observer hub for SCC C. Owns nothing; only collects log lines.
// Holds a non-owning Bank* once Bank::setAuditLogger wires it.
class AuditLogger {
private:
    std::vector<std::string> entries;
    Bank*                    bank;

public:
    AuditLogger();

    void setBank(Bank* b);

    void onAccountCreated(Account* a);
    void onLoanApproved(Loan* l);
    void onCustomerRegistered(Customer* c);

    // Routes a message through a NotificationCenter (which itself depends
    // on Bank). This closes one more cycle edge.
    void notify(NotificationCenter* nc, const std::string& msg);

    void log(const std::string& msg);

    const std::vector<std::string>& getEntries() const;
    void dump(std::ostream& out) const;
};

#endif
