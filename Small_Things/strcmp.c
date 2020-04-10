#include <stdio.h>

int strcmp(const char *str1, const char *str2) {
    register const char *c1, *c2;

    for (c1 = str1, c2 = str2; *c1 == *c2 && *c1 != '\0'; ++c1, ++c2) {
        ;
    }

    return *c1 - *c2;
}

void test(int expect, int actual) {
    if (expect == actual) {
        printf("[ \033[32mOK\033[0m ]\n");
    } else {
        printf("[ \033[31mNG\033[0m ]\n");
    }
}

int main(void) {
    test( 0, strcmp("12345",  "12345"));
    test( 1, strcmp("abcde",  "12345"));
    test(-1, strcmp("12345",  "abcde"));
    test( 1, strcmp("123450", "12345"));
    test(-1, strcmp("12345",  "123450"));

    return 0;
}

