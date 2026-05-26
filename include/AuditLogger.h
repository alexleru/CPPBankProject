#ifndef AUDIT_LOGGER_H
#define AUDIT_LOGGER_H

#include <vector>
#include <map>
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

    // Event-type histogram. Exercises std::map iteration / find / insert
    // and pairs cleanly with the unsigned-long counter shown in
    // TransactionBase: every entry recorded here is counted in an
    // unsigned-long bucket.
    std::map<std::string, unsigned long> eventCounts;

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

    // Renders the eventCounts map as a small summary block.
    void dumpEventSummary(std::ostream& out) const;

    // Looks up the count for an event type. Returns 0 if absent.
    unsigned long getEventCount(const std::string& eventType) const;

private:
    // Centralised increment so every recorded event lands in the map.
    void recordEvent(const std::string& eventType);
};

#endif
