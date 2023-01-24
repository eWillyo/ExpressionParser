#ifndef TYPES_H
#define TYPES_H

#include <string.h>
#include <stdexcept>

#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <new>

#include "functions.h"


namespace Math_solver {

	typedef struct value_vec {

		value_vec() {
			_value = glm::dvec4(0.0);
			_num_dims = 1;
			_is_vec = true;
		}

		value_vec(glm::dvec4 v, unsigned int d = 1) {
			_value = v;
			_num_dims = d;
			_is_vec = true;
		}

		unsigned int get_num_dims() const {
			return _num_dims;
		}

		bool is_mat() const {
			return !_is_vec;
		}

		double to_scalar() const {
			return _value[0];
		}

		glm::dvec2 to_vec2() const {
			return glm::dvec2(_value[0], _value[1]);
		}

		glm::dvec3 to_vec3() const {
			return glm::dvec3(_value[0], _value[1], _value[2]);
		}

		glm::dvec4 to_vec4() const {
			return glm::dvec4(_value[0], _value[1], _value[2], _value[3]);
		}

		void set_scalar(double v) {
			_num_dims = 1;
			_value[0] = v;
			_is_vec = true;
		}

		void set_vec2(glm::dvec2 v) {
			_num_dims = 2;
			_value[0] = v.x;
			_value[1] = v.y;
			_is_vec = true;
		}

		void set_vec3(glm::dvec3 v) {
			_num_dims = 3;
			_value[0] = v.x;
			_value[1] = v.y;
			_value[2] = v.z;
			_is_vec = true;
		}

		void set_vec4(glm::dvec4 v) {
			_num_dims = 4;
			_value[0] = v.x;
			_value[1] = v.y;
			_value[2] = v.z;
			_value[3] = v.w;
			_is_vec = true;
		}

		std::string to_str(unsigned int max_precision = 3) const;

	private:
		bool _is_vec; // common
		unsigned int _num_dims;
		glm::dvec4 _value;

	} value_vec_t;

	typedef struct value_mat {

		value_mat() {
			_value = glm::dmat4(1.0);
			_num_dims = 2;
			_is_vec = false;
		}

		value_mat(glm::dmat4 m, unsigned int d = 2) {
			_value = m;
			_num_dims = d;
			_is_vec = false;
		}

		unsigned int get_num_dims() const {
			return _num_dims;
		}

		bool is_mat() const {
			return !_is_vec;
		}

		glm::dmat2 to_mat2() const {
			return glm::dmat2(_value[0][0], _value[0][1],
				_value[1][0], _value[1][1]);
		}

		glm::dmat3 to_mat3() const {
			return glm::dmat3(_value[0][0], _value[0][1], _value[0][2],
				_value[1][0], _value[1][1], _value[1][2],
				_value[2][0], _value[2][1], _value[2][2]);
		}

		glm::dmat4 to_mat4() const {
			return glm::dmat4(_value[0][0], _value[0][1], _value[0][2], _value[0][3],
				_value[1][0], _value[1][1], _value[1][2], _value[1][3],
				_value[2][0], _value[2][1], _value[2][2], _value[2][3],
				_value[3][0], _value[3][1], _value[3][2], _value[3][3]);
		}

		void set_mat2(glm::dmat2 m) {
			_num_dims = 2;
			_value[0][0] = m[0][0]; _value[0][1] = m[0][1];
			_value[1][0] = m[1][0]; _value[1][1] = m[0][0];
			_is_vec = false;
		}

		void set_mat3(glm::dmat3 m) {
			_num_dims = 3;
			_value[0][0] = m[0][0]; _value[0][1] = m[0][1]; _value[0][2] = m[0][2];
			_value[1][0] = m[1][0]; _value[1][1] = m[1][1]; _value[1][2] = m[1][2];
			_value[2][0] = m[2][0]; _value[2][1] = m[2][1]; _value[2][2] = m[2][2];
			_is_vec = false;
		}

		void set_mat4(glm::dmat4 m) {
			_num_dims = 4;
			_value[0][0] = m[0][0]; _value[0][1] = m[0][1]; _value[0][2] = m[0][2]; _value[0][3] = m[0][3];
			_value[1][0] = m[1][0]; _value[1][1] = m[1][1]; _value[1][2] = m[1][2]; _value[1][3] = m[1][3];
			_value[2][0] = m[2][0]; _value[2][1] = m[2][1]; _value[2][2] = m[2][2]; _value[2][3] = m[2][3];
			_value[3][0] = m[3][0]; _value[3][1] = m[3][1]; _value[3][2] = m[3][2]; _value[3][3] = m[3][3];
			_is_vec = false;
		}

		std::string to_str(unsigned int max_precision = 3) const;

	private:
		bool _is_vec; // common 
		unsigned int _num_dims;
		glm::dmat4 _value;

	} value_mat_t;

	typedef struct value {

		value() {
			new(&vec) value_vec_t();
		}

		value(glm::dvec4 v, unsigned int d = 1) {
			new(&vec) value_vec_t(v, d);
		}

		value(glm::dmat4 m, unsigned int d = 2) {
			new(&mat) value_mat_t(m, d);
		}

		bool is_mat() const {
			return vec.is_mat();
		}

		union {
			value_vec_t vec;
			value_mat_t mat;
		};
	} value_t;

	value_t operator *(const value_t& left, const value_t& right);

	class BaseNode
	{
	public:
		virtual value_t value() = 0;
	};

	class NumNode : public BaseNode
	{
		value_t _value;
	public:
		NumNode(value_t value)
		{
			_value = value;
		}
		virtual value_t value()
		{
			return _value;
		}
	};

	class OperNode : public BaseNode
	{
		char oper;
		BaseNode* left;
		BaseNode* right;
	public:
		OperNode(char oper, BaseNode* left, BaseNode* right)
		{
			this->oper = oper;
			this->right = right;
			this->left = left;
		}
		virtual value_t value();
	};

	class FuncNode : public BaseNode
	{
		BaseNode* _expression1, * _expression2, * _expression3, * _expression4;
		TokenType _func;

	public:
		FuncNode(TokenType func, BaseNode* expression1, BaseNode* expression2 = nullptr, BaseNode* expression3 = nullptr, BaseNode* expression4 = nullptr)
		{
			_func = func;
			_expression1 = expression1;
			_expression2 = expression2;
			_expression3 = expression3;
			_expression4 = expression4;
		}
		virtual value_t value();
	};

}

#endif // !TYPES_H