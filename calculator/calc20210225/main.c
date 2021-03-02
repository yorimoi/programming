#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"

#define MAX_STACK_SIZE 100
static struct {
    int array[MAX_STACK_SIZE];
    int cur;
} STACK;

void push(int val) {
    //printf("[<%d]\n", val);
    STACK.array[STACK.cur] = val;
    ++STACK.cur;
}

int pop() {
    if (0 < STACK.cur) {
        //printf("[>%d]\n", STACK.array[STACK.cur - 1]);
        return STACK.array[--STACK.cur];
    } else {
        fprintf(stderr, "null stack");
        exit(1);
    }
}

Token* new_token(TokenKind kind) {
    Token *t = (Token *) malloc(sizeof(Token));
    t->kind = kind;
    return t;
}

Ast *new_node(AstKind kind, Ast *rhs, Ast *lhs) {
    Ast *node = (Ast *) malloc(sizeof(Ast));
    node->kind = kind;
    node->rhs = rhs;
    node->lhs = lhs;
    return node;
}

Ast *new_node_num(int val) {
    Ast *node = new_node(AK_Num, NULL, NULL);
    node->val = val;
    return node;
}

Token* tokenize(const char * input) {
    Token *head= (Token *) malloc(sizeof(Token));
    Token *cur = head;
    const char *p = input;

    while (*p) {
        if (isblank(*p)) {
            ++p;
            continue;
        }
        if (strchr("+-*/%()", *p) != NULL) {
            Token *t;
            switch (*p) {
                case '+': t = new_token(TK_Plus);   break;
                case '-': t = new_token(TK_Minus);  break;
                case '*': t = new_token(TK_Asta);   break;
                case '/': t = new_token(TK_Slash);  break;
                case '%': t = new_token(TK_Parce);  break;
                case '(': t = new_token(TK_LParen); break;
                case ')': t = new_token(TK_RParen); break;
            }
            cur->next = t;
            cur = t;
            ++p;
            continue;
        }
        if (isdigit(*p)) {
            int val = *p - '0';
            while (isdigit(*++p)) {
                val = val * 10 + *p - '0';
            }
            Token *t = new_token(TK_Num);
            t->val = val;
            cur->next = t;
            cur = t;
            continue;
        }

        fprintf(stderr, "invalid charactor: '%c'\n", *p);
        exit(1);
    }

    Token *tail = new_token(TK_EOF);
    cur->next = tail;
    return head->next;
}

Ast *expr(Token*);
Ast *add(Token*);
Ast *mul(Token*);
Ast *unary(Token*);
Ast *term(Token*);

_Bool consume(const TokenKind expect, Token *token) {
    if (token->kind == expect) {
        *token = *token->next;
        return true;
    }
    return false;
}

Ast *gen_ast(Token *token) {
    return expr(token);
}

// expr = add+
Ast *expr(Token *token) {
    return add(token);
}

// add = mul ('+' mul | '-' mul)?
Ast *add(Token *token) {
    Ast *node = mul(token);

    while (1) {
        if (consume(TK_Plus, token)) {
            node = new_node(AK_Add, node, mul(token));
        } else if (consume(TK_Minus, token)) {
            node = new_node(AK_Sub, node, mul(token));
        } else {
            return node;
        }
    }
}

// mul = unary ('*' unary | '/' unary | '%' unary)?
Ast *mul(Token *token) {
    Ast *node = unary(token);

    while (1) {
        if (consume(TK_Asta, token)) {
            node = new_node(AK_Mul, node, unary(token));
        } else if (consume(TK_Slash, token)) {
            node = new_node(AK_Div, node, unary(token));
        } else if (consume(TK_Parce, token)) {
            node = new_node(AK_Mod, node, unary(token));
        } else {
            return node;
        }
    }
}

// unary = '-'? term
Ast *unary(Token *token) {
    if (consume(TK_Minus, token)) {
        return new_node(AK_Sub, new_node_num(0), unary(token));
    } else {
        return term(token);
    }
}

// term = '(' expr ')' | [0-9][0-9]*
Ast *term(Token *token) {
    if (consume(TK_LParen, token)) {
        Ast *node = add(token);
        if (!consume(TK_RParen, token)) {
            fprintf(stderr, "Open parenthesis has no corresponding closing parenthesis\n");
            exit(1);
        }
        return node;
    }

    int val = token->val;
    if (consume(TK_Num, token)) {
        return new_node_num(val);
    }

    fprintf(stderr, "%d: unreachable", __LINE__);
    exit(1);
}

void calculate(Ast *node) {
    if (node->kind == AK_Num) {
        push(node->val);
        return;
    }

    calculate(node->rhs);
    calculate(node->lhs);

    int l = pop();
    int r = pop();

    switch (node->kind) {
        case AK_Add: push(r + l); break;
        case AK_Sub: push(r - l); break;
        case AK_Mul: push(r * l); break;
        case AK_Div: push(r / l); break;
        case AK_Mod: push(r % l); break;
        default: fprintf(stderr, "%d: unreachable", __LINE__); break;
    }
}

int main(void) {
    const char *input = "1+42/2-23*4+70+8%6*-3";

    Token *token = tokenize(input);

    Ast *node = gen_ast(token);

    calculate(node);

    printf("%s = %d\n", input, pop());

    return 0;
}

