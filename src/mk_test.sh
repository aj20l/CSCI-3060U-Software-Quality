#!/bin/bash
# mk_test.sh - helper used to create test files (run with:  source ./mk_test.sh)
#
# mk NAME   reads one test. The text is split into three files at the marker lines "--- terminal" and "--- transactions":
#
#   (text before the first marker)  -> tests/inputs/NAME.in        commands typed by the user
#   (text after "--- terminal")     -> tests/expected/NAME.out     expected terminal output
#   (text after "--- transactions") -> tests/expected/NAME.etf     expected daily transaction file
#
# Every program run starts by printing the same banner line, so mk puts it at the top of
# each .out file for you (change BANNER below if the program's banner ever changes).
# If a test has no "--- transactions" part, its .etf file is created empty.

BANNER="Games Distribution Front End. Enter a transaction code (type login to begin)."

mk() {
    local name="$1"
    mkdir -p tests/inputs tests/expected
    awk -v n="$name" -v banner="$BANNER" '
        BEGIN                     { f = "tests/inputs/" n ".in" }
        /^--- terminal$/          { f = "tests/expected/" n ".out"; print banner > f; next }
        /^--- transactions$/      { f = "tests/expected/" n ".etf"; next }
                                  { print > f }
    '
    touch "tests/expected/$name.out" "tests/expected/$name.etf"
}