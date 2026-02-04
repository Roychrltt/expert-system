#!/bin/bash

EXEC=./expert_system_test
TEST_DIR=tests
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

pass=0
fail=0

for input in $TEST_DIR/*.txt; do
    name=$(basename "$input" .txt)
    expected="$TEST_DIR/$name.out"
    output="$TEST_DIR/$name.tmp"

    $EXEC "$input" > "$output" 2>&1

    if diff -q "$output" "$expected" > /dev/null; then
        echo -e "${GREEN}✔ $name PASSED${NC}"
        ((pass++))
    else
        echo -e "${RED}✘ $name FAILED${NC}"
        echo "------ Expected ------"
        cat "$expected"
        echo "------ Got ------"
        cat "$output"
        echo "----------------------"
        ((fail++))
    fi
done

echo
echo "Passed: $pass"
echo "Failed: $fail"

rm $TEST_DIR/*.tmp 2>/dev/null

