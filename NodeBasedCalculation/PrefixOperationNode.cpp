#include "PrefixOperationNode.h"

template class PrefixOperationNode<double>;

std::map<std::string, std::function<double(double)>> PrefixOperationNode<double>::operators;

template<>
void PrefixOperationNode<double>::SetLeafsNodes(std::vector<std::shared_ptr<Node<double>>>& Nodes)
{
	if (Nodes.size() < 1)
		throw std::runtime_error("\nIncorrect no. of args for Prefix operator.");
	leaf = std::move(Nodes[0]);
}

template<>
void PrefixOperationNode<double>::Init()
{
	operators["-"] = [](double a) { return -a; };
	operators["--"] = [](double a) { return --a; };
	operators["++"] = [](double a) { return ++a; };
}

