#ifndef NOTIFICATION_CENTER_H
#define NOTIFICATION_CENTER_H

#include <vector>
#include <string>

class Customer;
class AuditLogger;
class Bank;

// Holds non-owning subscribers and a non-owning AuditLogger*. Bank
// owns the NotificationCenter itself.
class NotificationCenter {
private:
    std::vector<Customer*> subscribers;
    AuditLogger*           auditLog;

public:
    NotificationCenter(AuditLogger* log);

    void subscribe(Customer* c);
    void broadcast(Bank* bank, const std::string& msg);

    AuditLogger* getAuditLog() const;
};

#endif
