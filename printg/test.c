#include "printg.h"

#include <stdio.h>

int main(void) {
    function_tests();

    fprintg(2, "\n> Stdout\n");
    fprintg(1, " *[stdout]\n");
    fprintf(stdout, "  [stdout]\n");

    fprintg(2, "\n> Stderr\n");
    fprintg(2, " *[stderr]\n");
    fprintf(stderr, "  [stderr]\n");

    fprintg(2, "\n> Over 8 character string\n");
    printg(" *[long long string]\n");
    printf("  [long long string]\n");

    fprintg(2, "\n> Integer\n");
    printg(" *[%d]\n", 0);
    printf("  [%d]\n", 0);
    printg(" *[%d]\n", 42);
    printf("  [%d]\n", 42);
    printg(" *[%d]\n", 'A');
    printf("  [%d]\n", 'A');
    printg(" *[%d]\n", -1);
    printf("  [%d]\n", -1);
    printg(" *[%d+%d=%d]\n", -1, -2, -3);
    printf("  [%d+%d=%d]\n", -1, -2, -3);

    fprintg(2, "\n> Unsigned integer\n");
    printg(" *[%u]\n", 0);
    printf("  [%u]\n", 0);
    printg(" *[%u]\n", -1);
    printf("  [%u]\n", -1);
    printg(" *[%u+%u=%u]\n", 1, 2, 3);
    printf("  [%u+%u=%u]\n", 1, 2, 3);

    fprintg(2, "\n> Char\n");
    printg(" *[%c]\n", 'a');
    printf("  [%c]\n", 'a');
    printg(" *[%c]\n", 'A');
    printf("  [%c]\n", 'A');
    printg(" *[%c]\n", '0');
    printf("  [%c]\n", '0');
    printg(" *[%c]\n", '*');
    printf("  [%c]\n", '*');
    printg(" *[%c]\n", 65);
    printf("  [%c]\n", 65);
    printg(" *[%c%c%c%c%c%c]\n", 'A', 'B', 'C', 'D', 'E', 'F');
    printf("  [%c%c%c%c%c%c]\n", 'A', 'B', 'C', 'D', 'E', 'F');

    fprintg(2, "\n> String\n");
    printg(" *[%s]\n", "string");
    printf("  [%s]\n", "string");
    printg(" *[%sRED\033[0m]\n", "\033[31m");
    printf("  [%sRED\033[0m]\n", "\033[31m");
    printg(" *[%s, %s]\n", "Hello", "World!");
    printf("  [%s, %s]\n", "Hello", "World!");

    fprintg(2, "\n> Float\n");
    printg(" *[%f]\n", 123.456f);
    printf("  [%f]\n", 123.456f);
    printg(" *[%f]\n", 0.00001f);
    printf("  [%f]\n", 0.00001f);
    printg(" *[%f.%f]\n", 192.168f, 1.255f);
    printf("  [%f.%f]\n", 192.168f, 1.255f);

    fprintg(2, "\n> Percent\n");
    printg(" *[%%]\n");
    printf("  [%%]\n");
    printg(" *[%%%c%%]\n", '%');
    printf("  [%%%c%%]\n", '%');

    fprintg(2, "\n> No support\n");
    printg(" *[%V%I%M]\n");
    printf("  [%V%I%M]\n");

    fprintg(2, "\n> Fill space\n");
    printg(" *[%0d]\n", 123);
    printf("  [%0d]\n", 123);
    printg(" *[%10d]\n", 123);
    printf("  [%10d]\n", 123);
    printg(" *[%6u]\n", -1);
    printf("  [%6u]\n", -1);
    printg(" *[%4c]\n", 'A');
    printf("  [%4c]\n", 'A');
    printg(" *[%10s]\n", "aiueo");
    printf("  [%10s]\n", "aiueo");
    printg(" *[%10f]\n", 3.14);
    printf("  [%10f]\n", 3.14);
    printg(" *[%3d]\n", 0);
    printf("  [%3d]\n", 0);
    printg(" *[%3x]\n", 0);
    printf("  [%3x]\n", 0);

    fprintg(2, "\n> Hexadecimal\n");
    printg(" *[0x%x]\n", 255);
    printf("  [0x%x]\n", 255);
    printg(" *[0x%X]\n", 11259375);
    printf("  [0x%X]\n", 11259375);
    printg(" *[%x]\n", -42);
    printf("  [%x]\n", -42);
    printg(" *[%x%x%x]\n", '0', '1', '2');
    printf("  [%x%x%x]\n", '0', '1', '2');

    return 0;
}

