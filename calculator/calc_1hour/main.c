/* The time limit is 1 hour */

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "ast.h"

Token *token;

void push(int);
int pop();
Token *tokenize();
Ast *gen_ast();

void expr(Ast *node) {
    if (node->kind == NK_NUM) {
        push(node->val);
        return;
    }

    expr(node->lhs);
    expr(node->rhs);
    int l = pop();
    int r = pop();

    switch (node->kind) {
        case NK_ADD:
            push(l + r);
            break;
        case NK_SUB:
            push(l - r);
            break;
        case NK_MUL:
            push(l * r);
            break;
        case NK_DIV:
            if (r == 0) {
                fprintf(stderr, "division by zero\n");
                exit(1);
            }
            push(l / r);
            break;
        case NK_MOD:
            if (r == 0) {
                fprintf(stderr, "division by zero\n");
                exit(1);
            }
            push(l % r);
            break;
    }
}

int main(void) {
    const char *input = "1+42-5*8/4";

    token = tokenize(input);

    Ast *node = gen_ast();

    expr(node);

    printf("%s = %d\n", input, pop());

    return 0;
}

