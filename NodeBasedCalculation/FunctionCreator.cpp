#include "FunctionCreator.h"

template class FunctionCreator<double>;
using FunctionCreatorClass = FunctionCreator<double>;

bool FunctionCreatorClass::CreateFunction_NoArgs(std::string str)
{
	return false;
}
bool FunctionCreatorClass::CreateFunction(std::string str, int noOfArgs)
{
	return false;
}
bool FunctionCreatorClass::CreateFunction_WithRestrictions(std::string str, int noOfArgs)
{
	return false;
}