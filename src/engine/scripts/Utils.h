#pragma once

#ifndef FINALYEARPROJECT_UTILS_H
#define FINALYEARPROJECT_UTILS_H

namespace LouiEriksson {
	
	class Utils {
	
	public:
		
		static std::vector<std::string> Split(const std::string& _string, const char& _divider);
		
		static std::string Trim(const std::string& _string);
		
		static float Remap(const float& _val, const float& _aMin, const float& _aMax, const float& _bMin, const float& _bMax);
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_UTILS_H