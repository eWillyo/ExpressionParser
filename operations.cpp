#include "operations.h"
#include "constants.h"
#include "types.h"


double Radians(double degrees)
{
	return (degrees * (M_PI / 180));
}

double Degrees(double radians)
{
	return ((radians * 180) / M_PI);
}

double RoundOff(double value, unsigned int precision)
{
	double pow_10 = pow(10.0, (double)precision);
	return round(value * pow_10) / pow_10;
}

value_t Do_func(value_t value, double (*fnc)(double))
{
	for (unsigned int i = 0; i < value._num_dims; i++)
		value._value[i] = fnc((double)value._value[i]);

	return value;
}

value_t Do_scalar(value_t lvalue, value_t rvalue, double (*fnc)(double, double))
{
	value_t result;
	result._num_dims = lvalue._num_dims;

	for (unsigned int i = 0; i < lvalue._num_dims; i++)
		result._value[i] = fnc((double)lvalue._value[i], (double)rvalue._value[0]);

	return result;
}

value_t Do_oper(value_t lvalue, value_t rvalue, double (*fnc)(double, double))
{
	value_t result;
	result._num_dims = lvalue._num_dims;

	for (unsigned int i = 0; i < lvalue._num_dims; i++)
		result._value[i] = fnc((double)lvalue._value[i], (double)rvalue._value[i]);

	return result;
}
