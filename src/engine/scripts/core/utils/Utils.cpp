#include "Utils.h"

#include "../Debug.h"

#include <al.h>
#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>

#include <SDL_error.h>

#include <cmath>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace LouiEriksson::Engine {
	
	std::vector<std::string> Utils::Split(const std::string_view& _string, const char& _divider) {
		
		std::vector<std::string> result;
		
		// Init string stream.
		std::istringstream iss(_string.data());
		
		std::string substring;
		
		// Split string stream using divider and push into result.
		while (std::getline(iss, substring, _divider)) {
			result.emplace_back(substring);
		}
		
		return result;
	}
	
	std::string Utils::Trim(const std::string& _string) {
		
		// Use a regex pattern to trim the leading and trailing whitespace characters from the string.
		std::smatch sm;
		return std::regex_search(_string, sm, std::regex(R"(^(\S)(.+))")) ?
			sm[0] : _string;
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
	    }
	}
	
	void Utils::GLDumpError(const bool& _silent) {
		
		const auto glError = glGetError();
		if ((glError != GL_NONE) && !_silent) {
			
			std::stringstream ss;
			ss << "OpenGL Error [" << glError << "]: \"" <<
				glewGetErrorString(glError) << "\"";
			
			Debug::Log(ss.str(), LogType::Error);
		}
	}
	
	void Utils::SDLDumpError() {
		
		const auto* const sdlError = SDL_GetError();
		if (sdlError != nullptr && *sdlError != '\0') {
			
			std::stringstream ss;
			ss << "SDL Error \"" << sdlError << "\"";
			
			Debug::Log(ss.str(), LogType::Error);
			
		    SDL_ClearError();
		}
	}
	
	std::string Utils::Minimise(const std::string& _string) {
		return std::regex_replace(_string, std::regex(R"((\r\n)+|\r+|\n+|\t+|\s)"), "");
	}
	
	float Utils::Repeat(const float& _value, const float& _max) {
		return std::fmod(_max + std::fmod(_value, _max), _max);
	}
	
	float Utils::Repeat(const float& _value, const float& _min, const float& _max) {
		return _min + Repeat(_value - _max, _max - _min);
	}
	
	float Utils::WrapAngle(const float& _degrees) {
		return Repeat(_degrees, -180.0f, 180.0f);
	}
	
	glm::vec3 Utils::WrapAngle(const glm::vec3& _degrees) {
		return {
			WrapAngle(_degrees.x),
			WrapAngle(_degrees.y),
			WrapAngle(_degrees.z)
		};
	}
	
} // LouiEriksson::Engine