#include "Function.h"
#include <iostream>

namespace gw {
	Function::Function(std::string function) : m_functionTreeRoot(nullptr) {
		m_functionString = function;

		// Reduce function by replacing multi char operator to single char operator
		std::string functionStringReduced = function;
		const std::list<std::pair<std::string, std::string>> replacements = {{"abs", "a"}, {"exp", "e"}, {"ln", "l"}, {"max", "m"}, {"min", "n"}, {"sin", "s"}, {"cos", "c"}, {"sqrt", "q"}};
		for (const std::pair<std::string, std::string> replacement : replacements) {
			size_t start_pos = 0;
			while ((start_pos = functionStringReduced.find(replacement.first, start_pos)) != std::string::npos) {
				functionStringReduced.replace(start_pos, replacement.first.length(), replacement.second);
				start_pos += replacement.second.length();
			}
		}

		// Parse function to nodes
		float currentNumberValue = 0;
		int decimalIndex = 0;

		// Implementation of Shunting yard algorithm
		std::stack<FunctionNode> operatorStack;
		std::stack<FunctionNode> outputStack;
		bool nextMinusIsNumber = true;
		for (auto charIterator = functionStringReduced.begin(); charIterator != functionStringReduced.end(); ++charIterator) {
			switch (*charIterator) {
				case ' ':
					break;

				// Functions
				case 'a':
					operatorStack.push(NodeType::AbsoluteValue);
					assertNextOperatorIsLeftParenthesis(charIterator);
					break;
				
				case 'e':
					operatorStack.push(NodeType::Explonential);
					assertNextOperatorIsLeftParenthesis(charIterator);
					break;
				
				case 'l':
					operatorStack.push(NodeType::Logarithm);
					assertNextOperatorIsLeftParenthesis(charIterator);
					break;
				
				case 'q':
					operatorStack.push(NodeType::SquaredRoot);
					assertNextOperatorIsLeftParenthesis(charIterator);
					break;

				case 'c':
					operatorStack.push(NodeType::Cosinus);
					assertNextOperatorIsLeftParenthesis(charIterator);
					break;

				case 's':
					operatorStack.push(NodeType::Sinus);
					assertNextOperatorIsLeftParenthesis(charIterator);
					break;

				case 'n':
					operatorStack.push(NodeType::Min);
					assertNextOperatorIsLeftParenthesis(charIterator);
					operatorStack.push(NodeType::LeftParenthesis); // To ensure pre-comma expression is grouped
					break;

				case 'm':
					operatorStack.push(NodeType::Max);
					assertNextOperatorIsLeftParenthesis(charIterator);
					operatorStack.push(NodeType::LeftParenthesis); // To ensure pre-comma expression is grouped
					break;

				// Operators
				case '+':
					parseOperator(NodeType::Add, operatorStack, outputStack);
					nextMinusIsNumber = true;
					break;

				case '-':
					while (*(charIterator+1) == ' ') {
						charIterator++;
					}

					if (nextMinusIsNumber) {
						// Do "*-1"
						outputStack.push(FunctionNode(NodeType::Number, -1));
						parseOperator(NodeType::Multiply, operatorStack, outputStack);
					} else {
						// Do the two params substraction
						parseOperator(NodeType::Substract, operatorStack, outputStack);
					}
					nextMinusIsNumber = true;
					break;

				case '*':
					parseOperator(NodeType::Multiply, operatorStack, outputStack);
					nextMinusIsNumber = true;
					break;

				case '/':
					parseOperator(NodeType::Divide, operatorStack, outputStack);
					nextMinusIsNumber = true;
					break;

				case '%':
					parseOperator(NodeType::Modulo, operatorStack, outputStack);
					nextMinusIsNumber = true;
					break;

				case '^':
					parseOperator(NodeType::Power, operatorStack, outputStack);
					nextMinusIsNumber = true;
					break;

				// Parentheses
				case '(':
					operatorStack.push(NodeType::LeftParenthesis);
					nextMinusIsNumber = true;
					break;

				case ',':
					// Handled as a right parenthesis to close min and max function left parenthesis
				case ')':
					while (!operatorStack.empty() && operatorStack.top().m_type != NodeType::LeftParenthesis) {
						outputStack.push(operatorStack.top());
						operatorStack.pop();
					}
					
					if (operatorStack.empty()) {
						throw std::invalid_argument("Parentheses number missmatch");
					}

					operatorStack.pop();
					for (const NodeType nodeType : {AbsoluteValue, Explonential, Logarithm, SquaredRoot, Min, Max}) {
						if (operatorStack.top().m_type == nodeType) {
							outputStack.push(operatorStack.top());
							operatorStack.pop();
							break;
						}
					}
					nextMinusIsNumber = false;

					break;
				
				// Number or the unknown
				case 'x':
					nextMinusIsNumber = false;
					outputStack.push(FunctionNode(NodeType::Unknown));
					break;

				default:
					while (std::isdigit(*charIterator) || *charIterator == '.') {
						if (std::isdigit(*charIterator)) {
							if (decimalIndex == 0) {
								currentNumberValue = currentNumberValue * 10 + (*charIterator - '0');
							} else {
								currentNumberValue = currentNumberValue + (*charIterator - '0') / pow(10, decimalIndex);
								decimalIndex++;
							}
						} else if (*charIterator == '.') {
							decimalIndex = 1;
						}
						
						charIterator++;
					}
					nextMinusIsNumber = false;
					charIterator--;

					outputStack.push(FunctionNode(NodeType::Number, currentNumberValue));
					currentNumberValue = 0;
					decimalIndex = 0;

					break;
			}
		}

		while (!operatorStack.empty()) {
			const FunctionNode node = operatorStack.top();
			if (node.m_type == NodeType::LeftParenthesis) {
				throw std::invalid_argument("Parentheses number missmatch");
			}
			outputStack.push(operatorStack.top());
			operatorStack.pop();
		}

		// Transform stack into the node tree
		m_functionTreeRoot = transformFunctionTree(outputStack);
	}

	FunctionNode* Function::transformFunctionTree(std::stack<FunctionNode> &stack) {
		if (stack.empty()) {
			throw std::invalid_argument("End of operator stack while missing child in tree");
		}

		const FunctionNode imcompleteNode = stack.top();
		stack.pop();

		FunctionNode* node = nullptr;
		switch (imcompleteNode.m_type) {
			// Two child
			case Multiply:
			case Divide:
			case Add:
			case Substract:
			case Modulo:
			case Max:
			case Min:
			case Power:
				return new FunctionNode(imcompleteNode.m_type, transformFunctionTree(stack), transformFunctionTree(stack));

			// One child
			case AbsoluteValue:
			case Explonential:
			case Logarithm:
			case SquaredRoot:
			case Sinus:
			case Cosinus:
				return new FunctionNode(imcompleteNode.m_type, transformFunctionTree(stack));

			// No child
			case Number:
			case Unknown:
				return new FunctionNode(imcompleteNode.m_type, imcompleteNode.m_value);

			default:
				throw std::invalid_argument("Unhandled operator found");
		}
	}

	void Function::parseOperator(NodeType type, std::stack<FunctionNode> &operatorStack, std::stack<FunctionNode> &outputStack) {
		while (!operatorStack.empty()) {
			if (operatorStack.top().m_type == NodeType::LeftParenthesis) {
				break;
			}

			if (getOperatorPrecedence(operatorStack.top().m_type) < getOperatorPrecedence(type)) {
				break;
			}

			if (getOperatorPrecedence(operatorStack.top().m_type) == getOperatorPrecedence(type) && type == NodeType::Power) {
				break;
			}

			outputStack.push(operatorStack.top());
			operatorStack.pop();
		}

		operatorStack.push(type);
	}

	void Function::assertNextOperatorIsLeftParenthesis(std::string::iterator charIterator) {
		while (*(charIterator+1) == ' ') {
			charIterator++;
		}

		if (*(charIterator+1) != '(') {
			throw std::invalid_argument("Missing opening parenthesis after an operator");
		}
	}

	size_t Function::getOperatorPrecedence(NodeType type) {
		switch (type) {
			case NodeType::Power:
				return 3;
			case NodeType::Multiply:
			case NodeType::Divide:
			case NodeType::Modulo:
				return 2;
			case NodeType::Add:
			case NodeType::Substract:
				return 1;
			default:
				// Sin, Cos, Abs, Sqrt, ...
				return 4;
		}
	}

	float Function::evaluate(float x) const {
		return evaluate(m_functionTreeRoot, x);
	}

	float Function::evaluate(const FunctionNode* node, float x) const {
		float leftValue = 0;
		if (node->m_leftParam != nullptr) {
			leftValue = evaluate(node->m_leftParam, x);
		}

		float rightValue = 0;
		if (node->m_rightParam != nullptr) {
			rightValue = evaluate(node->m_rightParam, x);
		}

		switch (node->m_type) {
			// Two child
			case Multiply:
				return leftValue * rightValue;
			case Divide:
				return leftValue / rightValue;
			case Add:
				return leftValue + rightValue;
			case Substract:
				return leftValue - rightValue;
			case Modulo:
				return std::fmod(leftValue, rightValue);
			case Max:
				return (leftValue > rightValue) ? leftValue : rightValue;
			case Min:
				return (leftValue < rightValue) ? leftValue : rightValue;
			case Power:
				return pow(leftValue, rightValue);

			// One child
			case AbsoluteValue:
				return abs(leftValue);
			case Explonential:
				return exp(leftValue);
			case Logarithm:
				return log(leftValue);
			case SquaredRoot:
				return sqrt(leftValue);
			case Sinus:
				return sin(leftValue);
			case Cosinus:
				return cos(leftValue);

			// No child
			case Number:
				return node->m_value;
			case Unknown:
				return x;

			default:
				throw std::invalid_argument("Unhandled operator found");
		}
	}

	std::string Function::toString() const {
		return m_functionString;
	}
}
