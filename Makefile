
TARGET = CALENDAR
SRC_DIR = src
BUILD_DIR = build

CC = gcc
CFLAGS = -Wall -Wextra -I$(SRC_DIR) # includes all headers and does error checking

# grab all .c files in path then generates their obj files
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# default build
all: $(BUILD_DIR) $(TARGET)

# rule to create new build dirs
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# links all obj to final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

# creates all subdir object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean: 
	@rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Cleaned up build directory and executables"

