#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    TT_INT,

    TT_PLUS,
    TT_MINUS,
    TT_ASTERISK,
    TT_SLASH,

    TT_ILLEGAL,
    TT_EOF,
} TokenType;

typedef struct Token Token;
struct Token {
    TokenType type;
    Token *next;
    int val;
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

int consume(TokenType type);
Token *tokenize(char *str);
Token *new_token(TokenType type, Token *cur);
Token *new_token_num(Token *cur, int val);

Node *parse(void);
int codegen(Node *node);

int main(int argc, char **argv) {
    int i = 0;

    if (argc != 2) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    token = tokenize(argv[1]);
    while (token->type != TT_EOF) {
        switch(token->type) {
        case TT_PLUS:
            token = token->next;
            i += token->val;
            break;
        case TT_MINUS:
            token = token->next;
            i -= token->val;
            break;
        case TT_INT:
            i = token->val;
            break;
        }
        token = token->next;
    }
    //Node *node = parse();
    //codegen(node);

    return i;
}

//
// Tokenizer
//

int consume(TokenType type) {
    if (token->type != type) {
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
            cur = new_token(TT_PLUS, cur);
            continue;
        }
        if (*p == '-') {
            ++p;
            cur = new_token(TT_MINUS, cur);
            continue;
        }
        if (*p == '*') {
            ++p;
            cur = new_token(TT_ASTERISK, cur);
            continue;
        }
        if (*p == '/') {
            ++p;
            cur = new_token(TT_SLASH, cur);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token_num(cur, strtol(p, &p, 10));
            continue;
        }

        fprintf(stderr ,"Non expected: \"%c\"\n", *p);
        exit(22);
    }

    new_token(TT_EOF, cur);
    return head.next;
}

Token *new_token(TokenType type, Token *cur) {
    Token *tok = calloc(1, sizeof(Token));
    tok->type  = type;
    cur->next  = tok;
    return tok;
}

Token *new_token_num(Token *cur, int val) {
    Token *tok = new_token(TT_INT, cur);
    tok->val   = val;
    return tok;
}


//
// Parser
//

/*
 *  <expr> ::= <mul> ("+" <mul> | "-" <mul>)*
 *  <mul>  ::= <num> ("*" <num> | "/" <num>)*
 *  <num>  ::= "0-9" ("0-9")*
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
static Node *primary(void);

Node *parse(void) {
    Node head = {0};
    Node *code = &head;
    code = expr();
    return &head;
}

static Node *expr(void) {
    Node *node = mul();

    for (;;) {
        if (consume(TT_PLUS)) {
            node = new_binary(TT_PLUS, node, mul());
        } else if (consume(TT_MINUS)) {
            node = new_binary(TT_MINUS, node, mul());
        } else {
            return node;
        }
    }
}

static Node *mul(void) {
    Node *node = primary();

    for (;;) {
        if (consume(TT_ASTERISK)) {
            node = new_binary(TT_ASTERISK, node, primary());
        } else if (consume(TT_SLASH)) {
            node = new_binary(TT_SLASH, node, primary());
        } else {
            return node;
        }
    }
}

static Node *primary(void) {
    if (token->type != TT_INT) {
        fprintf(stderr, "expected a number\n");
        exit(42);
    }
    int val = token->val;
    token = token->next;

    return new_num(val);
}

int codegen(Node *node) {
    int i = 0;
    return i;
}

