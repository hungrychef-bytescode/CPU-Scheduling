# Makefile for schedsim CMSC 125 Laboratory Assignment 2

# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -pedantic -g -D_POSIX_C_SOURCE=200809L -Iinclude
LDFLAGS =

# Target executable
TARGET = schedsim

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Source and header files
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/parser.c \
#           $(SRC_DIR)/utils.c \
#           $(SRC_DIR)/fcfs.c \
#           $(SRC_DIR)/sjf.c \
#           $(SRC_DIR)/gantt.c \
#           $(SRC_DIR)/mlfq.c \
#           $(SRC_DIR)/metrics.c \
#           $(SRC_DIR)/stcf.c \
#           $(SRC_DIR)/rr.c

HEADERS = $(INC_DIR)/process.h \
		  $(INC_DIR)/scheduler.h \
		  $(INC_DIR)/parser.h

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# ── Default target ────────────────────────────────────────────────────────────
all: $(OBJ_DIR) $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build successful! Run with ./$(TARGET) --algorithm=FCFS --input=tests/workload.txt"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Create obj/ directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# ── Convenience targets ───────────────────────────────────────────────────────
run: all
	./$(TARGET) --algorithm=FCFS --input=tests/workload.txt

compare: all
	./$(TARGET) --compare --input=tests/workload.txt

# ── Clean ─────────────────────────────────────────────────────────────────────
clean:
	rm -f $(TARGET) $(OBJECTS)
	rm -rf $(OBJ_DIR)
	@echo "Clean complete"

# Rebuild everything from scratch
rebuild: clean all

# ── Help ──────────────────────────────────────────────────────────────────────
help:
	@echo "Makefile for schedsim - CPU Scheduling Simulator"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Compile the simulator (default)"
	@echo "  clean    - Remove binaries and object files"
	@echo "  rebuild  - Clean and rebuild from scratch"
	@echo "  run      - Build and run FCFS on tests/workload.txt"
	@echo "  compare  - Build and run all algorithms"
	@echo "  help     - Display this help message"

# ── Phony targets ─────────────────────────────────────────────────────────────
.PHONY: all clean rebuild run compare help