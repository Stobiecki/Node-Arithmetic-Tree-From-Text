#pragma once

enum CurrentSymbol
{
	NONE = 0,
	INT_VALUE = 1,
	DOUBLE_VALUE = 2,
	VARIABLE_OR_FUNCTION = 3,
	VARIABLE = 4,
	FUNCTION = 5,
};

enum BracketType
{
	NORMAL_B = 0,
	FUNCTION_B = 1,
	ARRAY_B = 2,
};

enum OperatorType
{
	NO_OPERATOR = 0,
	PREFIX_OPERATOR = 1,
	INFIX_OPERATOR = 2,
	POSTFIX_OPERATOR = 3,
};

enum NodeType
{
	SECTION = 0,
	VALUE = 1,
	VARIABLES = 2,
	FUNCTIONS = 3,
	OPERATORS_PREFIX = 4,
	OPERATORS_INFIX = 5,
	OPERATORS_POSTFIX = 6,
};

