#include "SectionNode.h"

template<typename T> class SectionNode;

using NodeTupleTemplate = std::tuple<std::string, size_t, size_t, NodeType>;
using FunctionTupleTemplate = std::tuple<std::string, size_t, size_t, int, std::vector<std::pair<int, int>>, std::string>;


SectionNode<double>::SectionNode(std::string str, size_t _start, size_t _len) : Node<double>()
{
	section = str;
	sectionStart = _start;
	len = _len;
}

std::shared_ptr<Node<double>> SectionNode<double>::ComplementEmptySection(
	std::vector<NodeTupleTemplate>& _preNodeHolder,
	std::vector<std::shared_ptr<Node<double>>>& _nodeHolder,
	std::vector<FunctionTupleTemplate>& _functions
)
{
	auto [_template, _nodes] = Expression<double>::ComplementEmptyNodes(_preNodeHolder, _nodeHolder, _functions);

	templateNodes = _template;
	unorderedNodes = _nodes;

	//calculate + rearenge nodes
	root = Expression<double>::RearrangeToGetRootNode(unorderedNodes);

	isReady = true;

	return root;
}