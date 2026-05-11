# Bank Account Management System Makefile
# Works with GNU make (Linux/macOS) and mingw32-make (Windows cmd.exe)

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++03 -I./include
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source files
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Utils.cpp \
          $(SRC_DIR)/Customer.cpp \
          $(SRC_DIR)/Globals.cpp \
          $(SRC_DIR)/Bank.cpp \
          $(SRC_DIR)/AgeVerifier.cpp

# Object files (placed in BUILD_DIR)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable base name (OS-specific extension is appended below)
TARGET_NAME = BankSystem

# Native library settings
NATIVE_SRC      = native/src/age_verifier.cpp
NATIVE_FLAGS    = -std=c++03 -O2 -Wall -shared

# ---------------------------------------------------------------
# OS detection: on Windows cmd.exe the env var OS = Windows_NT
# ---------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    EXE_EXT       := .exe
    RM_DIR        := if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
    RM_FILE       := if exist $(TARGET_NAME).exe del /Q $(TARGET_NAME).exe
    MKDIR         := if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    NATIVE_LIB    := native/windows/age_verifier.dll
    NATIVE_DIR    := native\windows
    MKDIR_NATIVE  := if not exist $(NATIVE_DIR) mkdir $(NATIVE_DIR)
    RM_NATIVE     := if exist $(NATIVE_LIB) del /Q $(subst /,\,$(NATIVE_LIB))
    DL_LIBS       :=
    NATIVE_PIC    :=
else
    EXE_EXT       :=
    RM_DIR        := rm -rf $(BUILD_DIR)
    RM_FILE       := rm -f $(TARGET_NAME)
    MKDIR         := mkdir -p $(BUILD_DIR)
    NATIVE_LIB    := native/linux/libage_verifier.so
    NATIVE_DIR    := native/linux
    MKDIR_NATIVE  := mkdir -p $(NATIVE_DIR)
    RM_NATIVE     := rm -f $(NATIVE_LIB)
    DL_LIBS       := -ldl
    NATIVE_PIC    := -fPIC
endif

TARGET   = $(TARGET_NAME)$(EXE_EXT)
LDFLAGS += $(DL_LIBS)

# Default target: build the native library and the application.
all: $(BUILD_DIR) native $(TARGET)

# Create build directory
$(BUILD_DIR):
	$(MKDIR)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files in BUILD_DIR
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the platform-appropriate native library.
native: $(NATIVE_LIB)

$(NATIVE_LIB): $(NATIVE_SRC) native/include/age_verifier.h
	$(MKDIR_NATIVE)
	$(CXX) $(NATIVE_FLAGS) $(NATIVE_PIC) $(NATIVE_SRC) -o $(NATIVE_LIB)

# Clean build artifacts (does NOT delete the native library by default,
# since it is treated as a shipped binary; use `clean-all` for that).
clean:
	$(RM_DIR)
	$(RM_FILE)

clean-all: clean
	$(RM_NATIVE)

# Phony targets
.PHONY: all native clean clean-all
