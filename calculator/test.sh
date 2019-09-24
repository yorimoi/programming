#!/bin/bash

function assert() {
    expected="$1"
    input="$2"

    ./cal.exe "$input"
    actual="$?"

    if [ "$expected" = "$actual" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 '0'
assert 42 '42'
assert 21 '5+20-4'
assert 21 '5+20-4*8'

echo OK
