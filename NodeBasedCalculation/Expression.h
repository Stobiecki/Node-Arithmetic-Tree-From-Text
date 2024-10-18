#pragma once

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <memory>
#include <stack>
#include <functional>
#include <variant>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <chrono>

#include "StaticData.h"

#include "Enums.h"
#include "Node.h"
#include "FunctionNode.h"
#include "PrefixOperationNode.h"
#include "InfixOperationNode.h"
#include "PostfixOperationNode.h"
#include "ValueNode.h"
#include "VariableNode.h"
#include "SectionNode.h"
#include "CustomFunction.h"
#include "CustomFunctionArgumentNode.h"

using NodeTupleTemplate = std::tuple<std::string, size_t, size_t, NodeType>;
using FunctionTupleTemplate = std::tuple<std::string, size_t, size_t, int, std::vector<std::pair<int, int>>, std::string>;

template <class T>
class CustomFunctionNode;

// Main class that functions as a semi-root-node for the specific expression
template <class T>
class Expression
{
private:
	// Helper function to find the closing tag that matches an opening tag
	std::string findClosingTag(const std::string& str, size_t& pos);
	// Helper function to check for function names and variable names
	bool isPartOfFunctionOrVariable(char c);
	// Helper function to check for digit
	bool isDigit(char c);
	// Helper function to check for digit or number
	bool isDigitNumber(char c);
	// Helper function to check for dot (decimal point)
	bool isDot(char c);
	// Helper function to check for comma (function argument)
	bool isComma(char c);
	void shiftCursorCleanup(size_t& val, int shift, std::string& strToReset);
	bool canBePrefix(const std::string& str, int pos);
	bool canBePostfix(
		const std::string& str,
		int pos,
		std::map<std::string, std::function<double(double)>>& postfixOperation
	);
	// Helper function to check for functions_
	bool isOrCouldBeOperator(
		char c,
		std::map<std::string, std::function<double(double)>>& prefixOperation,
		std::map<std::string, std::function<double(double, double)>>& infixOperation,
		std::map<std::string, std::function<double(double)>>& postfixOperation
	);

	OperatorType identifyOperator(
		std::string op,
		std::map<std::string, std::function<double(double)>>& prefixOperation,
		std::map<std::string, std::function<double(double, double)>>& infixOperation,
		std::map<std::string, std::function<double(double)>>& postfixOperation,
		bool couldBePrefix,
		bool couldBePostfix
	);
	void insertInOrder(
		std::vector<NodeTupleTemplate>& vec,
		const NodeTupleTemplate& newElement
	);
	void findAllTokens(const std::string& str);
	// sorting sections (brackets) based on position
	// for more clarity and ease of parsing
	void SortSections();
	// just for clarification sake (could be bloting/slowing the code)
	void SortFunctions();
	void NodeGeneration();

public:
	struct FunctionStack
	{
		int functionNameStart = 0;
		int functionNameEnd = 0;
		int noOfArgs = 0;
		std::vector<std::pair<int, int>> functionArgsPos;
		int functionArgStart = 0;
		int functionArgEnd = 0;

		FunctionStack() {};
	};

	std::string textRepres;
	std::shared_ptr<Node<T>> rootNode;
	bool handleSet = false;

	std::vector<std::tuple<std::string, size_t, size_t>> sections;
	std::vector<std::tuple<std::string, size_t, size_t>> values;
	std::vector<std::tuple<std::string, size_t, size_t>> variables;
	std::vector<FunctionTupleTemplate> functions_;
	std::vector<std::tuple<std::string, size_t, size_t, OperatorType>> operators;
	std::stack<FunctionStack> functionStack;
	// 1 -> +, -
	// 2 -> *, /
	// 3 -> power, root, etc.
	// 4 -> prefix-operators
	// 5 -> postfix-operators
	// 6 -> functions
	// 7 -> brackets
	static std::map<std::pair<std::string,NodeType>, int> precedence;

	// 1) string representatnion
	// 2) position
	// 3) length
	// 4) type
	std::vector<NodeTupleTemplate> preNodeHolder;

	// NodeHolder
	std::vector<std::shared_ptr<Node<double>>> nodeHolder;

	// hook used in cases when expression is used as a custom function 
	CustomFunctionNode<T>* cfn = nullptr;

	Expression(std::string str);

	void PrepareData();
	void PrepareDataWithoutCalculation();
	void LogDataToConsole();

	static void InitPrecedence();
	static std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>> ComplementEmptyNodes(
		std::vector<NodeTupleTemplate> & nodeHolder,
		std::vector<std::shared_ptr<Node<double>>> &preNodeHolder,
		std::vector<FunctionTupleTemplate>& _functions
	);
	static std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>> ExtractRangeOfNodes(
		std::vector<NodeTupleTemplate>& _preNodeHolder,
		std::vector<std::shared_ptr<Node<double>>>& _nodeHolder,
		size_t _start,
		size_t _end
	);
	static int getOperatorPriority(const std::shared_ptr<Node<double>>& node);
	static std::shared_ptr<Node<double>> RearrangeToGetRootNode(
		std::vector<std::shared_ptr<Node<double>>>& _nodeHolder
	);

	T Calculate()
	{
		return rootNode.get()->Calculate();
	}

	T CalculateCustom(CustomFunctionNode<T>* _cfn)
	{
		cfn = _cfn;
		return rootNode.get()->Calculate();
	}
};