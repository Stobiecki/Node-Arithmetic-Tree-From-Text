#include "Expression.h"

template class Expression<double>;
using ExpressionClass = Expression<double>;

std::map<std::pair<std::string, NodeType>, int> Expression<double>::precedence;

// private func
std::string ExpressionClass::findClosingTag(const std::string& str, size_t& pos)
{
	std::string tag;

	while (pos < str.length() && str[pos] != '>')
	{
		tag += str[pos];
		pos++;
	}
	tag += '>';

	return tag;
}
bool ExpressionClass::isPartOfFunctionOrVariable(char c)
{
	return std::isalpha(c) || c == '_';
}
bool ExpressionClass::isDigit(char c)
{
	return std::isdigit(c);
}
bool ExpressionClass::isDigitNumber(char c)
{
	return std::isalnum(c) || c == '_';
}
bool ExpressionClass::isDot(char c)
{
	return c == '.';
}
bool ExpressionClass::isComma(char c)
{
	return c == ',';
}
void ExpressionClass::shiftCursorCleanup(size_t& val, int shift, std::string& strToReset)
{
	val += shift;
	strToReset = "";
}
bool ExpressionClass::canBePrefix(const std::string& str, int pos)
{
	if (pos == 0)
		return true;

	char c = str[pos - 1];
	if (c == '(' || c == '{' || c == '[' || c == '|' || c == '<')
		return true;

	if (isDigit(c) || isDigitNumber(c) || isPartOfFunctionOrVariable(c))
		return false;

	if (c == ')' || c == '}' || c == ']' || c == '|' || c == '>')
		return false;

	return true;
}
bool ExpressionClass::canBePostfix(
	const std::string& str,
	int pos,
	std::map<std::string, std::function<double(double)>>& postfixOperation
)
{
	int len = 1;
	for (; pos + len <= str.length(); len++)
	{
		std::string substring = str.substr(pos, len);
		// Check if there exists any key in the map that starts with the substring
		auto it = postfixOperation.lower_bound(substring);
		if (it != postfixOperation.end() && it->first.find(substring) == 0)
		{
			if (pos + len == str.length())
				return true;
		}
		else
		{
			if (substring.back() == ')' ||
				substring.back() == '}' ||
				substring.back() == ']' ||
				substring.back() == '|' ||
				substring.back() == '>' ||
				pos + len == str.length())
			{
				return true;
			}
			else
				return false;
		}
	}

	return false;
}
bool ExpressionClass::isOrCouldBeOperator(
	char c,
	std::map<std::string, std::function<double(double)>>& prefixOperation,
	std::map<std::string, std::function<double(double, double)>>& infixOperation,
	std::map<std::string, std::function<double(double)>>& postfixOperation
)
{
	bool found = false;

	// Kopiowanie operatorów Prefix
	for (const auto& pair : (*StaticData::prefixOperation)) {
		if (pair.first.size() > 0 && pair.first[0] == c) {
			prefixOperation[pair.first] = pair.second;
			found = true;
		}
	}

	// Kopiowanie operatorów Infix
	for (const auto& pair : (*StaticData::infixOperation)) {
		if (pair.first.size() > 0 && pair.first[0] == c) {
			infixOperation[pair.first] = pair.second;
			found = true;
		}
	}

	// Kopiowanie operatorów Postfix
	for (const auto& pair : (*StaticData::postfixOperation)) {
		if (pair.first.size() > 0 && pair.first[0] == c) {
			postfixOperation[pair.first] = pair.second;
			found = true;
		}
	}

	return found;
}
OperatorType ExpressionClass::identifyOperator(
	std::string op,
	std::map<std::string, std::function<double(double)>>& prefixOperation,
	std::map<std::string, std::function<double(double, double)>>& infixOperation,
	std::map<std::string, std::function<double(double)>>& postfixOperation,
	bool couldBePrefix,
	bool couldBePostfix
)
{
	// PostfixOperation
	if (postfixOperation.find(op) != postfixOperation.end() && couldBePostfix)
	{
		return POSTFIX_OPERATOR;
	}

	// PrefixOperation
	if (prefixOperation.find(op) != prefixOperation.end() && couldBePrefix)
	{
		return PREFIX_OPERATOR;
	}

	// InfixOperation
	if (infixOperation.find(op) != infixOperation.end())
	{
		return INFIX_OPERATOR;
	}

	return NO_OPERATOR;
}
void ExpressionClass::insertInOrder(
	std::vector<NodeTupleTemplate>& vec,
	const NodeTupleTemplate& newElement
)
{
	auto position = std::lower_bound(
		vec.begin(),
		vec.end(),
		newElement,
		[](const NodeTupleTemplate& a,
			const NodeTupleTemplate& b) {
				return std::get<1>(a) < std::get<1>(b);
		}
	);

	// Wstaw nowy element w odpowiednie miejsce
	vec.insert(position, newElement);
}
std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>> ExpressionClass::ExtractRangeOfNodes(
	std::vector<NodeTupleTemplate> &_preNodeHolder,
	std::vector<std::shared_ptr<Node<double>>> &_nodeHolder,
	size_t _start, 
	size_t _end
)
{
	std::vector<NodeTupleTemplate> templateRange;
	std::vector<std::shared_ptr<Node<double>>> nodesRange;

	// preNodeHolder	-> tuple
	// nodeHolder		-> nodes

	bool rangeStartFound = false;
	int rangeStart = 0;

	auto it = std::remove_if(_preNodeHolder.begin(), _preNodeHolder.end(), [&](NodeTupleTemplate& node) {
		size_t position = std::get<1>(node);
		if ((position >= _start && position <= _end)) {
			size_t index = &node - &_preNodeHolder[0];
			if (!rangeStartFound)
			{
				rangeStartFound = !rangeStartFound;
				rangeStart = index;
			}
			templateRange.push_back(std::move(node));
			nodesRange.push_back(std::move(_nodeHolder[index]));
			return true; 
		}
		return false;
		});

	int amount = _preNodeHolder.end() - it;
	_preNodeHolder.erase(it, _preNodeHolder.end());
	_nodeHolder.erase(_nodeHolder.begin() + rangeStart, _nodeHolder.begin() + (rangeStart + amount));

	return { templateRange, nodesRange };
}
// 0 -> brackets (sections), value, variable, functions
// 1 -> +, -
// 2 -> *, /
// 3 -> power, root, etc.
// 4 -> postfix-operators
// 5 -> prefix-operators
int ExpressionClass::getOperatorPriority(const std::shared_ptr<Node<double>>& node)
{
	if (dynamic_cast<PrefixOperationNode<double>*>(node.get())) 
	{
		return 5;
	}
	else if (dynamic_cast<PostfixOperationNode<double>*>(node.get()))
	{
		return 4;
	}
	else if (dynamic_cast<InfixOperationNode<double>*>(node.get()))
	{
		// TO_DO / FIX_ME
		//
		// powinno to uwzglêdniaæ kolejnoœæ wykonywania 
		// dzia³añ zdefiniowany w zmiennej "precedence"
		return dynamic_cast<InfixOperationNode<double>*>(node.get())->precedence;
	}
	return 0;
}
std::shared_ptr<Node<double>> ExpressionClass::RearrangeToGetRootNode(
	std::vector<std::shared_ptr<Node<double>>>& _nodeHolder
)
{
	std::vector<std::shared_ptr<Node<double>>> operatorStack;
	std::vector<std::shared_ptr<Node<double>>> outputQueue;

	for (auto& node : _nodeHolder)
	{
		if (dynamic_cast<InfixOperationNode<double>*>(node.get())) 
		{
			while (!operatorStack.empty() && getOperatorPriority(operatorStack.back()) >= getOperatorPriority(node)) 
			{
				outputQueue.push_back(operatorStack.back());
				operatorStack.pop_back();
			}
			operatorStack.push_back(node);
		}
		else if (dynamic_cast<PrefixOperationNode<double>*>(node.get()))
			operatorStack.push_back(node);
		else if (dynamic_cast<PostfixOperationNode<double>*>(node.get()))
			outputQueue.push_back(node);
		else // dynamic_cast<ValueNode*>(node.get())
			outputQueue.push_back(node);
	}

	// Przenosimy pozosta³e operatory ze stosu do kolejki wyjœciowej
	while(!operatorStack.empty()) 
	{
		outputQueue.push_back(operatorStack.back());
		operatorStack.pop_back();
	}

	// Przekszta³canie kolejki wynikowej w drzewo
	std::vector<std::shared_ptr<Node<double>>> treeStack;
	for (auto& node : outputQueue) 
	{
		if (auto opNode = dynamic_cast<InfixOperationNode<double>*>(node.get()))
		{
			std::vector<std::shared_ptr<Node<double>>> tmpNodes;
			tmpNodes.push_back(std::move(treeStack.back()));
			treeStack.pop_back();
			tmpNodes.push_back(std::move(treeStack.back()));
			treeStack.pop_back();

			// just for understanding sake - first node taken form the stack
			// is right node, second is left - it makes it unnecessarily complicated
			// so... swap it is!

			std::swap(tmpNodes[0], tmpNodes[1]);

			opNode->SetLeafsNodes(tmpNodes);
			treeStack.push_back(std::move(node));
		}
		else if (auto prefixNode = dynamic_cast<PrefixOperationNode<double>*>(node.get()))
		{
			std::vector<std::shared_ptr<Node<double>>> tmpNodes;
			tmpNodes.push_back(std::move(treeStack.back()));
			treeStack.pop_back();

			prefixNode->SetLeafsNodes(tmpNodes);
			treeStack.push_back(std::move(node));
		}
		else if (auto suffixNode = dynamic_cast<PostfixOperationNode<double>*>(node.get()))
		{
			std::vector<std::shared_ptr<Node<double>>> tmpNodes;

			tmpNodes.push_back(std::move(treeStack.back()));
			treeStack.pop_back();

			suffixNode->SetLeafsNodes(tmpNodes);
			treeStack.push_back(std::move(node));
		}
		else //dynamic_cast< ALL THE OTHER NODES <double>*>(node.get())
			treeStack.push_back(std::move(node));
	}

	// Ostateczny korzeñ drzewa to jedyny element na stosie
	return std::move(treeStack.back());
}
void ExpressionClass::findAllTokens(const std::string& str)
{
	std::unordered_map<std::string, std::string> matchingBrackets = {
		{")", "("},
		{"}", "{"},
		{"]", "["},
		{"|", "|"},
		{"</sth>", "<sth>"} // Add other tags as necessary
	};

	std::stack<std::tuple<std::string, size_t, BracketType>> stack;
	std::vector<std::pair<size_t, size_t>> pairs;

	size_t originalStrLen = str.length();

	for (size_t i = 0; i < originalStrLen; ++i)
	{
		std::map<std::string, std::function<double(double)>> PrefixOperationCopy;
		std::map<std::string, std::function<double(double, double)>> InfixOperationCopy;
		std::map<std::string, std::function<double(double)>> PostfixOperationCopy;

		// '<' bracket
		std::string c = (str[i] == '<') ? findClosingTag(str, i) : std::string(1, str[i]);
		// Close bracket
		if (matchingBrackets.count(c) && (!stack.empty() && (std::get<0>(stack.top()) == "|" || c != "|")))
		{
			if (!stack.empty() && std::get<0>(stack.top()) == matchingBrackets[c])
			{
				if (std::get<2>(stack.top()) == BracketType::NORMAL_B)
				{
					pairs.push_back({ std::get<1>(stack.top()), i });
					stack.pop();
				}
				else if (std::get<2>(stack.top()) == BracketType::FUNCTION_B)
				{
					size_t start = std::get<1>(stack.top());
					size_t end = i;

					FunctionStack* fs = &functionStack.top();

					std::string correspondingBracket = "";

					if (auto it = matchingBrackets.find(c); it != matchingBrackets.end())
						correspondingBracket = it->second;

					if (str[i - 1] == correspondingBracket[0])
						fs->noOfArgs = 0;
					else if (fs->noOfArgs == 0)
					{
						fs->noOfArgs = 1;
						fs->functionArgEnd = i - 1;
						fs->functionArgsPos.push_back({ fs->functionArgStart, fs->functionArgEnd });
					}
					else if (fs->noOfArgs > 0)
					{
						fs->functionArgEnd = i - 1;
						fs->functionArgsPos.push_back({ fs->functionArgStart, fs->functionArgEnd });
					}

					std::string funcSection = str.substr(start, end - start + 1);
					std::string funcName = str.substr(fs->functionNameStart, fs->functionNameEnd - fs->functionNameStart);
					// check if function can be custom
					NodeType functionType = NodeType::CUSTOM_FUNCTIONS;
					bool found = false;
					for (const auto& pair : FunctionNode<double>::functions) 
						if (std::get<0>(pair.first) == funcName)
						{
							found = true;
							break;
						}
					if (found)
						functionType = NodeType::FUNCTIONS;
					functions_.push_back({ funcSection, start, end - start + 1, fs->noOfArgs, fs->functionArgsPos, funcName });
					insertInOrder(preNodeHolder, { funcSection, start, end - start + 1, functionType });
					fs->noOfArgs = fs->functionArgStart = fs->functionArgEnd = 0;
					fs->functionArgsPos.clear();
					functionStack.pop();
					stack.pop();
				}
			}
		}
		// Open bracket
		else if (c == "(" || c == "{" || c == "[" || c == "|" || c == "<sth>")
		{
			if (StaticData::state == CurrentSymbol::FUNCTION && c == "(") // function
			{
				FunctionStack* fs = &functionStack.top();
				stack.push({ c, (i - c.length() - (fs->functionNameEnd - fs->functionNameStart) + 1), BracketType::FUNCTION_B });
				StaticData::state = CurrentSymbol::NONE;
			}
			else // NORMAL_B
				stack.push({ c, (i - c.length() + 1), BracketType::NORMAL_B });
		}
		else if (isPartOfFunctionOrVariable(c[0]))
		{
			int rangeStart = i;
			int rangeEnd = i;
			StaticData::state = CurrentSymbol::VARIABLE_OR_FUNCTION;

			while (isDigitNumber(str[++i])) {}
			rangeEnd = i;

			if (str[i] == '(' || str[i] == '{' || str[i] == '[' || str[i] == '|' || str[i] == '<')
			{
				StaticData::state = CurrentSymbol::FUNCTION;
				functionStack.push(FunctionStack());
				FunctionStack* fs = &functionStack.top();
				fs->functionNameStart = rangeStart;
				fs->functionNameEnd = rangeEnd;
				fs->functionArgStart = i + 1;
			}
			else // variable
			{
				StaticData::symbol = str.substr(rangeStart, rangeEnd - rangeStart);
				// check if variable can be custom function arg
				NodeType variableType = NodeType::VARIABLES;
				auto it = std::find(StaticData::customFunctionArgsNames.begin(), StaticData::customFunctionArgsNames.end(), StaticData::symbol);
				if (it != StaticData::customFunctionArgsNames.end())
					variableType = NodeType::CUSTOM_FUNCTIONS_ARGUMENT;
				variables.push_back({ StaticData::symbol, rangeStart, rangeEnd - rangeStart });
				preNodeHolder.push_back({ StaticData::symbol, rangeStart, rangeEnd - rangeStart, variableType });
			}
			i--;
		}
		else if (isDigit(c[0]))
		{
			int rangeStart = i;
			int rangeEnd = i;
			while (isDigit(str[rangeEnd + 1]) || isDot(str[rangeEnd + 1]))
			{
				rangeEnd++;
				if (isDot(str[rangeEnd]))
					while (isDigit(str[rangeEnd + 1]))
						rangeEnd++;
			}

			StaticData::symbol = str.substr(rangeStart, rangeEnd - rangeStart + 1);
			values.push_back({ StaticData::symbol, rangeStart, rangeEnd - rangeStart + 1 });
			preNodeHolder.push_back({ StaticData::symbol, rangeStart, rangeEnd - rangeStart + 1 , NodeType::VALUE });
			shiftCursorCleanup(i, rangeEnd - rangeStart, StaticData::symbol);
		}
		else if (isDot(c[0]))
		{
			int rangeStart = i;
			int rangeEnd = i;
			while (isDigit(str[rangeEnd + 1]))
				rangeEnd++;

			StaticData::symbol = str.substr(rangeStart, rangeEnd - rangeStart + 1);
			values.push_back({ StaticData::symbol, rangeStart, rangeEnd - rangeStart + 1 });
			shiftCursorCleanup(i, rangeEnd - rangeStart, StaticData::symbol);
		}
		else if (isComma(c[0]))
		{
			FunctionStack* fs = &functionStack.top();
			if (fs->noOfArgs == 0)
				fs->noOfArgs = 2;
			else
				fs->noOfArgs++;
			fs->functionArgEnd = i - 1;
			fs->functionArgsPos.push_back({ fs->functionArgStart, fs->functionArgEnd });
			fs->functionArgStart = i + 1;
		}
		else if (isOrCouldBeOperator(c[0], PrefixOperationCopy, InfixOperationCopy, PostfixOperationCopy))
		{
			bool couldBePrefix = canBePrefix(str, i);
			bool couldBeSufix = canBePostfix(str, i, PostfixOperationCopy);
			int rangeStart = i;

			int iter = 1;
			while (identifyOperator(str.substr(rangeStart, iter), PrefixOperationCopy, InfixOperationCopy, PostfixOperationCopy, couldBePrefix, couldBeSufix) != OperatorType::NO_OPERATOR)
			{
				iter++;
				if (iter >= originalStrLen)
					break;
			}
			iter--;
			OperatorType operatorType = identifyOperator(str.substr(rangeStart, iter), PrefixOperationCopy, InfixOperationCopy, PostfixOperationCopy, couldBePrefix, couldBeSufix);
			NodeType nt;
			if (operatorType == PREFIX_OPERATOR) nt = OPERATORS_PREFIX;
			else if (operatorType == INFIX_OPERATOR) nt = OPERATORS_INFIX;
			else if (operatorType == POSTFIX_OPERATOR) nt = OPERATORS_POSTFIX;

			std::string op = str.substr(rangeStart, iter);
			operators.push_back({ op, rangeStart, iter, operatorType });
			preNodeHolder.push_back({ op, rangeStart, iter, nt });

			shiftCursorCleanup(i, iter - 1, StaticData::symbol);
		}
	}

	// Extract sections (brackets) based on pairs
	for (const auto& p : pairs)
	{
		sections.push_back({ str.substr(p.first, p.second - p.first + 1), p.first, p.second - p.first + 1 });
		insertInOrder(preNodeHolder, { str.substr(p.first, p.second - p.first + 1), p.first, p.second - p.first + 1, NodeType::SECTION });
	}

	return;
}
void ExpressionClass::SortSections()
{
	std::sort(sections.begin(), sections.end(),
		[](const std::tuple<std::string, size_t, size_t>& a,
			const std::tuple<std::string, size_t, size_t>& b) {
				return std::get<1>(a) < std::get<1>(b);
		});
}
void ExpressionClass::SortFunctions()
{
	std::sort(functions_.begin(), functions_.end(),
		[](const std::tuple<std::string, size_t, size_t, int, std::vector<std::pair<int, int>>, std::string>& a,
			const std::tuple<std::string, size_t, size_t, int, std::vector<std::pair<int, int>>, std::string>& b) {
				return std::get<1>(a) < std::get<1>(b);
		});
}
void ExpressionClass::NodeGeneration()
{
	int pos = 0;
	for (const auto& node : preNodeHolder)
	{
		switch (std::get<3>(node))
		{
		case NodeType::SECTION:
		{
			//NodeHolder
			std::string tmp(std::get<0>(preNodeHolder[pos]));
			nodeHolder.push_back(
				std::make_unique<SectionNode<double>>(
					tmp.substr(1, tmp.size() - 2),
					std::get<1>(preNodeHolder[pos]),
					std::get<2>(preNodeHolder[pos])
				)
			);
			break;
		}
		case NodeType::VALUE:
		{
			double tmp = std::stod(std::get<0>(preNodeHolder[pos]));
			nodeHolder.push_back(std::make_unique<ValueNode<double>>(tmp));
			break;
		}
		case NodeType::VARIABLES:
		{
			std::string tmp(std::get<0>(preNodeHolder[pos]));
			if (StaticData::variables.find(tmp) != StaticData::variables.end())
				nodeHolder.push_back(std::make_unique<VariableNode<double>>(tmp, &StaticData::variables[tmp]));
			else
				nodeHolder.push_back(std::make_unique<VariableNode<double>>("NaN", &StaticData::NaN_Symbol));
			break;
		}
		case NodeType::FUNCTIONS:
		{
			using std::get;
			size_t pos = get<1>(node);
			size_t len = get<2>(node);

			// gets correct tuple
			auto it = std::find_if(functions_.begin(), functions_.end(),
				[pos](const auto& func) {
					return std::get<1>(func) == pos;
				});

			int argsNo = get<3>(*it);

			if (it != functions_.end())
			{
				auto& opVariant = FunctionNode<double>::functions[{std::get<5>(*it), argsNo}];

				// Odwiedzanie wariantu, aby uzyskaæ funkcjê we w³aœciwym formacie
				auto func = std::visit([](auto&& op) -> std::function<double(std::vector<double>)> {
					using FuncType = std::decay_t<decltype(op)>;

					if constexpr (std::is_same_v<FuncType, std::function<double()>>) {
						return [op](std::vector<double> args) { return op(); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(double)>>) {
						return [op](std::vector<double> args) { return op(args[0]); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(double, double)>>) {
						return [op](std::vector<double> args) { return op(args[0], args[1]); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(double, double, double)>>) {
						return [op](std::vector<double> args) { return op(args[0], args[1], args[2]); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(double, double, double, double)>>) {
						return [op](std::vector<double> args) { return op(args[0], args[1], args[2], args[3]); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(double, double, double, double, double)>>) {
						return [op](std::vector<double> args) { return op(args[0], args[1], args[2], args[3], args[4]); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(double, double, double, double, double, double)>>) {
						return [op](std::vector<double> args) { return op(args[0], args[1], args[2], args[3], args[4], args[5]); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(double, double, double, double, double, double, double)>>) {
						return [op](std::vector<double> args) { return op(args[0], args[1], args[2], args[3], args[4], args[5], args[6]); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(double, double, double, double, double, double, double, double)>>) {
						return [op](std::vector<double> args) { return op(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]); };
					}
					else {
						throw std::invalid_argument("Unsupported function type");
					}
					}, opVariant);
				nodeHolder.push_back(std::make_unique<FunctionNode<double>>(get<0>(*it), argsNo, func, pos, len));
			}
			break;
		}
		case NodeType::OPERATORS_PREFIX:
		{
			nodeHolder.push_back(std::make_unique<PrefixOperationNode<double>>(
				&PrefixOperationNode<double>::operators[std::get<0>(node)]
			));
			break;
		}
		case NodeType::OPERATORS_INFIX:
		{
			std::string tmp = std::get<0>(node);
			int precedence = 3;
			if (auto it = InfixOperationNode<double>::precedences.find(tmp); it != InfixOperationNode<double>::precedences.end())
				precedence = it->second;
			nodeHolder.push_back(std::make_unique<InfixOperationNode<double>>(
				&InfixOperationNode<double>::operators[std::get<0>(node)],
				precedence
			));
			break;
		}
		case NodeType::OPERATORS_POSTFIX:
		{
			nodeHolder.push_back(std::make_unique<PostfixOperationNode<double>>(
				&PostfixOperationNode<double>::operators[std::get<0>(node)]
			));
			break;
		}
		case NodeType::CUSTOM_FUNCTIONS:
		{
			using std::get;
			std::string customFuncName = get<0>(node);
			size_t pos = get<1>(node);
			size_t len = get<2>(node);

			// gets correct tuple
			auto it = std::find_if(functions_.begin(), functions_.end(),
				[pos](const auto& func) {
					return std::get<1>(func) == pos;
				});

			int argsNo = get<3>(*it);

			if (it != functions_.end())
			{
				auto& opVariant = FunctionCreator<double>::customFunctions[{std::get<5>(*it), argsNo}];

				auto func = std::visit([](auto&& op) -> std::function<double(CustomFunctionNode<double>*)> {
					using FuncType = std::decay_t<decltype(op)>;

					if constexpr (std::is_same_v<FuncType, std::function<double()>>) {
						return [op](CustomFunctionNode<double>* cfn) -> double { return op(); };
					}
					else if constexpr (std::is_same_v<FuncType, std::function<double(CustomFunctionNode<double>*)>>) {
						return [op](CustomFunctionNode<double>* cfn) -> double { return op(cfn); };
					}
					else {
						throw std::invalid_argument("Unsupported function type");
					}
					}, opVariant);
				nodeHolder.push_back(std::make_unique<CustomFunctionNode<double>>(customFuncName, argsNo, func, pos, len));
			}
			break;
		}
		case NodeType::CUSTOM_FUNCTIONS_ARGUMENT: // if in here then Expression must serve as custom function
		{
			std::string tmp(std::get<0>(preNodeHolder[pos]));
			int argNo = std::stoi(tmp.substr(1, tmp.length()));
			nodeHolder.push_back(std::make_unique<CustomFunctionArgumentNode<double>>(
				tmp,
				argNo,
				this
			));
			break;
		}
		default:
			break;
		}
		pos++;
	}
}
std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>> ExpressionClass::ComplementEmptyNodes(
	std::vector<NodeTupleTemplate> &_preNodeHolder,
	std::vector<std::shared_ptr<Node<double>>> &_nodeHolder,
	std::vector<FunctionTupleTemplate>& _functions
)
{
	for (auto it = _nodeHolder.begin(); it != _nodeHolder.end(); )
	{
		int vector_pos = _nodeHolder.size() - (_nodeHolder.end() - it);
		if (dynamic_cast<SectionNode<double>*>(it->get()))
		{
			if (SectionNode<double>* node = dynamic_cast<SectionNode<double>*>(it->get()); node->isReady == false)
			{
				auto [template_tmp, nodes_tmp] = ExtractRangeOfNodes(_preNodeHolder, _nodeHolder, node->sectionStart + 1, node->sectionStart + node->len - 1);
				node->ComplementEmptySection(template_tmp, nodes_tmp, _functions);
			}
		}
		else if (dynamic_cast<FunctionNode<double>*>(it->get()))
		{
			if (FunctionNode<double>* node = dynamic_cast<FunctionNode<double>*>(it->get()); node->areNodesReady == false)
			{
				const auto funcIt = std::find_if(_functions.begin(), _functions.end(),
					[&](const FunctionTupleTemplate& func) {
						return std::get<1>(func) == node->sectionStart;
					}
				);
				std::vector<std::pair<int, int>> funcSections = std::get<4>(*funcIt);
				std::vector<std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>>> vecOfFuncArgsRanges;
				for (auto& range : funcSections)
				{
					auto [template_tmp, nodes_tmp] = ExtractRangeOfNodes(_preNodeHolder, _nodeHolder, range.first, range.second);
					vecOfFuncArgsRanges.push_back({ template_tmp, nodes_tmp });
				}
				node->ComplementEmptyArgsSection(vecOfFuncArgsRanges, _functions);
			}
		}
		else if (dynamic_cast<CustomFunctionNode<double>*>(it->get()))
		{
			if (CustomFunctionNode<double>* node = dynamic_cast<CustomFunctionNode<double>*>(it->get()); node->areNodesReady == false)
			{
				const auto funcIt = std::find_if(_functions.begin(), _functions.end(),
					[&](const FunctionTupleTemplate& func) {
						return std::get<1>(func) == node->sectionStart;
					}
				);
				std::vector<std::pair<int, int>> funcSections = std::get<4>(*funcIt);
				std::vector<std::pair<std::vector<NodeTupleTemplate>, std::vector<std::shared_ptr<Node<double>>>>> vecOfFuncArgsRanges;
				for (auto& range : funcSections)
				{
					auto [template_tmp, nodes_tmp] = ExtractRangeOfNodes(_preNodeHolder, _nodeHolder, range.first, range.second);
					vecOfFuncArgsRanges.push_back({ template_tmp, nodes_tmp });
				}
				node->ComplementEmptyArgsSection(vecOfFuncArgsRanges, _functions);
			}
		}
		it++;
	}

	return std::make_pair(_preNodeHolder, _nodeHolder);
}

// public func

ExpressionClass::Expression(std::string str) : textRepres(str) {}
void ExpressionClass::PrepareData()
{
	StaticData::currentExpression = this;
	findAllTokens(textRepres);
	SortSections();
	SortFunctions(); //could be slowing down the code
	NodeGeneration();
	ComplementEmptyNodes(this->preNodeHolder, this->nodeHolder, this->functions_);
	SectionNode<double> tmpSection(this->textRepres, 0, this->textRepres.size());
	this->rootNode = tmpSection.ComplementEmptySection(this->preNodeHolder, this->nodeHolder, this->functions_);
	Calculate();
}
void ExpressionClass::PrepareDataWithoutCalculation()
{
	StaticData::currentExpression = this;
	findAllTokens(textRepres);
	SortSections();
	SortFunctions(); //could be slowing down the code
	NodeGeneration();
	ComplementEmptyNodes(this->preNodeHolder, this->nodeHolder, this->functions_);
	SectionNode<double> tmpSection(this->textRepres, 0, this->textRepres.size());
	this->rootNode = tmpSection.ComplementEmptySection(this->preNodeHolder, this->nodeHolder, this->functions_);
}
void ExpressionClass::InitPrecedence()
{
	static bool alreadyInit = false;
	if (alreadyInit)
		return;

	precedence[std::make_pair("+", NodeType::OPERATORS_INFIX)] = 1;
	precedence[std::make_pair("-", NodeType::OPERATORS_INFIX)] = 1;

	precedence[std::make_pair("*", NodeType::OPERATORS_INFIX)] = 2;
	precedence[std::make_pair("/", NodeType::OPERATORS_INFIX)] = 2;

	precedence[std::make_pair("-", NodeType::OPERATORS_PREFIX)] = 4;
	precedence[std::make_pair("--", NodeType::OPERATORS_PREFIX)] = 4;
	precedence[std::make_pair("++", NodeType::OPERATORS_PREFIX)] = 4;

	precedence[std::make_pair("--", NodeType::OPERATORS_POSTFIX)] = 5;
	precedence[std::make_pair("++", NodeType::OPERATORS_POSTFIX)] = 5;
	precedence[std::make_pair("!", NodeType::OPERATORS_POSTFIX)] = 5;
	precedence[std::make_pair("?", NodeType::OPERATORS_POSTFIX)] = 5;
}
void ExpressionClass::LogDataToConsole()
{
	std::cout << "\n\n\n";
	std::cout << std::setw(16) << "repres:"
		<< "\t" << std::setw(16) << "pos:"
		<< "\t" << std::setw(16) << "len:";

	std::cout << "\n\nFound sections: \n";
	for (const auto& section : sections)
		std::cout << std::setw(16) << std::get<0>(section)
		<< "\t" << std::setw(16) << std::get<1>(section)
		<< "\t" << std::setw(16) << std::get<2>(section) << "\n";

	std::cout << "\n\nFound values: \n";
	for (const auto& value : values)
		std::cout << std::setw(16) << std::get<0>(value)
		<< "\t" << std::setw(16) << std::get<1>(value)
		<< "\t" << std::setw(16) << std::get<2>(value) << "\n";

	std::cout << "\n\nFound variables: \n";
	for (const auto& variable : variables)
		std::cout << std::setw(16) << std::get<0>(variable)
		<< "\t" << std::setw(16) << std::get<1>(variable)
		<< "\t" << std::setw(16) << std::get<2>(variable) << "\n";

	std::cout << "\n\nFound functions: \n";
	for (const auto& function : functions_)
	{
		std::cout << std::setw(16) << std::get<0>(function)
			<< "\t" << std::setw(16) << std::get<1>(function)
			<< "\t" << std::setw(16) << std::get<2>(function)
			<< "\t" << std::setw(16) << std::get<3>(function);

		std::stringstream ss;
		ss << "\n\t\tFunction args:\n";
		for (const auto& argsPos : std::get<4>(function))
		{
			ss << "\t\t\t\t"
				<< argsPos.first
				<< "-"
				<< argsPos.second
				<< ": "
				<< textRepres.substr(argsPos.first, argsPos.second - argsPos.first + 1)
				<< "\n";
		}
		ss << "\n";
		cout << ss.str();
	}

	std::cout << "\n\nFound operators: \n";
	for (const auto& op : operators)
		std::cout << std::setw(16) << std::get<0>(op)
		<< "\t" << std::setw(16) << std::get<1>(op)
		<< "\t" << std::setw(16) << std::get<2>(op)
		<< "\t" << std::setw(16) << StaticData::operatorTypeInterpreter[std::get<3>(op)] << "\n";

	std::cout << "\n\n\n\nPrecedence: \n";
	for (const auto& pnh : preNodeHolder)
		std::cout << std::setw(16) << std::get<0>(pnh)
		<< "\t" << std::setw(16) << std::get<1>(pnh)
		<< "\t" << std::setw(16) << std::get<2>(pnh)
		<< "\t" << std::setw(16) << StaticData::NodeTypeInterpreter[std::get<3>(pnh)] << "\n";

	std::cout << "\n\n\n\nResult value: ";

	auto start = std::chrono::high_resolution_clock::now();
	//double treeCalculationResult = this->Calculate();
	auto end = std::chrono::high_resolution_clock::now();

	//std::cout << treeCalculationResult;

	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	double duration_us = duration / 1000.0;
	std::cout << "\n\n\n\nTime of tree evaluation: " << duration_us << " ms" << std::endl;
}














