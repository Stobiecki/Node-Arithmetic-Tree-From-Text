#include "CustomFunction.h"

template class CustomFunctionNode<double>;

template <class T>
void CustomFunctionNode<T>::ComplementEmptyArgsSection(
	std::vector<std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>>>& _vecOfFuncArgsRanges,
	std::vector<FunctionTupleTemplate>& _functions
)
{
	for (int i = 0; i < noOfArgs; i++)
	{
		SectionNode<double> tmpSection("", 0, 0);
		sections.push_back(tmpSection.ComplementEmptySection(_vecOfFuncArgsRanges[i].first, _vecOfFuncArgsRanges[i].second, _functions));
	}
	areNodesReady = true;
}