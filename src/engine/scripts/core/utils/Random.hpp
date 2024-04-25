#include <random>

#ifndef FINALYEARPROJECT_RANDOM_HPP
#define FINALYEARPROJECT_RANDOM_HPP

namespace LouiEriksson::Engine {
	
	struct Random final {
		
		inline static std::mt19937 s_Generator;
		
		static void Init(size_t _seed) {
			s_Generator.seed(_seed);
		}
		
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
				static_assert([]{ return false; }(), "Type T must be either integral or floating point");
			}
			
			return result;
		}
	};
	
}


#endif //FINALYEARPROJECT_RANDOM_HPP