#include <iostream>
#include <stack>

#include "calculator.h"

void calculation(Ast* node, std::stack<int>& stack) {
	if (node->kind == NodeKind::Num) {
		stack.push(node->val);
		return;
	}

	calculation(node->lhs, stack);
	calculation(node->rhs, stack);

	switch (node->kind) {
		case NodeKind::Add: {
			int i = stack.top(); stack.pop();
			int j = stack.top(); stack.pop();
			stack.push(i + j);
			break;
		}

		case NodeKind::Sub: {
			int i = stack.top(); stack.pop();
			int j = stack.top(); stack.pop();
			stack.push(i - j);
			break;
		}

		case NodeKind::Mul: {
			int i = stack.top(); stack.pop();
			int j = stack.top(); stack.pop();
			stack.push(i * j);
			break;
		}

		case NodeKind::Div: {
			int i = stack.top(); stack.pop();
			int j = stack.top(); stack.pop();
			stack.push(i / j);
			break;
		}

		case NodeKind::Mod: {
			int i = stack.top(); stack.pop();
			int j = stack.top(); stack.pop();
			stack.push(i % j);
			break;
		}

		default:
			std::cerr << __FILE__ << ":" << __LINE__ << ": unreachable" << std::endl;
			exit(1);
	}
}

