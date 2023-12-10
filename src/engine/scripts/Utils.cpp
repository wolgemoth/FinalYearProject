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
	
	float Utils::Remap(const float& _val, const float& _aMin, const float& _aMax, const float& _bMin, const float& _bMax) {
		return _bMin + (_val - _aMin) * (_bMax - _bMin) / (_aMax - _aMin);
	}
	
} // LouiEriksson