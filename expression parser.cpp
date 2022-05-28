
// Expression parser to solve simple math problems..
// Original thought: 31.5.2010 (significantly inspired by one old project on "codeproject.com")

#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>

#include <math.h>

#define PI		3.14159265358979323846
#define E		2.71828182845904523536

#define VERBOSE false


double Radians(double degree)
{
	return (degree * (PI / 180));
}

double Degrees(double radians)
{
	return ((radians * 180) / PI);
}

class BaseNode
{
public:
	virtual double value() = 0;
};

class NumNode: public BaseNode
{
	double number;
public:
	NumNode(double value)
	{
		number = value;
	}
	virtual double value()
	{
		return number;
	}
};

class OperNode: public BaseNode
{
	char oper;
	BaseNode* left;
	BaseNode* right;
public:
	OperNode(char oper, BaseNode* left, BaseNode* right)
	{
		this->oper = oper;
		this->right = right;
		this->left = left;
	}
	virtual double value()
	{
		double leftValue = left->value();
		double rightValue = right->value();
		
		switch(oper)
		{
			case '+': return leftValue + rightValue;
			case '-': return leftValue - rightValue;
			case '*': return leftValue * rightValue;
			case '/': return leftValue / rightValue;
			case '^': return pow(leftValue, rightValue);
			default: throw std::runtime_error("Unknown operator: " + oper);;
		}
	}
};

class FuncNode : public BaseNode
{
	BaseNode* _expression;
	std::string _func;

public:
	FuncNode(std::string func, BaseNode* expression)
	{
		_func = func;
		_expression = expression;
	}
	virtual double value()
	{
		if (_func == std::string("RAD")) {
			return (Radians(_expression->value()));
		}
		else if (_func == std::string("DEG")) {
			return (Degrees(_expression->value()));
		}
		else if (_func == std::string("sin")) {
			return (sin(_expression->value()));
		}
		else if (_func == std::string("cos")) {
			return (cos(_expression->value()));
		}
		else if (_func == std::string("tan")) {
			return (tan(_expression->value()));
		}
		else if (_func == std::string("abs")) {
			return (fabs(_expression->value()));
		}
		else if (_func == std::string("ln")) {
			return (log(_expression->value()));
		}
		else if (_func == std::string("log")) {
			return (log10(_expression->value()));
		}
		else if (_func == std::string("exp")) {
			return (exp(_expression->value()));
		}
		else if (_func == std::string("sqrt")) {
			return (sqrt(_expression->value()));
		}
		/*else if (_func == std::string("pow")) {
			return (pow(_expression->value(), 2));
		}*/
		else 
			throw std::runtime_error("Unknown function: " + _func);

		// ...
	}
};

class Parser
{
public:
	enum TokenType
	{
		NONE,
		NUMBER,
		VARIABLE_ASSIGN,
		VARIABLE_SUBSTITUTION,
		END,
		RAD_FN,
		DEG_FN,
		SIN_FN,
		COS_FN,
		TAN_FN,
		ABS_FN,
		LN_FN,
		LOG_FN,
		EXP_FN,
		SQRT_FN,
		PLUS='+',
		MINUS='-',
		MULTIPLY='*',
		DIVIDE='/',
		POW='^',
		LHPAREN='(',
		RHPAREN=')',
		//EQUAL='='
	};

private:
	std::string program_;

	const char * pWord_;
	const char * pWordStart_;

	TokenType type_;
	std::string word_;
	double value_;

	std::vector<BaseNode*> nodes;

	static std::map<std::string, std::string> variables;

public:
	Parser(const std::string & program)
		: program_(program)
	{
		AddCommonVariables();
	}

	const double Evaluate(unsigned int precision = 2);
	const double Evaluate(const std::string & program, unsigned int precision = 2);

private:
	void AddCommonVariables();
	double RoundOff(double value, unsigned int precision);

	void CheckVariables();
	bool AssignVariable(std::string variableName);
	bool SubstituteVariable(std::string variableName);
	TokenType CheckFunction(std::string name);

	const TokenType GetToken(const bool ignoreSign = false);
	void AddSubtract(const bool get);
	void Power(const bool get);
	void Term(const bool get);
	void Primary(const bool get);

	inline void CheckToken(const TokenType wanted)
	{
		if(type_ != wanted)
		{
			std::ostringstream s;
			s << "'" << static_cast <char>(wanted) << "' expected.";
			throw std::runtime_error(s.str());
		}
	}
};

std::map<std::string, std::string> Parser::variables;

void Parser::AddCommonVariables()
{
	variables["pi"] = std::to_string(PI);
	variables["e"] = std::to_string(E);
}

double Parser::RoundOff(double value, unsigned int precision)
{
	double pow_10 = pow(10.0, (double)precision);
	return round(value * pow_10) / pow_10;
}

void Parser::CheckVariables()
{
	pWord_ = program_.c_str();
	type_ = NONE;

	while (type_ != END) {
		GetToken();

		if (type_ == VARIABLE_ASSIGN) {
			//std::cout << "Assign!" << std::endl;
			AssignVariable(word_);
			break;
		}

		if (type_ == VARIABLE_SUBSTITUTION)
			//std::cout << "Substitution!" << std::endl;
			SubstituteVariable(word_);
	}

	std::cout << "Expression: " << program_ << std::endl;
}

bool Parser::AssignVariable(std::string variableName)
{
	while (*pWord_ && isspace(*pWord_))
		++pWord_;

	if (*pWord_ != 0) {
		std::string variable_value = std::string((char*)pWord_, strlen(pWord_));
		variables[variableName] = variable_value;

		while (*pWord_ != 0)
			++pWord_;

		if (VERBOSE)
			std::cout << "VARIABLE_ASSIGNED(" << variableName << ": " << variable_value << ")" << std::endl;
		return true;
	}
	else
		return false;
}

bool Parser::SubstituteVariable(std::string variableName)
{
	auto search = variables.find(variableName);

	if (search != variables.end()) {
		std::string program_copy = program_;

		size_t index = 0;
		index = program_copy.find(variableName, index);

		auto start = program_copy.begin() + index;
		auto end = program_copy.begin() + index + variableName.size();
		program_copy.replace(start, end, variables[variableName]);
		if (VERBOSE)
			std::cout << "REPLACING: " << variableName << ", " << variables[variableName] << std::endl;

		program_ = program_copy;
		pWord_ = program_.c_str();

		return true;
	}
	else
		return false;
}

Parser::TokenType Parser::CheckFunction(std::string name)
{
	// functions
	if (name == std::string("ln")) {
		if (VERBOSE) 
			std::cout << "LN function" << std::endl;
		return type_ = TokenType(LN_FN);
	}
	else if (name == std::string("RAD")) {
		if (VERBOSE)
			std::cout << "RAD function" << std::endl;
		return type_ = TokenType(RAD_FN);
	}
	else if (name == std::string("DEG")) {
		if (VERBOSE)
			std::cout << "DEG function" << std::endl;
		return type_ = TokenType(DEG_FN);
	}
	else if (name == std::string("sin")) {
		if (VERBOSE)
			std::cout << "SIN function" << std::endl;
		return type_ = TokenType(SIN_FN);
	}
	else if (name == std::string("cos")) {
		if (VERBOSE)
			std::cout << "COS function" << std::endl;
		return type_ = TokenType(COS_FN);
	}
	else if (name == std::string("tan")) {
		if (VERBOSE)
			std::cout << "TAN function" << std::endl;
		return type_ = TokenType(TAN_FN);
	}
	else if (name == std::string("abs")) {
		if (VERBOSE)
			std::cout << "ABS function" << std::endl;
		return type_ = TokenType(ABS_FN);
	}
	else if (name == std::string("log")) {
		if (VERBOSE)
			std::cout << "LOG function" << std::endl;
		return type_ = TokenType(LOG_FN);
	}
	else if (name == std::string("exp")) {
		if (VERBOSE)
			std::cout << "EXP function" << std::endl;
		return type_ = TokenType(EXP_FN);
	}
	else if (name == std::string("sqrt")) {
		if (VERBOSE)
			std::cout << "SQRT function" << std::endl;
		return type_ = TokenType(SQRT_FN);
	}
	/*if (word_ == std::string("pow")) {
		std::cout << "POW function" << std::endl;
		return type_ = TokenType(POW_FN);
	}*/
	else
		return type_ = TokenType(NONE);
}

const Parser::TokenType Parser::GetToken(const bool ignoreSign)
{
	word_.erase(0, std::string::npos);

	while (*pWord_ && isspace(*pWord_))
		++pWord_;

	pWordStart_ = pWord_;

	if (*pWord_ == 0 &&
		type_ == END)
		throw std::runtime_error("Unexpected end of expression.");

	unsigned char cFirstCharacter = *pWord_;

	if (cFirstCharacter == 0)
	{
		word_ = "<end of expression>";
		if (VERBOSE)
			std::cout << "END" << std::endl;
		return type_ = END;
	}

	unsigned char cNextCharacter = *(pWord_ + 1);

	if ((!ignoreSign &&
		(cFirstCharacter == '+' || cFirstCharacter == '-') &&
		(isdigit(cNextCharacter) || cNextCharacter == '.')
		)
		|| isdigit(cFirstCharacter)
		|| (cFirstCharacter == '.' && isdigit(cNextCharacter)))
	{
		if ((cFirstCharacter == '+' || cFirstCharacter == '-'))
			pWord_++;

		while (isdigit(*pWord_) || *pWord_ == '.')
			pWord_++;

		word_ = std::string(pWordStart_, pWord_ - pWordStart_);

		std::istringstream is(word_);

		is >> value_;

		if (is.fail() || !is.eof())
			throw std::runtime_error("Bad numeric literal: " + word_);

		if (VERBOSE)
			std::cout << "NUMBER (" << value_ << ")" << std::endl;
		return type_ = NUMBER;
	}

	switch (cFirstCharacter)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
	case '(':
	case ')':
		word_ = std::string(pWordStart_, 1);
		++pWord_;
		if (VERBOSE)
			std::cout << cFirstCharacter << std::endl;
		return type_ = TokenType(cFirstCharacter);
	}

	if (!isalpha(cFirstCharacter))
	{
		if (cFirstCharacter < ' ')
		{
			std::ostringstream s;
			s << "Unexpected character (decimal " << int(cFirstCharacter) << ")";
			throw std::runtime_error(s.str());
		}
		else
			throw std::runtime_error("Unexpected character: " + std::string(1, cFirstCharacter));
	}
	else { 
		
		pWord_ = pWordStart_;

		while (*pWord_ && (isalnum(*pWord_) || (*pWord_ == '_')))
			++pWord_;
		
		word_ = std::string(pWordStart_, pWord_ - pWordStart_);

		// check functions
		TokenType t;
		if ((t = CheckFunction(word_)) != TokenType(NONE))
			return type_ = t;

		// check variables
		std::string variable_name = word_;

		while (*pWord_ && isspace(*pWord_))
			++pWord_;

		if (*pWord_ == '=') {// assignment
			++pWord_;
			return type_ = TokenType(VARIABLE_ASSIGN);
		}

		auto search = variables.find(variable_name);
		if (search != variables.end())  // substitution
			return type_ = TokenType(VARIABLE_SUBSTITUTION);

		throw std::runtime_error("Unexpected alphanumeric characters: " + word_);
	}
}

void Parser::Primary(const bool get)
{
	if(get)
		GetToken();

	switch(type_)
	{
		case NUMBER:
		{
			nodes.push_back(new NumNode(value_));
			GetToken(true);
			//Power(true);
			break;
		}
		case MINUS:
		{ 
			break;
		}
		case LHPAREN:
		{
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			//Power(true);
			break;
		}
		case RAD_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("RAD", nodes.back()));
			break;
		}
		case DEG_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("DEG", nodes.back()));
			break;
		}
		case SIN_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("sin", nodes.back()));
			break;
		}
		case COS_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("cos", nodes.back()));
			break;
		}
		case TAN_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("tan", nodes.back()));
			break;
		}
		case ABS_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("abs", nodes.back()));
			break;
		}
		case LN_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("ln", nodes.back()));
			break;
		}
		case LOG_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("log", nodes.back()));
			break;
		}
		case EXP_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("exp", nodes.back()));
			break;
		}
		case SQRT_FN:
		{
			GetToken(true);
			CheckToken(LHPAREN);
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			nodes.push_back(new FuncNode("sqrt", nodes.back()));
			break;
		}
		default:
			throw std::runtime_error("Unexpected token: " + word_);
	}
}

void Parser::Power(const bool get)
{
	Primary(get);

	while (true)
	{
		switch (type_)
		{
		case POW:
		{
			BaseNode* temp = nodes.back();
			Primary(true);
			nodes.push_back(new OperNode('^', temp, nodes.back()));
			break;
		}

		default:
			return;
		}
	}
}

void Parser::Term(const bool get)
{
	Power(get);

	while(true)
	{
		switch(type_)
		{
			case MULTIPLY:
				{
					BaseNode* temp = nodes.back();
					Power(true); // get the next value...
					nodes.push_back(new OperNode('*', temp, nodes.back()));
					break;
				}

			case DIVIDE:
				{
					BaseNode* temp = nodes.back();
					Power(true);

					if(nodes.back()->value() == 0.0)
						throw std::runtime_error("Dividing by zero");

					nodes.push_back(new OperNode('/', temp, nodes.back()));
					break;
				}

			default:
				return;
		}
	}
}

void Parser::AddSubtract(const bool get)
{
	Term(get);

	while(true)
	{
		switch(type_)
		{
			case PLUS:
				{
					BaseNode* temp = nodes.back();
					Term(true);
					nodes.push_back(new OperNode('+', temp, nodes.back()));
					break;
				}

			case MINUS:
				{
					BaseNode* temp = nodes.back();
					Term(true);
					nodes.push_back(new OperNode('-', temp, nodes.back()));
					break; 
				}

			default:
				return;
		}
	}
}

const double Parser::Evaluate(unsigned int precision)
{
	// substitute variables
	CheckVariables();

	if (type_ == VARIABLE_ASSIGN)
		return 0.0;

	pWord_ = program_.c_str();
	type_ = NONE;

	AddSubtract(true);

	if(type_ != END)
		throw std::runtime_error("Unexpected text at the end of expression " + std::string(pWordStart_));
	
	return RoundOff(nodes.back()->value(), precision);
}

const double Parser::Evaluate(const std::string & program, unsigned int precision)
{
	program_ = program;
	nodes.clear();

	return Evaluate(precision);
}

int main()
{
	std::string inputline;

	try
	{
		while(true)
		{
			std::cout << "Enter expression: ";
			std::getline(std::cin, inputline);

			if(inputline.empty())
				return 0;

			Parser p(inputline);
			double value = p.Evaluate(5);

			std::cout << "Result = " << value << std::endl;
		}
	}
	catch(std::exception & e)
	{
		std::cout << "exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}