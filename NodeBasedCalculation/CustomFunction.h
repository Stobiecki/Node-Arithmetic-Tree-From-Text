#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <variant>
#include <memory>
#include <cmath>

#include "Enums.h"
#include "ValueNode.h"
#include "Expression.h"
#include "SectionNode.h"
#include "FunctionNode.h"
#include "FunctionCreator.h"

// custom args for custom func are 
template<class T>
class CustomFunctionNode : public Node<T>
{
public:
	using RestrictionType = std::variant<
		std::function<T()>,					// e.g. ltz(T a)
		std::tuple<std::function<T(T)>, T>	// e.g. lt(T a, T b)
	>;

	int noOfArgs;
	bool areNodesReady = false;
	std::string funcName;
	std::vector<std::shared_ptr<Node<T>>> sections;
	int sectionStart;
	int len;
	
	std::function<T(CustomFunctionNode<T>*)> func;

	// map, that under specific no. corresponding to arg position,
	// contains vector that hold all restrictions for specific arg.
	// Restrictions here are composed of functions from FunctionCreator class
	std::map<int, std::vector<RestrictionType>> restrictions;

	// Constructor for no Nodes at all (pre-tree construction)
	CustomFunctionNode(std::string _funcName, int _noOfArgs, std::function<T(CustomFunctionNode<double>*)> _func, size_t _start, size_t _len)
		: Node<T>(), funcName(_funcName), noOfArgs(_noOfArgs), func(_func), sectionStart(_start), len(_len)
	{
		areNodesReady = false;
	}

	T Calculate() override
	{
		//return func({5.5});
		return func(this);
	}

	void ComplementEmptyArgsSection(
		std::vector<std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>>>& _vecOfFuncArgsRanges,
		std::vector<FunctionTupleTemplate>& _functions
	);
};