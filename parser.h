#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

class ini_parser
{
public:
	ini_parser(std::string fileName) {
		std::cout << "Created...\n";
		file.open(fileName);
		if((!file.is_open())) {
			throw std::exception{ "Filed to open file!" };
		}
		FillData();
	}
	~ini_parser() {
		std::cout << "Destructed...\n";
		file.close();
	}

	
	template<class T>
	T Get(std::string sect, std::string name) {	
		std::string key = MakeKey(sect, name);
		return data.count(key) ? data.find(key)->second : throw std::exception{ "No value found" };
	}

	template<>
	int Get(std::string sect, std::string name) {
		std::string key = MakeKey(sect, name);
		std::string value = data.count(key) ? data.find(key)->second : throw std::exception{ "No value found" };

		int valueInt = std::stoi(value);
		return valueInt;
	}

	template<>
	double Get(std::string sect, std::string name) {
		std::string key = MakeKey(sect, name);
		std::string value = data.count(key) ? data.find(key)->second : throw std::exception{ "No value found" };

		double valueInt = std::stod(value);
		return valueInt;
	}

private:
	std::map<std::string, std::string> data;
	std::ifstream file;

	// If line has comment function return iterator of ";", else return -1
	int FindComment(std::string& line) {
		if (line.find(";") == 0) {
			return 0;
		}
		else if (line.find(";") != std::string::npos) {
			return line.find(";");
		}
		else return -1;
	}

	// If line starts with "[" and has "]" return 1 (true), else return 0 (false)
	int FindSection(std::string& line) {
		if (line[0] == '[') {
			if (line.find("]") != std::string::npos) {
				return 1;
			}
			else return 0;
		}
		else return 0;
	}

	// Delete extra gaps between symbols
	void DeleteGap(std::string& str) {
		size_t start{ str.find(' ') };
		while (start != std::string::npos)
		{
			str.erase(start, 1);
			start = str.find(' ', start + 1);
		}
	}

	// Make key for map
	std::string MakeKey(std::string& sect, std::string& name) {
		if (sect.find("[") == std::string::npos) {
			sect.insert(sect.length(), "]");
			sect.insert(0, "[");
		}
		std::string key = sect + "=" + name;
		std::transform(key.begin(), key.end(), key.begin(),
			[](unsigned char ch) {return std::tolower(ch); });
		return key;
	}
		
	void FillData() {
		std::string sect = "INIT", name = "INIT", value = "INIT";
		std::string line = "INIT";
		std::string key = "INIT";

		while (!file.eof()) {
			std::getline(file, line);

			// If line empty, skip this iteration
			if (line.empty()) {
				continue;
			}

			// If line contains only comment, skip this iteration
			// Else erase comment 
			if (FindComment(line) == 0) {
				continue;
			}
			else if (FindComment(line) > 0) {
				line.erase(FindComment(line), line.length());
			}

			if (FindSection(line)) {
				sect = line;
				sect.erase(sect.find("]") + 1, sect.length());
				continue;
			}

			name = line;
			value = name.substr(name.find_last_of('=') + 1);
			name.erase(name.find("="));

			if (name.find(" ") != std::string::npos) { DeleteGap(name); }
			if (value.find(" ") != std::string::npos) { DeleteGap(value); }

			key = MakeKey(sect, name);
			data[key] = value;
		}
	}

};




void startParser() {	

	ini_parser test("testfile.ini");
	auto value = test.Get<double>("two", "x");
	std::cout << "Value: " << value << ", Type: " << typeid(value).name() << std::endl;
}