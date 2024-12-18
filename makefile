# Directories
SRC_DIR = src
BUILD_DIR = build/debug
INCLUDE_DIR = include
LIB_DIR = /usr/local/Cellar  # Adjust to correct library path

# Compiler and flags
CC = g++
CXXFLAGS = -std=c++17 -Wall -g  -fsanitize=address
INCLUDE_PATHS = -I$(INCLUDE_DIR) -I/usr/local/Cellar/sdl2/2.26.4/include/SDL2 \
				-I/usr/local/Cellar/sdl2_image/2.8.2_1/include/SDL2 \
				-I/usr/local/Cellar/cpp/FastNoiseLite

LIBRARY_PATHS = -L/usr/local/Cellar/sdl2/2.26.4/lib -L/usr/local/Cellar/sdl2_image/2.8.2_1/lib
LIBS = -lSDL2_image -lSDL2

# Source and object files
SRC_FILES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRC_FILES))

# Output binary
OBJ_NAME = $(BUILD_DIR)/play

# Targets
all: $(OBJ_NAME)

# Link the executable
$(OBJ_NAME): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_FILES) -o $(OBJ_NAME) $(LIBRARY_PATHS) $(LIBS) -fsanitize=address

# Compile source files to object files and generate dependencies
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) $(INCLUDE_PATHS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEP_FILES)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
