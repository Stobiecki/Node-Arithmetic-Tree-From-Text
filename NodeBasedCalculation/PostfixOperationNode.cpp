#include "PostfixOperationNode.h"

template class PostfixOperationNode<double>;

std::map<std::string, std::function<double(double)>> PostfixOperationNode<double>::operators;

template<>
void PostfixOperationNode<double>::SetLeafsNodes(std::vector<std::shared_ptr<Node<double>>>& Nodes)
{
	if (Nodes.size() < 1)
		throw std::runtime_error("\nIncorrect no. of args for Sufix operator.");
	leaf = std::move(Nodes[0]);
}

template<>
void PostfixOperationNode<double>::Init()
{
	operators["++"] = [](double a) { return a++; };
	operators["--"] = [](double a) { return a--; };
	operators["!"] = [](double a) {
		if (a < 0)
			return std::nan("");
		int _a = a, result = 1;
		for (int i = 1; i <= _a; i++)
			result *= i;
		return double(result);
	};
	operators["?"] = [](double a) {
		if (a < 0)
			return std::nan("");
		int _a = a, result = 0;
		for (int i = 1; i <= _a; i++)
			result += i;
		return double(result);
	};
}
