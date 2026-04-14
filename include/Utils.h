#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils {
public:
    static std::string generateCustomerId();
    static bool validateEmail(const std::string& email);
    static bool validatePhone(const std::string& phone);

private:
    Utils();
};

#endif // UTILS_H
