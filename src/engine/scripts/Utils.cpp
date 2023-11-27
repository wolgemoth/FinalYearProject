#include "stdafx.h"

#include "Utils.h"

namespace LouiEriksson {
	
	std::vector<std::string> Utils::Split(const std::string& _string, const char& _divider) {
		
		std::vector<std::string> result;
		
		std::istringstream iss(_string);
		
		std::string substring;
		while (std::getline(iss, substring, _divider)) {
			result.push_back(substring);
		}
		
		return result;
	}
	
	/// <summary>
	/// Trim leading and trailing whitespace characters from a c++ string.
	/// </summary>
	std::string Utils::Trim(const std::string& _string) {
		
		std::smatch sm;
		return std::regex_search(_string, sm, std::regex(R"(^(\S)(.+))")) ?
			sm[0] : _string;
	}
	
} // LouiEriksson