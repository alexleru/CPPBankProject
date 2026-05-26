#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>
#include <iosfwd>
#include "Constants.h"

// Forward declarations
class Transaction;   // SCC A partner: owned in history vector
class Customer;      // SCC C: non-owning back-pointer (forms mega-SCC)
class Bank;          // SCC C: non-owning back-pointer (forms mega-SCC)
class AuditLogger;   // SCC C: non-owning observer (forms mega-SCC)
class Account;

// C++03 typedef aliases (the only alias mechanism available without
// C++11 `using`). Demonstrates container typedef and iterator typedefs.
typedef std::vector<Transaction*>           TransactionHistory;
typedef TransactionHistory::iterator        TxHistoryIterator;
typedef TransactionHistory::const_iterator  TxHistoryConstIterator;

// Owns its transactions (deletes them in dtor).
// Back-pointers to Customer/Bank/AuditLogger are non-owning.
class Account {
public:
    // "Self typedef" inside a class — the C++03 idiom used in STL-style
    // containers (e.g. boost iterator_facade) to expose the wrapped type.
    typedef Account self_type;

private:
    AccountId                  accountId;
    Money                      balance;
    TransactionHistory         history;   // owning (was std::vector<Transaction*>)

    Customer*                  holder;    // non-owning (SCC C link)
    Bank*                      bank;      // non-owning (SCC C link)
    AuditLogger*               logger;    // non-owning (SCC C link)

public:
    Account(const AccountId& id, Money initialBalance,
            Customer* holder, Bank* bank, AuditLogger* logger);
    ~Account();

    // SCC A reciprocity: records transaction & numbers it.
    void addTransaction(Transaction* t);
    // Called by Transaction::apply() in SCC A.
    void debit(Money amt);
    void credit(Money amt);

    Money              getBalance()  const;
    const AccountId&   getId()       const;
    Customer*          getHolder()   const;
    Bank*              getBank()     const;
    AuditLogger*       getLogger()   const;

    const TransactionHistory& getHistory() const;

    // Setters used during late wiring (Customer creates Account, then Bank
    // assigns logger after construction).
    void setHolder(Customer* c);
    void setBank(Bank* b);
    void setLogger(AuditLogger* l);

    // Friend-function back-doors — the canonical C++ pattern for letting
    // a debug / admin free function reach into private state without
    // promoting it to a public getter/setter.
    friend void debugDumpAccount(const self_type& a, std::ostream& os);
    friend void forceAccountBalance(self_type& a, Money newBalance);
};

// Namespace-scope declarations of the friend back-doors, so callers
// can reach them via ordinary lookup (not only ADL).
void debugDumpAccount(const Account& a, std::ostream& os);
void forceAccountBalance(Account& a, Money newBalance);

#endif
