#ifndef FINALYEARPROJECT_UTILS_H
#define FINALYEARPROJECT_UTILS_H

namespace LouiEriksson {
	
	class Utils {
	
	public:
		
		static std::vector<std::string> Split(const std::string& _string, const char& _divider);
		
		static std::string Trim(const std::string& _string);
		
		static std::string TrimStart(const std::string& _string);
		
		static std::string TrimEnd(const std::string& _string);
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_UTILS_H