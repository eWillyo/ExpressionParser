#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "functions.h"
#include "types.h"


namespace Math_solver {

	double Radians(double degrees);
	double Degrees(double radians);
	double RoundOff(double value, unsigned int precision);
	double Factorial(double value);
	double Random(double value);

	std::string Format_number(double value, unsigned int precision);

	value_t Do_vector(value_t value, double (*fnc)(double));
	value_t Do_vector_scalar(value_t lvalue, value_t rvalue, double (*fnc)(double, double));
	value_t Do_vector_vector(value_t lvalue, value_t rvalue, double (*fnc)(double, double));
	value_t Do_matrix_scalar(value_t lvalue, value_t rvalue, double (*fnc)(double, double));

	value_t Do_func(TokenType func, BaseNode* expression1, BaseNode* expression2, BaseNode* expression3, BaseNode* expression4);

}

#endif // !OPERATIONS_H