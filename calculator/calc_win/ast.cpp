#include "ast.h"

// new node
Ast::Ast(NodeKind kind, Ast* rhs, Ast* lhs) {
	this->kind = kind;
	this->rhs  = rhs;
	this->lhs  = lhs;
	this->val  = 0;
}

// new node number
Ast::Ast(int val) {
	this->kind = NodeKind::Num;
	this->rhs  = nullptr;
	this->lhs  = nullptr;
	this->val  = val;
}

