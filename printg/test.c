#include "printg.h"

void assert(int expect, int actual) {
    if (expect == actual) {
        printf("[ \033[32mOK\033[0m ]\n");
    } else {
        printf("[ \033[31mNG\033[0m ]  expect %d, but got %d\n", expect, actual);
    }
}

int main(void) {

    fprintg(0, "test");

    return 0;
}

