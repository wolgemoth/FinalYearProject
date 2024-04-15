#ifndef FINALYEARPROJECT_DEBUG_H
#define FINALYEARPROJECT_DEBUG_H

#include <exception>
#include <string_view>

namespace LouiEriksson::Engine {

	enum LogType : char {
		Critical, /**< @brief Severe problems causing system failure.     */
		   Error, /**< @brief Major issues disrupting normal operations.  */
		 Warning, /**< @brief Potential issues that could cause problems. */
		    Info, /**< @brief General insights about application status.  */
		   Debug, /**< @brief General code debugging.                     */
		   Trace  /**< @brief In-depth tracking of system operations.     */
	};
	
	struct Debug final {
		
		static void Assert(const bool& _condition, const std::string_view& _message, const LogType& _type, const bool& _inline = false) noexcept;
		
		static void Log(const std::exception& e, const LogType& _type = LogType::Error, const bool& _inline = false) noexcept;
		
		static void Log(const std::string_view& _message, const LogType& _type = LogType::Debug, const bool& _inline = false) noexcept;
		
		static void Flush() noexcept;
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_DEBUG_H