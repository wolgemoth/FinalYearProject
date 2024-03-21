#ifndef FINALYEARPROJECT_UTILS_H
#define FINALYEARPROJECT_UTILS_H

#include <glm/ext/vector_float3.hpp>

#include <queue>
#include <string>
#include <vector>

namespace LouiEriksson::Engine {
	
	class Utils {
	
	public:
		
		/// <summary>
		/// Split a string in a collection of substrings using a divider.
		/// </summary>
		static std::vector<std::string> Split(const std::string& _string, const char& _divider);
		
		/// <summary>
		/// Trim leading and trailing whitespace characters from a string.
		/// </summary>
		static std::string Trim(const std::string& _string);
		
		/// <summary>
		/// Remap a float from one range to another.
		/// </summary>
		template<typename T>
		static T Remap(const T& _val, const T& _aMin, const T& _aMax, const T& _bMin, const T& _bMax) {
	
			// Remap function I made for a homework task in "Mathematics for Computer Graphics".
			return _bMin + (_val - _aMin) * (_bMax - _bMin) / (_aMax - _aMin);
		}
		
		/// <summary>
		/// Empties a queue.
		/// </summary>
		template<typename T>
		static void Empty(std::queue<T>& _queue) {
			
			while (!_queue.empty()) {
				_queue.pop();
			}
		}
		
		/// <summary>
		/// Dumps the latest OpenAL error to the console.
		/// </summary>
		static void ALDumpError(const bool& _silent = false);
		
		/// <summary>
		/// Dumps the latest OpenGL error to the console.
		/// </summary>
		static void GLDumpError(const bool& _silent = false);
		
		/// <summary>
		/// Repeats a value within the range 0 -> _max.
		/// </summary>
		static float Repeat(const float& _value, const float& _max);
		
		/// <summary>
		/// Repeats a value within the range _min -> _max.
		/// </summary>
		static float Repeat(const float& _value, const float& _min, const float& _max);
		
		/// <summary>
		/// Wraps an angle so that it remains within -180 to 180 degrees.
		/// </summary>
		static float WrapAngle(const float& _degrees);
		
		/// <summary>
		/// Wraps an angle so that it remains within -180 to 180 degrees.
		/// </summary>
		static glm::vec3 WrapAngle(const glm::vec3& _degrees);
		
		template<typename T, glm::precision P>
		static double SignedAngle(glm::vec<3, T, P> _a, glm::vec<3, T, P> _b, glm::vec<3, T, P> _axis) {
						
		    auto d = glm::dot  (_a, _b);
		    auto p = glm::cross(_a, _b);
		    
		    auto angle = glm::atan(glm::length(p), d);
		    
		    return angle * glm::sign(dot(p, _axis));
		}
		
		template<typename T, glm::precision P>
		static glm::vec<3, T, P> ChangeHandedness(const glm::vec<3, T, P>& _vec) {
			return glm::vec3 { -_vec.x, _vec.z, _vec.y };
		}
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_UTILS_H