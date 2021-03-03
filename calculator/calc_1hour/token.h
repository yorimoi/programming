typedef enum {
    TK_PLUS,      // '+'
    TK_MINUS,     // '-'
    TK_ASTERISK,  // '*'
    TK_SLASH,     // '/'
    TK_PARCENT,   // '%'
    TK_NUM,       // [0-9][0-9]*
    TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token {
    Token     *next;
    TokenKind kind;
    int       val;
};

