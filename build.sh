#!/bin/sh
# Build script for CPU Scheduling Algorithms - NetBSD

echo "═══════════════════════════════════════════════"
echo "  Building CPU Scheduling Simulator"
echo "═══════════════════════════════════════════════"
echo ""

# Create directories
mkdir -p obj results

# Compiler settings
CC="gcc"
CFLAGS="-Wall -Wextra -I./include"

echo "Compiling source files..."
echo ""

# Compile each source file
compile_file() {
    file=$1
    obj=$2
    num=$3
    echo "  [$num/8] Compiling $file..."
    $CC $CFLAGS -c src/$file -o obj/$obj || exit 1
}

compile_file "main.c" "main.o" 1
compile_file "fcfs.c" "fcfs.o" 2
compile_file "sjf.c" "sjf.o" 3
compile_file "srtf.c" "srtf.o" 4
compile_file "rr.c" "rr.o" 5
compile_file "priority.c" "priority.o" 6
compile_file "utils.c" "utils.o" 7
compile_file "export.c" "export.o" 8

echo ""
echo "Linking..."
$CC $CFLAGS -o scheduler obj/main.o obj/fcfs.o obj/sjf.o obj/srtf.o obj/rr.o obj/priority.o obj/utils.o obj/export.o || exit 1

echo ""
echo "═══════════════════════════════════════════════"
echo "✓ Build successful!"
echo "═══════════════════════════════════════════════"
echo ""
echo "Run with: ./scheduler"
echo ""
