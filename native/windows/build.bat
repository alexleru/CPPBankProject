@echo off
REM Build age_verifier.dll on Windows (MinGW / MSYS2 g++).
REM Run from the project root: native\windows\build.bat

setlocal
set SCRIPT_DIR=%~dp0
set NATIVE_ROOT=%SCRIPT_DIR%..
set SRC=%NATIVE_ROOT%\src\age_verifier.cpp
set OUT=%SCRIPT_DIR%age_verifier.dll

g++ -std=c++03 -O2 -Wall -shared -o "%OUT%" "%SRC%"
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)
echo Built %OUT%
endlocal
