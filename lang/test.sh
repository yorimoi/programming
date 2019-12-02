#!/bin/bash

gcc -o lang lang.c

try() {
    actual=$(./lang "$1")
    expected="$2"

    if [ "$actual" = "$expected" ]; then
        echo "$1 => $actual"
    else
        echo "$expected expected, but got $actual"
        exit 1
    fi
}

echo "=== basic ==="
try 0 0
try 99 99

echo "=== arithmetic operators ==="
try "+ 1 1" 2
try "+ 12 34" 46
try "- 43 21" 22
try "- 1 5" -4
try "* 3 5" 15
try "/ 20 4" 5
try "+ + + 1 2 3 4" 10
try "+ 1 + 2 + 3 4" 10
try "+ 2 *4 3" 14

echo "=== functions ==="
try "F[+ a a] F(1)" 2
try "F[* a 2] F(5)" 10
try "F[* a a] F(F(2))" 16
try "F[* a a] F(F(F(2)))" 256
try "F[* a b] F(3 5)" 15

echo "=== build-in functions ==="
try "P(5)" "5
5"

echo "=== fibonacci sequence ==="
./lang "F[P(a) F(b + a b)] F(1 1)" | head -n 10

echo OK
