#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Node.h"
#include "Expression.h"

using NodeTupleTemplate = std::tuple<std::string, size_t, size_t, NodeType>;
using FunctionTupleTemplate = std::tuple<std::string, size_t, size_t, int, std::vector<std::pair<int, int>>, std::string>;


template<class T>
class SectionNode : public Node<T>
{
public:
	bool isReady = false;
	int sectionStart;
	int len;
	std::string section;
	std::vector<std::shared_ptr<Node<T>>> unorderedNodes;
	std::vector<NodeTupleTemplate> templateNodes;
	std::shared_ptr<Node<T>> root;

	SectionNode() = delete;

	SectionNode(std::string str, size_t _start, size_t _len);

	std::shared_ptr<Node<T>> ComplementEmptySection(
		std::vector<NodeTupleTemplate>& _preNodeHolder,
		std::vector<std::shared_ptr<Node<double>>>& _nodeHolder,
		std::vector<FunctionTupleTemplate>& _functions
	);

	T Calculate()
	{
		return this->root.get()->Calculate();
	}
};