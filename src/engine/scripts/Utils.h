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
		/// Calculate the hashcode of a given object using std::hash.
		/// <remarks>
		/// This function will throw if the type given is not supported by std::hash.
		/// </remarks>
		/// <param name="_item">Item to calculate hash of.</param>
		/// </summary>
		template<typename T>
		static size_t GetHashcode(const T& _item);
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_UTILS_H