#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>


namespace Math_solver {

	void Print_info(const char* fmt, ...);
	void Print_warning(const char* file, int line, const char* func, const char* fmt, ...);
	void Throw_error(const char* file, int line, const char* func, const char* fmt, ...);

}

//#define INFO(par, ...) Print_info(par, ##__VA_ARGS__)
//#define WARNING(par, ...) Print_warning(par, ##__VA_ARGS__)
//#define ERROR(par, ...) Throw_error(par, ##__VA_ARGS__)

#endif // !ERROR_H
