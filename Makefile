# Mostly from makefiletutorial.com
EXEC := raytracer
CFLAGS := --std=c++17 -O3 -Wall -Wextra -Wunused -Wshadow -pedantic -Wwrite-strings -Wunused-value
ARGS := ""
CC := "clang++"

BUILD_DIR := ./build
SRC_DIRS := ./src
INCLUDE_DIRS := ./inc

# Find C files to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')

# String substitution for every C file.
# For example, hello.c turns into ./build/hello.o
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

# The final build step.
$(EXEC): $(OBJS)
	$(CC) -I $(INCLUDE_DIRS) $(CFLAGS) $(OBJS) -o $@

# Build step for C source
$(BUILD_DIR)/%.o: $(SRCS)
	mkdir -p $(dir $@)
	$(CC) -I $(INCLUDE_DIRS) $(CFLAGS) -c $< -o $@

.PHONY: clean run
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

setup:
	bear -- make

