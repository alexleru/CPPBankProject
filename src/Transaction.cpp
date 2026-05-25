#include "../include/Transaction.h"
#include "../include/Account.h"

Transaction::Transaction(Account* src, Account* dst, Money amt)
    : source(src), dest(dst), amount(amt), sequence(0),
      timestamp(std::time(NULL)) {}

Transaction::~Transaction() {}

void Transaction::apply() {
    // SCC A back-edge: invoke Account methods.
    if (source) source->debit(amount);
    if (dest)   dest->credit(amount);
}

Account*    Transaction::getSource()    const { return source; }
Account*    Transaction::getDest()      const { return dest; }
Money       Transaction::getAmount()    const { return amount; }
int         Transaction::getSequence()  const { return sequence; }
void        Transaction::setSequence(int s)   { sequence = s; }
std::time_t Transaction::getTimestamp() const { return timestamp; }
