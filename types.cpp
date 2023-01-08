#include "types.h"
#include "operations.h"

#include <sstream>
#include <iostream>


std::string value_t::to_str(unsigned int max_precision)
{
	std::ostringstream result;

	if (_num_dims == 1)
	{
		result << RoundOff(_value[0], max_precision);
		return result.str();
	}

	result << std::string("( ");

	for (unsigned int i = 0; i < _num_dims; i++)
	{
		result << RoundOff(_value[i], max_precision);

		if (i < (_num_dims - 1))
			result << std::string(", ");
	}

	result << std::string(" )");

	return result.str();
}

value_t OperNode::value()
{
	value_t leftValue = left->value();
	value_t rightValue = right->value();
	value_t result;

	unsigned int lnumdims = left->value()._num_dims;
	unsigned int rnumdims = right->value()._num_dims;

	if ((lnumdims == 1) && (rnumdims == 1)) // two scalars
	{
		switch (oper)
		{
		case '+': return value_t(glm::dvec4(leftValue._value[0] + rightValue._value[0]));
		case '-': return value_t(glm::dvec4(leftValue._value[0] - rightValue._value[0]));
		case '*': return value_t(glm::dvec4(leftValue._value[0] * rightValue._value[0]));
		case '/': return value_t(glm::dvec4(leftValue._value[0] / rightValue._value[0]));
		case '^': return value_t(glm::dvec4(pow(leftValue._value[0], rightValue._value[0])));
		default: throw std::runtime_error("Unknown scalar operator: " + oper);
		}
	}
	else if ((lnumdims == 1 && rnumdims != 1)) // one scalar, one vector
	{
		switch (oper)
		{
		case '+': return Do_scalar(rightValue, leftValue, [](auto a, auto b) {return a + b; });
		case '-': return Do_scalar(rightValue, leftValue, [](auto a, auto b) {return b - a; });
		case '*': return Do_scalar(rightValue, leftValue, [](auto a, auto b) {return a * b; });
		case '/': return Do_scalar(rightValue, leftValue, [](auto a, auto b) {return b / a; });
		default: throw std::runtime_error("Unknown scalar-vector operator: " + oper);
		}
	}
	else if ((lnumdims != 1 && rnumdims == 1)) // one vector, one scalar
	{
		switch (oper)
		{
		case '+': return Do_scalar(leftValue, rightValue, [](auto a, auto b) {return a + b; });
		case '-': return Do_scalar(leftValue, rightValue, [](auto a, auto b) {return a - b; });
		case '*': return Do_scalar(leftValue, rightValue, [](auto a, auto b) {return a * b; });
		case '/': return Do_scalar(leftValue, rightValue, [](auto a, auto b) {return a / b; });
		default: throw std::runtime_error("Unknown vector-scalar operator: " + oper);
		}
	}
	else if (lnumdims == rnumdims) // two (same-dimensional) vectors
	{
		switch (oper)
		{
		case '+': return Do_oper(leftValue, rightValue, [](auto a, auto b) {return a + b; });
		case '-': return Do_oper(leftValue, rightValue, [](auto a, auto b) {return a - b; });
		default: throw std::runtime_error("Unknown vector-vector operator: " + oper);
		}
	}
	else
		throw std::runtime_error("Different vectors");
}

value_t FuncNode::value()
{
	return Do_func(_func, _expression1, _expression2, _expression3);
}
