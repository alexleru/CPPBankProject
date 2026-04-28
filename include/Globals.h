#ifndef GLOBALS_H
#define GLOBALS_H

// Semantic primitive typedef
typedef int CustomerCounter;

extern CustomerCounter globalCustomerCounter;

// Default argument
void initializeGlobals(CustomerCounter start = 1000);

#endif // GLOBALS_H
