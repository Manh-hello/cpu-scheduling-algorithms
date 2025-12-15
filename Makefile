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
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Main target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(RESULTSDIR)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "✓ Build successful: $(TARGET)"

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "✓ Cleaned build files"

# Clean all including results
cleanall: clean
	rm -rf $(RESULTSDIR)/*.txt
	@echo "✓ Cleaned all files"

# Run with sample data
run: $(TARGET)
	./$(TARGET)

# Run tests
test: $(TARGET)
	@echo "Running tests..."
	@for file in $(DATADIR)/test_*.txt; do \
		echo "\n=== Testing with $$file ==="; \
		echo "1\n3\n$$file\n7\n8\nresults/$$(basename $$file .txt)_result.txt\n0" | ./$(TARGET); \
	done
	@echo "\n✓ All tests completed. Check results/ folder"

# Help
help:
	@echo "CPU Scheduling Algorithms - Makefile"
	@echo ""
	@echo "Usage:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run"
	@echo "  make test     - Run all test files"
	@echo "  make clean    - Remove build files"
	@echo "  make cleanall - Remove build and result files"
	@echo "  make help     - Show this help"

.PHONY: all clean cleanall run test help directories
