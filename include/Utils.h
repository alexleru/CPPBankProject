#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils {
public:
    static std::string generateCustomerId();
    static bool validateEmail(const std::string& email);
    static bool validatePhone(const std::string& phone);

    // Sample functions to be passed as function pointers
    static int add(int a, int b);
    static int multiply(int a, int b);

    // Function that takes a function pointer as a parameter
    // Syntax: int (*operation)(int, int)
    static void performOperation(int x, int y, int (*operation)(int, int));

private:
    Utils();
};

#endif // UTILS_H
