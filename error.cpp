#include "error.h"
#include "config.h"

#include <stdexcept>
#include <stdio.h>


namespace Math_solver {

	void Print_info(const char* fmt, ...)
	{
		va_list va;
		char text[255];

		if (VERBOSE && fmt != nullptr)
		{
			va_start(va, fmt);
			vsprintf_s(text, 255, fmt, va);
			va_end(va);

			// print info
			printf("%s\n", text);
		}

	}

	void Print_warning(const char* file, int line, const char* func, const char* fmt, ...)
	{
		va_list va;
		char formated_text[255];

		if (fmt == nullptr)
			return;

		va_start(va, fmt);
		vsprintf_s(formated_text, 255, fmt, va);
		va_end(va);

		// add debug info
		char output_text[300];
		sprintf_s(output_text, "Warning: \"%s\" (line: %d) function: \'%s\' -> %s", file, line, func, formated_text);

		// print warning
		printf("%s\n", output_text);
	}

	void Throw_error(const char* file, int line, const char* func, const char* fmt, ...)
	{
		va_list va;
		char formated_text[255];

		if (fmt == nullptr)
			return;

		va_start(va, fmt);
		vsprintf_s(formated_text, 255, fmt, va);
		va_end(va);

		// add debug info
		char output_text[300];
		sprintf_s(output_text, "Error: \"%s\" (line: %d) function: \'%s\' -> %s", file, line, func, formated_text);

		// throw exception
		throw std::runtime_error(output_text);
	}

}