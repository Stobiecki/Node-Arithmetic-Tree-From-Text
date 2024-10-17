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


using std::cout;
using std::endl;

using NodeTupleTemplate = std::tuple<std::string, size_t, size_t, NodeType>;
using FunctionTupleTemplate = std::tuple<std::string, size_t, size_t, int, std::vector<std::pair<int, int>>, std::string>;


template<class T>
class FunctionNode : public Node<T>
{
public:
	// Define a variant type to store different function signatures (0 up to 8)
	using FunctionVariant = std::variant<
		std::function<T()>,
		std::function<T(T)>,
		std::function<T(T, T)>,
		std::function<T(T, T, T)>,
		std::function<T(T, T, T, T)>,
		std::function<T(T, T, T, T, T)>,
		std::function<T(T, T, T, T, T, T)>,
		std::function<T(T, T, T, T, T, T, T)>,
		std::function<T(T, T, T, T, T, T, T, T)>
	>;

	static std::map<std::tuple<std::string, int>, FunctionVariant> functions;

	// Method to add a function with 0-8 arguments
	static void addFunction(const std::string& name, std::function<T()> func) { functions[{name, 0}] = func; }
	static void addFunction(const std::string& name, std::function<T(T)> func) { functions[{name, 1}] = func; }
	static void addFunction(const std::string& name, std::function<T(T, T)> func) { functions[{name, 2}] = func; }
	static void addFunction(const std::string& name, std::function<T(T, T, T)> func) { functions[{name, 3}] = func; }
	static void addFunction(const std::string& name, std::function<T(T, T, T, T)> func) { functions[{name, 4}] = func; }
	static void addFunction(const std::string& name, std::function<T(T, T, T, T, T)> func) { functions[{name, 5}] = func; }
	static void addFunction(const std::string& name, std::function<T(T, T, T, T, T, T)> func) { functions[{name, 6}] = func; }
	static void addFunction(const std::string& name, std::function<T(T, T, T, T, T, T, T)> func) { functions[{name, 7}] = func; }
	static void addFunction(const std::string& name, std::function<T(T, T, T, T, T, T, T, T)> func) { functions[{name, 8}] = func; }

	static void Init();

	int noOfArgs;
	bool areNodesReady = false;
	std::string funcName;
	std::vector<std::shared_ptr<Node<T>>> sections;
	std::vector<std::shared_ptr<Node<T>>> *sectionsHandle;
	int sectionStart;
	int len;

	std::function<T(std::vector<T>)> func;

	// Constructor for ready Nodes
	FunctionNode(std::string _funcName, int _noOfArgs, std::vector<std::shared_ptr<Node<T>>> _sections, std::function<T(std::vector<T>)> _func)
		: Node<T>(), funcName(_funcName), noOfArgs(_noOfArgs), sections(std::move(_sections)), func(_func)
	{
		areNodesReady = true;
	}

	// Constructor for unready Nodes
	FunctionNode(std::string _funcName, int _noOfArgs, std::vector<std::shared_ptr<Node<T>>>*_sections, std::function<T(std::vector<T>)> _func)
		: Node<T>(), funcName(_funcName), noOfArgs(_noOfArgs), sectionsHandle(_sections), func(_func)
	{
		areNodesReady = false;
	}

	// Constructor for no Nodes at all (pre-tree construction)
	FunctionNode(std::string _funcName, int _noOfArgs, std::function<T(std::vector<T>)> _func, size_t _start, size_t _len)
		: Node<T>(), funcName(_funcName), noOfArgs(_noOfArgs), func(_func), sectionStart(_start), len(_len)
	{
		areNodesReady = false;
	}

	// calculate each section of a function arguments
	std::vector<T> GatherArguments() 
	{
		std::vector<T> args;
		for (auto& section : sections) 
			args.push_back(section->Calculate());  // Call Calculate on each Node
		return args;
	}
	
	// invocing a function
    T invokeFunctionWithArgs(const std::vector<T>& args) 
	{
        return func(args);  // Directly invoke the stored function with the arguments
    }

    T Calculate() override 
	{
		//if (!this->isInvalid)
		//	return this->value;

        std::vector<T> args = GatherArguments();
        if (args.size() != noOfArgs)
            throw std::invalid_argument("Incorrect number of arguments provided to the function.");

		//this->isInvalid = false;
		return this->value = invokeFunctionWithArgs(args);
    }

	// marking sections for recalculations
	void markInvalid() override
	{
		//this->isInvalid = true;
		//for (auto& section : sections)
		//{
		//	section->markInvalid();  // Oznaczamy wszystkie dzieci jako brudne
		//}
	}

	void ComplementEmptyArgsSection(
		std::vector<std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>>>& _vecOfFuncArgsRanges,
		std::vector<FunctionTupleTemplate>& _functions
	);
};