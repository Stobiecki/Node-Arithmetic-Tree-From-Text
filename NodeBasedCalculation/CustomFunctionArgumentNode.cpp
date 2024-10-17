#include "CustomFunctionArgumentNode.h"

template class CustomFunctionArgumentNode<double>;

template <class T>
CustomFunctionArgumentNode<T>::CustomFunctionArgumentNode(std::string _name, int _argPos, Expression<T>* _expr):
	name(_name), argPos(_argPos), expressionHook(_expr) {}


