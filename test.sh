#!/bin/bash

# Script test tự động cho CPU Scheduling Algorithms
# Màu sắc
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "════════════════════════════════════════════════"
echo "  CPU Scheduling Algorithms - Auto Test Suite"
echo "════════════════════════════════════════════════"

# Tạo thư mục results nếu chưa có
mkdir -p results

# Kiểm tra xem đã compile chưa
if [ ! -f "./scheduler" ]; then
    echo -e "${YELLOW}⚠ Chưa compile. Đang build...${NC}"
    make
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Build failed!${NC}"
        exit 1
    fi
    echo -e "${GREEN}✓ Build successful${NC}"
fi

# Test với từng file
test_count=0
pass_count=0

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Running Tests"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

for testfile in data/test_*.txt; do
    if [ -f "$testfile" ]; then
        test_count=$((test_count + 1))
        filename=$(basename "$testfile" .txt)
        output="results/${filename}_result.txt"
        
        echo ""
        echo -e "${BLUE}▶ Testing: $filename${NC}"
        
        # Chạy test với input tự động
        # Input: 3 (đọc từ file) -> tên file -> 7 (so sánh tất cả) -> 8 (bật export) -> output file -> 0 (thoát)
        echo -e "3\n$testfile\n8\n$output\n7\n0" | ./scheduler > /dev/null 2>&1
        
        if [ -f "$output" ]; then
            echo -e "  ${GREEN}✓ Test passed${NC}"
            echo -e "  ${GREEN}→ Output: $output${NC}"
            
            # Đếm số dòng trong output
            lines=$(wc -l < "$output")
            echo -e "  ℹ Size: $lines lines"
            
            pass_count=$((pass_count + 1))
        else
            echo -e "  ${RED}✗ Test failed - No output file${NC}"
        fi
    fi
done

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Test Summary"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo -e "Total tests: $test_count"
echo -e "${GREEN}Passed: $pass_count${NC}"
echo -e "${RED}Failed: $((test_count - pass_count))${NC}"

if [ $pass_count -eq $test_count ]; then
    echo -e "\n${GREEN}🎉 All tests passed!${NC}"
else
    echo -e "\n${YELLOW}⚠ Some tests failed${NC}"
fi

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Generated Files"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
ls -lh results/*.txt 2>/dev/null || echo "No files generated"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  View Results"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "Use: cat results/test_simple_result.txt"
echo "Or:  less results/test_convoy_result.txt"
echo ""
