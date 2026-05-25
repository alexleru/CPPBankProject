#include "../include/LoggingVisitor.h"
#include "../include/Deposit.h"
#include "../include/Withdrawal.h"
#include "../include/Transfer.h"
#include "../include/LoanPayment.h"
#include "../include/Account.h"
#include "../include/Utils.h"
#include <ostream>

LoggingVisitor::LoggingVisitor(std::ostream& stream) : out(stream) {}

void LoggingVisitor::visit(Deposit* d) {
    if (!d) return;
    out << "  [LOG] DEPOSIT  #" << d->getSequence()
        << " amount=" << Utils::formatCurrency(d->getAmount());
    if (d->getDest()) out << " to=" << d->getDest()->getId();
    out << "\n";
}

void LoggingVisitor::visit(Withdrawal* w) {
    if (!w) return;
    out << "  [LOG] WITHDRAW #" << w->getSequence()
        << " amount=" << Utils::formatCurrency(w->getAmount());
    if (w->getSource()) out << " from=" << w->getSource()->getId();
    out << "\n";
}

void LoggingVisitor::visit(Transfer* t) {
    if (!t) return;
    out << "  [LOG] TRANSFER #" << t->getSequence()
        << " amount=" << Utils::formatCurrency(t->getAmount());
    if (t->getSource()) out << " from=" << t->getSource()->getId();
    if (t->getDest())   out << " to="   << t->getDest()->getId();
    out << "\n";
}

void LoggingVisitor::visit(LoanPayment* l) {
    if (!l) return;
    out << "  [LOG] LOANPAY  #" << l->getSequence()
        << " amount=" << Utils::formatCurrency(l->getAmount());
    if (l->getSource()) out << " from=" << l->getSource()->getId();
    out << "\n";
}
