# Makefile for CPU Scheduling Algorithms
# NetBSD System Programming

CC = gcc
CFLAGS = -Wall -Wextra -I./include
TARGET = scheduler
SRCDIR = src
OBJDIR = obj
INCDIR = include
DATADIR = data
RESULTSDIR = results

# Source files
SRCS = $(SRCDIR)/main.c $(SRCDIR)/fcfs.c $(SRCDIR)/sjf.c $(SRCDIR)/srtf.c \
       $(SRCDIR)/rr.c $(SRCDIR)/priority.c $(SRCDIR)/utils.c $(SRCDIR)/export.c

OBJS = $(OBJDIR)/main.o $(OBJDIR)/fcfs.o $(OBJDIR)/sjf.o $(OBJDIR)/srtf.o \
       $(OBJDIR)/rr.o $(OBJDIR)/priority.o $(OBJDIR)/utils.o $(OBJDIR)/export.o

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[1;33m
BLUE = \033[0;34m
NC = \033[0m

# Main target
all: directories $(TARGET)
	@echo "$(GREEN)✓ Build successful: $(TARGET)$(NC)"

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(RESULTSDIR)

# Link object files to create executable
$(TARGET): $(OBJS)
	@echo "$(YELLOW)Linking...$(NC)"
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "$(BLUE)Compiling $<...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	@echo "$(RED)Cleaning...$(NC)"
	@rm -rf $(OBJDIR) $(TARGET)
	@echo "$(GREEN)✓ Cleaned!$(NC)"

# Clean all including results
cleanall: clean
	@rm -rf $(RESULTSDIR)/*.txt
	@echo "$(GREEN)✓ Cleaned all files$(NC)"

# Run with sample data
run: $(TARGET)
	./$(TARGET)

# Run with default processes.txt
runfile: $(TARGET)
	@echo "3" | ./$(TARGET)

# Run tests
test: $(TARGET)
	@echo "$(BLUE)Running tests...$(NC)"
	@for file in $(DATADIR)/test_*.txt; do \
		if [ -f "$$file" ]; then \
			echo ""; \
			echo "$(YELLOW)=== Testing with $$(basename $$file) ===$(NC)"; \
			base=$$(basename $$file .txt); \
			echo "3\n$$file\n8\nresults/$${base}_result.txt\n7\n0" | ./$(TARGET) > /dev/null 2>&1; \
			if [ -f "results/$${base}_result.txt" ]; then \
				echo "$(GREEN)✓ Test passed: results/$${base}_result.txt$(NC)"; \
			else \
				echo "$(RED)✗ Test failed$(NC)"; \
			fi; \
		fi; \
	done
	@echo ""
	@echo "$(GREEN)✓ All tests completed$(NC)"

# Help
help:
	@echo "CPU Scheduling Algorithms - Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run interactively"
	@echo "  make runfile  - Run with data/processes.txt"
	@echo "  make test     - Run all test files"
	@echo "  make clean    - Remove build files"
	@echo "  make cleanall - Remove build and result files"
	@echo "  make help     - Show this help"

.PHONY: all clean cleanall run runfile test help directories
