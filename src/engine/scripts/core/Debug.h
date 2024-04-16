#ifndef FINALYEARPROJECT_DEBUG_H
#define FINALYEARPROJECT_DEBUG_H

#include <exception>
#include <string_view>

namespace LouiEriksson::Engine {
	
	/**
	 * @enum LogType
	 * @brief Enumeration representing different types of log messages.
	 *
	 * The LogType enum represents different levels of log messages that can be used to categorize log entries.
	 * Each log type is assigned a character value which is used to differentiate between log messages.
	 */
	enum LogType : char {
		Critical, /**< @brief Severe problems causing system failure.     */
		   Error, /**< @brief Major issues disrupting normal operations.  */
		 Warning, /**< @brief Potential issues that could cause problems. */
		    Info, /**< @brief General insights about application status.  */
		   Debug, /**< @brief General code debugging.                     */
		   Trace  /**< @brief In-depth tracking of system operations.     */
	};
	
	/**
	 * @brief The Debug class provides a set of static methods for debugging and logging.
	 *
	 * The Debug class is a utility class that provides various methods for debugging and logging purposes.
	 * It includes methods for performing assertions, triggering breakpoints, flushing the log output,
	 * and logging messages with different log types.
	 */
	struct Debug final {
		
		/**
		 * @brief Asserts a condition and logs a message if the condition is false.
		 * By default, the log type is set to `Debug`.
		 *
		 * @param[in] _condition The condition to assert.
		 * @param[in] _message The message to log if the condition is false.
		 * @param[in] _type (optional) The type of log message to log.
		 * @param[in] _inline (optional) A flag indicating if the log message should be displayed inline.
		 */
		static void Assert(const bool& _condition, const std::string_view& _message, const LogType& _type = LogType::Debug, const bool& _inline = false) noexcept;
		
		/**
		 * @brief Triggers a breakpoint and logs a message.
		 *
		 * This static method is used to trigger a breakpoint and log a message with a specified log type.
		 * The default message is "Breakpoint hit!" and the default log type is LogType::Trace.
		 *
		 * @param[in] _message (optional) The message to log.
		 * @param[in] _type (optional) The log type of the message.
		 */
		static void Break(const std::string_view& _message = "Breakpoint hit!", const LogType& _type = LogType::Trace) noexcept;
		
		/**
		 * @brief Flushes the log output.
		 *
		 * This static method is used to flush the log output. It flushes the output stream `std::cout` to ensure that any buffered log messages are immediately written to the output device.
		 *
		 * @note This method is declared `noexcept`, indicating that it does not throw any exceptions.
		 *
		 * @par Related Function
		 * - Debug::Flush()
		 */
		static void Flush() noexcept;
		
		/**
		 * @brief Logs an exception with a specified log type.
		 *
		 * This static method is used to log an exception with a specified log type.
		 * By default, the log type is set to `LogType::Error`.
		 *
		 * @param[in] e The exception to log.
		 * @param[in] _type (optional) The log type of the message.
		 * @param[in] _inline (optional) A flag indicating if the log message should be displayed inline.
		 *
		 * @note This method is declared `noexcept`, indicating that it does not throw any exceptions.
		 *
		 * @par Related Functions
		 * - Debug::Log(const std::string_view&, const LogType&, const bool&)
		 */
		static void Log(const std::exception& e, const LogType& _type = LogType::Error, const bool& _inline = false) noexcept;
		
		/**
		 * @brief Logs a message with a specified log type.
		 *
		 * This function logs a message with a specified log type.
		 * By default, the log type is set to `Debug`.
		 *
		 * @param[in] _message The message to be logged.
		 * @param[in] _type (optional) The log type for the message (default is `LogType::Debug`).
		 * @param[in] _inline (optional) Specifies whether the log message should be displayed inline (default is `false`).
		 */
		static void Log(const std::string_view& _message, const LogType& _type = LogType::Debug, const bool& _inline = false) noexcept;
	
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_DEBUG_H