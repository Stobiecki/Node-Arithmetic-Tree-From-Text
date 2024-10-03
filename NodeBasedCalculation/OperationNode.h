#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Node.h"

using std::cout;
using std::endl;

template<class T>
class OperationNode : public Node<T>
{
public:
	virtual ~OperationNode() override;
	virtual void SetLeafsNodes(std::vector<std::unique_ptr<Node<T>>>& Nodes) {}; // can be purely virtual
};