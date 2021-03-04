#include <iostream>
#include <stack>

#include "ast.h"
#include "calculator.h"
#include "parser.h"
#include "token.h"

/* EBNF
    expr = add+
    add  = mul ('+' mul | '-' mul)*
    mul  = term ('*' term | '/' term | '%' term)*
    term = 0 | ([1-9][0-9]*)
 */

int main() {
	const std::string input = "3 + 42 - 123 * 2";
	std::stack<int> stack;

	// Tokenize
	Token* token = tokenize(input);

	// Generate AST
	Ast* node = gen_ast(token);

	// Calculation
	calculation(node, stack);

	// Result
	std::cout << input << " = " << stack.top() << std::endl;
}

