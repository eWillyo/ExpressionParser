#pragma once

#include "types.h"


double Radians(double degrees);
double Degrees(double radians);
value_t Do_func(value_t value, double (*fnc)(double));
value_t Do_scalar(value_t lvalue, value_t rvalue, double (*fnc)(double, double));
value_t Do_oper(value_t lvalue, value_t rvalue, double (*fnc)(double, double));


