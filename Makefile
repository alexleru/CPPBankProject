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
          $(SRC_DIR)/Bank.cpp

# Object files (placed in BUILD_DIR)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable base name (OS-specific extension is appended below)
TARGET_NAME = BankSystem

# ---------------------------------------------------------------
# OS detection: on Windows cmd.exe the env var OS = Windows_NT
# ---------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    EXE_EXT := .exe
    RM_DIR  := if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
    RM_FILE := if exist $(TARGET_NAME).exe del /Q $(TARGET_NAME).exe
    MKDIR   := if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
else
    EXE_EXT :=
    RM_DIR  := rm -rf $(BUILD_DIR)
    RM_FILE := rm -f $(TARGET_NAME)
    MKDIR   := mkdir -p $(BUILD_DIR)
endif

TARGET = $(TARGET_NAME)$(EXE_EXT)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	$(MKDIR)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files in BUILD_DIR
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	$(RM_DIR)
	$(RM_FILE)

# Phony targets
.PHONY: all clean
