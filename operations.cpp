#include "constants.h"
#include "types.h"
#include "operations.h"
#include "error.h"

#include <random>

#include <vector>
#include <iostream>
#include <algorithm>


namespace Math_solver {

	// RAND() 
	std::random_device dev;
	std::mt19937 rng(dev());

	// create distributions
	std::uniform_int_distribution<std::mt19937::result_type> dist_in_range(0, RANDOM_MAX); // distribution in range [0, RANDOM_MAX]
	//std::uniform_int_distribution<std::mt19937::result_type> dist_in_rand_max;
	//std::normal_distribution<double> normal_dist(mean, stddeviation);  // N(mean, stddeviation)
	// RAND() 

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

	double Factorial(double value)
	{
		if (ceil(value) == value && value > 0) // integer
		{
			double result = value;

			for (unsigned long i = (unsigned long)(result - 1); i > 0; i--)
				result *= i;

			return result;
		}
		else
			Throw_error(__FILE__, __LINE__, __func__, "Parameter must be integer number greater than zero");

		return 0;
	}

	double Random(double value)
	{
		if (ceil(value) == value && value >= 0 && value < RANDOM_MAX) // integer
		{
			return (double)(dist_in_range(rng) % (unsigned int)value);
		}
		else
			Throw_error(__FILE__, __LINE__, __func__, "Parameter must be integer number in range 0..999");

		return 0;
	}

	std::string Format_number(double value, unsigned int precision)
	{
		value = RoundOff(value, precision);

		std::string result = std::to_string(fabs(value));
		bool is_decimal = (ceil(value) != value);
		bool is_negative = (value < 0);

		size_t decimal_pos = result.find('.');
		std::string integer_part = result.substr(0, decimal_pos);
		std::string decimal_part = result.substr(decimal_pos + 1);

		// integer part
		std::reverse(integer_part.begin(), integer_part.end());

		for (std::string::size_type i = 0; i < integer_part.size(); i++) { // add thousands separators
			if ((i != 0) && ((i + 1) % 4 == 0))
				integer_part.insert(i, 1, ',');
		}

		std::reverse(integer_part.begin(), integer_part.end());

		if (is_negative)
			integer_part.insert(0, 1, '-');

		result = integer_part;

		// decimal part
		if (is_decimal) {
			std::reverse(decimal_part.begin(), decimal_part.end());

			while (decimal_part[0] == '0') { // erase end zeroes
				decimal_part.erase(0, 1);
			}

			std::reverse(decimal_part.begin(), decimal_part.end());

			if (decimal_part.length() != 0) {
				result += '.';
				result += decimal_part;
			}
		}

		return result;
	}

	value_t Do_vector(value_t value, double (*fnc)(double))
	{
		glm::dvec4 result_temp(0);

		for (unsigned int i = 0; i < value.vec.get_num_dims(); i++)
			result_temp[i] = fnc((double)value.vec.to_vec4()[i]);

		return value_t(result_temp, value.vec.get_num_dims());
	}

	value_t Do_vector_scalar(value_t lvalue, value_t rvalue, double (*fnc)(double, double))
	{
		glm::dvec4 result_temp(0);

		for (unsigned int i = 0; i < lvalue.vec.get_num_dims(); i++)
			result_temp[i] = fnc((double)lvalue.vec.to_vec4()[i], (double)rvalue.vec.to_vec4()[0]);

		return value_t(result_temp, lvalue.vec.get_num_dims());
	}

	value_t Do_vector_vector(value_t lvalue, value_t rvalue, double (*fnc)(double, double))
	{
		glm::dvec4 result_temp(0);

		for (unsigned int i = 0; i < lvalue.vec.get_num_dims(); i++)
			result_temp[i] = fnc((double)lvalue.vec.to_vec4()[i], (double)rvalue.vec.to_vec4()[i]);

		return value_t(result_temp, lvalue.vec.get_num_dims());
	}

	value_t Do_matrix_scalar(value_t lvalue, value_t rvalue, double (*fnc)(double, double))
	{
		glm::dmat4 result_temp = glm::dmat4(0);

		for (unsigned int i = 0; i < lvalue.mat.get_num_dims(); i++)
			for (unsigned int j = 0; j < lvalue.mat.get_num_dims(); j++)
				result_temp[i][j] = fnc((double)lvalue.mat.to_mat4()[i][j], (double)rvalue.vec.to_vec4()[0]);

		return value_t(result_temp, lvalue.vec.get_num_dims());
	}

	value_t Do_func(TokenType func, BaseNode* expression1, BaseNode* expression2, BaseNode* expression3, BaseNode* expression4)
	{
		value_t result, first, second, third, fourth;
		unsigned int num_dims;

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
		case SINH_FN:
			return Do_vector(expression1->value(), sinh);
		case COSH_FN:
			return Do_vector(expression1->value(), cosh);
		case TANH_FN:
			return Do_vector(expression1->value(), tanh);
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
			default: Throw_error(__FILE__, __LINE__, __func__, "Wrong n-dimensional vector");
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
			default: Throw_error(__FILE__, __LINE__, __func__, "Wrong n-dimensional vector");
			}

			return result;
		case DOT_PRODUCT_FN:
			first = expression1->value();
			second = expression2->value();

			if (first.vec.get_num_dims() != second.vec.get_num_dims())
				Throw_error(__FILE__, __LINE__, __func__, "Different dimension count at n-dimensional function");

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
			default: Throw_error(__FILE__, __LINE__, __func__, "Wrong n-dimensional vector");
			}

			return result;
		case CROSS_PRODUCT_FN:
			first = expression1->value();
			second = expression2->value();

			if ((first.vec.get_num_dims() != 3) && (second.vec.get_num_dims() != 3))
				Throw_error(__FILE__, __LINE__, __func__, "Different dimension count at n-dimensional function");

			result.vec.set_vec3(glm::cross(first.vec.to_vec3(), second.vec.to_vec3()));

			return result;
		case MIX_FN:
			first = expression1->value();
			second = expression2->value();
			third = expression3->value();

			if (first.vec.get_num_dims() != second.vec.get_num_dims())
				Throw_error(__FILE__, __LINE__, __func__, "Different dimension count at n-dimensional function");

			if (third.vec.get_num_dims() != 1)
				Throw_error(__FILE__, __LINE__, __func__, "Third parameter of function must be scalar value");

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
			default: Throw_error(__FILE__, __LINE__, __func__, "Wrong n-dimensional vector");
			}

			return result;
		case MAT2_FN:
			first = expression1->value();
			num_dims = expression1->value().mat.get_num_dims();
			
			if (!first.is_mat())
				Throw_error(__FILE__, __LINE__, __func__, "Parameter must be matrix");

			switch (num_dims)
			{
			case 2:
				result.mat.set_mat2(first.mat.to_mat2());
				break;
			case 3:
				result.mat.set_mat2(first.mat.to_mat3());
				break;
			case 4:
				result.mat.set_mat2(first.mat.to_mat4());
				break;
			default: Throw_error(__FILE__, __LINE__, __func__, "Wrong base of matrix");
			}

			return result;
		case MAT3_FN:
			first = expression1->value();
			num_dims = expression1->value().mat.get_num_dims();

			if (!first.is_mat())
				Throw_error(__FILE__, __LINE__, __func__, "Parameter must be matrix");

			switch (num_dims)
			{
			case 2:
				result.mat.set_mat3(first.mat.to_mat2());
				break;
			case 3:
				result.mat.set_mat3(first.mat.to_mat3());
				break;
			case 4:
				result.mat.set_mat3(first.mat.to_mat4());
				break;
			default: Throw_error(__FILE__, __LINE__, __func__, "Wrong base of matrix");
			}

			return result;
		case MAT4_FN:
			first = expression1->value();
			num_dims = expression1->value().mat.get_num_dims();

			if (!first.is_mat())
				Throw_error(__FILE__, __LINE__, __func__, "Parameter must be matrix");

			switch (num_dims)
			{
			case 2:
				result.mat.set_mat4(first.mat.to_mat2());
				break;
			case 3:
				result.mat.set_mat4(first.mat.to_mat3());
				break;
			case 4:
				result.mat.set_mat4(first.mat.to_mat4());
				break;
			default: Throw_error(__FILE__, __LINE__, __func__, "Wrong base of matrix");
			}

			return result;
		case SCALE_FN:
			first = expression1->value();
			second = expression2->value();

			if (!first.is_mat())
				Throw_error(__FILE__, __LINE__, __func__, "First parameter must be 4x4 matrix");
			if (second.is_mat() || second.vec.get_num_dims() != 3)
				Throw_error(__FILE__, __LINE__, __func__, "Second parameter must be 3D vector");

			result.mat.set_mat4(glm::scale((glm::mat4)first.mat.to_mat4(), (glm::vec3)second.vec.to_vec3()));

			return result;
		case ROTATE_FN:
			first = expression1->value();
			second = expression2->value();
			third = expression3->value();

			if (!first.is_mat() || first.mat.get_num_dims() != 4)
				Throw_error(__FILE__, __LINE__, __func__, "First parameter must be 4x4 matrix");
			if (second.vec.get_num_dims() != 1)
				Throw_error(__FILE__, __LINE__, __func__, "Second parameter must be scalar");
			if (third.is_mat() || third.vec.get_num_dims() != 3)
				Throw_error(__FILE__, __LINE__, __func__, "Third parameter must be 3D vector");

			result.mat.set_mat4(glm::rotate(first.mat.to_mat4(), second.vec.to_scalar(), third.vec.to_vec3()));

			return result;
		case TRANSLATE_FN:
			first = expression1->value();
			second = expression2->value();

			if (!first.is_mat() || first.mat.get_num_dims() != 4)
				Throw_error(__FILE__, __LINE__, __func__, "First parameter must be 4x4 matrix");
			if (second.is_mat() || second.vec.get_num_dims() != 3)
				Throw_error(__FILE__, __LINE__, __func__, "Second parameter must be 3D vector");

			result.mat.set_mat4(glm::translate(first.mat.to_mat4(), second.vec.to_vec3()));

			return result;
		case INVERSE_TRANSPOSE_FN:
			first = expression1->value();

			if (!first.is_mat())
				Throw_error(__FILE__, __LINE__, __func__, "Parameter must be matrix");

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
			default: Throw_error(__FILE__, __LINE__, __func__, "Wrong n-dimensional vectors");
			}

			return result;
		case PERSPECTIVE_PROJ_FN:
			first = expression1->value();
			second = expression2->value();
			third = expression3->value();
			fourth = expression4->value();

			if (first.is_mat() || second.is_mat() || third.is_mat() || fourth.is_mat())
				Throw_error(__FILE__, __LINE__, __func__, "None of the parameters can't be matrix");
			if (first.vec.get_num_dims() != 1 ||
				second.vec.get_num_dims() != 1 ||
				third.vec.get_num_dims() != 1 ||
				fourth.vec.get_num_dims() != 1)
				Throw_error(__FILE__, __LINE__, __func__, "All parameters must be scalar");

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
				Throw_error(__FILE__, __LINE__, __func__, "None of the parameters can't be matrix");
			if (first.vec.get_num_dims() != 1 ||
				second.vec.get_num_dims() != 1 ||
				third.vec.get_num_dims() != 1 ||
				fourth.vec.get_num_dims() != 1)
				Throw_error(__FILE__, __LINE__, __func__, "All parameters must be scalar");

			result.mat.set_mat4(glm::ortho(
				first.vec.to_scalar(),
				second.vec.to_scalar(),
				third.vec.to_scalar(),
				fourth.vec.to_scalar()));

			return result;
		case RAND_FN:
			first = expression1->value();

			if (first.is_mat())
				Throw_error(__FILE__, __LINE__, __func__, "Parameter can't be matrix");
			if (first.vec.get_num_dims() != 1)
				Throw_error(__FILE__, __LINE__, __func__, "Parameter must be scalar");

			result.vec.set_scalar(Random(first.vec.to_scalar()));

			return result;
		default:
			Throw_error(__FILE__, __LINE__, __func__, "Unknown function: %d", func);
		}

		return result;
	}

}