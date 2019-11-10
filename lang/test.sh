#!/bin/bash

gcc -o lang lang.c

try() {
    expected="$1"
    actual=$(./lang "$2")

    if [ "$actual" = "$expected" ]; then
        echo "$2 => $actual"
    else
        echo "$expected expected, but got $actual"
        exit 1
    fi
}

echo "=== basic ==="
try 0 0
try 99 99

echo "=== arithmetic operators ==="
try 2 "+ 1 1"
try 46 "+ 12 34"
try 22 "- 43 21"
try -4 "- 1 5"
try 15 "* 3 5"
try 5 "/ 20 4"
try 10 "+ + + 1 2 3 4"
try 10 "+ 1 + 2 + 3 4"
try 14 "+ 2 *4 3"

echo OK
