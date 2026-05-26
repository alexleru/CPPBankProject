/* age_verifier.cpp -- cross-platform implementation of verify_age_21.
 *
 * Built into:
 *   native/windows/age_verifier.dll  (Windows)
 *   native/linux/libage_verifier.so  (Linux)
 *
 * All business logic lives here. The only platform-specific pieces are the
 * export attribute (declared in age_verifier.h) and the time-of-day call,
 * which uses the C standard library on both platforms.
 */
#define AGE_VERIFIER_BUILD
#include "../include/age_verifier.h"

#include <ctime>

namespace {

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    static const int table[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int d = table[month - 1];
    if (month == 2 && isLeapYear(year)) d = 29;
    return d;
}

bool isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 9999) return false;
    if (month < 1 || month > 12)    return false;
    if (day   < 1)                   return false;
    return day <= daysInMonth(month, year);
}

/* Compute completed years between two dates (birth -> reference).
 * Returns a negative number if the birth date is in the future. */
int yearsBetween(int bd, int bm, int by, int rd, int rm, int ry) {
    int age = ry - by;
    if (rm < bm || (rm == bm && rd < bd)) {
        age -= 1;
    }
    return age;
}

} // anonymous namespace

extern "C" AGE_API int verify_age_21(int day, int month, int year) {
    if (!isValidDate(day, month, year)) {
        return AGE_VERIFIER_BAD_INPUT;
    }

    std::time_t now = std::time(0);
    std::tm*    lt  = std::localtime(&now);
    if (lt == 0) {
        return AGE_VERIFIER_BAD_INPUT;
    }

    int curDay   = lt->tm_mday;
    int curMonth = lt->tm_mon + 1;
    int curYear  = lt->tm_year + 1900;

    int age = yearsBetween(day, month, year, curDay, curMonth, curYear);
    return age >= 21 ? AGE_VERIFIER_OK : AGE_VERIFIER_UNDER;
}
