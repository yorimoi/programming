#include <string.h>
#include <ctype.h>

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
        //printf("[.]\n");
        return 0;
    }
}

Token* new_token(TokenKind kind) {
    Token *t = (Token *) malloc(sizeof(Token *));
    t->kind = kind;
    return t;
}

Ast *new_node(AstKind kind, Ast *rhs, Ast *lhs) {
    Ast *node = (Ast *) malloc(sizeof(Ast *));
    node->kind = kind;
    node->rhs = rhs;
    node->lhs = lhs;
    return node;
}

Ast *new_node_num(int val) {
    Ast *node = (Ast *) malloc(sizeof(Ast *));
    node->kind = AK_Num;
    node->val = val;
    return node;
}

Token* tokenize(const char * input) {
    Token *head= (Token *) malloc(sizeof(Token *));
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

Ast *add();
Ast *mul();
Ast *num();

void consume() {
    TOKEN = TOKEN->next;
}

// expr = add
Ast *gen_ast() {
    return add();
}

// add = mul ('+' mul | '-' mul)?
Ast *add() {
    Ast *node = mul();

    while (1) {
        if (TOKEN->kind == TK_Plus) {
            consume();
            node = new_node(AK_Add, node, mul());
        } else if (TOKEN->kind == TK_Minus) {
            consume();
            node = new_node(AK_Sub, node, mul());
        } else {
            return node;
        }
    }
}

// mul = num ('*' num | '/' num | '%' num)?
Ast *mul() {
    Ast *node = num();

    while (1) {
        if (TOKEN->kind == TK_Asta) {
            consume();
            node = new_node(AK_Mul, node, num());
        } else if (TOKEN->kind == TK_Slash) {
            consume();
            node = new_node(AK_Div, node, num());
        } else if (TOKEN->kind == TK_Parce) {
            consume();
            node = new_node(AK_Mod, node, num());
        } else {
            return node;
        }
    }
}

// num = '(' expr ')' | ('1'..'9')('0'..'9')*
Ast *num() {
    if (TOKEN->kind == TK_LParen) {
        consume();
        Ast *node = add();
        if (TOKEN->kind != TK_RParen) {
            fprintf(stderr, "Open parenthesis has no corresponding closing parenthesis\n");
            exit(1);
        }
        consume();
        return node;
    }
    int val = TOKEN->val;
    consume();
    return new_node_num(val);
}

void expr(Ast *node) {
    if (node->kind == AK_Num) {
        push(node->val);
        return;
    }

    expr(node->rhs);
    expr(node->lhs);

    switch (node->kind) {
        case AK_Add: {
            int l = pop();
            int r = pop();
            push(r + l);
            break;
        }
        case AK_Sub: {
            int l = pop();
            int r = pop();
            push(r - l);
            break;
        }
        case AK_Mul: {
            int l = pop();
            int r = pop();
            push(r * l);
            break;
        }
        case AK_Div: {
            int l = pop();
            int r = pop();
            push(r / l);
            break;
        }
        case AK_Mod: {
            int l = pop();
            int r = pop();
            push(r % l);
            break;
        }
        default: printf("unreachable"); break;
    }
}

int main(void) {
    const char *input = "1+42/2-23*4+70+8%6";

    TOKEN = tokenize(input);

    Ast *node = gen_ast();

    expr(node);

    printf("%s = %d\n", input, pop());

    return 0;
}

