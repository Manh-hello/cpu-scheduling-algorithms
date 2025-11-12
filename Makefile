# Makefile for CPU Scheduling - BSD Make
CC = gcc
CFLAGS = -Wall -Wextra -I./include
TARGET = scheduler

all: obj_dir $(TARGET)
@echo "Build successful: $(TARGET)"

obj_dir:
@mkdir -p obj
@mkdir -p results

$(TARGET): obj/main.o obj/fcfs.o obj/sjf.o obj/srtf.o obj/rr.o obj/priority.o obj/utils.o obj/export.o
@echo "Linking..."
$(CC) $(CFLAGS) -o $(TARGET) obj/main.o obj/fcfs.o obj/sjf.o obj/srtf.o obj/rr.o obj/priority.o obj/utils.o obj/export.o

obj/main.o: src/main.c
$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/fcfs.o: src/fcfs.c
$(CC) $(CFLAGS) -c src/fcfs.c -o obj/fcfs.o

obj/sjf.o: src/sjf.c
$(CC) $(CFLAGS) -c src/sjf.c -o obj/sjf.o

obj/srtf.o: src/srtf.c
$(CC) $(CFLAGS) -c src/srtf.c -o obj/srtf.o

obj/rr.o: src/rr.c
$(CC) $(CFLAGS) -c src/rr.c -o obj/rr.o

obj/priority.o: src/priority.c
$(CC) $(CFLAGS) -c src/priority.c -o obj/priority.o

obj/utils.o: src/utils.c
$(CC) $(CFLAGS) -c src/utils.c -o obj/utils.o

obj/export.o: src/export.c
$(CC) $(CFLAGS) -c src/export.c -o obj/export.o

clean:
rm -rf obj/*.o $(TARGET)

cleanall: clean
rm -rf results/*.txt

run: $(TARGET)
./$(TARGET)

.PHONY: all clean cleanall run obj_dir
