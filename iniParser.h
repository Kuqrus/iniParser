#pragma once
#include <fstream>
#include <list>
#include <map>
#include <string>


typedef std::string Category;
typedef std::string ParamName;
typedef std::string ParamValue;


enum PARAM_TYPE
{
	EMPTY,
	BOOL,
	INT,
	FLT,
	STR,
};

struct Params
{
	PARAM_TYPE		m_ParamType;
	ParamName		m_Name;
	ParamValue		m_Value;
};

class Parser
{
public:
	Parser(const std::string& fileName);

	void Refresh();

	std::string GetParamType(const Params& param);

	Params GetParam(const ParamName& name);

	Params GetParam(const Category& category, const ParamName& name);

	template<typename T = bool>
	T GetValue(const Params& data) const
	{
		if (data.m_ParamType != BOOL)
			throw std::invalid_argument("Wrong data type! You trying to get Bool value from non-Bool data.");

		return data.m_Value == "true" ? true : false;
	}

	template<>
	int GetValue<int>(const Params& data) const;

	// stdstof isnt working it cuts of part after dot
	template<>
	float GetValue<float>(const Params& data) const;

	template<>
	std::string GetValue<std::string>(const Params& data) const;


private:
	std::string m_FileName;
	std::map<Category, std::list<Params>> m_Data;


private:
	void OpenAndRead();

	PARAM_TYPE FindParamType(const ParamValue& value) const;
};