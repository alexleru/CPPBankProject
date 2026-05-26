#include "../include/Account.h"
#include "../include/Transaction.h"
#include "../include/Utils.h"

#include <ostream>

Account::Account(const AccountId& id, Money initialBalance,
                 Customer* h, Bank* b, AuditLogger* l)
    : accountId(id), balance(initialBalance),
      holder(h), bank(b), logger(l) {}

Account::~Account() {
    // Owns its transactions. Uses the typedef'd iterator alias.
    for (TxHistoryIterator it = history.begin();
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

const TransactionHistory& Account::getHistory() const { return history; }

void Account::setHolder(Customer* c)    { holder = c; }
void Account::setBank(Bank* b)          { bank   = b; }
void Account::setLogger(AuditLogger* l) { logger = l; }

// ---------------------------------------------------------------------------
// Friend-function back-doors. Defined as plain free functions in the
// enclosing namespace; the friend declarations inside Account let them
// read / mutate private members directly. The chunker should see these
// as Tier-A acyclic helpers tied to Account.
// ---------------------------------------------------------------------------
void debugDumpAccount(const Account& a, std::ostream& os) {
    os << "[DEBUG Account]"
       << " id="      << a.accountId
       << " balance=" << Utils::formatCurrency(a.balance)
       << " txCount=" << a.history.size()
       << " holder="  << (a.holder ? "<set>" : "<null>")
       << " bank="    << (a.bank   ? "<set>" : "<null>")
       << " logger="  << (a.logger ? "<set>" : "<null>")
       << "\n";
}

void forceAccountBalance(Account& a, Money newBalance) {
    a.balance = newBalance;
}
