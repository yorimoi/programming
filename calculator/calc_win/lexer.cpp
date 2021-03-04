#include <iostream>
#include <string>

#include "token.h"

Token::Token() {
	this->next = nullptr;
	this->kind = TokenKind::Null;
	this->val  = NULL;
}
Token::Token(int val) {
	this->next = nullptr;
	this->kind = TokenKind::Num;
	this->val  = val;
}
Token::Token(TokenKind kind) {
	this->next = nullptr;
	this->kind = kind;
	this->val  = NULL;
}

static void invalid_charactor(const char p) {
	std::cerr << "invalid charactor: '" << p << "'" << std::endl;
	exit(1);
}

Token *tokenize(const std::string& input) {
	Token* head = new Token();
	Token* cur = head;

	for (auto p = input.begin(); p != input.end(); ++p) {
		// skip whitespace
		if (isblank(*p)) {
			continue;
		}

		// operator
		if (strchr("+-*/%", *p)) {
			switch (*p) {
				case '+': cur->next = new Token(TokenKind::Plus);     break;
				case '-': cur->next = new Token(TokenKind::Minus);    break;
				case '*': cur->next = new Token(TokenKind::Asterisk); break;
				case '/': cur->next = new Token(TokenKind::Slash);    break;
				case '%': cur->next = new Token(TokenKind::Parcent);  break;
				default: invalid_charactor(*p);
			}
			cur = cur->next;
			continue;
		}

		// number
		if (isdigit(*p)) {
			int val = *p - '0';
			while (++p != input.end() && isdigit(*p)) {
				val = val * 10 + *p - '0';
			}
			cur = cur->next = new Token(val);
			--p;
			continue;
		}

		invalid_charactor(*p);
	}

	cur->next = new Token(TokenKind::Eof);
	return head->next;
}

