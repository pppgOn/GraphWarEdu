#ifndef _Function_
#define _Function_

#include <list>
#include <stack>
#include <string>
#include <cmath>
#include <stack>
#include <stdexcept>

// Factor from seconds to function advancement (example: 2 means advance by 2 on the graph each seconds) 
#define FUNCTION_RESOLUTION_TIME_FACTOR 3

namespace gw {
	enum NodeType { Number, Unknown, Multiply, Divide, Add, Substract, AbsoluteValue, Explonential, Logarithm, SquaredRoot, Modulo, Power, Max, Min, Sinus, Cosinus, LeftParenthesis, RightParenthesis };

	struct FunctionNode {
		FunctionNode(NodeType type) : m_type(type), m_value(0), m_leftParam(nullptr), m_rightParam(nullptr) {};
		FunctionNode(NodeType type, float value) : m_type(type), m_value(value), m_leftParam(nullptr), m_rightParam(nullptr) {};
		FunctionNode(NodeType type, FunctionNode *leftParam) : m_type(type), m_value(0), m_leftParam(leftParam), m_rightParam(nullptr) {};
		FunctionNode(NodeType type, FunctionNode *leftParam, FunctionNode *rightParam) : m_type(type), m_value(0), m_leftParam(leftParam), m_rightParam(rightParam) {};
		NodeType m_type;
		float m_value;
		FunctionNode *m_leftParam;
		FunctionNode *m_rightParam;
	};


	class Function{
		public:
			Function(std::string function);
			~Function();
			FunctionNode* transformFunctionTree(std::stack<FunctionNode> &stack);
			float evaluate(float x) const;
			std::string toString() const;

		private:
			static size_t getOperatorPrecedence(NodeType type);
			static void parseOperator(NodeType type, std::stack<FunctionNode> &operatorStack, std::stack<FunctionNode> &outputStack);
			float evaluate(const FunctionNode* node, float x) const;

			std::string m_functionString;

			FunctionNode *m_functionTreeRoot;
	};
}

#endif
