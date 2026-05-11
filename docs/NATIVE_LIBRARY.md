# Native Library: `age_verifier`

A tiny cross-platform native library that validates whether a person is at
least 21 years old, given a birth date in numeric `day/month/year` format.

The application loads the platform-appropriate binary at runtime through
`LoadLibrary` (Windows) or `dlopen` (Linux). The host code and the library
share **the same C++ source** for the business logic — only the export macro
differs per platform, declared via `_WIN32` / `__linux__` in
[../native/include/age_verifier.h](../native/include/age_verifier.h).

## Layout

```
native/
  include/age_verifier.h    Public C ABI (extern "C")
  src/age_verifier.cpp      Shared implementation (date math + system time)
  windows/                  Prebuilt age_verifier.dll lives here at runtime
    build.bat               Windows build script (MinGW / MSYS2 g++)
  linux/                    Prebuilt libage_verifier.so lives here at runtime
    build.sh                Linux build script
```

The host wrapper that does the dynamic loading is in
[../include/AgeVerifier.h](../include/AgeVerifier.h) and
[../src/AgeVerifier.cpp](../src/AgeVerifier.cpp).

## Public ABI

```c
int verify_age_21(int day, int month, int year);
//  1  -> age >= 21
//  0  -> age <  21
// -1  -> invalid date (out-of-range day/month/year, non-leap Feb 29, ...)
```

The symbol is exported with C linkage and the platform default calling
convention, so it is resolvable directly via `GetProcAddress` / `dlsym`
without name mangling.

## Building

### Via the top-level Makefile (recommended)

```bash
make native    # builds only the native library for the current OS
make           # builds the native library AND the application
```

The Makefile picks `native/windows/age_verifier.dll` on Windows
(`OS=Windows_NT`) and `native/linux/libage_verifier.so` everywhere else.

### Via the standalone scripts

```bash
# Windows (cmd.exe / PowerShell; requires g++ in PATH)
native\windows\build.bat

# Linux
chmod +x native/linux/build.sh
./native/linux/build.sh
```

Both scripts invoke `g++` with `-std=c++03 -O2 -Wall -shared` (plus `-fPIC`
on Linux). Any C++03 compiler works; on Windows MSVC produces an equivalent
DLL with `cl /LD native\src\age_verifier.cpp /Fenative\windows\age_verifier.dll`.

## How the application picks a binary

`src/AgeVerifier.cpp` uses preprocessor detection at compile time:

```cpp
#if defined(_WIN32)
    static const char* DEFAULT_LIB_PATH = "native\\windows\\age_verifier.dll";
#elif defined(__linux__)
    static const char* DEFAULT_LIB_PATH = "native/linux/libage_verifier.so";
#endif
```

The path is relative to the working directory of the application (the
project root, by convention). On Linux, link the application with `-ldl`
so `dlopen`/`dlsym` are available; the top-level Makefile does this
automatically.

## Trying it from the menu

After `make`, run the application from the project root:

```bash
./BankSystem        # Linux
BankSystem.exe      # Windows
```

Pick **option 4 -- Verify Age (21+, native library)** and enter a birth
date as three numbers. The program prints `TRUE` / `FALSE` based on the
native library's result, or a diagnostic if the library could not be
loaded.

## Extending the library

The structure is intentionally minimal so new native features can be added
without churn:

1. Add another `extern "C"` function in `native/src/age_verifier.cpp` (or a
   sibling translation unit). Declare it in `native/include/age_verifier.h`.
2. Add a `dlsym`/`GetProcAddress` lookup for the new symbol in the host
   wrapper, and a typed function-pointer typedef.
3. Surface it through a new method on `AgeVerifier` (or a new wrapper
   class for an unrelated feature).
