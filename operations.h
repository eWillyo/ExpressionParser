#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "types.h"


double Radians(double degrees);
double Degrees(double radians);
double RoundOff(double value, unsigned int precision);

value_t Do_vector(value_t value, double (*fnc)(double));
value_t Do_scalar(value_t lvalue, value_t rvalue, double (*fnc)(double, double));
value_t Do_oper(value_t lvalue, value_t rvalue, double (*fnc)(double, double));

value_t Do_func(std::string func, BaseNode* _expression1, BaseNode* _expression2, BaseNode* _expression3);

#endif