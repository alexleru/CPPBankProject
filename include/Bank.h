#ifndef BANK_H
#define BANK_H

#include <vector>
#include <string>
#include "Constants.h"

// Forward declarations of SCC C peers. Full definitions live in their
// own headers; .cpp completes the includes to form the dependency cycle.
class Customer;
class BranchManager;
class AuditLogger;
class NotificationCenter;

// Aggregate root for SCC C.
// Ownership:
//   - customers (vector) : owned (delete in dtor)
//   - managers  (vector) : owned (delete in dtor)
//   - logger             : owned (delete in dtor)
//   - notificationCenter : owned (delete in dtor)
class Bank {
private:
    std::string                  name;
    std::vector<Customer*>       customers;
    std::vector<BranchManager*>  managers;
    AuditLogger*                 logger;
    NotificationCenter*          notificationCenter;

public:
    Bank(const std::string& name);
    ~Bank();

    void registerCustomer(Customer* c);
    void addBranchManager(BranchManager* m);
    void setAuditLogger(AuditLogger* l);
    void setNotificationCenter(NotificationCenter* nc);

    const std::string&                  getName()                const;
    AuditLogger*                        getAuditLogger()         const;
    NotificationCenter*                 getNotificationCenter()  const;
    const std::vector<Customer*>&       getCustomers()           const;
    const std::vector<BranchManager*>&  getBranchManagers()      const;
};

#endif
