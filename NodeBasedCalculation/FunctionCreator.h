#pragma once
#include <vector>
#include <map>

#include "Node.h"
#include "Expression.h"
#include "FunctionNode.h"
#include "CustomFunction.h"

template<class T>
class CustomFunctionNode;

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