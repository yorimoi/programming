typedef enum {
    NK_ADD,
    NK_SUB,
    NK_MUL,
    NK_DIV,
    NK_MOD,
    NK_NUM,
} NodeKind;

typedef struct Ast Ast;
struct Ast {
    Ast      *lhs;
    Ast      *rhs;
    NodeKind kind;
    int      val;
};

