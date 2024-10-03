#pragma once

#include <iostream>
#include <vector>

#include "Node.h"

using std::cout;
using std::endl;

template<class T>
class ValueNode : public Node<T>
{
public:
	ValueNode() = default;

	ValueNode(T val) : Node<T>()
	{
		this->value = val;
	}
	T Calculate() override
	{
		return this->value;
	}
};