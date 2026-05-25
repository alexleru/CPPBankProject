#include "../include/Account.h"
#include "../include/Transaction.h"

Account::Account(const AccountId& id, Money initialBalance,
                 Customer* h, Bank* b, AuditLogger* l)
    : accountId(id), balance(initialBalance),
      holder(h), bank(b), logger(l) {}

Account::~Account() {
    // Owns its transactions.
    for (std::vector<Transaction*>::iterator it = history.begin();
         it != history.end(); ++it) {
        delete *it;
    }
}

void Account::addTransaction(Transaction* t) {
    history.push_back(t);
    if (t) t->setSequence(static_cast<int>(history.size()));
}

void Account::debit(Money amt)  { balance -= amt; }
void Account::credit(Money amt) { balance += amt; }

Money              Account::getBalance() const { return balance; }
const AccountId&   Account::getId()      const { return accountId; }
Customer*          Account::getHolder()  const { return holder; }
Bank*              Account::getBank()    const { return bank; }
AuditLogger*       Account::getLogger()  const { return logger; }

const std::vector<Transaction*>& Account::getHistory() const { return history; }

void Account::setHolder(Customer* c)    { holder = c; }
void Account::setBank(Bank* b)          { bank   = b; }
void Account::setLogger(AuditLogger* l) { logger = l; }
