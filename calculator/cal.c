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
    TT_PERCENT,  // '%'
    TT_POWER,    // "**"
    TT_AND,      // '&'
    TT_OR,       // '|'
    TT_XOR,      // '^'
    TT_FACT,     // '!' Factorial

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
        fprintf(stderr, "stack null");
        exit(1);
    }
    return ary[--sp];
}


//
// math
//

int pow_(int j, int i) {
    if (!i--) return 1;
    int k = j;
    for (; i; --i) k *= j;
    return k;
}

int factorial(int val) {
    if (val < 0) {
        fprintf(stderr, "factorial(): not support minus\n");
        return 0;
    }
    int fact = 1;
    for (; val;) fact *= val--;
    return fact;
}

//
// string
//

int strcmp_(const char *str1, const char *str2)
{
    for (int i = 0;; ++i) {
        if (str1[i] == 0 && str2[i] == 0) {
            return 0;
        }
        if (str1[i] != str2[i]) {
            break;
        }
    }

    return 1;
}

int strncmp_(const char *str1, const char *str2, int n)
{
    for (int i = 0; i < n; ++i) {
        if (str1[i] == 0 && str2[i] == 0) {
            return 0;
        }
        if (str1[i] != str2[i]) {
            return 1;
        }
    }

    return 0;
}

unsigned int strlen_(const char *str)
{
    unsigned int len = 0;

    while (*str++) {
        ++len;
    }

    return len;
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
            cur = new_token(TT_PLUS, cur, *p++);
            continue;
        }
        if (*p == '-') {
            cur = new_token(TT_MINUS, cur, *p++);
            continue;
        }
        if (*p == '*') {
            if (*(p+1) == '*') {
                cur = new_token(TT_POWER, cur, *p++);
                cur = new_token(TT_POWER, cur, *p++);
                continue;
            }
            cur = new_token(TT_ASTERISK, cur, *p++);
            continue;
        }
        if (*p == '/') {
            cur = new_token(TT_SLASH, cur, *p++);
            continue;
        }
        if (*p == '%') {
            cur = new_token(TT_PERCENT, cur, *p++);
            continue;
        }
        if (*p == '!') {
            cur = new_token(TT_FACT, cur, *p++);
            continue;
        }
        if (*p == '&') {
            cur = new_token(TT_AND, cur, *p++);
            continue;
        }
        if (*p == '|') {
            cur = new_token(TT_OR, cur, *p++);
            continue;
        }
        if (*p == '^') {
            cur = new_token(TT_XOR, cur, *p++);
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

        fprintf(stderr, "Non expected: \"%c\"\n", *p++);
        return NULL;
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
 *  <expr>    ::= <add> ("&"|"|"|"^" <add>)*
 *  <add>     ::= <mul> ("+" <mul> | "-" <mul>)*
 *  <mul>     ::= <unary> ("*"|"**"|"/"|"%" <unary>)*
 *  <unary>   ::= ('+' | '-')? <fact>
 *  <fact>    ::= <primary> '!'?
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
static Node *add(void);
static Node *mul(void);
static Node *unary(void);
static Node *fact(void);
static Node *primary(void);

static Node *expr(void) {
    Node *node = add();

    for (;;) {
        if (consume('&')) {
            node = new_binary(TT_AND, node, add());
        } else if (consume('|')) {
            node = new_binary(TT_OR, node, add());
        } else if (consume('^')) {
            node = new_binary(TT_XOR, node, add());
        } else {
            return node;
        }
    }
}

static Node *add(void) {
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
            if (consume('*')) {
                node = new_binary(TT_POWER, node, unary());
            } else {
                node = new_binary(TT_ASTERISK, node, unary());
            }
        } else if (consume('/')) {
            node = new_binary(TT_SLASH, node, unary());
        } else if (consume('%')) {
            node = new_binary(TT_PERCENT, node, unary());
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
        return fact();
    }
}

static Node *fact(void) {
    Node *node = primary();
    if (consume('!')) {
        return new_binary(TT_FACT, new_num(0), node);
    }
    return node;
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
        exit(1);
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

    int i = pop(); // Right
    int j = pop(); // Lift

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
        if (i == 0) {
            fprintf(stderr, "Divide by Zero\n");
            exit(1);
        }
        push(j / i);
        break;
    case TT_PERCENT:
        if (i == 0) {
            fprintf(stderr, "Divide by Zero\n");
            exit(1);
        }
        push(j % i);
        break;
    case TT_POWER:
        push(pow_(j, i));
        break;
    case TT_FACT:
        push(factorial(i));
        break;
    case TT_AND:
        push(i&j);
        break;
    case TT_OR:
        push(i|j);
        break;
    case TT_XOR:
        push(i^j);
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

    if (!strcmp_(argv[1], "repl")) {
        char input[128];
        printf("Wellcome to my calculator REPL!\n");
        printf("Type `quit` to exit\n\n");
        while (1) {
            printf(">> ");
            fgets(input, 127, stdin);
            if (strlen_(input) == 1) {
                continue;
            }
            if (!strncmp_(input, "quit", 4)) {
                return 0;
            }
            token = tokenize(input);
            if (token) {
                Node *node = expr();
                gen(node);
                printf("%d\n", pop());
            }
        }
    } else {
        token = tokenize(argv[1]);
        if (token) {
            Node *node = expr();
            gen(node);
        } else {
            fprintf(stderr, "Failed tokenize\n");
            return 1;
        }
    }

    return pop();
}

