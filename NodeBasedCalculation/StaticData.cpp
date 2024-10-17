#include "StaticData.h"

CurrentSymbol StaticData::state = CurrentSymbol::NONE;
std::string StaticData::symbol = "";

std::map<std::string, double> StaticData::variables = {
    {"a",123.},
    {"b",-1.},
    {"c",0.},
    {"x",5.},
    {"y",1.},
    {"z",2.}
};
std::vector<std::string> StaticData::functions;

std::map<std::string, std::function<double(double)>>* StaticData::prefixOperation = &PrefixOperationNode<double>::operators;
std::map<std::string, std::function<double(double, double)>>* StaticData::infixOperation = &InfixOperationNode<double>::operators;
std::map<std::string, std::function<double(double)>>* StaticData::postfixOperation = &PostfixOperationNode<double>::operators;

std::map<OperatorType, std::string> StaticData::operatorTypeInterpreter = {
    {OperatorType::NO_OPERATOR, "None"},
    {OperatorType::PREFIX_OPERATOR, "Prefix"},
    {OperatorType::INFIX_OPERATOR, "Infix"},
    {OperatorType::POSTFIX_OPERATOR, "Postfix"},
};

std::map<NodeType, std::string> StaticData::NodeTypeInterpreter = {
    {NodeType::SECTION, "Section"},
    {NodeType::VALUE, "Value"},
    {NodeType::VARIABLES, "Variables"},
    {NodeType::FUNCTIONS, "Function"},
    {NodeType::OPERATORS_PREFIX, "Prefix_operator"},
    {NodeType::OPERATORS_INFIX, "Infix_operator"},
    {NodeType::OPERATORS_POSTFIX, "Postfix_operator"},
    {NodeType::CUSTOM_FUNCTIONS, "Custom_Function"},
    {NodeType::CUSTOM_FUNCTIONS_ARGUMENT, "Custom_Function_Argument"},
};

std::vector<std::string> StaticData::customFunctionArgsNames = {
    "x1",
    "x2",
    "x3",
    "x4",
    "x5",
    "x6",
    "x7",
    "x8",
};

Expression<double>* StaticData::currentExpression = nullptr;

double StaticData::NaN_Symbol = std::numeric_limits<double>::quiet_NaN();

void StaticData::DataInit()
{
    // essential init
    PrefixOperationNode<double>::Init();
    InfixOperationNode<double>::Init();
    PostfixOperationNode<double>::Init();
    FunctionNode<double>::Init();
    Expression<double>::InitPrecedence();

    // post-essential data Init
    FunctionCreator<double>::Init();
}