#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Node.h"
#include "FunctionCreator.h"
#include "Expression.h"
#include "CustomFunction.h"

template <class T>
class Expression;

template<class T>
class CustomFunctionArgumentNode : public Node<T>
{
public:
	std::string name;
	int argPos;
	std::shared_ptr<Node<T>> providedArg = nullptr;
	std::vector<std::shared_ptr<Node<T>>>* argHandle;
	bool argHandleSet = false;
	Expression<T> *expressionHook;

	CustomFunctionArgumentNode(std::string _name, int _argPos, Expression<T>* expr);

	T Calculate() override
	{
		if (!argHandleSet)
		{
			argHandle = &(expressionHook->cfn->sections);
			argHandleSet = true;
		}
		return (*argHandle)[argPos-1].get()->Calculate();
	}
};