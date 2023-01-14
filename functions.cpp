#include <map>
#include <iostream>
#include "functions.h"


namespace Math_solver {

	// for internal purposes
	std::map<std::string, TokenType> mapStringToTokenType =
	{
		{ "RAD", TokenType::RAD_FN },
		{ "DEG", TokenType::DEG_FN },
		{ "sin", TokenType::SIN_FN },
		{ "cos", TokenType::COS_FN },
		{ "tan", TokenType::TAN_FN },
		{ "asin", TokenType::ASIN_FN },
		{ "acos", TokenType::ACOS_FN },
		{ "atan", TokenType::ATAN_FN },
		{ "abs", TokenType::ABS_FN },
		{ "ln", TokenType::LN_FN },
		{ "log", TokenType::LOG_FN },
		{ "exp", TokenType::EXP_FN },
		{ "sqrt", TokenType::SQRT_FN },
		{ "vec2", TokenType::VEC2_FN },
		{ "vec3", TokenType::VEC3_FN },
		{ "vec4", TokenType::VEC4_FN },
		{ "length", TokenType::LENGTH_FN },
		{ "normalize", TokenType::NORMALIZE_FN },
		{ "dot", TokenType::DOT_PRODUCT_FN },
		{ "cross", TokenType::CROSS_PRODUCT_FN },
		{ "mix", TokenType::MIX_FN },
		{ "mat2", TokenType::MAT2_FN },
		{ "mat3", TokenType::MAT3_FN },
		{ "mat4", TokenType::MAT4_FN },
		{ "scale", TokenType::SCALE_FN },
		{ "rotate", TokenType::ROTATE_FN },
		{ "translate", TokenType::TRANSLATE_FN },
		{ "invtranspose", TokenType::INVERSE_TRANSPOSE_FN },
		{ "perspective", TokenType::PERSPECTIVE_PROJ_FN },
		{ "ortho", TokenType::ORTHO_PROJ_FN }
	};

	TokenType CheckFunction(std::string name, bool verbose)
	{
		TokenType token = mapStringToTokenType[name];

		switch (token)
		{
		case TokenType::RAD_FN:
			if (verbose)
				std::cout << "RAD function" << std::endl;
			return TokenType(RAD_FN);

		case TokenType::DEG_FN:
			if (verbose)
				std::cout << "DEG function" << std::endl;
			return TokenType(DEG_FN);

		case TokenType::SIN_FN:
			if (verbose)
				std::cout << "SIN function" << std::endl;
			return TokenType(SIN_FN);

		case TokenType::COS_FN:
			if (verbose)
				std::cout << "COS function" << std::endl;
			return TokenType(COS_FN);

		case TokenType::TAN_FN:
			if (verbose)
				std::cout << "TAN function" << std::endl;
			return TokenType(TAN_FN);

		case TokenType::ASIN_FN:
			if (verbose)
				std::cout << "ASIN function" << std::endl;
			return TokenType(ASIN_FN);

		case TokenType::ACOS_FN:
			if (verbose)
				std::cout << "ACOS function" << std::endl;
			return TokenType(ACOS_FN);

		case TokenType::ATAN_FN:
			if (verbose)
				std::cout << "ATAN function" << std::endl;
			return TokenType(ATAN_FN);

		case TokenType::ABS_FN:
			if (verbose)
				std::cout << "ABS function" << std::endl;
			return TokenType(ABS_FN);

		case TokenType::LN_FN:
			if (verbose)
				std::cout << "LN function" << std::endl;
			return TokenType(LN_FN);

		case TokenType::LOG_FN:
			if (verbose)
				std::cout << "LOG function" << std::endl;
			return TokenType(LOG_FN);

		case TokenType::EXP_FN:
			if (verbose)
				std::cout << "EXP function" << std::endl;
			return TokenType(EXP_FN);

		case TokenType::SQRT_FN:
			if (verbose)
				std::cout << "SQRT function" << std::endl;
			return TokenType(SQRT_FN);

		case TokenType::VEC2_FN:
			if (verbose)
				std::cout << "VEC2 function" << std::endl;
			return TokenType(VEC2_FN);

		case TokenType::VEC3_FN:
			if (verbose)
				std::cout << "VEC3 function" << std::endl;
			return TokenType(VEC3_FN);

		case TokenType::VEC4_FN:
			if (verbose)
				std::cout << "VEC4 function" << std::endl;
			return TokenType(VEC4_FN);

		case TokenType::LENGTH_FN:
			if (verbose)
				std::cout << "LENGTH function" << std::endl;
			return TokenType(LENGTH_FN);

		case TokenType::NORMALIZE_FN:
			if (verbose)
				std::cout << "NORMALIZE function" << std::endl;
			return TokenType(NORMALIZE_FN);

		case TokenType::DOT_PRODUCT_FN:
			if (verbose)
				std::cout << "DOT PRODUCT function" << std::endl;
			return TokenType(DOT_PRODUCT_FN);

		case TokenType::CROSS_PRODUCT_FN:
			if (verbose)
				std::cout << "CROSS PRODUCT function" << std::endl;
			return TokenType(CROSS_PRODUCT_FN);

		case TokenType::MIX_FN:
			if (verbose)
				std::cout << "MIX function" << std::endl;
			return TokenType(MIX_FN);

		case TokenType::MAT2_FN:
			if (verbose)
				std::cout << "MAT2 function" << std::endl;
			return TokenType(MAT2_FN);

		case TokenType::MAT3_FN:
			if (verbose)
				std::cout << "MAT3 function" << std::endl;
			return TokenType(MAT3_FN);

		case TokenType::MAT4_FN:
			if (verbose)
				std::cout << "MAT4 function" << std::endl;
			return TokenType(MAT4_FN);

		case TokenType::SCALE_FN:
			if (verbose)
				std::cout << "SCALE function" << std::endl;
			return TokenType(SCALE_FN);

		case TokenType::ROTATE_FN:
			if (verbose)
				std::cout << "ROTATE function" << std::endl;
			return TokenType(ROTATE_FN);

		case TokenType::TRANSLATE_FN:
			if (verbose)
				std::cout << "TRANSLATE function" << std::endl;
			return TokenType(TRANSLATE_FN);

		case TokenType::INVERSE_TRANSPOSE_FN:
			if (verbose)
				std::cout << "INVERSE TRANSPOSE function" << std::endl;
			return TokenType(INVERSE_TRANSPOSE_FN);

		case TokenType::PERSPECTIVE_PROJ_FN:
			if (verbose)
				std::cout << "PERSPECTIVE PROJECTION function" << std::endl;
			return TokenType(PERSPECTIVE_PROJ_FN);

		case TokenType::ORTHO_PROJ_FN:
			if (verbose)
				std::cout << "ORTHO PROJECTION function" << std::endl;
			return TokenType(ORTHO_PROJ_FN);

		default:
			if (verbose)
				std::cout << "<unknown function>" << std::endl;
			return TokenType(NONE);
		}
	}

}