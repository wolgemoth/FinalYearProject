#include "Debug.h"

#ifdef _WIN32
	#include <windows.h>
#endif

#include <iostream>
#include <string>

namespace LouiEriksson::Engine::Debug {
	
	struct Print final {
		
		static std::string GetName(const LogType& _type) noexcept {
			
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
					std::cerr << e.what() << std::endl;
					
					throw e;
				}
			}
			catch (...) {
				result = "???";
			}
			
			return result;
		}
		
#if __linux__ | __APPLE__
		
		static void ANSI(const std::string& _message, const LogType& _type) noexcept {
			
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
							std::cerr << ANSI_MAGENTA << _message << ANSI_RESET << '\n';
							break;
						}
						case Error: {
							std::cerr << ANSI_RED << _message << ANSI_RESET << '\n';
							break;
						}
						case Warning: {
							std::clog << ANSI_YELLOW << _message << ANSI_RESET << '\n';
							break;
						}
						case Info: {
							std::cout << ANSI_CYAN << _message << ANSI_RESET << '\n';
							break;
						}
						case Debug: {
							std::clog << ANSI_WHITE << _message << ANSI_RESET << '\n';
							break;
						}
						case Trace: {
							std::cerr << ANSI_BG_WHITE << ANSI_BLACK << _message << ANSI_RESET << '\n';
							break;
						}
						default: {
							std::cerr << ANSI_BG_MAGENTA << ANSI_BLACK << _message << ANSI_RESET << '\n';
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

		static void WIN32(const std::string& _message, const LogType& _type) noexcept {
		
			try {
				try {
					
					const auto* h = GetStdHandle(STD_OUTPUT_HANDLE);
					const auto previous_attr = GetConsoleTextAttribute(h);
					
					switch (_type) {
						case Critical: {
                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_MAGENTA);
							std::cerr << _message << '\n';
							break;
						}
						case Error: {
                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_RED);
							std::cerr << _message << '\n';
							break;
						}
						case Warning: {
                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_YELLOW);
							std::clog << _message << '\n';
							break;
						}
						case Info: {
                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_CYAN);
							std::cout << _message << '\n';
							break;
						}
						case Debug: {
                            SetConsoleTextAttribute(h, BACKGROUND_BLACK | FOREGROUND_WHITE);
							std::clog << _message << '\n';
							break;
						}
						case Trace: {
                            SetConsoleTextAttribute(h, BACKGROUND_WHITE | FOREGROUND_BLACK);
							std::cerr << _message << '\n';
							break;
						}
						default: {
                            SetConsoleTextAttribute(h, BACKGROUND_MAGENTA | FOREGROUND_BLACK);
							std::cerr << _message << '\n';
						}
					}
					
                    SetConsoleTextAttribute(h, previous_attr);
				}
				catch (const std::exception& e) {
					std::cerr << e.what() << std::endl;
					
					throw e;
				}
			}
			catch (...) {
				std::cout << _message << std::endl;
			}
		}
		
#endif
	
	};
	
	void Assert(const bool& _condition, const std::string& _message, const LogType& _type) noexcept {
		
		if (!_condition) {
			Log(_message, _type);
		}
	}
	
	void Log(const std::exception& e, const LogType& _type) noexcept {
		Log(e.what(), _type);
	}
	
	void Log(const std::string& _message, const LogType& _type) noexcept {
	
		try {
			try {
				
				#ifdef __linux__
			        Print::ANSI(_message, _type);
			    #elif _WIN32
			        Print::WIN32(_message, _type);
			    #elif __APPLE__
			        Print::ANSI(_message, _type);
				#else
					std::cout << Print::ToString(_type) << ": " << _message << std::endl;
			    #endif
			}
			catch (const std::exception& e) {
				std::cerr << "LOG_ERR: " << e.what() << std::endl;
			}
		}
		catch (...) {}
	}
	
} // LouiEriksson::Engine