#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    TT_INT,      // Integer
    TT_BRA,      // '(' ')'

    TT_PLUS,     // '+'
    TT_MINUS,    // '-'
    TT_ASTERISK, // '*'
    TT_SLASH,    // '/'

    TT_ILLEGAL,
    TT_EOF,
} TokenType;

typedef struct Token Token;
struct Token {
    TokenType type;
    Token *next;
    int val;
    char ch;
};

typedef struct Node Node;
struct Node {
    TokenType type;
    Node *next;
    Node *lhs;
    Node *rhs;
    int val;
};

Token *token;

int consume(char op);
Token *tokenize(char *str);
Token *new_token(TokenType type, Token *cur, char ch);
Token *new_token_num(Token *cur, int val);

void gen(Node *node);


//
// stack
//

int sp = 0, ary[100];

void push(int val) {
    ary[sp++] = val;
}

int pop() {
    if (sp < 1) {
        fprintf(stderr, "stack null\n");
        exit(1);
    }
    return ary[--sp];
}



//
// Tokenizer
//

int consume(char op) {
    if (token->ch != op) {
        return 0;
    }
    token = token->next;
    return 1;
}

Token *tokenize(char *str) {
    char *p = str;
    Token head = {0};
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            ++p;
            continue;
        }

        if (*p == '+') {
            ++p;
            cur = new_token(TT_PLUS, cur, '+');
            continue;
        }
        if (*p == '-') {
            ++p;
            cur = new_token(TT_MINUS, cur, '-');
            continue;
        }
        if (*p == '*') {
            ++p;
            cur = new_token(TT_ASTERISK, cur, '*');
            continue;
        }
        if (*p == '/') {
            ++p;
            cur = new_token(TT_SLASH, cur, '/');
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token_num(cur, strtol(p, &p, 10));
            continue;
        }

        if (*p == '(' || *p == ')') {
            cur = new_token(TT_BRA, cur, *p);
            ++p;
            continue;
        }

        fprintf(stderr ,"Non expected: \"%c\"\n", *p);
        exit(22);
    }

    new_token(TT_EOF, cur, 0);
    return head.next;
}

Token *new_token(TokenType type, Token *cur, char ch) {
    Token *tok = calloc(1, sizeof(Token));
    tok->type  = type;
    tok->ch    = ch;
    cur->next  = tok;
    return tok;
}

Token *new_token_num(Token *cur, int val) {
    Token *tok = new_token(TT_INT, cur, 0);
    tok->val   = val;
    return tok;
}


//
// Parser
//

/*
 *  <expr>    ::= <mul> ("+" <mul> | "-" <mul>)*
 *  <mul>     ::= <unary> ("*" <unary> | "/" <unary>)*
 *  <unary>   ::= ('+' | '-')? <primary>
 *  <primary> ::= <num> | '(' <expr> ')'
 *  <num>     ::= "0-9" ("0-9")*
 */

static Node *new_node(TokenType type) {
    Node *node = calloc(1, sizeof(Node));
    node->type = type;
    return node;
}

static Node *new_binary(TokenType type, Node *lhs, Node *rhs) {
    Node *node = new_node(type);
    node->lhs  = lhs;
    node->rhs  = rhs;
    return node;
}

static Node *new_num(int val) {
    Node *node = new_node(TT_INT);
    node->val  = val;
    return node;
}

static Node *expr(void);
static Node *mul(void);
static Node *unary(void);
static Node *primary(void);

static Node *expr(void) {
    Node *node = mul();

    for (;;) {
        if (consume('+')) {
            node = new_binary(TT_PLUS, node, mul());
        } else if (consume('-')) {
            node = new_binary(TT_MINUS, node, mul());
        } else {
            return node;
        }
    }
}

static Node *mul(void) {
    Node *node = unary();

    for (;;) {
        if (consume('*')) {
            node = new_binary(TT_ASTERISK, node, unary());
        } else if (consume('/')) {
            node = new_binary(TT_SLASH, node, unary());
        } else {
            return node;
        }
    }
}

static Node *unary(void) {
    if (consume('+')) {
        return unary();
    } else if (consume('-')) {
        return new_binary(TT_MINUS, new_num(0), unary());
    } else {
        return primary();
    }
}

static Node *primary(void) {
    if (consume('(')) {
        Node *node = expr();
        if (!consume(')')) {
            fprintf(stderr, "expected ')'\n");
            exit(1);
        }
        return node;
    }

    if (token->type != TT_INT) {
        fprintf(stderr, "expected a number\n");
        exit(42);
    }
    int val = token->val;
    token = token->next;

    return new_num(val);
}

void gen(Node *node) {
    if (node->type == TT_INT) {
        push(node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    int i = pop();
    int j = pop();

    switch(node->type) {
    case TT_PLUS:
        push(j + i);
        break;
    case TT_MINUS: {
        push(j - i);
        break;
    }
    case TT_ASTERISK:
        push(j * i);
        break;
    case TT_SLASH:
        push(j / i);
        break;
    default:
        break;
    }

    return;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    token = tokenize(argv[1]);
    Node *node = expr();
    gen(node);

    return pop();
}

