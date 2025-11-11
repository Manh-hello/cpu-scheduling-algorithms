# Makefile for CPU Scheduling Simulator
# NetBSD - System Programming

CC = cc
CFLAGS = -Wall -Wextra -O2 -I./include
LDFLAGS = 

SRC_DIR = src
INC_DIR = include
DATA_DIR = data
BUILD_DIR = build
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TARGET = $(BIN_DIR)/cpu_scheduler

# Colors
GREEN = \033[0;32m
YELLOW = \033[1;33m
RED = \033[0;31m
NC = \033[0m

.PHONY: all
all: directories $(TARGET)
	@echo "$(GREEN)✓ Build completed!$(NC)"
	@echo "Run: $(YELLOW)./$(TARGET)$(NC)"

.PHONY: directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)Linking...$(NC)"
	@$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "$(GREEN)✓ Created: $(TARGET)$(NC)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/scheduler.h
	@echo "$(YELLOW)Compiling: $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run
run: all
	@./$(TARGET)

.PHONY: test
test: all
	@echo "$(GREEN)=== Testing with processes.txt ===$(NC)"
	@echo "3\n$(DATA_DIR)/processes.txt\n7\n0" | ./$(TARGET) || true

.PHONY: clean
clean:
	@echo "$(RED)Cleaning...$(NC)"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "$(GREEN)✓ Cleaned!$(NC)"

.PHONY: rebuild
rebuild: clean all

.PHONY: help
help:
	@echo "$(YELLOW)╔════════════════════════════════════╗$(NC)"
	@echo "$(YELLOW)║  CPU Scheduler - NetBSD           ║$(NC)"
	@echo "$(YELLOW)╚════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "Available targets:"
	@echo "  $(GREEN)make$(NC)        - Build project"
	@echo "  $(GREEN)make run$(NC)    - Build and run"
	@echo "  $(GREEN)make test$(NC)   - Quick test"
	@echo "  $(GREEN)make clean$(NC)  - Remove build files"
	@echo "  $(GREEN)make rebuild$(NC) - Clean and build"
	@echo "  $(GREEN)make help$(NC)   - Show this help"
	@echo ""

.PHONY: info
info:
	@echo "$(YELLOW)Project Structure:$(NC)"
	@echo "  Sources: $(words $(SOURCES)) files"
	@echo "  Objects: $(words $(OBJECTS)) files"
	@echo "  Target:  $(TARGET)"
	@echo ""
	@echo "$(YELLOW)Source files:$(NC)"
	@ls -1 $(SRC_DIR)/*.c | sed 's|^|  - |'

.PHONY: show-data
show-data:
	@echo "$(YELLOW)Data Files:$(NC)"
	@ls -lh $(DATA_DIR)/*.txt

.PHONY: check
check:
	@echo "$(YELLOW)Checking project structure...$(NC)"
	@test -d $(INC_DIR) && echo "$(GREEN)✓$(NC) include/" || echo "$(RED)✗$(NC) include/"
	@test -d $(SRC_DIR) && echo "$(GREEN)✓$(NC) src/" || echo "$(RED)✗$(NC) src/"
	@test -d $(DATA_DIR) && echo "$(GREEN)✓$(NC) data/" || echo "$(RED)✗$(NC) data/"
	@test -f $(INC_DIR)/scheduler.h && echo "$(GREEN)✓$(NC) scheduler.h" || echo "$(RED)✗$(NC) scheduler.h"
	@test -f $(SRC_DIR)/main.c && echo "$(GREEN)✓$(NC) main.c" || echo "$(RED)✗$(NC) main.c"
	@echo ""
	@echo "$(YELLOW)Data files:$(NC)"
	@test -f $(DATA_DIR)/processes.txt && echo "$(GREEN)✓$(NC) processes.txt" || echo "$(RED)✗$(NC) processes.txt"
	@test -f $(DATA_DIR)/test_convoy.txt && echo "$(GREEN)✓$(NC) test_convoy.txt" || echo "$(RED)✗$(NC) test_convoy.txt"
	@test -f $(DATA_DIR)/test_starvation.txt && echo "$(GREEN)✓$(NC) test_starvation.txt" || echo "$(RED)✗$(NC) test_starvation.txt"
	@test -f $(DATA_DIR)/test_roundrobin.txt && echo "$(GREEN)✓$(NC) test_roundrobin.txt" || echo "$(RED)✗$(NC) test_roundrobin.txt"
	@test -f $(DATA_DIR)/test_mixed.txt && echo "$(GREEN)✓$(NC) test_mixed.txt" || echo "$(RED)✗$(NC) test_mixed.txt"
