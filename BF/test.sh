#!/bin/bash

try() {
    expected="$1"
    input="$2"

    ./mybf "$input"
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$expected expected, but got $actual"
        exit 1
    fi
}

try 1 "+."
try 2 "+++>+<-."
try 65 ",.(input:A)" << END
A
END
try 51 "+++[->+++++++++++++++++<]>."

echo OK
