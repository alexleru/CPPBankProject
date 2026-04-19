# Bank Account Management System Makefile
# Works on Linux / macOS and on Windows (MinGW64, cmd.exe or PowerShell).

# Compiler settings
CXX      = g++
CXXFLAGS = -std=c++03 -I./include
LDFLAGS  =

# Directories
SRC_DIR   = src
INC_DIR   = include
BUILD_DIR = build

# ---------------------------------------------------------------------------
# Platform detection
#   On Windows, MinGW's make exposes the OS variable as "Windows_NT".
#   We pick the right shell commands and executable extension accordingly.
# ---------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    EXE_EXT  := .exe
    BUILD_DIR_WIN := $(subst /,\,$(BUILD_DIR))
    MKDIR_CMD := if not exist $(BUILD_DIR_WIN) mkdir $(BUILD_DIR_WIN)
else
    EXE_EXT  :=
endif

# Source files
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Utils.cpp \
          $(SRC_DIR)/Customer.cpp \
          $(SRC_DIR)/Transaction.cpp \
          $(SRC_DIR)/Account.cpp \
          $(SRC_DIR)/SavingsAccount.cpp \
          $(SRC_DIR)/CheckingAccount.cpp \
          $(SRC_DIR)/LoanAccount.cpp \
          $(SRC_DIR)/Globals.cpp \
          $(SRC_DIR)/Bank.cpp \
          $(SRC_DIR)/BondCalculator.cpp

# Object files (placed in BUILD_DIR)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable name (adds .exe on Windows so PowerShell finds it)
TARGET = BankSystem$(EXE_EXT)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory (platform-aware)
$(BUILD_DIR):
ifeq ($(OS),Windows_NT)
	@cmd /C "$(MKDIR_CMD)"
else
	mkdir -p $(BUILD_DIR)
endif

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files in BUILD_DIR
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts (platform-aware)
clean:
ifeq ($(OS),Windows_NT)
	-@if exist $(BUILD_DIR_WIN) cmd /C "rmdir /S /Q $(BUILD_DIR_WIN)"
	-@if exist $(TARGET)        cmd /C "del /Q $(TARGET)"
else
	rm -rf $(BUILD_DIR) $(TARGET)
endif

# Phony targets
.PHONY: all clean
