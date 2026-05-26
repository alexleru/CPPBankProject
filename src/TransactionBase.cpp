#include "../include/TransactionBase.h"

unsigned long TransactionBase::instanceCounter = 0UL;

TransactionBase::TransactionBase()
    : instanceId(++instanceCounter) {}

TransactionBase::~TransactionBase() {}

unsigned long TransactionBase::getInstanceId() const {
    return instanceId;
}

unsigned long TransactionBase::getInstanceCount() {
    return instanceCounter;
}
