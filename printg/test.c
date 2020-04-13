#include "printg.h"

#include <stdio.h>

void assert(int expect, int actual) {
    if (expect == actual) {
        printf("[ \033[32mOK\033[0m ]\n");
    } else {
        printf("[ \033[31mNG\033[0m ]  expect %d, but got %d\n", expect, actual);
    }
}

int main(void) {
    printg("=== test ===\n");

    fprintg(1, "stdout\n");
    fprintg(2, "errout\n");

    printg("long long long long long string\n");
    printg("42 => %d\n", 42);
    printg("65 => %d\n", 'A');

    return 0;
}

