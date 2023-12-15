#include "stdafx.h"

#include "Utils.h"

namespace LouiEriksson {
	
	/// <summary>
	/// Split a string in a collection of substrings using a divider.
	/// </summary>
	std::vector<std::string> Utils::Split(const std::string& _string, const char& _divider) {
		
		std::vector<std::string> result;
		
		// Init string stream.
		std::istringstream iss(_string);
		
		std::string substring;
		
		// Split string stream using divider and push into result.
		while (std::getline(iss, substring, _divider)) {
			result.push_back(substring);
		}
		
		return result;
	}
	
	/// <summary>
	/// Trim leading and trailing whitespace characters from a string.
	/// </summary>
	std::string Utils::Trim(const std::string& _string) {
		
		// Use a regex pattern to trim the leading and trailing whitespace characters from the string.
		std::smatch sm;
		return std::regex_search(_string, sm, std::regex(R"(^(\S)(.+))")) ?
			sm[0] : _string;
	}
	
	/// <summary>
	/// Remap a float from one range to another.
	/// </summary>
	float Utils::Remap(const float& _val, const float& _aMin, const float& _aMax, const float& _bMin, const float& _bMax) {
		
		// Remap function I made for a homework task in "Mathematics for Computer Graphics".
		return _bMin + (_val - _aMin) * (_bMax - _bMin) / (_aMax - _aMin);
	}
	
} // LouiEriksson