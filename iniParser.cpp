#include "iniParser.h"

const char* ParamTypes[5] = { "Empty", "Bool", "Integer", "Float", "String" };

Parser::Parser(const std::string& fileName) : m_FileName(fileName)
{
	OpenAndRead();
}

void Parser::Refresh()
{
	OpenAndRead();
}

std::string Parser::GetParamType(const Params& param)
{
	return ParamTypes[param.m_ParamType];
}

Params Parser::GetParam(const ParamName& name)
{
	for (const auto& [category, params] : m_Data)
	{
		for (const auto& param : m_Data[category])
			if (param.m_Name == name)
				return param;
	}
	return { EMPTY, "__EMPTY", "__EMPTY" };
}

Params Parser::GetParam(const Category& category, const ParamName& name)
{
	for (const auto& data : m_Data[category])
		if (data.m_Name == name)
			return data;

	return { EMPTY, "EMPTY", "EMPTY" };
}

template<>
int Parser::GetValue<int>(const Params& data) const
{
	if (data.m_ParamType != INT)
		throw std::invalid_argument("Wrong data type! You trying to get Integer value from non-Integer data.");

	return std::stoi(data.m_Value);
}

// stdstof isnt working it cuts of part after dot
template<>
float Parser::GetValue<float>(const Params& data) const
{
	if (data.m_ParamType != FLT)
		throw std::invalid_argument("Wrong data type! You trying to get Float value from non-Float data.");

	size_t dot = data.m_Value.find(".");
	int digitsAfterDot = static_cast<int>(data.m_Value.size() - dot) - 1;

	// Value before dot times quantity of digits after dot
	int tempVal = std::stoi(data.m_Value.substr(0, dot)) * (10 * digitsAfterDot);
	// Plus value after dot
	tempVal += std::stoi(data.m_Value.substr(dot - 1));
	// Divided so dot comes in place
	float retVal = static_cast<float>(tempVal) / (10 * digitsAfterDot);

	return retVal;
}

template<>
std::string Parser::GetValue<std::string>(const Params& data) const
{
	if (data.m_ParamType != STR)
		throw std::invalid_argument("Wrong data type! You trying to get String value from non-String data.");

	return data.m_Value;
}


void Parser::OpenAndRead()
{
	std::ifstream file(m_FileName);
	if (!file.is_open())
	{
		return;
	}
	std::string line;
	Category currentCategory;
	while (std::getline(file, line))
	{
		if (line.size() <= 1)
			continue;
		if (line.find('=') == std::string::npos)
		{
			currentCategory = line;
			m_Data[currentCategory];
			continue;
		}
		ParamName name = line.substr(0, line.find("="));
		ParamValue value = line.substr(name.size() + 1);
		m_Data[currentCategory].push_back({ FindParamType(value), name, value });
	}
}

PARAM_TYPE Parser::FindParamType(const ParamValue& value) const
{
	if (value == "true" || value == "false")
		return BOOL;

	int dotsCount = 0;
	for (size_t i = 0; i < value.size(); i++)
	{
		if (!std::isdigit(value[i]))
		{
			if (value[i] == '.')
			{
				dotsCount++;
				continue;
			}
			else
				break;
		}
		if (i == value.size() - 1)
		{
			if (dotsCount == 0)
				return INT;
			else if (dotsCount == 1)
				return FLT;
		}
	}
	return STR;
}