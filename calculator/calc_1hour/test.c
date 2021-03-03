#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "ast.h"

int OK, NG;
extern Token *token;

void push(int);
int pop();
Token *tokenize();
Ast *gen_ast();
void expr(Ast *);

void assert_eq(const char *input, int expect) {
    token = tokenize(input);
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
    assert_eq("2+42-7", 37);
    assert_eq("3+4*5", 23);

    if (NG == 0) {
        printf("\nresult \x1b[32mok\x1b[0m; %d success; %d failed;\n", OK, NG);
        exit(0);
    } else {
        printf("\nresult \x1b[31mfailed\x1b[0m; %d success; %d failed;\n", OK, NG);
        exit(NG);
    }
}

