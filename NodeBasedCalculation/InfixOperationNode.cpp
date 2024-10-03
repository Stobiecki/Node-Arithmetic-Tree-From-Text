#include "InfixOperationNode.h"
#include <type_traits>

template class InfixOperationNode<double>;

std::map<std::string, std::function<double(double, double)>> InfixOperationNode<double>::operators;
std::map<std::string, int> InfixOperationNode<double>::precedences;

template<>
void InfixOperationNode<double>::SetLeafsNodes(std::vector<std::shared_ptr<Node<double>>>& Nodes)
{
	if (Nodes.size() < 2)
		throw std::runtime_error("\nIncorrect no. of args for Infix operator.");

	leftLeaf = std::move(Nodes[0]);
	rightLeaf = std::move(Nodes[1]);
}

template<>
void InfixOperationNode<double>::Init()
{
	operators["+"] = [](double a, double b) { return a + b; };
	operators["-"] = [](double a, double b) { return a - b; };
	operators["*"] = [](double a, double b) { return a * b; };
	operators["/"] = [](double a, double b) { return b != 0 ? a / b : std::nan(""); };
	operators["%"] = [](double a, double b) { return (int)a % (int)b; };
	operators["^"] = [](double a, double b) { return pow(a,b); };
	operators["//"] = [](double a, double b) { return (int)a / (int)b; };

	precedences["+"] = 1;
	precedences["-"] = 1;
	precedences["*"] = 2;
	precedences["/"] = 2;
	precedences["^"] = 3;
	precedences["%"] = 3;
	precedences["//"] = 3;
}