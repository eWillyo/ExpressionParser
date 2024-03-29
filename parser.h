#ifndef PARSER_H
#define PARSER_H

#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>

#include "constants.h"
#include "operations.h"
#include "functions.h"
#include "types.h"
#include "error.h"


namespace Math_solver {

	class Parser
	{

	private:
		std::string program_;

		const char* pWord_;
		const char* pWordStart_;

		unsigned int dim_row_index_;
		unsigned int max_dim_;

		bool filling_multi_;

		TokenType type_;
		std::string word_;
		value_t value_;
		glm::dvec4 value_temp_;

		std::vector<BaseNode*> nodes;

		static std::map<std::string, std::string> variables;

		bool is_result_;

	public:
		Parser(const std::string& program)
			: program_(program)
		{
			AddCommonVariables();
		}

		const value_t Evaluate();
		const value_t Evaluate(const std::string& program);

		bool IsResult() const { return is_result_; }

	private:
		void AddCommonVariables();

		void CheckVariables();
		bool AssignVariable(std::string variableName);
		void SubstituteVariable(std::string variableName, std::string& program, const char* from);

		void ExecuteOneParameterFunction(TokenType functionName);
		void ExecuteTwoParameterFunction(TokenType functionName);
		void ExecuteThreeParameterFunction(TokenType functionName);
		void ExecuteFourParameterFunction(TokenType functionName);
		void CreateVector();
		void CreateMatrix(TokenType t);

		const TokenType GetToken(const bool ignoreSign = false);
		void Primary(const bool get);
		void Power(const bool get);
		void Term(const bool get);
		void AddSubtract(const bool get);

		inline void CheckToken(const TokenType wanted)
		{
			if (type_ != wanted)
			{
				std::ostringstream s;
				s << "'" << static_cast <char>(wanted) << "' expected.";
				Throw_error(__FILE__, __LINE__, __func__, s.str().c_str());
			}
		}
	};

}

#endif // !PARSER_H