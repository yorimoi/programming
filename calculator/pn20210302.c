#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    Plus,
    Minus,
    Asterisk,
    Slash,
    Num,
} TokenKind;

typedef struct Token Token;
struct Token {
    Token *next;
    TokenKind kind;
    int val;
};

Token *tokenize(const char *input) {
    Token *head = (Token *) malloc(sizeof(Token));
    Token *cur = head;
    const char *p = input;

    while (*p) {
        if (isblank(*p)) {
            ++p;
            continue;
        }
        if (strchr("+-*/", *p)) {
            Token *t = (Token *) malloc(sizeof(Token));
            switch (*p) {
                case '+': t->kind = Plus;     break;
                case '-': t->kind = Minus;    break;
                case '*': t->kind = Asterisk; break;
                case '/': t->kind = Slash;    break;
            }
            cur = cur->next = t;
            ++p;
            continue;
        }
        if ('0' <= *p && *p <= '9') {
            int val = *p - '0';
            while (*++p) {
                if ('0' <= *p && *p <= '9') {
                    val = val * 10 + *p - '0';
                } else {
                    break;
                }
            }
            Token *t = (Token *) malloc(sizeof(Token));
            t->kind = Num;
            t->val = val;
            cur = cur->next = t;
            continue;
        }

        fprintf(stderr, "invalid char: '%c'", *p);
        exit(1);
    }
    return head;
}

int calculate(Token *token) {
    *token = *token->next;
    switch (token->kind) {
        case Plus:     return calculate(token) + calculate(token);
        case Minus:    return calculate(token) - calculate(token);
        case Asterisk: return calculate(token) * calculate(token);
        case Slash:    return calculate(token) / calculate(token);
        case Num:      return token->val;
    }
    fprintf(stderr, "invalid token\n");
    exit(1);
}

int main(void) {
    const char *input = "/ - * + 1 2 34 5 6";

    Token *token = tokenize(input);

    int result = calculate(token);

    printf("%s = %d\n", input, result);

    return 0;
}

