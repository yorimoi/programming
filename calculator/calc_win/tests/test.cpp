#include "pch.h"

#include "../ast.h"
#include "../calculator.h"
#include "../parser.h"
#include "../token.h"

static int run(const std::string& input) {
	std::stack<int> stack;

	Token* token = tokenize(input);
	Ast* node = gen_ast(token);
	calculation(node, stack);

	return stack.top();
}

TEST(TestCaseName, TestName) {
	EXPECT_EQ( 0, run("0"));
	EXPECT_EQ(42, run("42"));
	EXPECT_EQ(21, run("5+20-4"));
	EXPECT_EQ(47, run("5+6*7"));
	EXPECT_EQ(-3, run("1+2*3/4-5%6"));
	EXPECT_EQ(41, run(" 12 + 34 - 5 "));
}

