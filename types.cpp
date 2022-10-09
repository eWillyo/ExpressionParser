#include "types.h"
#include "operations.h"

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
	else if (lnumdims == rnumdims) // two (same) vectors
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
	value_t result;

	if (_func == std::string("RAD")) {
		return Do_func(_expression1->value(), Radians);
	}
	else if (_func == std::string("DEG")) {
		return Do_func(_expression1->value(), Degrees);
	}
	else if (_func == std::string("sin")) {
		return Do_func(_expression1->value(), sin);
	}
	else if (_func == std::string("cos")) {
		return Do_func(_expression1->value(), cos);
	}
	else if (_func == std::string("tan")) {
		return Do_func(_expression1->value(), tan);
	}
	else if (_func == std::string("asin")) {
		return Do_func(_expression1->value(), asin);
	}
	else if (_func == std::string("acos")) {
		return Do_func(_expression1->value(), acos);
	}
	else if (_func == std::string("atan")) {
		return Do_func(_expression1->value(), atan);
	}
	else if (_func == std::string("abs")) {
		return Do_func(_expression1->value(), fabs);
	}
	else if (_func == std::string("ln")) {
		return Do_func(_expression1->value(), log);
	}
	else if (_func == std::string("log")) {
		return Do_func(_expression1->value(), log10);
	}
	else if (_func == std::string("exp")) {
		return Do_func(_expression1->value(), exp);
	}
	else if (_func == std::string("sqrt")) {
		return Do_func(_expression1->value(), sqrt);
	}
	else if (_func == std::string("length")) {

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
	else if (_func == std::string("normalize")) {

		switch (_expression1->value()._num_dims)
		{
		case 1:
			throw std::runtime_error("Unknown operation on scalar");
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
		}

		return result;
	}
	else if (_func == std::string("dot")) {

		value_t left = _expression1->value();
		value_t right = _expression2->value();

		if (left._num_dims != right._num_dims)
			throw std::runtime_error("Different vector length at DOT product");

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
		default: throw std::runtime_error("Unknown scalar operation");
		}

		return result;
	}
	else if (_func == std::string("cross")) {

		value_t left = _expression1->value();
		value_t right = _expression2->value();

		if ((left._num_dims != 3) && (right._num_dims != 3))
			throw std::runtime_error("CROSS product needs 3D vectors");

		result._num_dims = 3;
		result.vec3(glm::cross(left.to_vec3(), right.to_vec3()));

		return result;
	}
	else if (_func == std::string("mix")) {

		value_t first = _expression1->value();
		value_t second = _expression2->value();
		value_t third = _expression3->value();

		if (first._num_dims != second._num_dims)
			throw std::runtime_error("Wrong number of dimensions for MIX function");

		if (third._num_dims != 1)
			throw std::runtime_error("Third parameter of MIX function must be scalar value");

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
		default: throw std::runtime_error("Wrong number of dimensions for MIX function");
		}

		return result;
	}
	else
		throw std::runtime_error("Unknown function: " + _func);

	// etc...
}
