#include "main.h"

Token* tokenize(const char * input);
Ast *gen_ast();
void expr(Ast *node);

int OK, NG;

void assert_eq(const char *input, int expect) {
    TOKEN = tokenize(input);
    Ast *node = gen_ast();
    expr(node);
    int actual = pop();
    if (actual == expect) {
        printf("%s ... \x1b[32mok\x1b[0m\n", input);
        ++OK;
    } else {
        printf("%s ... \x1b[31mfailed\x1b[0m; expect %d. but got %d\n", input, expect, actual);
        ++NG;
    }
}

void test() {
    assert_eq("0", 0);
    assert_eq("42", 42);
    assert_eq("5+20-4", 21);
    assert_eq("5+6*7", 47);
    assert_eq("1+2*3/4-5%6", -3);
    assert_eq(" 12 + 34 - 5 ", 41);
    assert_eq("5*(9-6)", 15);
    assert_eq("(3+5)/2", 4);

    if (NG == 0) {
        printf("result \x1b[32mok\x1b[0m; %d success; %d failed;\n", OK, NG);
    } else {
        printf("result \x1b[31mfailed\x1b[0m; %d success; %d failed;\n", OK, NG);
    }

    exit(NG);
}

