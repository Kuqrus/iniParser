#include <iostream>

//#include "parser.h"

#include "iniParser.h"

typedef     bool            ValBool;
typedef     int             ValInt;
typedef     float           ValFloat;
typedef     std::string     ValStr;

int main()
{
	/*
	try
	{
		startParser();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	*/

	Parser cfg("cfg.ini");

	std::cout << cfg.GetValue<ValStr>(cfg.GetParam("version"))			<< std::endl;
	std::cout << cfg.GetValue<ValStr>(cfg.GetParam("password"))			<< std::endl;
	std::cout << cfg.GetValue<ValBool>(cfg.GetParam("enable_logging"))	<< std::endl;
	std::cout << cfg.GetValue<ValInt>(cfg.GetParam("font_size"))		<< std::endl;
	std::cout << cfg.GetValue<ValFloat>(cfg.GetParam("cache_size"))		<< std::endl;

	std::cout << std::endl;

	std::cout << cfg.GetParam("enable_logging").m_ParamType				<< std::endl;
	std::cout << cfg.GetParamType(cfg.GetParam("enable_logging"))		<< std::endl;
}
