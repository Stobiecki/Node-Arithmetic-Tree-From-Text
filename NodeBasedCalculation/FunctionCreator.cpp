#include "FunctionCreator.h"

template class FunctionCreator<double>;
using FunctionCreatorClass = FunctionCreator<double>;

template <class T>
std::map<std::tuple<std::string, int>, typename FunctionCreator<T>::FunctionVariant> FunctionCreator<T>::customFunctions;

bool FunctionCreatorClass::CreateFunction_NoArgs(std::string name, std::string calculation)
{
	// 1) creation of a function placeholder
	// 2) creation of a tree husk
	// 3) apply arguments
	// 4) apply restrictions

	std::shared_ptr<Expression<double>> treeRoot = std::make_shared<Expression<double>>(calculation);
	treeRoot.get()->PrepareDataWithoutCalculation();
	//std::cout << "val: " << treeRoot.Calculate();

	auto lambda = [root = std::move(treeRoot)]() mutable {
		return root.get()->Calculate();
	};

	customFunctions[{name, 0}] = std::move(lambda);

	return false;
}
bool FunctionCreatorClass::CreateFunction(std::string name, std::string calculation, int noOfArgs)
{
	std::shared_ptr<Expression<double>> treeRoot = std::make_shared<Expression<double>>(calculation);
	treeRoot.get()->PrepareDataWithoutCalculation();
	treeRoot.get()->LogDataToConsole();
	auto lambda = [root = std::move(treeRoot)](CustomFunctionNode<double>* cfn) mutable {
		return root.get()->CalculateCustom(cfn);
		};
	customFunctions[{name, noOfArgs}] = std::move(lambda);
	return false;
}
bool FunctionCreatorClass::CreateFunction_WithRestrictions(std::string name, std::string calculation, int noOfArgs)
{
	return false;
}

void FunctionCreatorClass::Init()
{
	//CreateFunction_NoArgs("x_plus_y", "x+y");
	//CreateFunction("x_plus_x1", "x+x1", 1);
	CreateFunction("custom", "1+x1*x2/3", 2);
	CreateFunction("custom2", "x1*x2", 2);
	CreateFunction("custom3", "(x1/4)+x2", 2);
}