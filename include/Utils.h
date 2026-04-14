#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "Enums.h"

class Utils {
public:
    static std::string generateCustomerId();
    static std::string generateAccountId();
    static bool validateEmail(const std::string& email);
    static bool validatePhone(const std::string& phone);
    static std::string accountTypeToString(AccountType type);

private:
    Utils();
};

#endif // UTILS_H
