#pragma once
#include <string>

enum class TokenKind : int {
	Null,
	Plus,      // '+'
	Minus,     // '-'
	Asterisk,  // '*'
	Slash,     // '/'
	Parcent,   // '%'
    Num,       // [0-9][0-9]*
    Eof,       // EOF
};

class Token {
public:
	Token*    next;
	TokenKind kind;
	int       val;

	Token();
	Token(int);
	Token(TokenKind);
};

Token* tokenize(const std::string& input);

