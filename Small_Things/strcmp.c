#include <stdio.h>

int strcmp(const char *str1, const char *str2) {
    register const char *c1, *c2;

    for (c1 = str1, c2 = str2; *c1 == *c2 && *c1 != '\0'; ++c1, ++c2) {
        ;
    }

    return *c1 - *c2;
}

int strncmp(const char *str1, const char *str2, size_t len) {
    register const char *c1, *c2;

    for (c1 = str1, c2 = str2; (*c1 == *c2 && *c1 != '\0') || !--len; ++c1, ++c2) {
        ;
    }

    return *c1 - *c2;
}

void assert(int expect, int actual) {
    if (expect == actual) {
        printf("[ \033[32mOK\033[0m ]\n");
    } else {
        printf("[ \033[31mNG\033[0m ]  expect %d, but got %d\n", expect, actual);
    }
}

int main(void) {
    printf("=== strcmp ===\n");
    assert( 0, strcmp("12345",  "12345"));
    assert( 1, strcmp("abcde",  "12345"));
    assert(-1, strcmp("12345",  "abcde"));
    assert( 1, strcmp("123450", "12345"));
    assert(-1, strcmp("12345",  "123450"));

    printf("=== strncmp ===\n");
    assert(   0, strncmp("12311x", "12300",   3));
    assert(   1, strncmp("12311x", "12300",   4));
    assert(  -1, strncmp("12300",  "12311x",  4));
    assert(   0, strncmp("12300",  "12300",  10));
    assert( 120, strncmp("12300x", "12300",  10));
    assert(-120, strncmp("12300",  "12300x", 10));

    return 0;
}

