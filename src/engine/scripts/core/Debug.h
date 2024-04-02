#ifndef FINALYEARPROJECT_DEBUG_H
#define FINALYEARPROJECT_DEBUG_H

#include <exception>
#include <string>

namespace LouiEriksson::Engine {

	enum LogType : char {
		Critical,
		Error,
		Warning,
		Info,
		Debug,
		Trace
	};
	
	struct Debug final {
		
		static void Assert(const bool& _condition, const std::string& _message, const LogType& _type) noexcept;
		
		static void Log(const std::exception& e, const LogType& _type = LogType::Error) noexcept;
		
		static void Log(const std::string& _message, const LogType& _type = LogType::Debug) noexcept;
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_DEBUG_H