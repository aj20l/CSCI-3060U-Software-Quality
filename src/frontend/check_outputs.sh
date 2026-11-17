#!/bin/bash
# check_outputs.sh - compares the actual results of a test run with the expected results.
#
# Usage:  ./check_outputs.sh [run_name]      (default: latest)
#
# For every test it compares two pairs of files:
#   expected/NAME.out  with  outputs/run_name/NAME.out   (terminal output)
#   expected/NAME.etf  with  outputs/run_name/NAME.atf   (daily transaction file)
# It prints PASS or FAIL for each test, saves the details of every difference in
# tests/outputs/run_name/report.txt, and ends with a summary. The script's exit
# status is 0 only if every test passed.

RUN_NAME=${1:-latest}
EXPECTED_DIR=tests/expected
OUTPUT_DIR=tests/outputs/$RUN_NAME
REPORT=$OUTPUT_DIR/report.txt

# Normalize a file by removing Windows line endings and ensuring it ends with a newline
normalize() { tr -d '\r' < "$1" | awk 1; }

passed=0
failed=0
: > "$REPORT"

for expected in "$EXPECTED_DIR"/*.out; do
    name=$(basename "$expected" .out)
    problems=""

    # Terminal output, compared ignoring whitespace differences (so that a test that prints an extra space or two still passes)
    if ! diff -b <(normalize "$EXPECTED_DIR/$name.out") <(normalize "$OUTPUT_DIR/$name.out") \
            > "$OUTPUT_DIR/$name.out.diff" 2>&1; then
        problems="$problems terminal-output"
        { echo "=== $name: terminal output differs (< expected, > actual)"
          cat "$OUTPUT_DIR/$name.out.diff"; } >> "$REPORT"
    fi

    # Daily transaction file, compared ignoring whitespace differences (so that a test that prints an extra space or two still passes)
    if ! diff <(normalize "$EXPECTED_DIR/$name.etf") <(normalize "$OUTPUT_DIR/$name.atf") \
            > "$OUTPUT_DIR/$name.atf.diff" 2>&1; then
        problems="$problems transaction-file"
        { echo "=== $name: transaction file differs (< expected, > actual)"
          cat "$OUTPUT_DIR/$name.atf.diff"; } >> "$REPORT"
    fi

    rm -f "$OUTPUT_DIR/$name.out.diff" "$OUTPUT_DIR/$name.atf.diff"

    if [ -z "$problems" ]; then
        echo "PASS  $name"
        passed=$((passed + 1))
    else
        echo "FAIL  $name  ($problems )"
        failed=$((failed + 1))
    fi
done

echo
echo "Run '$RUN_NAME': $passed passed, $failed failed (details in $REPORT)"
[ "$failed" -eq 0 ]