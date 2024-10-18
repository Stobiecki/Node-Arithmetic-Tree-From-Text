#pragma once
#include <vector>
#include <map>

#include "Node.h"
#include "Expression.h"
#include "FunctionNode.h"
#include "CustomFunction.h"

template<class T>
class CustomFunctionNode;


//
// How it works?
// 
// We have 4 essential classes: FunctionCreator, CustomFunctionNode, CustomFunctionArgumentNode, Expression
// 
// CREATION:
// After calling for methods such as CreateFunction() the new function is being created.
// New user-created function/lambda is being stored inside of a static map called "customFunctions".
// We can think of a newly made funtion as of a lambda that stores an Expression instance. It effectively
// means that a new function is an already built tree. Arguments that can be passed to the function using
// arguments named x1, x2, ... , x8 - these are represented in the lambda's expression as a
// CustomFunctionArgumentNode node. These nodes don't contain any value, nor have any children in a
// typical sense. When created, they receive info about the Expression instance that they are in.
//
// USAGE:
// When creation of a CustomFunctionNode takes place, the info about itself (cfn) is passed to the 
// expression stored in the lambda (so that it can be saved in the Expression). This enables linkage 
// between CustomFunctionArgumentNode and CustomFunctionNode.
// 
// CustomFunctionArgumentNode -> Expression -> CustomFunctionNode
// 
// This is essential to optimize the process of passing arguments to the function. Ideal is to reuse each
// of the passed arguments, not to copy each of them.
// 
// For example:
// function definition:		Custom(x1)
// function expression:		cos(x1)+x1
// 
// function invocation:		Custom(1*2)
// 
// Whenever function will be invoked like this argument (here it is x1) can be reused multiple times
// in multiple places, so CustomFunctionNode holds each of the arguments ready for CustomFunctionArgumentNode
// to access them when needed, without bloating Expression or a newly created function.
// 
// It is important to remember that the function/lambda dosen't take ownership of cfn, it is only passed to
// let CustomFunctionArgumentNode access CustomFunctionNode's arguments.
//
template<class T>
class FunctionCreator
{
public:
	using RestrictionType = std::variant<
		std::function<T()>,					// e.g. ltz(T a)
		std::tuple<std::function<T(T)>, T>	// e.g. lt(T a, T b)
	>;
	//FIX_ME
	using FunctionVariant = std::variant<
		std::function<T()>,
		std::function<T(CustomFunctionNode<T>*)>
	>;

	static std::map<std::tuple<std::string, int>, FunctionVariant> customFunctions;

	// This section corresponds to 2 args func where "a"
	// is specific arg and "b" is custom no.

	// less then
	static bool lt(T a, T b)
	{
		return a < b;
	}
	// less equal
	static bool le(T a, T b)
	{
		return a <= b;
	}
	// greater than
	static bool gt(T a, T b)
	{
		return a > b;
	}
	// greater equal
	static bool ge(T a, T b)
	{
		return a >= b;
	}
	// equals
	static bool eq(T a, T b)
	{
		return a == b;
	}
	// not equal
	static bool neq(T a, T b)
	{
		return a != b;
	}

	// This section corresponds to 2 args func where "a"
	// is specific arg

	// less then zero
	static bool ltz(T a)
	{
		return a < 0.0;
	}
	// less equal zero
	static bool lez(T a)
	{
		return a <= 0.0;
	}
	// greater than zero
	static bool gtz(T a)
	{
		return a > 0.0;
	}
	// greater equal zero
	static bool gez(T a)
	{
		return a >= 0.0;
	}
	// equals zero
	static bool eqz(T a)
	{
		return a == 0.0;
	}
	// not equal zero
	static bool neqz(T a)
	{
		return a != 0.0;
	}

	static bool CreateFunction_NoArgs(std::string name, std::string calculation);

	// arguments in functions should be named accordingly:
	// x1, x2, x3, x4, etc...
	static bool CreateFunction(std::string name, std::string calculation, int noOfArgs);
	static bool CreateFunction_WithRestrictions(std::string name, std::string calculation, int noOfArgs);

	static void Init();
};