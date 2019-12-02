#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

static char *p;
static char  func[26][100];

__attribute__((noreturn)) static void error(char *fmt, ... ) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

static void read_until(char c, char *buf) {
    for (; *p != c; ++p, ++buf) {
        *buf = *p;
    }
    ++p;
    *buf = '\0';
}

static void skip() {
    while (isspace(*p)) {
        p++;
    }
}

static void expect(char c) {
    if (*p != c) {
        error("%c expected: %s", c, p);
    }
    ++p;
}

static int eval(int *args);

static int eval_string(char *code, int *args) {
    char *orig = p;
    p = code;
    int val;
    while (*p) {
        val = eval(args);
    }
    p = orig;
    return val;
}

static int eval(int *args) {
    while(*p) {
        skip();

        // Function parameter
        if ('a' <= *p && *p <= 'z') {
            return args[*p++ - 'a'];
        }

        // Function definition
        if ('A' <= *p && *p <= 'Z' && p[1] == '[') {
            char name = *p;
            p += 2;
            read_until(']', func[name - 'A']);
            return eval(args);
        }

        // 'P' print primitive
        if (*p == 'P') {
            ++p;
            expect('(');
            int val = eval(args);
            expect(')');
            printf("%d\n", val);
            return val;
        }

        // Function application
        if ('A' <= *p && *p <= 'Z' && p[1] == '(') {
            int newargs[26];
            char name = *p;
            p += 2;

            int i = 0;
            for (skip(); *p != ')'; skip()) {
                newargs[i++] = eval(args);
            }

            expect(')');

            return eval_string(func[name - 'A'], newargs);
        }

        // Literal numbers
        if(isdigit(*p)) {
            return strtol(p, &p, 10);
        }

        // Arithmetic operators
        if(strchr("+-*/", *p)) {
            int op = *p++;
            int x  = eval(args);
            int y  = eval(args);
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
        printf("%d\n", eval(0));

    return 0;
}
