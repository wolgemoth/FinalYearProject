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
	
} // LouiEriksson