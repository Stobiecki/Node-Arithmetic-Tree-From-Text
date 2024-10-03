#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include "OperationNode.h"
#include "Node.h"
#include "ValueNode.h"

using std::cout;
using std::endl;

template<class T>
class PrefixOperationNode : public OperationNode<T>
{
public:
	static std::map<std::string, std::function<T(T)>> operators;
	static void Init();

	PrefixOperationNode(std::function<T(T)>* func, std::shared_ptr<Node<T>> leafNode):
		prefixOperation(func), leaf(std::move(leafNode)) {}

	PrefixOperationNode(std::function<T(T)>* func) : prefixOperation(func) {}

	T Calculate() override
	{
		//if (!this->isInvalid)
		//	return this->value;

		//this->isInvalid = false;
		return this->value = (*prefixOperation)(leaf.get()->Calculate());
	}

	void markInvalid() override
	{
		//this->isInvalid = true;
		//leaf->markInvalid();
	}

	void SetLeafsNodes(std::vector<std::shared_ptr<Node<double>>>& Nodes);

	std::function<double(double)> *prefixOperation;
	std::shared_ptr<Node<T>> leaf;
};
