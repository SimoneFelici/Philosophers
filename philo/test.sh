#!/bin/bash

# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

echo "🧪 Testing Philosophers..."
echo "========================="

# Compila il progetto
echo "Compiling..."
make re
if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Compilation failed${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Compilation successful${NC}"

# Test 1: Morte precisa
echo -e "\n${YELLOW}Test 1: Precise death timing${NC}"
timeout 5s ./philo 2 310 200 100 > test1.out 2>&1
if grep -q "died" test1.out; then
    death_time=$(grep "died" test1.out | awk '{print $1}')
    if [ "$death_time" -le 320 ] && [ "$death_time" -ge 300 ]; then
        echo -e "${GREEN}✅ Death timing correct: ${death_time}ms${NC}"
    else
        echo -e "${RED}❌ Death timing incorrect: ${death_time}ms (should be ~310ms)${NC}"
    fi
else
    echo -e "${RED}❌ No death detected${NC}"
fi

# Test 2: Single philosopher
echo -e "\n${YELLOW}Test 2: Single philosopher${NC}"
timeout 2s ./philo 1 800 200 200 > test2.out 2>&1
if grep -q "died" test2.out && grep -q "has taken a fork" test2.out; then
    echo -e "${GREEN}✅ Single philosopher test passed${NC}"
else
    echo -e "${RED}❌ Single philosopher test failed${NC}"
fi

# Test 3: No death scenario
echo -e "\n${YELLOW}Test 3: No death scenario${NC}"
timeout 3s ./philo 4 500 200 200 > test3.out 2>&1
if ! grep -q "died" test3.out; then
    echo -e "${GREEN}✅ No death scenario passed${NC}"
else
    echo -e "${RED}❌ Unexpected death occurred${NC}"
fi

# Test 4: Meal limit
echo -e "\n${YELLOW}Test 4: Meal limit${NC}"
./philo 3 800 200 200 3 > test4.out 2>&1
if ! grep -q "died" test4.out; then
    meal_count=$(grep "is eating" test4.out | wc -l)
    if [ "$meal_count" -eq 9 ]; then  # 3 philosophers * 3 meals
        echo -e "${GREEN}✅ Meal limit test passed (${meal_count} meals)${NC}"
    else
        echo -e "${YELLOW}⚠️  Meal count: ${meal_count} (expected 9)${NC}"
    fi
else
    echo -e "${RED}❌ Unexpected death with meal limit${NC}"
fi

# Test 5: Invalid arguments
echo -e "\n${YELLOW}Test 5: Invalid arguments${NC}"
./philo 0 100 100 100 > test5.out 2>&1
if [ $? -ne 0 ]; then
    echo -e "${GREEN}✅ Invalid arguments handled correctly${NC}"
else
    echo -e "${RED}❌ Invalid arguments not handled${NC}"
fi

# Test 6: Memory leaks (se valgrind è disponibile)
if command -v valgrind &> /dev/null; then
    echo -e "\n${YELLOW}Test 6: Memory leaks${NC}"
    timeout 5s valgrind --leak-check=full --error-exitcode=1 ./philo 3 500 200 200 3 > test6.out 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ No memory leaks detected${NC}"
    else
        echo -e "${RED}❌ Memory leaks detected${NC}"
    fi
else
    echo -e "${YELLOW}⚠️  Valgrind not available, skipping memory test${NC}"
fi

# Cleanup
rm -f test*.out

echo -e "\n${GREEN}🎉 Testing completed!${NC}"
