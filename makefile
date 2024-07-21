# Directories
SRC_DIR = src
BUILD_DIR = build/debug
INCLUDE_DIR = include
LIB_DIR = lib

# Compiler and flags
CC = g++
CXXFLAGS = -std=c++17 -Wall -g -O0
INCLUDE_PATHS = -I$(INCLUDE_DIR)
LIBRARY_PATHS = -L$(LIB_DIR)
LIBS = -lSDL2

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Output binary
OBJ_NAME = $(BUILD_DIR)/play

# Targets
all: $(OBJ_NAME)

# Link the executable
$(OBJ_NAME): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_FILES) -o $(OBJ_NAME) $(LIBRARY_PATHS) $(LIBS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CXXFLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
