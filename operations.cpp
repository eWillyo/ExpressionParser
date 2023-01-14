#include "constants.h"
#include "types.h"
#include "operations.h"


namespace Math_solver {

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
		glm::dvec4 result_temp(0);

		for (unsigned int i = 0; i < value.vec.get_num_dims(); i++)
			result_temp[i] = fnc((double)value.vec.to_vec4()[i]);

		return value_t(result_temp, value.vec.get_num_dims());
	}

	value_t Do_scalar(value_t lvalue, value_t rvalue, double (*fnc)(double, double))
	{
		glm::dvec4 result_temp(0);

		for (unsigned int i = 0; i < lvalue.vec.get_num_dims(); i++)
			result_temp[i] = fnc((double)lvalue.vec.to_vec4()[i], (double)rvalue.vec.to_vec4()[0]);

		return value_t(result_temp, lvalue.vec.get_num_dims());//fill_value_vec(lvalue.vec.get_num_dims(), result_temp);
	}

	value_t Do_oper(value_t lvalue, value_t rvalue, double (*fnc)(double, double))
	{
		glm::dvec4 result_temp(0);

		for (unsigned int i = 0; i < lvalue.vec.get_num_dims(); i++)
			result_temp[i] = fnc((double)lvalue.vec.to_vec4()[i], (double)rvalue.vec.to_vec4()[i]);

		return value_t(result_temp, lvalue.vec.get_num_dims());//fill_value_vec(lvalue.vec.get_num_dims(), result_temp);
	}

	value_t Do_func(TokenType func, BaseNode* expression1, BaseNode* expression2, BaseNode* expression3, BaseNode* expression4)
	{
		value_t result, first, second, third, fourth;

		switch (func)
		{
		case RAD_FN:
			return Do_vector(expression1->value(), Radians);
		case DEG_FN:
			return Do_vector(expression1->value(), Degrees);
		case SIN_FN:
			return Do_vector(expression1->value(), sin);
		case COS_FN:
			return Do_vector(expression1->value(), cos);
		case TAN_FN:
			return Do_vector(expression1->value(), tan);
		case ASIN_FN:
			return Do_vector(expression1->value(), asin);
		case ACOS_FN:
			return Do_vector(expression1->value(), acos);
		case ATAN_FN:
			return Do_vector(expression1->value(), atan);
		case ABS_FN:
			return Do_vector(expression1->value(), fabs);
		case LN_FN:
			return Do_vector(expression1->value(), log);
		case LOG_FN:
			return Do_vector(expression1->value(), log10);
		case EXP_FN:
			return Do_vector(expression1->value(), exp);
		case SQRT_FN:
			return Do_vector(expression1->value(), sqrt);
		case LENGTH_FN:
			switch (expression1->value().vec.get_num_dims())
			{
			case 1:
				result.vec.set_scalar(expression1->value().vec.to_scalar());
				break;
			case 2:
				result.vec.set_scalar(glm::length(expression1->value().vec.to_vec2()));
				break;
			case 3:
				result.vec.set_scalar(glm::length(expression1->value().vec.to_vec3()));
				break;
			case 4:
				result.vec.set_scalar(glm::length(expression1->value().vec.to_vec4()));
				break;
			default: throw std::runtime_error("Wrong n-dimensional vector");
			}

			return result;
		case NORMALIZE_FN:
			switch (expression1->value().vec.get_num_dims())
			{
			case 2:
				result.vec.set_vec2(glm::normalize(expression1->value().vec.to_vec2()));
				break;
			case 3:
				result.vec.set_vec3(glm::normalize(expression1->value().vec.to_vec3()));
				break;
			case 4:
				result.vec.set_vec4(glm::normalize(expression1->value().vec.to_vec4()));
				break;
			default: throw std::runtime_error("Wrong n-dimensional vector");
			}

			return result;
		case DOT_PRODUCT_FN:
			first = expression1->value();
			second = expression2->value();

			if (first.vec.get_num_dims() != second.vec.get_num_dims())
				throw std::runtime_error("Different dimension count at n-dimensional function");

			switch (first.vec.get_num_dims())
			{
			case 2:
				result.vec.set_scalar(glm::dot(first.vec.to_vec2(), second.vec.to_vec2()));
				break;
			case 3:
				result.vec.set_scalar(glm::dot(first.vec.to_vec3(), second.vec.to_vec3()));
				break;
			case 4:
				result.vec.set_scalar(glm::dot(first.vec.to_vec4(), second.vec.to_vec4()));
				break;
			default: throw std::runtime_error("Wrong n-dimensional vector");
			}

			return result;
		case CROSS_PRODUCT_FN:
			first = expression1->value();
			second = expression2->value();

			if ((first.vec.get_num_dims() != 3) && (second.vec.get_num_dims() != 3))
				throw std::runtime_error("Different dimension count at n-dimensional function");

			result.vec.set_vec3(glm::cross(first.vec.to_vec3(), second.vec.to_vec3()));

			return result;
		case MIX_FN:
			first = expression1->value();
			second = expression2->value();
			third = expression3->value();

			if (first.vec.get_num_dims() != second.vec.get_num_dims())
				throw std::runtime_error("Different dimension count at n-dimensional function");

			if (third.vec.get_num_dims() != 1)
				throw std::runtime_error("Third parameter of function must be scalar value");

			switch (first.vec.get_num_dims())
			{
			case 1:
				result.vec.set_scalar(glm::mix(first.vec.to_scalar(), second.vec.to_scalar(), third.vec.to_scalar()));
				break;
			case 2:
				result.vec.set_vec2(glm::mix(first.vec.to_vec2(), second.vec.to_vec2(), third.vec.to_scalar()));
				break;
			case 3:
				result.vec.set_vec3(glm::mix(first.vec.to_vec3(), second.vec.to_vec3(), third.vec.to_scalar()));
				break;
			case 4:
				result.vec.set_vec4(glm::mix(first.vec.to_vec4(), second.vec.to_vec4(), third.vec.to_scalar()));
				break;
			default: throw std::runtime_error("Wrong n-dimensional vectors");
			}

			return result;
		case SCALE_FN:
			first = expression1->value();
			second = expression2->value();

			if (!first.is_mat())
				throw std::runtime_error("First parameter must be 4x4 matrix");
			if (second.is_mat() || second.vec.get_num_dims() != 3)
				throw std::runtime_error("Wrong second parameter");

			result.mat.set_mat4(glm::scale((glm::mat4)first.mat.to_mat4(), (glm::vec3)second.vec.to_vec3()));

			return result;
		case ROTATE_FN:
			first = expression1->value();
			second = expression2->value();
			third = expression3->value();

			if (!first.is_mat() || first.mat.get_num_dims() != 4)
				throw std::runtime_error("First parameter must be 4x4 matrix");
			if (second.vec.get_num_dims() != 1)
				throw std::runtime_error("Second parameter must be scalar");
			if (third.is_mat() || third.vec.get_num_dims() != 3)
				throw std::runtime_error("Third parameter must be 3D vector");

			result.mat.set_mat4(glm::rotate(first.mat.to_mat4(), second.vec.to_scalar(), third.vec.to_vec3()));

			return result;
		case TRANSLATE_FN:
			first = expression1->value();
			second = expression2->value();

			if (!first.is_mat() || first.mat.get_num_dims() != 4)
				throw std::runtime_error("First parameter must be 4x4 matrix");
			if (second.is_mat() || second.vec.get_num_dims() != 3)
				throw std::runtime_error("Second parameter must be 3D vector");

			result.mat.set_mat4(glm::translate(first.mat.to_mat4(), second.vec.to_vec3()));

			return result;
		case INVERSE_TRANSPOSE_FN:
			first = expression1->value();

			if (!first.is_mat())
				throw std::runtime_error("Parameter must be matrix");

			switch (first.vec.get_num_dims())
			{
			case 2:
				result.mat.set_mat2(glm::inverseTranspose(first.mat.to_mat2()));
				break;
			case 3:
				result.mat.set_mat3(glm::inverseTranspose(first.mat.to_mat3()));
				break;
			case 4:
				result.mat.set_mat4(glm::inverseTranspose(first.mat.to_mat4()));
				break;
			default: throw std::runtime_error("Wrong n-dimensional vectors");
			}

			return result;
		case PERSPECTIVE_PROJ_FN:
			first = expression1->value();
			second = expression2->value();
			third = expression3->value();
			fourth = expression4->value();

			if (first.is_mat() || second.is_mat() || third.is_mat() || fourth.is_mat())
				throw std::runtime_error("None of parameters can't be matrix");
			if (first.vec.get_num_dims() != 1 ||
				second.vec.get_num_dims() != 1 ||
				third.vec.get_num_dims() != 1 ||
				fourth.vec.get_num_dims() != 1)
				throw std::runtime_error("All parameters have to be scalar");

			result.mat.set_mat4(glm::perspective(
				first.vec.to_scalar(),
				second.vec.to_scalar(),
				third.vec.to_scalar(),
				fourth.vec.to_scalar()));

			return result;
		case ORTHO_PROJ_FN:
			first = expression1->value();
			second = expression2->value();
			third = expression3->value();
			fourth = expression4->value();

			if (first.is_mat() || second.is_mat() || third.is_mat() || fourth.is_mat())
				throw std::runtime_error("None of parameters can't be matrix");
			if (first.vec.get_num_dims() != 1 ||
				second.vec.get_num_dims() != 1 ||
				third.vec.get_num_dims() != 1 ||
				fourth.vec.get_num_dims() != 1)
				throw std::runtime_error("All parameters have to be scalar");

			result.mat.set_mat4(glm::ortho(
				first.vec.to_scalar(),
				second.vec.to_scalar(),
				third.vec.to_scalar(),
				fourth.vec.to_scalar()));

			return result;
		default:
			throw std::runtime_error("Unknown function: " + func);
		}
	}

}