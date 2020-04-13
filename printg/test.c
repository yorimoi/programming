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
    fprintg(2, "> Stdout\n");
    fprintg(1, "[stdout]\n");

    fprintg(2, "> Errout\n");
    fprintg(2, "[errout]\n");

    fprintg(2, "> Over 8 character string\n");
    printg("[long long long long long string]\n");

    fprintg(2, "> Integer\n");
    printg("[%d]\n", 0);
    printg("[%d]\n", 42);
    printg("[%d]\n", 'A');
    printg("[%d]\n", -1);
    printg("[%u]\n", 0);
    printg("[%u]\n", -1);

    return 0;
}

