#include <random>

#ifndef FINALYEARPROJECT_RANDOM_HPP
#define FINALYEARPROJECT_RANDOM_HPP

namespace LouiEriksson::Engine {
	
	struct Random final {
		
		inline static std::mt19937 s_Generator;
		
		/**
		 * @brief Initializes the random number generator with a given seed value.
		 *
		 * This function initializes the static random number generator with the specified seed value.
		 *
		 * @param[in] _seed The seed value used to initialize the random number generator.
		 *
		 * @remark It is recommended to call this function before using any other random number generation functions in the `Random` class.
		 */
		static void Init(const size_t& _seed) {
			s_Generator.seed(_seed);
		}
		
		/**
		 * @brief Generate a random value within a specified range.
		 *
		 * This template function generates a random value within the specified range. The function supports both integral and floating-point types.
		 *
		 * @tparam T The type of the range boundaries.
		 * @param[in] _min The minimum value of the range (inclusive).
		 * @param[in] _max The maximum value of the range (inclusive).
		 * @return A random value within the specified range.
		 *
		 * @note For integral types, the range boundaries are inclusive. For floating-point types, the range boundaries are inclusive at the lower end and exclusive at the upper end.
		 *
		 * @remark The function uses a static random number generator to ensure random values are generated consistently within the same program execution. The generator can be initialized by calling the `Init()` method of the `Random` class.
		 */
		template <typename T>
		static T Range(const T& _min, const T& _max) {
			
			T result;
			
			if constexpr(std::is_integral_v<T> ) {
				std::uniform_int_distribution<T> dis(_min, _max);
				result = dis(s_Generator);
			}
			else if constexpr(std::is_floating_point_v<T> ) {
				std::uniform_real_distribution<T> dis(_min, _max);
				result = dis(s_Generator);
			}
			else {
				static_assert([]{ return false; }(), "TypeID T must be either integral or floating point");
			}
			
			return result;
		}
		
		/**
		 * @brief Generates a random vector within the given range.
		 *
		 * This function generates a random vector within the specified range.
		 *
		 * @tparam L       The number of components in the vector.
		 * @tparam T       The data type of the vector's components.
		 * @tparam Q       The qualifier of the vector (e.g., `glm::defaultp`, `glm::highp`).
		 * @param[in] _min The minimum values of the range.
		 * @param[in] _max The maximum values of the range.
		 *
		 * @return A randomly generated vector within the given range.
		 */
		template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
		static glm::vec<L, T, Q> Range(const glm::vec<L, T, Q>& _min, const glm::vec<L, T, Q>& _max) {
			
			static_assert(L != 0, "Vector must have at least 1 component!");
			
			glm::vec<L, T, Q> result;
			
			for (glm::length_t i = 0; i < L; ++i) {
				result[i] = Range(_min[i], _max[i]);
			}
			
			return result;
		}
		
		/**
		 * @brief Generates a random direction vector.
		 *
		 * @tparam L The number of components in the vector.
		 * @tparam T The data type of the vector's components.
		 * @tparam Q (optional) The qualifier of the vector.
		 * @return Resulting direction vector.
		 *
		 * @remarks The result of this function is guaranteed to be normalised.
		 */
		template <glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
		static glm::vec<L, T, Q> Direction() {
			return glm::normalize(Range(glm::vec<L, T, Q>(-1.0), glm::vec<L, T, Q>(1.0)));
		}
	};
	
}


#endif //FINALYEARPROJECT_RANDOM_HPP