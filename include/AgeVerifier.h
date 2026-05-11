#ifndef AGE_VERIFIER_WRAPPER_H
#define AGE_VERIFIER_WRAPPER_H

#include <string>

// RAII wrapper around the cross-platform age_verifier native library.
//
// On construction the wrapper loads the platform-appropriate binary
// (native/windows/age_verifier.dll on _WIN32, native/linux/libage_verifier.so
// on __linux__) via LoadLibrary / dlopen, and resolves the verify_age_21
// symbol. Use isLoaded() to check whether the library is usable before
// calling verify().
class AgeVerifier {
public:
    enum Result {
        AGE_OK         =  1,   // age >= 21
        AGE_UNDER      =  0,   // age <  21
        AGE_BAD_INPUT  = -1,   // invalid date
        AGE_LIB_ERROR  = -2    // native library not loaded / symbol missing
    };

    AgeVerifier();
    ~AgeVerifier();

    bool        isLoaded()   const;
    std::string libraryPath() const;
    std::string lastError()  const;

    Result verify(int day, int month, int year) const;

private:
    // C++03: disable copying explicitly.
    AgeVerifier(const AgeVerifier&);
    AgeVerifier& operator=(const AgeVerifier&);

    void*       handle;     // HMODULE on Windows, void* on Linux
    void*       symbol;     // resolved function pointer
    std::string libPath;
    std::string error;
};

#endif // AGE_VERIFIER_WRAPPER_H
