#include <stdlib.h>
#include "token.h"
#include "ast.h"

extern Token *token;

Ast *new_node(NodeKind kind, Ast *rhs, Ast *lhs) {
    Ast *node = (Ast *) malloc(sizeof(Ast *));
    node->rhs = rhs;
    node->lhs = lhs;
    node->kind = kind;
    return node;
}

Ast *new_node_num(int val) {
    Ast *node = (Ast *) malloc(sizeof(Ast *));
    node->kind = NK_NUM;
    node->val = val;
    return node;
}

Ast *add();
Ast *mul();
Ast *num();
void consume();

Ast *gen_ast() {
    return add();
}

// add = mul ('+' mul | '-' mul)?
Ast *add() {
    Ast *node = mul();
    while (1) {
        if (token->kind == TK_PLUS) {
            consume();
            node = new_node(NK_ADD, node, mul());
        } else if (token->kind == TK_MINUS) {
            consume();
            node = new_node(NK_SUB, node, mul());
        } else {
            return node;
        }
    }
}

// mul = num ('*' num | '/' num | '%' num)?
Ast *mul() {
    Ast *node = num();
    while (1) {
        if (token->kind == TK_ASTERISK) {
            consume();
            node = new_node(NK_MUL, node, num());
        } else if (token->kind == TK_SLASH) {
            consume();
            node = new_node(NK_DIV, node, num());
        } else if (token->kind == TK_PARCENT) {
            consume();
            node = new_node(NK_MOD, node, num());
        } else {
            return node;
        }
    }
}

// num = [0-9][0-9]*
Ast *num() {
    int val = token->val;
    consume();
    return new_node_num(val);
}

