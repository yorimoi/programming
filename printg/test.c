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
    printg("[%d+%d=%d]\n", -1, -2, -3);

    fprintg(2, "> Unsigned integer\n");
    printg("[%u]\n", 0);
    printg("[%u]\n", -1);
    printg("[%u+%u=%u]\n", 1, 2, 3);

    fprintg(2, "> Char\n");
    printg("[%c]\n", 'a');
    printg("[%c]\n", 'A');
    printg("[%c]\n", '0');
    printg("[%c]\n", '*');
    printg("[%c]\n", 65);
    printg("[%c%c%c%c%c]\n", 'A', 'B', 'C', 'D', 'E', 'F');

    fprintg(2, "> String\n");
    printg("[%s]\n", "string");
    printg("[%sRED\033[0m]\n", "\033[31m");
    printg("[%s, %s]\n", "Hello", "World!");

    fprintg(2, "> Float\n");
    printg("[%f]\n", 123.456f);
    printg("[%f]\n", 0.00001f);
    printg("[%f.%f]\n", 192.168f, 1.255f);

    fprintg(2, "> Percent\n");
    printg("[%%]\n");
    printg("[%%%c%%]\n", '%');

    fprintg(2, "> No support\n");
    printg("[%V%I%M]\n");

    return 0;
}

