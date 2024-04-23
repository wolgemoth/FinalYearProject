#ifndef FINALYEARPROJECT_TYPES_HPP
#define FINALYEARPROJECT_TYPES_HPP

#include <glm/common.hpp>
#include <glm/ext.hpp>

namespace LouiEriksson::Engine {
	
	using   tick_t = long double;
	using scalar_t = float;
}

namespace std {
	
    template<glm::length_t L, typename T, glm::qualifier Q>
	struct hash<glm::vec<L, T, Q>> {
		
        size_t operator()(glm::vec<L, T, Q> _value) const {
	       
			size_t result = 0U;
			
			hash<T> hashing_func;
			
	        for (size_t i = 0U; i < L; ++i) {
                result ^= hashing_func(_value[i]) << i;
	        }
			
            return result;
        }
    };
	
	template<typename T, glm::qualifier Q>
	struct hash<glm::qua<T, Q>> {
		
        size_t operator()(glm::qua<T, Q> _value) const {
			
			size_t result = 0U;
			
			hash<T> hashing_func;
			
	        for (size_t i = 0U; i < 4U; ++i) {
                result ^= hasher(_value[i]) << i;
	        }
			
            return result;
        }
    };
	
}

#endif //FINALYEARPROJECT_TYPES_HPP