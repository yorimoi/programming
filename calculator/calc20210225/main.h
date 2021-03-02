#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TK_Plus,   // '+'
    TK_Minus,  // '-'
    TK_Asta,   // '*'
    TK_Slash,  // '/'
    TK_Parce,  // '%'
    TK_LParen, // '('
    TK_RParen, // ')'
    TK_Num,    // '0'..'9'
    TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token {
    Token     *next;
    TokenKind kind;
    int       val;
};

typedef enum {
    AK_Add,
    AK_Sub,
    AK_Mul,
    AK_Div,
    AK_Mod,
    AK_Num,
} AstKind;

typedef struct Ast Ast;
struct Ast {
    Ast     *lhs;
    Ast     *rhs;
    AstKind kind;
    int     val;
};

