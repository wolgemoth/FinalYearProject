#ifndef FINALYEARPROJECT_UTILS_H
#define FINALYEARPROJECT_UTILS_H

#include "../Debug.h"
#include "glm/geometric.hpp"

#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>

#include <json.hpp>

#include <array>
#include <cmath>
#include <cstddef>
#include <exception>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace LouiEriksson::Engine {
	
	/**
	 * @file Utils.hpp
	 *
	 * @brief Contains various utility functions.
	 */
	class Utils final {
	
	public:
		
		/**
		 * @brief Splits a string into a vector of substrings based on a delimiter.
		 *
		 * The provided string is split into substrings using the specified delimiter.
		 * The substrings are then stored in a vector and returned.
		 * If the template argument is std::string, the substrings are stored as std::string objects.
		 * If the template argument is std::string_view, the substrings are stored as std::string_view objects.
		 *
		 * @param[in] _string The string to split.
		 * @param[in] _divider The delimiter character to split the string by.
		 * @param[in] _capacity (optional) The initial capacity of the result vector. Defaults to 0.
		 *
		 * @tparam T The type of the resulting substrings. Must be either std::string or std::string_view.
		 *
		 * @return std::vector<T> A vector containing the splitted substrings.
		 *
		 * @note The function assumes that the provided delimiter is a single character.
		 * @note If _capacity is specified, the internal vector capacity is set to that value.
		 * @note The last word of the string is always included in the result, even if it is not delimited.
		 * @note The function assumes that the template argument is std::string or std::string_view.
		 */
		template<typename T>
		static std::vector<T> Split(const std::string_view& _string, const char& _divider, const size_t& _capacity = 0) {
			
			static_assert(std::is_same_v<T, std::string_view> || std::is_same_v<T, std::string>,
                  "Template argument must be std::string or std::string_view");
    
			std::vector<T> result;
			result.reserve(_capacity);
			
			// Boost library version of string splitting.
			// (https://www.boost.org/doc/libs/1_54_0/doc/html/string_algo/usage.html#idp166856528)
			std::string::size_type start = 0, end;
			
			while((end = _string.find(_divider, start)) != std::string::npos) {
			    result.emplace_back(_string.substr(start, end - start));
			    start = end + 1;
			}
			
			// Last word is not delimited.
			result.emplace_back(_string.substr(start));
			
			return result;
		}
		
		/**
		 * @brief Trim leading and trailing whitespace characters from a string.
		 *
		 * This function takes a string and removes any leading and trailing whitespace characters from it.
		 *
		 * @param[in] _string The input string to be trimmed.
		 *
		 * @return std::string The trimmed string.
		 */
		static std::string Trim(const std::string& _string);
		
		/**
		 * @brief Remap a number from one range to another.
		 *
		 * This template function takes a value (_val) and remaps it from the range [_aMin, _aMax] to the range [_bMin, _bMax].
		 *
		 * @tparam T The type of the input and output values.
		 * @param[in] _val The value to be remapped.
		 * @param[in] _aMin The minimum value of the source range.
		 * @param[in] _aMax The maximum value of the source range.
		 * @param[in] _bMin The minimum value of the target range.
		 * @param[in] _bMax The maximum value of the target range.
		 *
		 * @return T The remapped value.
		 */
		template<typename T>
		static T Remap(const T& _val, const T& _aMin, const T& _aMax, const T& _bMin, const T& _bMax) {
	
			// Remap function I made for a homework task in "Mathematics for Computer Graphics".
			return _bMin + (_val - _aMin) * (_bMax - _bMin) / (_aMax - _aMin);
		}
		
		/**
		 * @brief Empties a queue.
		 *
		 * This template function takes a reference to a queue and pops all elements from it until it is empty.
		 *
		 * @tparam T The type of elements in the queue.
		 * @param[in,out] _queue The queue to be emptied.
		 */
		template<typename T>
		static void Empty(std::queue<T>& _queue) {
			
			while (!_queue.empty()) {
				_queue.pop();
			}
		}
		
		/**
		 * @brief Dumps the latest OpenAL error to the console.
		 *
		 * This function retrieves the latest OpenAL error using alGetError() and logs the corresponding error message to the console.
		 * If the _silent parameter is set to true, the error message will not be logged.
		 *
		 * @param[in] _silent (optional) Flag indicating whether the error message should be logged or not. Defaults to false.
		 */
		static void ALDumpError(const bool& _silent = false);
		
		/**
		 * @brief Dumps the latest OpenGL error to the console.
		 *
		 * This function retrieves the latest OpenGL error using glGetError() and logs the corresponding error message to the console.
		 * If the _silent parameter is set to true, the error message will not be logged.
		 *
		 * @param[in] _silent (optional) Flag indicating whether the error message should be logged or not. Defaults to false.
		 */
		static void GLDumpError(const bool& _silent = false);
		
		/**
		 * @brief Dumps the latest SDL error to the console.
		 *
		 * This function retrieves the latest SDL error using SDL_GetError() and logs the corresponding error message to the console.		 *
		 */
		static void SDLDumpError();
		
		/**
		 * @brief Minimises a string by stripping out line endings, tabs, and spaces.
		 *
		 * This static function takes a string and removes any line endings, tabs, and spaces from it.
		 *
		 * @param[in] _string The input string to be minimised.
		 *
		 * @return std::string The minimised string.
		 */
		static std::string Minimise(const std::string& _string);
		
		/**
		 * @brief Repeats a value within the range 0 -> _max.
		 *
		 * This static function takes a value _value and repeats it within the range 0 -> _max.
		 *
		 * @param[in] _value The value to be repeated.
		 * @param[in] _max The maximum value of the range.
		 *
		 * @return float The repeated value within the range 0 -> _max.
		 */
		static float Repeat(const float& _value, const float& _max);
		
		/**
		 * @brief Repeats a value within the given range.
		 *
		 * This static function takes a value `_value` and repeats it within the range `_min` to `_max`.
		 *
		 * @param[in] _value The value to be repeated.
		 * @param[in] _min The minimum value of the range.
		 * @param[in] _max The maximum value of the range.
		 *
		 * @return float The repeated value within the range `_min` to `_max`.
		 */
		static float Repeat(const float& _value, const float& _min, const float& _max);
		
		/**
		 * @brief Wraps an angle so that it remains within -180 to 180 degrees.
		 *
		 * This static function takes an angle in degrees and wraps it within the range of -180 to 180 degrees.
		 * If the angle is already within this range, it is returned as it is.
		 *
		 * @param[in] _degrees The angle in degrees to be wrapped.
		 *
		 * @return float The wrapped angle in degrees within the range -180 to 180.
		 */
		static float WrapAngle(const float& _degrees);
		
		/**
		 * @brief Wraps the components of an euler angle so that they each remain within -180 to 180 degrees.
		 *
		 * This function takes an euler angle with components in degrees and wraps each component so that it is within the range of -180 to 180 degrees.
		 * If the angle is already within this range, it is returned as it is.
		 *
		 * @param[in] _degrees The angle in degrees to be wrapped.
		 *
		 * @return glm::vec3 The wrapped angle in degrees within the range -180 to 180.
		 */
		static glm::vec3 WrapAngle(const glm::vec3& _degrees);
		
		/**
		 * @brief Convert a JSON value to the specified type.
		 *
		 * This template function converts a JSON value to the specified type.
		 *
		 * @tparam T The type to convert the JSON value to.
		 * @param[in] _json The JSON value to convert.
		 * @return T The converted value of type T.
		 */
		template <typename T>
		inline static T As(const nlohmann::json& _json);
		
		/**
		 * @brief Attempts to parse a string into an optional value of type T.
		 *
		 * This function attempts to convert the given string into a value of type T.
		 * If the conversion is successful, the resulting value is wrapped in an optional object and returned.
		 * If the conversion fails, an empty optional object is returned.
		 *
		 * @tparam T The type of value to parse the string into.
		 * @param[in] _str The string to parse.
		 * @return An optional value of type T, containing the parsed value if the conversion is successful, or an empty optional if the conversion fails.
		 *
		 * @note The type T must provide a specialize definition of this function in order to support parsing for that type.
		 */
		template <typename T>
		inline static std::optional<T> TryParse(const std::string_view& _str) noexcept {
			
			T r;
			
		    char* e;                      // (end)
			static constexpr auto b = 10; // (base)
			
			try {
				
				     if constexpr (std::is_same_v<T, int                >) { r = static_cast<T>(std::strtol  (_str.data(), &e, b)); }
				else if constexpr (std::is_same_v<T, short              >) { r = static_cast<T>(std::strtol  (_str.data(), &e, b)); }
				else if constexpr (std::is_same_v<T, long               >) { r =                std::strtol  (_str.data(), &e, b ); }
				else if constexpr (std::is_same_v<T, long long          >) { r =                std::strtoll (_str.data(), &e, b ); }
				else if constexpr (std::is_same_v<T, unsigned int       >) { r = static_cast<T>(std::strtoul (_str.data(), &e, b)); }
				else if constexpr (std::is_same_v<T, unsigned short     >) { r = static_cast<T>(std::strtoul (_str.data(), &e, b)); }
				else if constexpr (std::is_same_v<T, unsigned long      >) { r =                std::strtoul (_str.data(), &e, b ); }
				else if constexpr (std::is_same_v<T, unsigned long long >) { r =                std::strtoull(_str.data(), &e, b ); }
				else if constexpr (std::is_same_v<T, float              >) { r =                std::strtof  (_str.data(), &e    ); }
				else if constexpr (std::is_same_v<T, double             >) { r =                std::strtod  (_str.data(), &e    ); }
				else if constexpr (std::is_same_v<T, long double        >) { r =                std::strtold (_str.data(), &e    ); }
				else if constexpr (std::is_same_v<T, char         > ||
				                   std::is_same_v<T, unsigned char> ||
								   std::is_same_v<T, signed char  > ||
						           std::is_same_v<T, char16_t     > ||
				                   std::is_same_v<T, char32_t     > ||
		                           std::is_same_v<T, wchar_t      >)
			    {
					if (!_str.empty()) {
			            r = static_cast<T>(_str[0]);
					}
					else {
						e = const_cast<char*>(_str.data());
					}
				}
				else if constexpr (std::is_same_v<T, bool>) {
			        r = _str == "true" || _str == "True" || _str == "TRUE" || _str == "T" || _str == "1";
				}
				else {
					e = const_cast<char*>(_str.data());
					
					Debug::Log("No specialisation exists for parsing string to T", LogType::Error);
				}
			}
			catch (std::exception& err) {
				
				/* Shouldn't ever happen but catch anyway... */
				
				e = const_cast<char*>(_str.data());
				
				Debug::Log(err);
				Debug::Break();
			}
			
			return (e == _str.data()) ?
		        std::optional<T>(std::nullopt) :
				std::optional<T>(r);
		}
		
		/**
		 * @brief Attempts to parse a string into a value of type T.
		 *
		 * This function attempts to convert the given string into a value of type T.
		 * If the conversion is successful, the resulting value is wrapped in an optional object and returned.
		 * If the conversion fails, an exception is thrown.
		 *
		 * @tparam T The type of value to parse the string into.
		 * @param[in] _str The string to parse.
		 * @return A value of type T, containing the parsed value.
		 *
		 * @note The type T must provide a specialize definition of this function in order to support parsing for that type.
		 */
		template <typename T>
		inline static T Parse(const std::string_view& _str) {
			
			T r;
			
		    char* e;                      // (end)
			static constexpr auto b = 10; // (base)
			
			     if constexpr (std::is_same_v<T, int                >) { r = static_cast<T>(std::strtol  (_str.data(), &e, b)); }
			else if constexpr (std::is_same_v<T, short              >) { r = static_cast<T>(std::strtol  (_str.data(), &e, b)); }
			else if constexpr (std::is_same_v<T, long               >) { r =                std::strtol  (_str.data(), &e, b ); }
			else if constexpr (std::is_same_v<T, long long          >) { r =                std::strtoll (_str.data(), &e, b ); }
			else if constexpr (std::is_same_v<T, unsigned int       >) { r = static_cast<T>(std::strtoul (_str.data(), &e, b)); }
			else if constexpr (std::is_same_v<T, unsigned short     >) { r = static_cast<T>(std::strtoul (_str.data(), &e, b)); }
			else if constexpr (std::is_same_v<T, unsigned long      >) { r =                std::strtoul (_str.data(), &e, b ); }
			else if constexpr (std::is_same_v<T, unsigned long long >) { r =                std::strtoull(_str.data(), &e, b ); }
			else if constexpr (std::is_same_v<T, float              >) { r =                std::strtof  (_str.data(), &e    ); }
			else if constexpr (std::is_same_v<T, double             >) { r =                std::strtod  (_str.data(), &e    ); }
			else if constexpr (std::is_same_v<T, long double        >) { r =                std::strtold (_str.data(), &e    ); }
			else if constexpr (std::is_same_v<T, char         > ||
			                   std::is_same_v<T, unsigned char> ||
							   std::is_same_v<T, signed char  > ||
					           std::is_same_v<T, char16_t     > ||
			                   std::is_same_v<T, char32_t     > ||
	                           std::is_same_v<T, wchar_t      >)
		    {
				if (!_str.empty()) {
		            r = static_cast<T>(_str[0]);
				}
				else {
					e = const_cast<char*>(_str.data());
				}
			}
			else if constexpr (std::is_same_v<T, bool>) {
		        r = _str == "true" || _str == "True" || _str == "TRUE" || _str == "T" || _str == "1";
			}
			else {
				e = const_cast<char*>(_str.data());
				
				throw std::runtime_error("No specialisation exists for parsing string to T");
			}
			
			if (e == _str.data()) {
				throw std::runtime_error("Failed to parse the input string to the specified type T");
			}
			
			return r;
		}
		
		/**
		 * @brief Calculate the signed angle between two vectors around a specified axis.
		 *
		 * This template function calculates the signed angle in radians between two vectors (_a and _b) around the specified axis (_axis).
		 *
		 * @tparam T The type of the vector components.
		 * @tparam P The vector component precision.
		 * @param[in] _a The first vector.
		 * @param[in] _b The second vector.
		 * @param[in] _axis The axis vector.
		 *
		 * @return T The signed angle in radians.
		 */
		template<typename T, glm::precision P>
		static T SignedAngle(glm::vec<3, T, P> _a, glm::vec<3, T, P> _b, glm::vec<3, T, P> _axis) {
						
		    const auto d = glm::dot  (_a, _b);
		    const auto p = glm::cross(_a, _b);
		    
		    const auto angle = std::atan(glm::length(p), d);
		    
		    return angle * glm::sign(dot(p, _axis));
		}
		
		/**
		 * @brief Converts an std::vector to an std::array of a specified size using move semantics.
		 *
		 * This function takes an std::vector and converts its elements to an std::array
		 * of a specified size. It throws an std::runtime_error if the size of the vector
		 * does not match the size of the array.
		 *
		 * @tparam T The type of elements in the vector and array.
		 * @tparam N The size of the array.
		 * @param[in,out] _vector The vector to be converted.
		 * @return The resulting array.
		 * @throws std::runtime_error if the size of the vector does not match the size of the array.
		 */
		template<typename T, size_t N>
		static std::array<T, N> MoveToArray(const std::vector<T>& _vector) {
			
		    static_assert(N > 0 && N <= std::numeric_limits<size_t>::max(), "Invalid array size.");

			if (_vector.size() != N) {
				
				Debug::Log("Vector -> Array size mismatch! (" + std::to_string(_vector.size()) + ", " + std::to_string(N) + ")");
				
				throw std::runtime_error("Vector -> Array size mismatch!");
			}
			
			std::array<T, N> result{};
			std::move(
				_vector.begin(),
				_vector.begin() + (_vector.size() > result.size() ? result.size() : _vector.size()),
			     result.begin()
			);
			
			return result;
		}
		
		/**
		 * @brief Converts an std::array to an std::vector using move semantics.
		 *
		 * This function takes an std::array of any type and size and moves its elements
		 * into an std::vector. The size of the resulting vector will be equal to the size
		 * of the original array. If the original array has fewer elements than the size
		 * of the vector, only the available elements are moved into the vector.
		 *
		 * @tparam T The type of elements in the array.
		 * @tparam N The size of the array.
		 * @param[in,out] _array The std::array whose elements are to be moved to the std::vector.
		 * @return std::vector<T> The resulting std::vector with moved elements.
		 *
		 * @note The function assumes that the array size is greater than 0 and less than
		 * or equal to the maximum value of size_t. If this condition is not met, a
		 * static_assert will be triggered.
		 */
		template<typename T, size_t N>
		static std::vector<T> MoveToVector(const std::array<T, N>& _array) {
			
		    static_assert(N > 0 && N <= std::numeric_limits<size_t>::max(), "Invalid array size.");

			std::vector<T> result(N);
			std::move(
				_array.begin(),
				_array.begin() + (_array.size() > result.size() ? result.size() : _array.size()),
				result.begin()
			);
			
			return result;
		}
		
		/**
		 * @brief Moves all elements from one vector to another.
		 *
		 * This function moves all elements from the source vector (_from) to the destination vector (_to).
		 * The elements are moved using std::make_move_iterator to take advantage of move semantics, which can
		 * be more efficient than copying elements individually.
		 * After the move, the source vector will be empty.
		 *
		 * @tparam T The type of elements in the vectors.
		 * @param[in] _from The source vector from which elements will be moved.
		 * @param[in] _to The destination vector to which elements will be moved.
		 */
		template<typename T>
		static void MoveInto(std::vector<T>& _from, std::vector<T>& _to) {
			
			_to.insert(
				_to.end(),
				std::make_move_iterator(_from.begin()),
                std::make_move_iterator(_from.end())
			);
			
			_from.erase(_from.begin(), _from.end());
		}
		
		/**
	     * Copies the elements from one vector to another.
	     *
	     * This function copies all the elements from the source vector `_from` to the destination vector `_to`.
	     * The elements are added to the end of the destination vector in the same order as they appear in the source vector.
	     *
	     * @tparam T The type of elements in the vector.
	     * @param[in] _from The source vector containing elements to be copied.
	     * @param[in,out] _to The destination vector to which the elements will be copied.
	     *
	     * @note The function does not clear the destination vector before copying. If there are existing elements in the
	     * destination vector, the copied elements are added to the end of the vector without removing the existing elements.
	     *
	     * @see std::vector
	     */
		template<typename T>
		static void CopyInto(const std::vector<T>& _from, std::vector<T>& _to) {
		
			_to.insert(
				  _to.end(),
				_from.begin(),
                _from.end()
			);
		}
		
		/**
		 * @brief Changes the handedness of a 3D vector.
		 *
		 * This function takes a 3D vector and returns a new vector with changed handedness.
		 *
		 * @tparam T The underlying type of the vector components.
		 * @tparam P The precision of the vector components.
		 * @param[in] _vec The input vector.
		 * @return The new vector with changed handedness.
		 */
		template<typename T, glm::precision P>
		static glm::vec<3, T, P> ChangeHandedness(const glm::vec<3, T, P>& _vec) {
			return { -_vec.x, _vec.z, _vec.y };
		}
		
		/**
		 * @brief Converts a 2D index to a 1D index using a given stride.
		 *
		 * This templated function converts a 2D index (_index) to a 1D index using the provided stride (_stride) value.
		 *
		 * @tparam T The type of the elements.
		 * @param[in] _index The 2-dimensional index values to be converted.
		 * @param[in] _stride The stride to use during the conversion.
		 * @return The one-dimensional representation of the index.
		 *
         * @note Only integral types are allowed for T.
		 * @see https://en.wikipedia.org/wiki/Stride_of_an_array
		 */
		template <typename T>
		static T To1D(const std::array<T, 2>& _index, const T& _stride) {
			
			static_assert(std::is_integral_v<T>, "Only integer types are allowed.");

			return (_index[1] * _stride) + _index[0];
		}
		
		/**
		* @brief Converts a 3D index to a 1D index using given strides.
		*
		* This templated function converts a 3D index (_index) to a 1D index using the provided strides (_stride).
		*
		* @tparam T The type of the elements.
		* @param[in] _index The 3-dimensional index values to be converted.
		* @param[in] _stride The stride values for each dimension.
		* @return The one-dimensional representation of the index.
		*
		* @note Only integral types are allowed for T.
		* @see https://en.wikipedia.org/wiki/Stride_of_an_array
		*/
		template <typename T>
		static T To1D(const std::array<T, 3>& _index, const std::array<T, 2>& _stride) {
			
			static_assert(std::is_integral_v<T>, "Only integer types are allowed.");

			return (_index[2] * (_stride[1] * _stride[0])) + (_index[1] * _stride[0]) + _index[0];
		}
		
		/**
		 * @brief Converts a 4D index to a 1D index using the given strides.
		 *
		 * @details This function takes a 4-dimensional index and calculates the corresponding 1-dimensional index
		 *          based on the given stride values.
		 *
		 * @tparam T The type of the elements.
		 * @param[in] _index The 4-dimensional index values to be converted.
		 * @param[in] _stride The stride values for each dimension.
		 * @return The one-dimensional representation of the index.
		 *
		 * @note Only integral types are allowed for T.
		 * @see https://en.wikipedia.org/wiki/Stride_of_an_array
		 */
		template <typename T>
		static T To1D(const std::array<T, 4>& _index, const std::array<T, 3>& _stride) {
			
			static_assert(std::is_integral_v<T>, "Only integer types are allowed.");

			return (_index[3] * (_stride[2] * _stride[1] * _stride[0])) + (_index[2] * (_stride[1] * _stride[0])) + (_index[1] * _stride[0]) + _index[0];
		}
		
		/**
		 * @brief Converts a 1D index to a 2D index using the specified stride.
		 *
		 * @details This function takes a 1-dimensional index and calculates the corresponding 2-dimensional index
		 *          based on the given stride.
		 *
		 * @tparam T The type of the elements.
		 * @param[in] _i The index to convert.
		 * @param[in] _stride The stride values for each dimension.
		 * @return std::array<T, 2> The 2D coordinates corresponding to the input value.
		 *
		 * @note Only integral types are allowed for T.
		 * @see https://en.wikipedia.org/wiki/Stride_of_an_array
		 */
		template <typename T>
		static std::array<T, 2> To2D(const T& _i, const T& _stride) {
			
			static_assert(std::is_integral_v<T>, "Only integer types are allowed.");

			return {
				_i / _stride,
				_i % _stride
			};
		}
		
		/**
		 * @brief Converts a 1D index to a 3D index using the provided strides.
		 *
		 * @details This function takes a 1-dimensional index and calculates the corresponding 3-dimensional index
		 *          based on the given strides.
		 *
		 * @tparam T The type of the elements.
		 * @param[in] _i The index to convert.
		 * @param[in] _stride The stride values for each dimension.
		 * @return std::array<T, 3> The 3D coordinates corresponding to the input value.
		 *
		 * @note Only integral types are allowed for T.
		 * @see https://en.wikipedia.org/wiki/Stride_of_an_array
		 */
		template <typename T>
		static std::array<T, 3> To3D(const T& _i, const std::array<T, 2>& _stride) {
			
			static_assert(std::is_integral_v<T>, "Only integer types are allowed.");

			const auto w1 = _stride[0] * _stride[1],
			           w2 = _i % w1;
			
			return {
				_i / w1,
				w2 / _stride[1],
				w2 % _stride[1]
			};
		}
		
		/**
		 * @brief Converts a 1D index to a 4D index using the provided strides.
		 *
		 * @details This function takes a 1-dimensional index and calculates the corresponding 4-dimensional index
		 *          based on the given strides.
		 *
		 * @tparam T The type of the elements.
		 * @param[in] _i The index to convert.
		 * @param[in] _stride The stride values for each dimension.
		 * @return std::array<T, 3> The 3D coordinates corresponding to the input value.
		 *
		 * @note Only integral types are allowed for T.
		 * @see https://en.wikipedia.org/wiki/Stride_of_an_array
		 */
		template <typename T>
		static std::array<T, 4> To4D(const T& _i, const std::array<T, 3>& _stride) {
			
			static_assert(std::is_integral_v<T>, "Only integer types are allowed.");

			const auto w1 = _stride[2] * _stride[1],
			           w2 = w1 * _stride[0],
			           w3 = _i % w2;
			
			return {
				 _i / w2,
			     w3 / w1,
			    (w3 % w1) / _stride[2],
			    (w3 % w1) % _stride[2]
			};
		}
	};
	
	template<>
	inline std::string Utils::As(const nlohmann::json& _json) {
		
		/*
		 * Marr, G., 2017. Method to get string representations of values · Issue #642 · nlohmann/json [online]. GitHub. Available from: https://github.com/nlohmann/json/issues/642 [Accessed 14 Apr 2024].
		 */
		
		std::string result;
		
		if (_json.type() == nlohmann::json::value_t::string) {
		    result = _json.get<std::string>();
		}
		else {
			result = _json.dump();
		}
	
	    return result;
	}
	
	template<>
	[[deprecated("Redundant operation: string_view to string conversion is not necessary.")]]
	inline std::optional<std::string> Utils::TryParse(const std::string_view& _str) noexcept {
		return std::string(_str);
	}
	
	template<>
	[[deprecated("Redundant operation: string_view to string_view conversion is not necessary.")]]
	inline std::optional<std::string_view> Utils::TryParse(const std::string_view& _str) noexcept {
		return std::string(_str);
	}
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_UTILS_H