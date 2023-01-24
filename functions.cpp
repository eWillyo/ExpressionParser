#include <map>
#include <iostream>

#include "functions.h"
#include "error.h"


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
		{ "ortho", TokenType::ORTHO_PROJ_FN },
		{ "rand", TokenType::RAND_FN}
	};

	TokenType CheckFunction(std::string name)
	{
		TokenType token = mapStringToTokenType[name];

		switch (token)
		{
		case TokenType::RAD_FN:
			Print_info("RAD function");
			return TokenType(RAD_FN);

		case TokenType::DEG_FN:
			Print_info("DEG function");
			return TokenType(DEG_FN);

		case TokenType::SIN_FN:
			Print_info("SIN function");
			return TokenType(SIN_FN);

		case TokenType::COS_FN:
			Print_info("COS function");
			return TokenType(COS_FN);

		case TokenType::TAN_FN:
			Print_info("TAN function");
			return TokenType(TAN_FN);

		case TokenType::ASIN_FN:
			Print_info("ASIN function");
			return TokenType(ASIN_FN);

		case TokenType::ACOS_FN:
			Print_info("ACOS function");
			return TokenType(ACOS_FN);

		case TokenType::ATAN_FN:
			Print_info("ATAN function");
			return TokenType(ATAN_FN);

		case TokenType::ABS_FN:
			Print_info("ABS function");
			return TokenType(ABS_FN);

		case TokenType::LN_FN:
			Print_info("LN function");
			return TokenType(LN_FN);

		case TokenType::LOG_FN:
			Print_info("LOG function");
			return TokenType(LOG_FN);

		case TokenType::EXP_FN:
			Print_info("EXP function");
			return TokenType(EXP_FN);

		case TokenType::SQRT_FN:
			Print_info("SQRT function");
			return TokenType(SQRT_FN);

		case TokenType::VEC2_FN:
			Print_info("VEC2 function");
			return TokenType(VEC2_FN);

		case TokenType::VEC3_FN:
			Print_info("VEC3 function");
			return TokenType(VEC3_FN);

		case TokenType::VEC4_FN:
			Print_info("VEC4 function");
			return TokenType(VEC4_FN);

		case TokenType::LENGTH_FN:
			Print_info("LENGTH function");
			return TokenType(LENGTH_FN);

		case TokenType::NORMALIZE_FN:
			Print_info("NORMALIZE function");
			return TokenType(NORMALIZE_FN);

		case TokenType::DOT_PRODUCT_FN:
			Print_info("DOT PRODUCT function");
			return TokenType(DOT_PRODUCT_FN);

		case TokenType::CROSS_PRODUCT_FN:
			Print_info("CROSS PRODUCT function");
			return TokenType(CROSS_PRODUCT_FN);

		case TokenType::MIX_FN:
			Print_info("MIX function");
			return TokenType(MIX_FN);

		case TokenType::MAT2_FN:
			Print_info("MAT2 function");
			return TokenType(MAT2_FN);

		case TokenType::MAT3_FN:
			Print_info("MAT3 function");
			return TokenType(MAT3_FN);

		case TokenType::MAT4_FN:
			Print_info("MAT4 function");
			return TokenType(MAT4_FN);

		case TokenType::SCALE_FN:
			Print_info("SCALE function");
			return TokenType(SCALE_FN);

		case TokenType::ROTATE_FN:
			Print_info("ROTATE function");
			return TokenType(ROTATE_FN);

		case TokenType::TRANSLATE_FN:
			Print_info("TRANSLATE function");
			return TokenType(TRANSLATE_FN);

		case TokenType::INVERSE_TRANSPOSE_FN:
			Print_info("INVERSE TRANSPOSE function");
			return TokenType(INVERSE_TRANSPOSE_FN);

		case TokenType::PERSPECTIVE_PROJ_FN:
			Print_info("PERSPECTIVE PROJECTION function");
			return TokenType(PERSPECTIVE_PROJ_FN);

		case TokenType::ORTHO_PROJ_FN:
			Print_info("ORTHO PROJECTION function");
			return TokenType(ORTHO_PROJ_FN);

		case TokenType::RAND_FN:
			Print_info("RAND function");
			return TokenType(RAND_FN);

		default:
			Print_info("<unknown function>");
			return TokenType(NONE);
		}
	}

}