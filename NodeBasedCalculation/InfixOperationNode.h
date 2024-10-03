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
class InfixOperationNode : public OperationNode<T>
{
public:
	static std::map<std::string, std::function<T(T, T)>> operators;
	static std::map<std::string, int> precedences;
	static void Init();

	// basic construcor - moves all the resources and takes ownership of all the Nodes
	InfixOperationNode(std::function<T(T, T)>* func, std::shared_ptr<Node<T>> leftLeafNode, std::shared_ptr<Node<T>> rightLeafNode) :
		infixOperation(func), leftLeaf(std::move(leftLeafNode)), rightLeaf(std::move(rightLeafNode)) {}

	// restricted constructor - only provide function (all the nodes have to be provided later)
	InfixOperationNode(std::function<T(T, T)>* func, int _precedence = 3) : infixOperation(func), precedence(_precedence)
	{
		if (_precedence > 3)
			precedence = 3;
		else if (_precedence < 1)
			precedence = 1;
	}

	T Calculate() override
	{
		//if (!this->isInvalid)
		//	return this->value;

		//this->isInvalid = false;
		return this->value = (*infixOperation)(leftLeaf.get()->Calculate(), rightLeaf.get()->Calculate());
	}

	void markInvalid() override
	{
		//this->isInvalid = true;
		//leftLeaf->markInvalid();
		//rightLeaf->markInvalid();
	}

	void SetLeafsNodes(std::vector<std::shared_ptr<Node<double>>>& Nodes);

	std::function<double(double, double)>* infixOperation;
	std::shared_ptr<Node<T>> leftLeaf;
	std::shared_ptr<Node<T>> rightLeaf;
	int precedence = 3;
};