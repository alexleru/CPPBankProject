#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "Constants.h"

// Function-pointer typedef
typedef int (*BinaryIntOp)(int, int);

class Utils {
public:
    static CustomerId generateCustomerId();
    static bool validateEmail(const std::string& email);
    static bool validatePhone(const std::string& phone);

    // Sample functions to be passed as function pointers
    static int add(int a, int b);
    static int multiply(int a, int b);

    // Function-pointer parameter declared via the BinaryIntOp typedef
    static void performOperation(int x, int y, BinaryIntOp operation);

private:
    Utils();
};

#endif // UTILS_H
