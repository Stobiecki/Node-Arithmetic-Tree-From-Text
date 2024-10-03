#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "Node.h"

using std::cout;
using std::endl;


template<class T>
class VariableNode : public Node<T>
{
public:
	std::string name;
	T *variablePtr = nullptr;

	VariableNode() = default;

	VariableNode(std::string _name, T *val) : Node<T>()
	{
		name = _name;
		variablePtr = val;
		this->value = *val;
	}
	T Calculate() override
	{
		return *variablePtr;
	}
};