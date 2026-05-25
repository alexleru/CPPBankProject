# Bank Account Management System Makefile
# Works on Linux / macOS and on Windows (MinGW64, cmd.exe or PowerShell).

# Compiler settings
CXX      = g++
CXXFLAGS = -std=c++03 -Wall -Wextra -I./include
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
          $(SRC_DIR)/Globals.cpp \
          $(SRC_DIR)/BondCalculator.cpp \
          $(SRC_DIR)/Transaction.cpp \
          $(SRC_DIR)/Account.cpp \
          $(SRC_DIR)/TransactionVisitor.cpp \
          $(SRC_DIR)/Deposit.cpp \
          $(SRC_DIR)/Withdrawal.cpp \
          $(SRC_DIR)/Transfer.cpp \
          $(SRC_DIR)/LoanPayment.cpp \
          $(SRC_DIR)/LoggingVisitor.cpp \
          $(SRC_DIR)/Bank.cpp \
          $(SRC_DIR)/Customer.cpp \
          $(SRC_DIR)/Loan.cpp \
          $(SRC_DIR)/AuditLogger.cpp \
          $(SRC_DIR)/NotificationCenter.cpp \
          $(SRC_DIR)/BranchManager.cpp \
          $(SRC_DIR)/RiskAnalyzer.cpp \
          $(SRC_DIR)/ReportEngine.cpp \
          $(SRC_DIR)/ReportFilter.cpp \
          $(SRC_DIR)/ReportSection.cpp \
          $(SRC_DIR)/ReportFormatter.cpp \
          $(SRC_DIR)/ReportWriter.cpp

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
