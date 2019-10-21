#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token Kind
typedef enum {
    TK_OP,
    TK_NUM,
    TK_EOF,
} Kind;

typedef struct Token Token;
struct Token {
    Kind kind;
    Token *next;
    char *str;
    int val;
};

Token *token;

void tokenize(char *p);
void new_token(Kind kind);
void new_token_op(Kind kind, char *op);
void new_token_num(Kind kind, int val);

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Invalid argument\n");
        return 1;
    }

    tokenize(argv[1]);

    return 0;
}

void tokenize(char *p) {
    while (*p) {
        if (isspace(*p)) {
            ++p;
            continue;
        }

        if (strstr(p, "+-")) {
            new_token_op(TK_OP, p);
            ++p;
            continue;
        }

        if (isdigit(*p)) {
            new_token_num(TK_NUM, strtol(p, &p, 10));
            continue;
        }

        fprintf(stderr, "Invalid token\n");
        exit(1);
    }
}

void new_token(Kind kind) {
    Token *t = (Token *) malloc(sizeof(Token));
    t->kind = kind;
    token->next = t;
    token = t;
}

void new_token_op(Kind kind, char *op) {
    new_token(kind);
    token->str = op;
}

void new_token_num(Kind kind, int val) {
    new_token(kind);
    token->val = val;
}
