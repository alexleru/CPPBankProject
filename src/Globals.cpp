#include "../include/Globals.h"

CustomerCounter globalCustomerCounter = 1000;

void initializeGlobals(CustomerCounter start) {
    globalCustomerCounter = start;
}
