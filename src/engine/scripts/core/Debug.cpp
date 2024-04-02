#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-avoid-endl"

#include "Debug.h"

#ifdef _WIN32
	#include <windows.h>
#endif

#include <exception>
#include <iostream>
#include <string>

namespace LouiEriksson::Engine {
	
	struct Print final {
		
		static std::string ToString(const LogType& _type) noexcept {
			
			std::string result;
			
			try {
				try {
					
					switch (_type) {
						case Critical: { result = "CRITICAL"; break; }
						case Error:    { result = "ERROR";    break; }
						case Warning:  { result = "WARNING";  break; }
						case Info:     { result = "INFO";     break; }
						case Debug:    { result = "DEBUG";    break; }
						case Trace:    { result = "TRACE";    break; }
						default:       { result = "UNKNOWN"; }
					}
				}
				catch (const std::exception& e) {
					Debug::Log(e);
					
					throw e;
				}
			}
			catch (...) {
				result = "???";
			}
			
			return result;
		}
		
#if __linux__ | __APPLE__
		
		static void ANSI(const std::string& _message, const LogType& _type, const bool& _inline) noexcept {
			
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
			
			try {
				try {
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
						}
					}
				}
				catch (const std::exception& e) {
					std::cerr << "ANSI_LOG_ERR: " << e.what() << std::endl;
					
					throw e;
				}
			}
			catch (...) {
				std::cout << _message << std::endl;
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

		static void WIN32(const std::string& _message, const LogType& _type, const bool& _inline) noexcept {
		
			try {
				try {
					
					const auto* h = GetStdHandle(STD_OUTPUT_HANDLE);
					const auto previous_attr = GetConsoleTextAttribute(h);
					
					switch (_type) {
						case Critical: {
                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_MAGENTA);
							std::cout << _message << '\a';
                            SetConsoleTextAttribute(h, previous_attr);
							
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
				catch (const std::exception& e) {
					std::cerr << "WIN32_LOG_ERR: " << e.what() << std::endl;
					
					throw e;
				}
			}
			catch (...) {
				std::cout << _message << std::endl;
			}
		}
		
#endif
	
	};
	
	void Debug::Assert(const bool& _condition, const std::string& _message, const LogType& _type, const bool& _inline) noexcept {
		
		if (!_condition) {
			Debug::Log(_message, _type, _inline);
		}
	}
	
	void Debug::Log(const std::exception& e, const LogType& _type, const bool& _inline) noexcept {
		Debug::Log(e.what(), _type, _inline);
	}
	
	void Debug::Log(const std::string& _message, const LogType& _type, const bool& _inline) noexcept {
	
		try {
			try {
				
				#ifdef __linux__
			        Print::ANSI(_message, _type, _inline);
			    #elif _WIN32
			        Print::WIN32(_message, _type, _inline);
			    #elif __APPLE__
			        Print::ANSI(_message, _type, _inline);
				#else
					std::cout << Print::ToString(_type) << ": " << _message;
				
					if (_inline) {
						std::cout << std::flush;
					}
					else {
						std::cout << std::endl;
					}
				
			    #endif
			}
			catch (const std::exception& e) {
				std::cerr << "LOG_ERR: " << e.what() << std::endl;
			}
		}
		catch (...) {}
	}
	
	void Debug::Flush() noexcept {
		
		try {
			try {
				std::cout << std::flush;
			}
			catch (std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}
		catch (...) {}
	}
	
} // LouiEriksson::Engine

#pragma clang diagnostic pop