#include "ast.h"
#include "parser.h"
#include "token.h"

static Ast* expr(Token* token);
static Ast* add(Token* token);
static Ast* mul(Token* token);
static Ast* term(Token* token);

// program = expr*
Ast* gen_ast(Token* token) {
	return expr(token);
}

// expr = add+
static Ast* expr(Token* token) {
	return add(token);
}

// add = mul ('+' mul | '-' mul)*
static Ast* add(Token* token) {
	Ast* node = mul(token);
	while (1) {
		if (token->kind == TokenKind::Plus) {
			*token = *token->next;
			node = new Ast(NodeKind::Add, node, mul(token));
		} else if (token->kind == TokenKind::Minus) {
			*token = *token->next;
			node = new Ast(NodeKind::Sub, node, mul(token));
		} else {
			return node;
		}
	}
}

// mul = term ('*' term | '/' term | '%' term)*
static Ast* mul(Token* token) {
	Ast* node = term(token);
	while (1) {
		if (token->kind == TokenKind::Asterisk) {
			*token = *token->next;
			node = new Ast(NodeKind::Mul, node, term(token));
		} else if (token->kind == TokenKind::Slash) {
			*token = *token->next;
			node = new Ast(NodeKind::Div, node, term(token));
		} else if (token->kind == TokenKind::Parcent) {
			*token = *token->next;
			node = new Ast(NodeKind::Mod, node, term(token));
		} else {
			return node;
		}
	}
}

// term = [0-9][0-9]*
static Ast* term(Token* token) {
	int val = token->val;
	*token = *token->next;
	return new Ast(val);
}

