#include "../include/AgeVerifier.h"

#include <sstream>

// Platform-specific dynamic-loader API. The application picks the correct
// native binary at compile time, so the host code stays the same on both
// operating systems.
#if defined(_WIN32)
    #include <windows.h>
    static const char* DEFAULT_LIB_PATH = "native\\windows\\age_verifier.dll";
#elif defined(__linux__)
    #include <dlfcn.h>
    static const char* DEFAULT_LIB_PATH = "native/linux/libage_verifier.so";
#else
    #error "AgeVerifier: only Windows (_WIN32) and Linux (__linux__) are supported."
#endif

namespace {
    // Mirrors the native ABI: int verify_age_21(int, int, int)
    typedef int (*VerifyFn)(int, int, int);
    const char* const SYMBOL_NAME = "verify_age_21";
}

AgeVerifier::AgeVerifier()
    : handle(0), symbol(0), libPath(DEFAULT_LIB_PATH) {

#if defined(_WIN32)
    HMODULE h = LoadLibraryA(libPath.c_str());
    if (h == 0) {
        std::ostringstream oss;
        oss << "LoadLibrary('" << libPath << "') failed (error "
            << GetLastError() << ").";
        error = oss.str();
        return;
    }
    FARPROC p = GetProcAddress(h, SYMBOL_NAME);
    if (p == 0) {
        std::ostringstream oss;
        oss << "GetProcAddress('" << SYMBOL_NAME << "') failed (error "
            << GetLastError() << ").";
        error = oss.str();
        FreeLibrary(h);
        return;
    }
    handle = static_cast<void*>(h);
    symbol = reinterpret_cast<void*>(p);

#else // __linux__
    void* h = dlopen(libPath.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (h == 0) {
        const char* msg = dlerror();
        std::ostringstream oss;
        oss << "dlopen('" << libPath << "') failed: "
            << (msg ? msg : "unknown error");
        error = oss.str();
        return;
    }
    dlerror(); // clear pending error before dlsym
    void* p = dlsym(h, SYMBOL_NAME);
    const char* derr = dlerror();
    if (derr != 0) {
        std::ostringstream oss;
        oss << "dlsym('" << SYMBOL_NAME << "') failed: " << derr;
        error = oss.str();
        dlclose(h);
        return;
    }
    handle = h;
    symbol = p;
#endif
}

AgeVerifier::~AgeVerifier() {
    if (handle == 0) return;
#if defined(_WIN32)
    FreeLibrary(static_cast<HMODULE>(handle));
#else
    dlclose(handle);
#endif
}

bool        AgeVerifier::isLoaded()    const { return symbol != 0; }
std::string AgeVerifier::libraryPath() const { return libPath; }
std::string AgeVerifier::lastError()   const { return error; }

AgeVerifier::Result AgeVerifier::verify(int day, int month, int year) const {
    if (symbol == 0) return AGE_LIB_ERROR;
    VerifyFn fn = reinterpret_cast<VerifyFn>(symbol);
    int r = fn(day, month, year);
    switch (r) {
        case  1: return AGE_OK;
        case  0: return AGE_UNDER;
        case -1: return AGE_BAD_INPUT;
        default: return AGE_LIB_ERROR;
    }
}
