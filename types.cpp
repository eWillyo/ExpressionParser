#include "types.h"
#include "operations.h"
#include "config.h"
#include "error.h"

#include <sstream>
#include <iostream>
#include <iomanip>


namespace Math_solver {

	std::string value_vec_t::to_str(unsigned int max_precision) const
	{
		std::ostringstream result;

		if (_num_dims == 1)
		{
			if (FORMAT_RESULT)
				result << Format_number(_value[0], max_precision);
			else
				result << std::to_string(RoundOff(_value[0], max_precision));

			return result.str();
		}

		result << std::string("( ");

		for (unsigned int i = 0; i < _num_dims; i++)
		{
			if (FORMAT_RESULT)
				result << Format_number(_value[i], max_precision);
			else
				result << std::to_string(RoundOff(_value[i], max_precision));

			if (i < (_num_dims - 1))
				result << std::string(", ");
		}

		result << std::string(" )");

		return result.str();
	}

	std::string value_mat_t::to_str(unsigned int max_precision) const
	{
		std::ostringstream result;

		result << std::endl;

		for (unsigned int i = 0; i < _num_dims; i++)
		{
			result << std::string("( ");

			for (unsigned int j = 0; j < _num_dims; j++)
			{
				if (FORMAT_RESULT)
					result << Format_number(_value[i][j], max_precision);
				else
					result << std::to_string(RoundOff(_value[i][j], max_precision));

				if (j < (_num_dims - 1))
					result << std::string(", ");
			}

			result << std::string(" )");
			result << std::endl;
		}

		return result.str();
	}

	value_t operator *(const value_t& left, const value_t& right)
	{
		value_t result;

		if (!left.is_mat())
			Throw_error(__FILE__, __LINE__, __func__, "Bad operand order");

		if (right.is_mat())
		{
			if (left.mat.get_num_dims() != right.mat.get_num_dims())
				Throw_error(__FILE__, __LINE__, __func__, "Different bases");

			switch (right.mat.get_num_dims())
			{
			case 2:
				result.mat.set_mat2(left.mat.to_mat2() * right.mat.to_mat2());
				break;
			case 3:
				result.mat.set_mat3(left.mat.to_mat3() * right.mat.to_mat3());
				break;
			case 4:
				result.mat.set_mat4(left.mat.to_mat4() * right.mat.to_mat4());
				break;
			default:
				Throw_error(__FILE__, __LINE__, __func__, "Wrong number of dimensions");
			}
		}
		else // right is vector
		{
			if (left.mat.get_num_dims() != right.vec.get_num_dims())
				Throw_error(__FILE__, __LINE__, __func__, "Different bases");

			switch (right.vec.get_num_dims())
			{
			case 2:
				result.vec.set_vec2(left.mat.to_mat2() * right.vec.to_vec2());
				break;
			case 3:
				result.vec.set_vec3(left.mat.to_mat3() * right.vec.to_vec3());
				break;
			case 4:
				result.vec.set_vec4(left.mat.to_mat4() * right.vec.to_vec4());
				break;
			default:
				Throw_error(__FILE__, __LINE__, __func__, "Wrong number of dimensions");
			}
		}

		return result;
	}

	value_t OperNode::value()
	{
		value_t leftValue = left->value();
		value_t rightValue = right->value();
		value_t result;

		unsigned int lnumdims = left->value().vec.get_num_dims();
		unsigned int rnumdims = right->value().vec.get_num_dims();

		if (!leftValue.is_mat() && !rightValue.is_mat())
		{
			if ((lnumdims == 1) && (rnumdims == 1)) // two scalars
			{
				switch (oper)
				{
				case '+': return value_t(glm::dvec4(leftValue.vec.to_vec4()[0] + rightValue.vec.to_vec4()[0]));
				case '-': return value_t(glm::dvec4(leftValue.vec.to_vec4()[0] - rightValue.vec.to_vec4()[0]));
				case '*': return value_t(glm::dvec4(leftValue.vec.to_vec4()[0] * rightValue.vec.to_vec4()[0]));
				case '/': return value_t(glm::dvec4(leftValue.vec.to_vec4()[0] / rightValue.vec.to_vec4()[0]));
				case '%': 
					if (ceil(leftValue.vec.to_vec4()[0]) == leftValue.vec.to_vec4()[0] && 
						ceil(rightValue.vec.to_vec4()[0]) == rightValue.vec.to_vec4()[0]) // check if integer
						return value_t(glm::dvec4((long)leftValue.vec.to_vec4()[0] % (long)rightValue.vec.to_vec4()[0]));
					else
						Throw_error(__FILE__, __LINE__, __func__, "Both operand must be integer");
				case '^': return value_t(glm::dvec4(pow(leftValue.vec.to_vec4()[0], rightValue.vec.to_vec4()[0])));
				default: Throw_error(__FILE__, __LINE__, __func__, "Unknown scalar operator: %c", oper);
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
				default: Throw_error(__FILE__, __LINE__, __func__, "Unknown scalar-vector operator: %c", oper);
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
				default: Throw_error(__FILE__, __LINE__, __func__, "Unknown vector-scalar operator: %c", oper);
				}
			}
			else if (lnumdims == rnumdims) // two (same-dimensional) vectors
			{
				switch (oper)
				{
				case '+': return Do_oper(leftValue, rightValue, [](auto a, auto b) {return a + b; });
				case '-': return Do_oper(leftValue, rightValue, [](auto a, auto b) {return a - b; });
				default: Throw_error(__FILE__, __LINE__, __func__, "Unknown vector-vector operator: %c", oper);
				}
			}
		}

		if (leftValue.is_mat() && rightValue.is_mat()) // matrices
		{
			if (lnumdims == rnumdims) {
				switch (oper)
				{
				case '*': return (leftValue * rightValue);
				default: Throw_error(__FILE__, __LINE__, __func__, "Unknown matrix-matrix operator: %c", oper);
				}
			}
			else
				Throw_error(__FILE__, __LINE__, __func__, "Different bases");
		}

		if (leftValue.is_mat() && !rightValue.is_mat()) // matrix-vector
		{
			if (lnumdims == rnumdims) {
				switch (oper)
				{
				case '*': return (leftValue * rightValue);
				default: Throw_error(__FILE__, __LINE__, __func__, "Unknown matrix-vector operator: %c", oper);
				}
			}
			else
				Throw_error(__FILE__, __LINE__, __func__, "Different bases");
		}

		Throw_error(__FILE__, __LINE__, __func__, "Wrong operation");

		return result;
	}

	value_t FuncNode::value()
	{
		return Do_func(_func, _expression1, _expression2, _expression3, _expression4);
	}

}