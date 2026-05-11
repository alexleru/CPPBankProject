/* age_verifier.h -- public C ABI of the cross-platform age-verification library.
 *
 * The same header is consumed by:
 *   - the library translation unit itself (with AGE_VERIFIER_BUILD defined),
 *   - the host application that loads the library dynamically via
 *     LoadLibrary / dlopen.
 *
 * The exported symbol uses C linkage and the platform default calling
 * convention so it can be located by GetProcAddress / dlsym without name
 * mangling.
 */
#ifndef AGE_VERIFIER_H
#define AGE_VERIFIER_H

#if defined(_WIN32)
    #if defined(AGE_VERIFIER_BUILD)
        #define AGE_API __declspec(dllexport)
    #else
        #define AGE_API __declspec(dllimport)
    #endif
#else
    #if defined(AGE_VERIFIER_BUILD)
        #define AGE_API __attribute__((visibility("default")))
    #else
        #define AGE_API
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Return codes for verify_age_21. */
#define AGE_VERIFIER_UNDER       0   /* age < 21 */
#define AGE_VERIFIER_OK          1   /* age >= 21 */
#define AGE_VERIFIER_BAD_INPUT  -1   /* day/month/year out of range */

/* Compare (day, month, year) with the current system date and return:
 *   AGE_VERIFIER_OK         if the resulting age is >= 21,
 *   AGE_VERIFIER_UNDER      if the resulting age is < 21,
 *   AGE_VERIFIER_BAD_INPUT  if the date is not a real calendar date.
 */
AGE_API int verify_age_21(int day, int month, int year);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AGE_VERIFIER_H */
