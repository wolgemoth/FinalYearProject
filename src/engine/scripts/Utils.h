#ifndef FINALYEARPROJECT_UTILS_H
#define FINALYEARPROJECT_UTILS_H

namespace LouiEriksson {
	
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
		static float Remap(const float& _val, const float& _aMin, const float& _aMax, const float& _bMin, const float& _bMax);
		
		/// <summary>
		/// Empties a queue.
		/// </summary>
		template<typename T>
		static void Empty(std::queue<T>& _queue);
		
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
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_UTILS_H