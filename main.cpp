
// Expression parser to solve simple math problems..
// Original thought: 31.5.2010 (significantly inspired by one old project on "codeproject.com")
// Author: Vilém Pantlík

#include "parser.h"
#include "config.h"


int main()
{
	std::string inputline;

	try
	{
		while(true)
		{
			std::cout << "Enter expression: ";
			std::getline(std::cin, inputline);

			if(inputline.empty())
				return 0;

			Math_solver::Parser p(inputline);
			Math_solver::value_t value = p.Evaluate();

			if (value.is_mat())
				std::cout << "Result: " << value.mat.to_str(PRECISION) << std::endl;
			else
				std::cout << "Result: " << value.vec.to_str(PRECISION) << std::endl;
		}
	}
	catch(std::exception & e)
	{
		std::cout << /*"Exception: " <<*/ e.what() << std::endl;
		return 1;
	}
	return 0;
}
