#ifndef FINALYEARPROJECT_DEBUG_HPP
#define FINALYEARPROJECT_DEBUG_HPP

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-avoid-endl"

#include <csignal>
#include <exception>
#include <iostream>
#include <string_view>

#ifdef _WIN32
	#include <windows.h>
	#include <intrin.h>
#endif

/* Debugging assertions and traps
 * Portable Snippets - https://github.com/nemequ/portable-snippets
 * Created by Evan Nemerson <evan@nemerson.com>
 *
 *   To the extent possible under law, the authors have waived all
 *   copyright and related or neighboring rights to this code.  For
 *   details, see the Creative Commons Zero 1.0 Universal license at
 *   https://creativecommons.org/publicdomain/zero/1.0/
 */
#pragma region Debugging assertions and traps

#if !defined(PSNIP_NDEBUG) && defined(NDEBUG) && !defined(PSNIP_DEBUG)
#  define PSNIP_NDEBUG 1
#endif

#if defined(__has_builtin) && !defined(__ibmxl__)
#  if __has_builtin(__builtin_debugtrap)
#    define psnip_trap() __builtin_debugtrap()
#  elif __has_builtin(__debugbreak)
#    define psnip_trap() __debugbreak()
#  endif
#endif
#if !defined(psnip_trap)
#  if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#    define psnip_trap() __debugbreak()
#  elif defined(__ARMCC_VERSION)
#    define psnip_trap() __breakpoint(42)
#  elif defined(__ibmxl__) || defined(__xlC__)
#    include <builtins.h>
#    define psnip_trap() __trap(42)
#  elif defined(__DMC__) && defined(_M_IX86)
#    define psnip_trap(void) __asm int 3h;
#  elif defined(__i386__) || defined(__x86_64__)
#    define psnip_trap(void) __asm__ __volatile__("int3");
#  elif defined(__thumb__)
#    define psnip_trap(void) __asm__ __volatile__(".inst 0xde01");
#  elif defined(__aarch64__)
#    define psnip_trap(void) __asm__ __volatile__(".inst 0xd4200000");
#  elif defined(__arm__)
#    define psnip_trap(void) __asm__ __volatile__(".inst 0xe7f001f0");
#  elif defined (__alpha__) && !defined(__osf__)
#    define psnip_trap(void) __asm__ __volatile__("bpt");
#  elif defined(_54_)
#    define psnip_trap(void) __asm__ __volatile__("ESTOP");
#  elif defined(_55_)
#    define psnip_trap(void) __asm__ __volatile__(";\n .if (.MNEMONIC)\n ESTOP_1\n .else\n ESTOP_1()\n .endif\n NOP");
#  elif defined(_64P_)
#    define psnip_trap(void) __asm__ __volatile__("SWBP 0");
#  elif defined(_6x_)
#    define psnip_trap(void) __asm__ __volatile__("NOP\n .word 0x10000000");
#  elif defined(__STDC_HOSTED__) && (__STDC_HOSTED__ == 0) && defined(__GNUC__)
#    define psnip_trap() __builtin_trap()
#  else
#    include <signal.h>
#    if defined(SIGTRAP)
#      define psnip_trap() raise(SIGTRAP)
#    else
#      define psnip_trap() raise(SIGABRT)
#    endif
#  endif
#endif

#if defined(HEDLEY_LIKELY)
#  define PSNIP_DBG_LIKELY(expr) HEDLEY_LIKELY(expr)
#elif defined(__GNUC__) && (__GNUC__ >= 3)
#  define PSNIP_DBG_LIKELY(expr) __builtin_expect(!!(expr), 1)
#else
#  define PSNIP_DBG_LIKELY(expr) (!!(expr))
#endif

#if !defined(PSNIP_NDEBUG) || (PSNIP_NDEBUG == 0)
#  define psnip_dbg_assert(expr) do { \
    if (!PSNIP_DBG_LIKELY(expr)) { \
      psnip_trap(); \
    } \
  } while (0)
#else
#  define psnip_dbg_assert(expr)
#endif

#pragma endregion Debugging assertions and traps

namespace LouiEriksson::Engine {
	
	/**
	 * @enum LogType
	 * @brief Enumeration representing different types of log messages.
	 *
	 * The LogType enum represents different levels of log messages that can be used to categorize log entries.
	 * Each log type is assigned a character value which is used to differentiate between log messages.
	 */
	 enum LogType : unsigned char {
		   Trace = 1U,       /**< @brief In-depth tracking of system operations.     */
		   Debug = 1U << 1U, /**< @brief General code debugging.                     */
		    Info = 1U << 2U, /**< @brief General insights about application status.  */
		 Warning = 1U << 3U, /**< @brief Potential issues that could cause problems. */
		   Error = 1U << 4U, /**< @brief Major issues disrupting normal operations.  */
		Critical = 1U << 5U, /**< @brief Severe problems causing system failure.     */
	};
	
	class Print final {
		
		friend struct Debug;
		
		static constexpr const char* ToString(const LogType& _type) noexcept {
			
			const char* result = "UNKNOWN";
			
			switch (_type) {
				case Critical: { result = "CRITICAL"; break; }
				case Error:    { result = "ERROR";    break; }
				case Warning:  { result = "WARNING";  break; }
				case Info:     { result = "INFO";     break; }
				case Debug:    { result = "DEBUG";    break; }
				case Trace:    { result = "TRACE";    break; }
			}
			
			return result;
		}
		
		static void Fallback(const std::string_view& _message, const LogType& _type, const bool& _inline) {
			
			std::cout << Print::ToString(_type) << ": " << _message;
			
			if (_inline && _type != LogType::Info) {
				std::cout << std::flush;
			}
			else {
				std::cout << std::endl;
			}
		}
		
#if __linux__ | __APPLE__
		
		static void ANSI(const std::string_view& _message, const LogType& _type, const bool& _inline) {
			
			/* ANSI TEXT COLORS */
			#define ANSI_RESET   "\033[0m"
			#define ANSI_BLACK   "\033[30m"
			#define ANSI_RED     "\033[31m"
			#define ANSI_GREEN   "\033[32m"
			#define ANSI_YELLOW  "\033[33m"
			#define ANSI_BLUE    "\033[34m"
			#define ANSI_MAGENTA "\033[35m"
			#define ANSI_CYAN    "\033[36m"
			#define ANSI_WHITE   "\033[37m"
			
			/* ANSI BACKGROUND COLORS */
			#define ANSI_BG_BLACK   "\033[40m"
			#define ANSI_BG_RED     "\033[41m"
			#define ANSI_BG_GREEN   "\033[42m"
			#define ANSI_BG_YELLOW  "\033[43m"
			#define ANSI_BG_BLUE    "\033[44m"
			#define ANSI_BG_MAGENTA "\033[45m"
			#define ANSI_BG_CYAN    "\033[46m"
			#define ANSI_BG_WHITE   "\033[47m"
			
			switch (_type) {
				
				case Critical: {
					std::cout << ANSI_MAGENTA << _message << ANSI_RESET << '\a';
					
					if (_inline) {
						std::cout << std::flush;
					}
					else {
						std::cout << std::endl;
					}
					
					break;
				}
				case Error: {
					std::cout << ANSI_RED << _message << ANSI_RESET;
					
					if (_inline) {
						std::cout << std::flush;
					}
					else {
						std::cout << std::endl;
					}
					
					break;
				}
				case Warning: {
					std::cout << ANSI_YELLOW << _message << ANSI_RESET;
					
					if (_inline) {
						std::cout << std::flush;
					}
					else {
						std::cout << std::endl;
					}
					
					break;
				}
				case Info: {
					std::cout << ANSI_CYAN << _message << ANSI_RESET;
					
					if (!_inline) {
						std::cout << '\n';
					}
					
					break;
				}
				case Debug: {
					std::cout << ANSI_WHITE << _message << ANSI_RESET;
					
					if (_inline) {
						std::cout << std::flush;
					}
					else {
						std::cout << std::endl;
					}
					
					break;
				}
				case Trace: {
					std::cout << ANSI_BG_WHITE << ANSI_BLACK << _message << ANSI_RESET;
					
					if (_inline) {
						std::cout << std::flush;
					}
					else {
						std::cout << std::endl;
					}
					
					break;
				}
				default: {
					std::cout << ANSI_BG_MAGENTA << ANSI_BLACK << _message << ANSI_RESET;
					
					if (_inline) {
						std::cout << std::flush;
					}
					else {
						std::cout << std::endl;
					}
					
					break;
				}
			}
			
			#undef ANSI_RESET
			#undef ANSI_BLACK
			#undef ANSI_RED
			#undef ANSI_GREEN
			#undef ANSI_YELLOW
			#undef ANSI_BLUE
			#undef ANSI_MAGENTA
			#undef ANSI_CYAN
			#undef ANSI_WHITE
			
			#undef ANSI_BG_BLACK
			#undef ANSI_BG_RED
			#undef ANSI_BG_GREEN
			#undef ANSI_BG_YELLOW
			#undef ANSI_BG_BLUE
			#undef ANSI_BG_MAGENTA
			#undef ANSI_BG_CYAN
			#undef ANSI_BG_WHITE
		}

#elif _WIN32

		void SetCAttr(const HANDLE* const _h, const WORD& _attribute) {
			
		    if (!SetConsoleTextAttribute(_h, _attribute)) {
		        throw std::runtime_error("Failed to set the console text attribute.");
		    }
		}

		static void WIN32(const std::string_view& _message, const LogType& _type, const bool& _inline) {
		
			try {
				
				const auto* const h = GetStdHandle(STD_OUTPUT_HANDLE);
				
				if (h != nullptr && h != INVALID_HANDLE_VALUE) {
					
					CONSOLE_SCREEN_BUFFER_INFO cinfo;
					if (GetConsoleScreenBufferInfo(h, &cinfo)) {
						
						const auto& previous_attr = cinfo.wAttributes;
						
						switch (_type) {
							case Critical: {
	                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_MAGENTA);
								std::cout << _message;
	                            SetConsoleTextAttribute(h, previous_attr);
								
								Beep(800, 200);
								
								if (_inline) {
									std::cout << std::flush;
								}
								else {
									std::cout << std::endl;
								}
								
								break;
							}
							case Error: {
	                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_RED);
								std::cout << _message;
	                            SetConsoleTextAttribute(h, previous_attr);
								
								if (_inline) {
									std::cout << std::flush;
								}
								else {
									std::cout << std::endl;
								}
								
								break;
							}
							case Warning: {
	                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_YELLOW);
								std::cout << _message;
	                            SetConsoleTextAttribute(h, previous_attr);
								
								if (_inline) {
									std::cout << std::flush;
								}
								else {
									std::cout << std::endl;
								}
								
								break;
							}
							case Info: {
	                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_CYAN);
								std::cout << _message;
	                            SetConsoleTextAttribute(h, previous_attr);
								
								if (!_inline) {
									std::cout << '\n';
								}
								
								break;
							}
							case Debug: {
	                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_WHITE);
								std::cout << _message;
	                            SetConsoleTextAttribute(h, previous_attr);
								
								if (_inline) {
									std::cout << std::flush;
								}
								else {
									std::cout << std::endl;
								}
								
								break;
							}
							case Trace: {
	                            SetConsoleTextAttribute(h, BACKGROUND_WHITE | FOREGROUND_BLACK);
								std::cout << _message;
	                            SetConsoleTextAttribute(h, previous_attr);
								
								if (_inline) {
									std::cout << std::flush;
								}
								else {
									std::cout << std::endl;
								}
								
								break;
							}
							default: {
	                            SetConsoleTextAttribute(h, BACKGROUND_MAGENTA | FOREGROUND_BLACK);
								std::cout << _message;
	                            SetConsoleTextAttribute(h, previous_attr);
								
								if (_inline) {
									std::cout << std::flush;
								}
								else {
									std::cout << std::endl;
								}
							}
						}
						
					}
					else {
						throw std::runtime_error("Failed to get the console screen buffer info.");
					}
				}
				else {
					throw std::runtime_error("Failed to get the standard output handle.");
				}
			}
			catch (const std::exception& e) {
				std::cerr << "WIN32_LOG_ERR: " << e.what() << std::endl;
				
				throw e;
			}
		}
		
#endif
	
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
		static void Assert(const bool& _condition, const std::string_view& _message, const LogType& _type = LogType::Debug, const bool& _inline = false) noexcept {
			
			if (!_condition) {
				Debug::Log(_message, _type, _inline);
			}
		}
		
		/**
		 * @brief Triggers a breakpoint.
		 *
		 * @note This function only has an effect on Debug builds.
		 *
		 * @warning Please note that if the function cannot identify the correct signal for a breakpoint, no breakpoint will occur.
		 */
		static void Break() noexcept  {
		
#if !defined(NDEBUG) || _DEBUG
			
			Flush();
			
			try {
				psnip_trap();
			}
			catch (const std::exception& e) {
				std::cerr << "BREAKPOINT_ERR: " << e.what() << "\n";
			}
			
#endif
		}
		
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
		static void Flush() noexcept {
			
			try {
				try {
					std::cout << std::flush;
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
				}
			}
			catch (...) {}
		}
		
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
		static void Log(const std::exception& e, const LogType& _type = LogType::Error, const bool& _inline = false) noexcept  {
			Debug::Log(e.what(), _type, _inline);
		}
		
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
		static void Log(const std::string_view& _message, const LogType& _type = LogType::Debug, const bool& _inline = false) noexcept {
			
			try {
				try {
					
#ifdef __linux__
						try {
				            Print::ANSI(_message, _type, _inline);
						}
						catch (const std::exception& e) {
							Print::Fallback(_message, _type, _inline);
							throw e;
						}
#elif _WIN32
						try {
				            Print::WIN32(_message, _type, _inline);
						}
						catch (const std::exception& e) {
							Print::Fallback(_message, _type, _inline);
							throw e;
						}
#elif __APPLE__
						try {
				            Print::ANSI(_message, _type, _inline);
						}
						catch (const std::exception& e) {
							Print::Fallback(_message, _type, _inline);
							throw e;
						}
#else
						Print::Fallback(_message, _type, _inline);
#endif

#if !defined(NDEBUG) || _DEBUG
						if (_type == LogType::Critical) {
							Break();
						}
#endif
				
				}
				catch (const std::exception& e) {
					std::cerr << "LOG_ERR: " << e.what() << std::endl;
				}
			}
			catch (...) {}
		}
	
	};
	
} // LouiEriksson::Engine

#pragma clang diagnostic pop

#endif //FINALYEARPROJECT_DEBUG_HPP