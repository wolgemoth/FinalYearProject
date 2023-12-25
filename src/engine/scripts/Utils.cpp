#include "stdafx.h"

#include "Utils.h"

#include <cmath>

namespace LouiEriksson {
	
	std::vector<std::string> Utils::Split(const std::string& _string, const char& _divider) {
		
		std::vector<std::string> result;
		
		// Init string stream.
		std::istringstream iss(_string);
		
		std::string substring;
		
		// Split string stream using divider and push into result.
		while (std::getline(iss, substring, _divider)) {
			result.push_back(substring);
		}
		
		return result;
	}
	
	std::string Utils::Trim(const std::string& _string) {
		
		// Use a regex pattern to trim the leading and trailing whitespace characters from the string.
		std::smatch sm;
		return std::regex_search(_string, sm, std::regex(R"(^(\S)(.+))")) ?
			sm[0] : _string;
	}
	
	float Utils::Remap(const float& _val, const float& _aMin, const float& _aMax, const float& _bMin, const float& _bMax) {
		
		// Remap function I made for a homework task in "Mathematics for Computer Graphics".
		return _bMin + (_val - _aMin) * (_bMax - _bMin) / (_aMax - _aMin);
	}
	
	template<typename T>
	void Utils::Empty(std::queue<T>& _queue) {
		
		while (!_queue.empty()) {
			_queue.pop();
		}
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
	
} // LouiEriksson