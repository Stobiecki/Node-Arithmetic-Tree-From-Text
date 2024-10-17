#include "FunctionNode.h"

// Explicitly instantiate the template for double
template class FunctionNode<double>;

// Define the static map for the template class
template <class T>
std::map<std::tuple<std::string, int>, typename FunctionNode<T>::FunctionVariant> FunctionNode<T>::functions;

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
}

template <class T>
double Sin(T a)
{
    return std::sin(a);
}

// Define the Init method for FunctionNode<T>
template <class T>
void FunctionNode<T>::Init() 
{
    // Insert the "pow" function into the map with correct types
    functions[{ "pow", 2 }] = [](T x, T y) { return std::pow(x, y); };
    functions[{ "root", 2 }] = [](T x, T y) {
        if ((std::fmod(x, 2.0) != 0) || (std::fmod(x, 2.0) == 0 && y >= 0))
        {
            return std::pow(y, 1.0/x);
        }
        else
            return std::nan("");
    };
    functions[{ "add", 2 }] = [](T x, T y) { return x+y; };
    functions[{ "minus", 2 }] = minus<double>;

    functions[{ "Sin", 1 }] = Sin<double>;
    functions[{ "sin", 1 }] = static_cast<double(*)(double)>(std::sin);

    functions[{ "cos", 1 }] = static_cast<double(*)(double)>(std::cos);
    functions[{ "asin", 1 }] = static_cast<double(*)(double)>(std::asin);
    functions[{ "acos", 1 }] = static_cast<double(*)(double)>(std::acos);
}