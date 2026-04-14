# Bank Account Management System Makefile

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++03 -Wall -Wextra -pedantic -I./include
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

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
          $(SRC_DIR)/Bank.cpp

# Object files (placed in BUILD_DIR)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable name
TARGET = BankSystem

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files in BUILD_DIR
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean