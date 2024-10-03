#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>

#include "Enums.h"

#include "FunctionNode.h"
#include "PrefixOperationNode.h"
#include "InfixOperationNode.h"
#include "PostfixOperationNode.h"
#include "Expression.h"

template<typename T> class Expression;

class StaticData
{
public:
	static CurrentSymbol state;
	static std::string symbol;

	static std::map<std::string, double> variables;
	static std::vector<std::string> functions;

	static std::map<std::string, std::function<double(double)>> *prefixOperation;
	static std::map<std::string, std::function<double(double, double)>> *infixOperation;
	static std::map<std::string, std::function<double(double)>> *postfixOperation;

	static std::map<OperatorType, std::string> operatorTypeInterpreter;
	static std::map<NodeType, std::string> NodeTypeInterpreter;
	
	static Expression<double>* currentExpression;

	static double NaN_Symbol;

	static void DataInit();
};