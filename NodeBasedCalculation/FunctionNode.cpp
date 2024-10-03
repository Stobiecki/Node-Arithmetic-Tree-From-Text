#include "FunctionNode.h"

// Explicitly instantiate the template for double
template class FunctionNode<double>;

// Define the static map for the template class
template <class T>
std::map<std::tuple<std::string, int>, typename FunctionNode<T>::OperatorVariant> FunctionNode<T>::functions;

template <class T>
T minus(T x, T y)
{
    return x - y;
}

template <class T>
void FunctionNode<T>::ComplementEmptyArgsSection(
    std::vector<std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>>>& _vecOfFuncArgsRanges,
    std::vector<FunctionTupleTemplate>& _functions
)
{
    for (int i=0; i<noOfArgs; i++)
    {
        SectionNode<double> tmpSection("", 0, 0);
        sections.push_back(tmpSection.ComplementEmptySection(_vecOfFuncArgsRanges[i].first, _vecOfFuncArgsRanges[i].second, _functions));
        //sections.push_back(Expression<T>::RearrangeToGetRootNode(_vecOfFuncArgsRanges[i].second));
    }
    areNodesReady = true;
    //TO_DO
    // 
    // Dodanie porz�dkowania sekcji w taki spos�b, �eby mo�naby�o
    // doda� korzenie jako Node w wektorze "sections". Te w�z�y maj�
    // reprezentowa� korzenie ka�dego argumentu funkcji.
    // 
    // Funkcja ta Teoretycznie nie musi nic zwraca� (a je�li ju� to
    // samego siebie)
    // 
    //TO_DO
}


// Define the Init method for FunctionNode<T>
template <class T>
void FunctionNode<T>::Init() 
{
    // Insert the "pow" function into the map with correct types
    functions[{ "pow", 2 }] = [](T x, T y) { return std::pow(x, y); };
    functions[{ "add", 2 }] = [](T x, T y) { return x+y; };
    functions[{ "minus", 2 }] = minus<double>;
}