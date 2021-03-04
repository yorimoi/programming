#pragma once

enum class NodeKind {
	Add,  // '+'
	Sub,  // '-'
	Mul,  // '*'
	Div,  // '/'
	Mod,  // '%'
	Num,  // ®”
};

class Ast {
public:
	NodeKind kind;
	Ast*     rhs;
	Ast*     lhs;
	int      val;

	// new node
	Ast(NodeKind kind, Ast* rhs, Ast* lhs);
	// new node number
	Ast(int val);
};

