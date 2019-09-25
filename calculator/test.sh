#!/bin/bash

ERRCNT=0

function assert() {
    expected="$1"
    input="$2"

    ./cal.exe "$input"
    actual="$?"

    if [ "$expected" = "$actual" ]; then
        echo -e "[  \033[32mOK\033[0m  ] $input => $actual"
    else
        echo -e "[  \033[31mNG\033[0m  ] $input => $expected expected, but got $actual"
        # exit 1
        ERRCNT=$((ERRCNT+1))
    fi
}

assert 0 '0'
assert 42 '42'
assert 21 '5+20-4'

assert 14 '1*2+3*4'
assert 10 '4-3*2+12'
assert 0 '6/3-2*9'

if [ $ERRCNT -eq 0 ]; then
    echo -e "\033[36mSUCCESS\033[0m"
else
    echo -e "\033[31mFAIL\033[0m ... $ERRCNT"
fi
