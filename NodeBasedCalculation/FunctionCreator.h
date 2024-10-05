#pragma once
#include <vector>
#include <map>

#include "Node.h"
#include "Expression.h"
#include "FunctionNode.h"

template<class T>
class FunctionCreator
{
public:
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

	static bool CreateFunction_NoArgs(std::string str);
	static bool CreateFunction(std::string str, int noOfArgs);
	static bool CreateFunction_WithRestrictions(std::string str, int noOfArgs);
};