# Project Name (generate executable with this name)
TARGET = json-reader

# Compiler
CC = g++

# Folders
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Flags, Libraries and Includes
CFLAGS = -g -Wall -I$(INC_DIR) -std=c++17
LDFLAGS = 
LIBS = 

# Code Lists
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default make
all: build $(BIN_DIR)/$(TARGET)

# Build project
# Create object files directory
# Create binary directory
# Link object files into a binary
build:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean objects in object directory and the binary in bin directory
clean:
	@rm -rf $(OBJ_DIR)/*
	@rm -rf $(BIN_DIR)/$(TARGET)

# Non-File Targets
.PHONY: all build clean
