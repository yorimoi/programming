#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

static char *p;

__attribute__((noreturn)) static void error(char *fmt, ... ) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

static int eval(int arg) {
    while(*p) {
        if(isspace(*p)) {
            p++;
            continue;
        }

        if(isdigit(*p)) {
            return strtol(p, &p, 10);
        }

        if(strchr("+-*/", *p)) {
            int op = *p++;
            int x  = eval();
            int y  = eval();
            switch(op) {
                case '+': return x + y;
                case '-': return x - y;
                case '*': return x * y;
                case '/': return x / y;
            }
        }

        error("Invalid character: %c", *p);
    }
    return 0;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "Invalid argument\n");
        exit(1);
    }
    p = argv[1];
    while(*p)
        printf("%d\n", eval());

    return 0;
}
