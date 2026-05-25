#include "../include/NotificationCenter.h"
#include "../include/Customer.h"
#include "../include/AuditLogger.h"
#include "../include/Bank.h"
#include <iostream>

NotificationCenter::NotificationCenter(AuditLogger* log) : auditLog(log) {}

void NotificationCenter::subscribe(Customer* c) {
    if (c) subscribers.push_back(c);
}

void NotificationCenter::broadcast(Bank* bank, const std::string& msg) {
    std::string prefix;
    if (bank) prefix = "[" + bank->getName() + "] ";
    for (size_t i = 0; i < subscribers.size(); ++i) {
        Customer* c = subscribers[i];
        if (c) std::cout << "  [NC] -> " << c->getFullName()
                         << ": " << prefix << msg << "\n";
    }
    if (auditLog) auditLog->log("Broadcast: " + msg);
}

AuditLogger* NotificationCenter::getAuditLog() const { return auditLog; }
