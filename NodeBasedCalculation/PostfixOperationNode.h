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
class PostfixOperationNode : public OperationNode<T>
{
public:
	static std::map<std::string, std::function<T(T)>> operators;
	static void Init();

	PostfixOperationNode(std::function<T(T)>* func, std::shared_ptr<Node<T>> leafNode) :
		postfixOperation(func), leaf(std::move(leafNode)) {}

	PostfixOperationNode(std::function<T(T)>* func) : postfixOperation(func) {}

	T Calculate() override
	{
		//if (!this->isInvalid)
		//	return this->value;

		//this->isInvalid = false;
		return this->value = (*postfixOperation)(leaf.get()->Calculate());
	}

	void markInvalid() override
	{
		//this->isInvalid = true;
		//leaf->markInvalid();
	}

	void SetLeafsNodes(std::vector<std::shared_ptr<Node<double>>>& Nodes);

	std::function<double(double)>* postfixOperation;
	std::shared_ptr<Node<T>> leaf;
};