#include "parser.h"


namespace Math_solver {

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

	void Parser::ExecuteOneParameterFunction(TokenType functionName)
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

	void Parser::ExecuteTwoParameterFunction(TokenType functionName)
	{
		BaseNode* first_par, * second_par;

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

	void Parser::ExecuteThreeParameterFunction(TokenType functionName)
	{
		BaseNode* first_par, * second_par, * third_par;
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

	void Parser::ExecuteFourParameterFunction(TokenType functionName)
	{
		BaseNode* first_par, * second_par, * third_par, * fourth_par;

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

		CheckToken(COMMA);

		AddSubtract(true);
		fourth_par = nodes.back();

		CheckToken(RHPAREN);
		GetToken(true);

		nodes.push_back(new FuncNode(functionName, first_par, second_par, third_par, fourth_par));
	}

	void Parser::CreateVector()
	{
		dim_row_index_ = 0;
		filling_multi_ = true;

		value_ = value_t(glm::dvec4(0), max_dim_);

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

	void Parser::CreateMatrix()
	{
		value_ = value_t(glm::dmat4(1.0), max_dim_); // Identity matrix

		GetToken(true);
		CheckToken(LHPAREN);

		GetToken(true);
		CheckToken(RHPAREN);

		nodes.push_back(new NumNode(value_));
		max_dim_ = 1;
		GetToken(true);
	}

	const TokenType Parser::GetToken(const bool ignoreSign)
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

			is >> value_temp_[dim_row_index_];
			value_ = value_t(value_temp_, max_dim_);// fill_value_vec(max_dim_, value_temp_);

			if (is.fail() || !is.eof())
				throw std::runtime_error("Bad numeric literal: " + word_);

			if (VERBOSE)
				std::cout << "SCALAR (" << value_.vec.to_vec4()[dim_row_index_] << ") [" << dim_row_index_ << "]" << std::endl;

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
			if ((t = CheckFunction(word_, VERBOSE)) != TokenType(NONE))
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
		if (get)
			GetToken();

		switch (type_)
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
			ExecuteOneParameterFunction(RAD_FN);
			break;
		}
		case DEG_FN:
		{
			ExecuteOneParameterFunction(DEG_FN);
			break;
		}
		case SIN_FN:
		{
			ExecuteOneParameterFunction(SIN_FN);
			break;
		}
		case COS_FN:
		{
			ExecuteOneParameterFunction(COS_FN);
			break;
		}
		case TAN_FN:
		{
			ExecuteOneParameterFunction(TAN_FN);
			break;
		}
		case ASIN_FN:
		{
			ExecuteOneParameterFunction(ASIN_FN);
			break;
		}
		case ACOS_FN:
		{
			ExecuteOneParameterFunction(ACOS_FN);
			break;
		}
		case ATAN_FN:
		{
			ExecuteOneParameterFunction(ATAN_FN);
			break;
		}
		case ABS_FN:
		{
			ExecuteOneParameterFunction(ABS_FN);
			break;
		}
		case LN_FN:
		{
			ExecuteOneParameterFunction(LN_FN);
			break;
		}
		case LOG_FN:
		{
			ExecuteOneParameterFunction(LOG_FN);
			break;
		}
		case EXP_FN:
		{
			ExecuteOneParameterFunction(EXP_FN);
			break;
		}
		case SQRT_FN:
		{
			ExecuteOneParameterFunction(SQRT_FN);
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
		case MAT2_FN:
		{
			max_dim_ = 2;
			CreateMatrix();
			break;
		}
		case MAT3_FN:
		{
			max_dim_ = 3;
			CreateMatrix();
			break;
		}
		case MAT4_FN:
		{
			max_dim_ = 4;
			CreateMatrix();
			break;
		}
		case LENGTH_FN:
		{
			ExecuteOneParameterFunction(LENGTH_FN);
			break;
		}
		case NORMALIZE_FN:
		{
			ExecuteOneParameterFunction(NORMALIZE_FN);
			break;
		}
		case DOT_PRODUCT_FN:
		{
			ExecuteTwoParameterFunction(DOT_PRODUCT_FN);
			break;
		}
		case CROSS_PRODUCT_FN:
		{
			ExecuteTwoParameterFunction(CROSS_PRODUCT_FN);
			break;
		}
		case MIX_FN:
		{
			ExecuteThreeParameterFunction(MIX_FN);
			break;
		}
		case SCALE_FN:
		{
			ExecuteTwoParameterFunction(SCALE_FN);
			break;
		}
		case ROTATE_FN:
		{
			ExecuteThreeParameterFunction(ROTATE_FN);
			break;
		}
		case TRANSLATE_FN:
		{
			ExecuteTwoParameterFunction(TRANSLATE_FN);
			break;
		}
		case INVERSE_TRANSPOSE_FN:
		{
			ExecuteOneParameterFunction(INVERSE_TRANSPOSE_FN);
			break;
		}
		case PERSPECTIVE_PROJ_FN:
		{
			ExecuteFourParameterFunction(PERSPECTIVE_PROJ_FN);
			break;
		}
		case ORTHO_PROJ_FN:
		{
			ExecuteFourParameterFunction(ORTHO_PROJ_FN);
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

		while (true)
		{
			switch (type_)
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

				if (nodes.back()->value().vec.to_vec4()[0] == 0.0)
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

		while (true)
		{
			switch (type_)
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

	const value_t Parser::Evaluate()
	{
		// substitute variables
		CheckVariables();

		if (type_ == VARIABLE_ASSIGN)
			return value_t();

		pWord_ = program_.c_str();
		type_ = NONE;

		dim_row_index_ = 0;

		max_dim_ = 1;
		filling_multi_ = false;

		// solve expression
		AddSubtract(true);

		if (type_ != END)
			throw std::runtime_error("Unexpected text at the end of expression " + std::string(pWordStart_));

		return nodes.back()->value();
	}

	const value_t Parser::Evaluate(const std::string& program)
	{
		program_ = program;
		nodes.clear();

		return Evaluate();
	}

}