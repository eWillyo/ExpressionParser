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

value_t Do_vector(value_t value, double (*fnc)(double))
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

value_t Do_func(std::string func, BaseNode* _expression1, BaseNode* _expression2, BaseNode* _expression3)
{
	value_t result;

	if (func == std::string("RAD")) {
		return Do_vector(_expression1->value(), Radians);
	}
	else if (func == std::string("DEG")) {
		return Do_vector(_expression1->value(), Degrees);
	}
	else if (func == std::string("sin")) {
		return Do_vector(_expression1->value(), sin);
	}
	else if (func == std::string("cos")) {
		return Do_vector(_expression1->value(), cos);
	}
	else if (func == std::string("tan")) {
		return Do_vector(_expression1->value(), tan);
	}
	else if (func == std::string("asin")) {
		return Do_vector(_expression1->value(), asin);
	}
	else if (func == std::string("acos")) {
		return Do_vector(_expression1->value(), acos);
	}
	else if (func == std::string("atan")) {
		return Do_vector(_expression1->value(), atan);
	}
	else if (func == std::string("abs")) {
		return Do_vector(_expression1->value(), fabs);
	}
	else if (func == std::string("ln")) {
		return Do_vector(_expression1->value(), log);
	}
	else if (func == std::string("log")) {
		return Do_vector(_expression1->value(), log10);
	}
	else if (func == std::string("exp")) {
		return Do_vector(_expression1->value(), exp);
	}
	else if (func == std::string("sqrt")) {
		return Do_vector(_expression1->value(), sqrt);
	}
	else if (func == std::string("length")) {

		result._num_dims = 1;

		switch (_expression1->value()._num_dims)
		{
		case 1:
			result._value[0] = _expression1->value()._value[0];
			break;
		case 2:
			result._value[0] = glm::length(_expression1->value().to_vec2());
			break;
		case 3:
			result._value[0] = glm::length(_expression1->value().to_vec3());
			break;
		case 4:
			result._value[0] = glm::length(_expression1->value().to_vec4());
			break;
		}

		return result;
	}
	else if (func == std::string("normalize")) {

		switch (_expression1->value()._num_dims)
		{
		case 2:
			result._num_dims = 2;
			result.vec2(glm::normalize(_expression1->value().to_vec2()));
			break;
		case 3:
			result._num_dims = 3;
			result.vec3(glm::normalize(_expression1->value().to_vec3()));
			break;
		case 4:
			result._num_dims = 4;
			result.vec4(glm::normalize(_expression1->value().to_vec4()));
			break;
		default:
			throw std::runtime_error("Wrong n-dimensional vector");
		}

		return result;
	}
	else if (func == std::string("dot")) {

		value_t left = _expression1->value();
		value_t right = _expression2->value();

		if (left._num_dims != right._num_dims)
			throw std::runtime_error("Different dimension count at n-dimensional function");

		result._num_dims = 1;

		switch (left._num_dims)
		{
		case 2:
			result._value[0] = glm::dot(left.to_vec2(), right.to_vec2());
			break;
		case 3:
			result._value[0] = glm::dot(left.to_vec3(), right.to_vec3());
			break;
		case 4:
			result._value[0] = glm::dot(left.to_vec4(), right.to_vec4());
			break;
		default: throw std::runtime_error("Wrong n-dimensional vector");
		}

		return result;
	}
	else if (func == std::string("cross")) {

		value_t left = _expression1->value();
		value_t right = _expression2->value();

		if ((left._num_dims != 3) && (right._num_dims != 3))
			throw std::runtime_error("Different dimension count at n-dimensional function");

		result._num_dims = 3;
		result.vec3(glm::cross(left.to_vec3(), right.to_vec3()));

		return result;
	}
	else if (func == std::string("mix")) {

		value_t first = _expression1->value();
		value_t second = _expression2->value();
		value_t third = _expression3->value();

		if (first._num_dims != second._num_dims)
			throw std::runtime_error("Different dimension count at n-dimensional function");

		if (third._num_dims != 1)
			throw std::runtime_error("Third parameter of function must be scalar value");

		result._num_dims = first._num_dims;
		switch (result._num_dims)
		{
		case 1:
			result._value[0] = glm::mix(first._value[0], second._value[0], third._value[0]);
			break;
		case 2:
			result.vec2(glm::mix(first.to_vec2(), second.to_vec2(), third._value[0]));
			break;
		case 3:
			result.vec3(glm::mix(first.to_vec3(), second.to_vec3(), third._value[0]));
			break;
		case 4:
			result.vec4(glm::mix(first.to_vec4(), second.to_vec4(), third._value[0]));
			break;
		default: throw std::runtime_error("Wrong n-dimensional vectors");
		}

		return result;
	}
	else
		throw std::runtime_error("Unknown function: " + func);

	// etc...
}