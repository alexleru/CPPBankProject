#include "../include/Bank.h"
#include "../include/Customer.h"
#include "../include/BranchManager.h"
#include "../include/AuditLogger.h"
#include "../include/NotificationCenter.h"

Bank::Bank(const std::string& n)
    : name(n), logger(NULL), notificationCenter(NULL) {}

Bank::~Bank() {
    for (size_t i = 0; i < customers.size(); ++i) delete customers[i];
    for (size_t i = 0; i < managers.size();  ++i) delete managers[i];
    delete logger;
    delete notificationCenter;
}

void Bank::registerCustomer(Customer* c) {
    if (!c) return;
    customers.push_back(c);
    if (logger)             logger->onCustomerRegistered(c);
    if (notificationCenter) notificationCenter->broadcast(this,
        "Customer registered: " + c->getFullName());
}

void Bank::addBranchManager(BranchManager* m) {
    if (m) managers.push_back(m);
}

void Bank::setAuditLogger(AuditLogger* l) {
    logger = l;
    if (l) l->setBank(this);
}

void Bank::setNotificationCenter(NotificationCenter* nc) {
    notificationCenter = nc;
}

const std::string&  Bank::getName()                const { return name; }
AuditLogger*        Bank::getAuditLogger()         const { return logger; }
NotificationCenter* Bank::getNotificationCenter()  const { return notificationCenter; }

const std::vector<Customer*>& Bank::getCustomers() const {
    return customers;
}
const std::vector<BranchManager*>& Bank::getBranchManagers() const {
    return managers;
}
