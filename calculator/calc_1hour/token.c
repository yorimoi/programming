#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

extern Token *token;

Token *new_token(TokenKind kind) {
    Token *t = (Token *) malloc(sizeof(Token *));
    t->kind = kind;
    return t;
}

Token *tokenize(const char *input) {
    Token *head = (Token *) malloc(sizeof(Token *));
    Token *cur = head;
    const char *p = input;

    while (*p) {
        if (strchr("+-*/%", *p)) {
            Token *t;
            switch (*p) {
                case '+': t = new_token(TK_PLUS);     break;
                case '-': t = new_token(TK_MINUS);    break;
                case '*': t = new_token(TK_ASTERISK); break;
                case '/': t = new_token(TK_SLASH);    break;
                case '%': t = new_token(TK_PARCENT);  break;
            }
            cur = cur->next = t;
            ++p;
            continue;
        }
        if (isdigit(*p)) {
            int val = *p - '0';
            while (isdigit(*++p)) {
                val = val * 10 + *p - '0';
            }
            Token *t = new_token(TK_NUM);
            t->val = val;
            cur = cur->next = t;
            continue;
        }

        fprintf(stderr, "invalid charactor: '%c'", *p);
        exit(1);
    }

    Token *t = new_token(TK_EOF);
    cur->next = t;
    return head->next;
}

void consume() {
    token = token->next;
}

