#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>


namespace Math_solver {

	enum TokenType
	{
		NONE,
		SCALAR,
		VARIABLE_ASSIGN,
		VARIABLE_SUBSTITUTION,
		END,
		RAD_FN,
		DEG_FN,
		SIN_FN,
		COS_FN,
		TAN_FN,
		ASIN_FN,
		ACOS_FN,
		ATAN_FN,
		ABS_FN,
		LN_FN,
		LOG_FN,
		EXP_FN,
		SQRT_FN,
		VEC2_FN,
		VEC3_FN,
		VEC4_FN,
		LENGTH_FN,
		NORMALIZE_FN,
		DOT_PRODUCT_FN,
		CROSS_PRODUCT_FN,
		MIX_FN,
		MAT2_FN,
		MAT3_FN,
		MAT4_FN,
		SCALE_FN,
		ROTATE_FN,
		TRANSLATE_FN,
		INVERSE_TRANSPOSE_FN,
		PERSPECTIVE_PROJ_FN = 34,
		ORTHO_PROJ_FN,
		RAND_FN,
		PLUS = '+',
		MINUS = '-',
		MULTIPLY = '*',
		DIVIDE = '/',
		MODULO = '%',
		FACTORIAL = '!',
		POW = '^',
		LHPAREN = '(',
		RHPAREN = ')',
		COMMA = ','
	};


	TokenType CheckFunction(std::string name, bool verbose);

}

#endif // !FUNCTIONS_H
