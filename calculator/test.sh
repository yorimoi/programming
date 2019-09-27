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

assert   0 '0'
assert  42 '42'
assert  21 '5+20-4'

assert  14 '1*2+3*4'
assert  10 '4-3*2+12'
assert  16 '6/3-2+16'

assert  15 '5*(9-6)'
assert   4 '(3+5)/2'
assert   1 '((1-2)*3)+4'

assert  10 '-10+20'
assert  10 '- -10'
assert  10 '- - +10'

assert   7 '7%3+2*3'
assert  15 '3%7*(2+3)'

assert   1 '10**0'
assert  32 '2**5'
assert  64 '2**2**3' # Python 3.7.0 => 256 ???
assert  64 '2**6'
assert 128 '2**8-2**7'

assert 120 '5!'
assert   0 '4!-(4*3*2*1)'

assert   2 '2!!'
assert   0 '-4!+4!'
assert  28 '-(1+2)!+34'

assert   4 '5&12' # 0101 & 1100 = 0100
assert  13 '5|12' # 0101 | 1100 = 1101
assert   9 '5^12' # 0101 ^ 1100 = 1001
assert   7 '1&2|3^4' # 0001 & 0010 | 0011 ^ 0100 = 0111

assert   2 '~10-~12' # (000...1010 => 111...0101) - (000...1100 => 111...0011)

assert  19 '0xf+0x4'
assert  20 '0b1010+10'
assert   6 '0b1*0x2*3'

if [ $ERRCNT -eq 0 ]; then
    echo -e "\033[36mSUCCESS\033[0m"
else
    echo -e "\033[31mFAIL\033[0m ... $ERRCNT"
fi
