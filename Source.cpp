#include <iostream>
#include "parser.h"

int main()
{
	try
	{
		startParser();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
    
}