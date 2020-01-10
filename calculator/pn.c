#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define skip_whitespace() while(*p==0x20)++p;

char *p;

void skip_whitespace() {
    while (*p==' ') ++p;
}

int eval() {
    while (*p) {
        skip_whitespace();

        if(strchr("+-*/%", *p)) {
            char op = *p++;
            int x = eval();
            int y = eval();
            switch (op) {
                case '+': return x + y;
                case '-': return x - y;
                case '*': return x * y;
                case '/': return x / y;
                case '%': return x % y;
            }
        }

        if ('0'<=*p && *p<='9') {
            return strtol(p, &p, 10);
        }

        fprintf(stderr, "invalid token: \"%s\"", p);
        exit(1);
    }

    return 0;
}

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "invalid argument\n");
        exit(1);
    }

    char input[64];
    strncpy(input, argv[1], 63);
    input[63] = '\0';

    p = input;
    printf("\"%s\" = %d\n", input, eval());

    return 0;
}

