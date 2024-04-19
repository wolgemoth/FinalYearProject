#include "Utils.h"

#include "../Debug.h"

#include <al.h>
#include <GL/glew.h>
#include <cstddef>
#include <glm/ext/vector_float3.hpp>

#include <SDL_error.h>

#include <cmath>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace LouiEriksson::Engine {
	
	std::string Utils::Trim(const std::string& _string) {
		
		std::string result;
		
		const auto first = _string.find_first_not_of(" \t\r\n");
		
		if (std::string::npos != first) {
			
			const auto last = _string.find_last_not_of(" \t\r\n");
			
			result = _string.substr(first, (last - first + 1));
		}
		else {
			result = _string;
		}
		
		return result;
	}
	
	void Utils::ALDumpError(const bool& _silent) {
	
		const auto error = alGetError();
	    if (error != AL_NO_ERROR) {
		   
		    switch (error) {
				case AL_INVALID_NAME:      { Debug::Log("OpenAL Error \"AL_INVALID_NAME\"",      LogType::Error); break; }
				case AL_INVALID_ENUM:      { Debug::Log("OpenAL Error \"AL_INVALID_ENUM\"",      LogType::Error); break; }
				case AL_INVALID_VALUE:     { Debug::Log("OpenAL Error \"AL_INVALID_VALUE\"",     LogType::Error); break; }
				case AL_INVALID_OPERATION: { Debug::Log("OpenAL Error \"AL_INVALID_OPERATION\"", LogType::Error); break; }
				case AL_OUT_OF_MEMORY:     { Debug::Log("OpenAL Error \"AL_OUT_OF_MEMORY\"",     LogType::Error); break; }
				default: {
					Debug::Log("OpenAL Error \"Unknown\"", LogType::Error);
				}
		    }
			
			Debug::Break("BREAK_AL_ERR");
	    }
	}
	
	void Utils::GLDumpError(const bool& _silent) {
		
		const auto glError = glGetError();
		if ((glError != GL_NONE) && !_silent) {
			
			std::stringstream ss;
			ss << "OpenGL Error [" << glError << "]: \"" <<
				glewGetErrorString(glError) << "\"";
			
			Debug::Log(ss.str(), LogType::Error);
			Debug::Break("BREAK_GL_ERR");
		}
	}
	
	void Utils::SDLDumpError() {
		
		const auto* const sdlError = SDL_GetError();
		if (sdlError != nullptr && *sdlError != '\0') {
			
			std::stringstream ss;
			ss << "SDL Error \"" << sdlError << "\"";
			
			Debug::Log(ss.str(), LogType::Error);
			Debug::Break("BREAK_SDL_ERR");
			
		    SDL_ClearError();
		}
	}
	
	std::string Utils::Minimise(const std::string& _string) {
		
		static const auto ws = std::regex(R"((\r\n)+|\r+|\n+|\t+|\s)", std::regex::optimize);
		return std::regex_replace(_string, ws, "");
	}
	
} // LouiEriksson::Engine