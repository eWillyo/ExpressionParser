
// Expression parser to solve simple math problems..
// Original thought: 31.5.2010 (significantly inspired by one old project on "codeproject.com")


#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>

#include "operations.h"
#include "types.h"
#include "constants.h"


#define VERBOSE		false


class Parser
{
public:
	enum TokenType
	{
		NONE,
		SCALAR,
		VARIABLE_ASSIGN,
		VARIABLE_SUBSTITUTION,
		END,
		RAD_FN,
		DEG_FN,
		SIN_FN,
		COS_FN,
		TAN_FN,
		ASIN_FN,
		ACOS_FN,
		ATAN_FN,
		ABS_FN,
		LN_FN,
		LOG_FN,
		EXP_FN,
		SQRT_FN,
		VEC2_FN,
		VEC3_FN,
		VEC4_FN,
		LENGTH_FN,
		NORMALIZE_FN,
		DOT_PRODUCT_FN,
		CROSS_PRODUCT_FN,
		MIX_FN,
		PLUS='+',
		MINUS='-',
		MULTIPLY='*',
		DIVIDE='/',
		POW='^',
		LHPAREN='(',
		RHPAREN=')',
		COMMA=','
	};

private:
	std::string program_;

	const char * pWord_;
	const char * pWordStart_;

	unsigned int dim_row_index_;
	unsigned int dim_col_index_;
	unsigned int max_dim_;

	bool filling_multi_;

	TokenType type_;
	std::string word_;
	value_t value_;

	std::vector<BaseNode*> nodes;

	static std::map<std::string, std::string> variables;

public:
	Parser(const std::string & program)
		: program_(program)
	{
		AddCommonVariables();
	}

	double RoundOff(double value, unsigned int precision);
	std::string Value_to_str(value_t value, unsigned int precision = 2);

	const value_t Evaluate();
	const value_t Evaluate(const std::string & program);

private:
	void AddCommonVariables();

	void CheckVariables();
	bool AssignVariable(std::string variableName);
	void SubstituteVariable(std::string variableName, std::string& program, const char* from);

	TokenType CheckFunction(std::string functionName);
	void ExecuteOneOneParameterFunction(std::string functionName);
	void ExecuteTwoParameterFunction(std::string functionName);
	void ExecuteThreeParameterFunction(std::string functionName);
	void CreateVector();

	const TokenType GetToken(const bool ignoreSign = false);
	void Primary(const bool get);
	void Power(const bool get);
	void Term(const bool get);
	void AddSubtract(const bool get);

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
	variables["pi"] = std::to_string(M_PI);
	variables["e"] = std::to_string(M_E);
}

void Parser::CheckVariables()
{
	pWord_ = program_.c_str();
	type_ = NONE;

	while (type_ != END) {
		const char* before = pWord_;

		GetToken(true);

		if (type_ == VARIABLE_ASSIGN) {
			AssignVariable(word_);
			break;
		}

		if (type_ == VARIABLE_SUBSTITUTION)
			SubstituteVariable(word_, program_, before);
	}

	if (VERBOSE)
		std::cout << "Expression: " << program_ << std::endl;
}

bool Parser::AssignVariable(std::string variableName)
{
	while (*pWord_ && isspace(*pWord_))
		++pWord_;

	if (*pWord_ != 0) {
		std::string variable_value = std::string((char*)pWord_, strlen(pWord_));

		// avoid cyclic assignment
		pWord_ = variable_value.c_str();
		type_ = NONE;

		while (type_ != END) {
			const char* before = pWord_;

			GetToken(true);

			if (type_ == VARIABLE_SUBSTITUTION) {
				if (word_ == variableName) {
					if (VERBOSE)
						std::cout << "Cyclic assigment: " << variable_value << std::endl;
					SubstituteVariable(variableName, variable_value, before);
				}
			}
		}

		pWord_ = program_.c_str();
		type_ = VARIABLE_ASSIGN;
		variables[variableName] = variable_value;

		if (VERBOSE)
			std::cout << "VARIABLE_ASSIGNED(" << variableName << ": " << variable_value << ")" << std::endl;

		return true;
	}
	else
		return false;
}

void Parser::SubstituteVariable(std::string variableName, std::string& program, const char* from)
{
	std::string program_copy = program;

	while (*from && isspace(*from))
		++from;

	if (*from != 0) {

		size_t variable_length = variableName.length();
		size_t index = from - program.c_str();

		auto start = program_copy.begin() + index;
		auto end = start + variable_length;
		program_copy.replace(start, end, variables[variableName]);

		if (VERBOSE)
			std::cout << "REPLACING: " << variableName << ", " << variables[variableName] << std::endl;

		program = program_copy;
		pWord_ = program.c_str();
		type_ = NONE;
	}
}

Parser::TokenType Parser::CheckFunction(std::string name)
{
	// functions
	if (name == std::string("RAD")) {

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
	else if (name == std::string("asin")) {

		if (VERBOSE)
			std::cout << "ASIN function" << std::endl;

		return type_ = TokenType(ASIN_FN);
	}
	else if (name == std::string("acos")) {

		if (VERBOSE)
			std::cout << "ACOS function" << std::endl;

		return type_ = TokenType(ACOS_FN);
	}
	else if (name == std::string("atan")) {

		if (VERBOSE)
			std::cout << "ATAN function" << std::endl;

		return type_ = TokenType(ATAN_FN);
	}
	else if (name == std::string("abs")) {

		if (VERBOSE)
			std::cout << "ABS function" << std::endl;

		return type_ = TokenType(ABS_FN);
	}
	if (name == std::string("ln")) {

		if (VERBOSE)
			std::cout << "LN function" << std::endl;

		return type_ = TokenType(LN_FN);
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
	else if (name == std::string("vec2")) {

		if (VERBOSE)
			std::cout << "VEC2 function" << std::endl;

		return type_ = TokenType(VEC2_FN);
	}
	else if (name == std::string("vec3")) {

		if (VERBOSE)
			std::cout << "VEC3 function" << std::endl;

		return type_ = TokenType(VEC3_FN);
	}
	else if (name == std::string("vec4")) {

		if (VERBOSE)
			std::cout << "VEC4 function" << std::endl;

		return type_ = TokenType(VEC4_FN);
	}
	else if (name == std::string("length")) {

		if (VERBOSE)
			std::cout << "LENGTH function" << std::endl;

		return type_ = TokenType(LENGTH_FN);
	}
	else if (name == std::string("normalize")) {

		if (VERBOSE)
			std::cout << "NORMALIZE function" << std::endl;

		return type_ = TokenType(NORMALIZE_FN);
	}
	else if (name == std::string("dot")) {

		if (VERBOSE)
			std::cout << "DOT PRODUCT function" << std::endl;

		return type_ = TokenType(DOT_PRODUCT_FN);
	}
	else if (name == std::string("cross")) {

		if (VERBOSE)
			std::cout << "CROSS PRODUCT function" << std::endl;

		return type_ = TokenType(CROSS_PRODUCT_FN);
	}
	else if (name == std::string("mix")) {

		if (VERBOSE)
			std::cout << "MIX function" << std::endl;

		return type_ = TokenType(MIX_FN);
	}
	else
		return type_ = TokenType(NONE);
}

void Parser::ExecuteOneOneParameterFunction(std::string functionName)
{
	BaseNode* first_par;

	GetToken(true);
	CheckToken(LHPAREN);

	AddSubtract(true);
	first_par = nodes.back();

	CheckToken(RHPAREN);
	GetToken(true);

	nodes.push_back(new FuncNode(functionName, first_par));
}

void Parser::ExecuteTwoParameterFunction(std::string functionName)
{
	BaseNode *first_par, *second_par;

	GetToken(true);
	CheckToken(LHPAREN);

	AddSubtract(true);
	first_par = nodes.back();

	CheckToken(COMMA);

	AddSubtract(true);
	second_par = nodes.back();

	CheckToken(RHPAREN);
	GetToken(true);

	nodes.push_back(new FuncNode(functionName, first_par, second_par));
}

void Parser::ExecuteThreeParameterFunction(std::string functionName)
{
	BaseNode *first_par, *second_par, *third_par;
	GetToken(true);
	CheckToken(LHPAREN);

	AddSubtract(true);
	first_par = nodes.back();

	CheckToken(COMMA);

	AddSubtract(true);
	second_par = nodes.back();

	CheckToken(COMMA);

	AddSubtract(true);
	third_par = nodes.back();

	CheckToken(RHPAREN);
	GetToken(true);

	nodes.push_back(new FuncNode(functionName, first_par, second_par, third_par));
}

void Parser::CreateVector()
{
	dim_row_index_ = 0;
	filling_multi_ = true;

	for (unsigned int i = 0; i < max_dim_; i++)
		value_._value[i] = 0.0;

	GetToken(true);
	CheckToken(LHPAREN);

	do {
		if (type_ == RHPAREN)
			break;

		AddSubtract(true);
	} while (type_ == COMMA);

	nodes.push_back(new NumNode(value_));

	filling_multi_ = false;
	dim_row_index_ = 0;
	max_dim_ = 1;
	GetToken(true);
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
		word_ = "<End of expression>";

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

		is >> value_._value[dim_row_index_];
		value_._num_dims = max_dim_;

		if (is.fail() || !is.eof())
			throw std::runtime_error("Bad numeric literal: " + word_);

		if (VERBOSE)
			std::cout << "SCALAR (" << value_._value[dim_row_index_] << ") [" << dim_row_index_ << "]" << std::endl;

		return type_ = SCALAR;
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
	case ',':
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
		case SCALAR:
		{
			if (!filling_multi_)
			{
				nodes.push_back(new NumNode(value_));
				GetToken(true);
			}
			else
				AddSubtract(true);
			break;
		}
		case MINUS:
		{ 
			break;
		}
		case COMMA:
		{
			if (filling_multi_)
			{
				++dim_row_index_;
				if (dim_row_index_ == max_dim_)
					throw std::runtime_error("Unexpected dimension index");
			}
			else
				throw std::runtime_error("Unexpected character: \",\"");
			break;
		}
		case LHPAREN:
		{
			AddSubtract(true);
			CheckToken(RHPAREN);
			GetToken(true);
			break;
		}
		case RHPAREN:
		{
			break;
		}
		case RAD_FN:
		{
			ExecuteOneOneParameterFunction("RAD");
			break;
		}
		case DEG_FN:
		{
			ExecuteOneOneParameterFunction("DEG");
			break;
		}
		case SIN_FN:
		{
			ExecuteOneOneParameterFunction("sin");
			break;
		}
		case COS_FN:
		{
			ExecuteOneOneParameterFunction("cos");
			break;
		}
		case TAN_FN:
		{
			ExecuteOneOneParameterFunction("tan");
			break;
		}
		case ASIN_FN:
		{
			ExecuteOneOneParameterFunction("asin");
			break;
		}
		case ACOS_FN:
		{
			ExecuteOneOneParameterFunction("acos");
			break;
		}
		case ATAN_FN:
		{
			ExecuteOneOneParameterFunction("atan");
			break;
		}
		case ABS_FN:
		{
			ExecuteOneOneParameterFunction("abs");
			break;
		}
		case LN_FN:
		{
			ExecuteOneOneParameterFunction("ln");
			break;
		}
		case LOG_FN:
		{
			ExecuteOneOneParameterFunction("log");
			break;
		}
		case EXP_FN:
		{
			ExecuteOneOneParameterFunction("exp");
			break;
		}
		case SQRT_FN:
		{
			ExecuteOneOneParameterFunction("sqrt");
			break;
		}
		case VEC2_FN:
		{
			max_dim_ = 2;
			CreateVector();
			break;
		}
		case VEC3_FN:
		{
			max_dim_ = 3;
			CreateVector();
			break;
		}
		case VEC4_FN:
		{
			max_dim_ = 4;
			CreateVector();
			break;
		}
		case LENGTH_FN:
		{
			ExecuteOneOneParameterFunction("length");
			break;
		}
		case NORMALIZE_FN:
		{
			ExecuteOneOneParameterFunction("normalize");
			break;
		}
		case DOT_PRODUCT_FN:
		{
			ExecuteTwoParameterFunction("dot");
			break;
		}
		case CROSS_PRODUCT_FN:
		{
			ExecuteTwoParameterFunction("cross");
			break;
		}
		case MIX_FN:
		{
			ExecuteThreeParameterFunction("mix");
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

					if(nodes.back()->value()._value[0] == 0.0)
						throw std::runtime_error("Division by zero!");

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

double Parser::RoundOff(double value, unsigned int precision)
{
	double pow_10 = pow(10.0, (double)precision);
	return round(value * pow_10) / pow_10;
}

std::string Parser::Value_to_str(value_t value, unsigned int precision)
{
	std::ostringstream result;

	if (value._num_dims == 1)
	{
		//result << std::to_string((double)RoundOff(value._value[0], precision));
		result << std::to_string(value._value[0]).substr(0, std::to_string(value._value[0]).find(".") + precision + 1);
		return result.str();
	}

	result << std::string("( ");

	for (unsigned int i = 0; i < value._num_dims; i++)
	{
		//result << std::to_string((double)RoundOff(value._value[i], precision));
		result << std::to_string(value._value[i]).substr(0, std::to_string(value._value[i]).find(".") + precision + 1);
		
		if (i < (value._num_dims - 1))
			result << std::string(", ");
	}

	result << std::string(" )");

	return result.str();
}

const value_t Parser::Evaluate()
{
	// substitute variables
	CheckVariables();

	if (type_ == VARIABLE_ASSIGN)
		return value_t();

	pWord_ = program_.c_str();
	type_ = NONE;

	dim_row_index_ = 0;
	dim_col_index_ = 0;

	max_dim_ = 1;
	filling_multi_ = false;

	// solve expression
	AddSubtract(true);

	if(type_ != END)
		throw std::runtime_error("Unexpected text at the end of expression " + std::string(pWordStart_));
	
	return nodes.back()->value();
}

const value_t Parser::Evaluate(const std::string & program)
{
	program_ = program;
	nodes.clear();

	return Evaluate();
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
			value_t value = p.Evaluate();

			std::cout << "Result: " << p.Value_to_str(value, 3) << std::endl;
		}
	}
	catch(std::exception & e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}