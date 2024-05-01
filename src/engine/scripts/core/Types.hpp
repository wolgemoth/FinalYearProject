#ifndef FINALYEARPROJECT_TYPES_HPP
#define FINALYEARPROJECT_TYPES_HPP

#include <glm/common.hpp>
#include <glm/ext.hpp>

namespace LouiEriksson::Engine {
	
	using   tick_t = long double;
	using scalar_t = float;
	
	using ivec1 = glm::vec<1, int>;
	using ivec2 = glm::vec<2, int>;
	using ivec3 = glm::vec<3, int>;
	using ivec4 = glm::vec<4, int>;
	
	using vec1 = glm::vec<1, scalar_t>;
	using vec2 = glm::vec<2, scalar_t>;
	using vec3 = glm::vec<3, scalar_t>;
	using vec4 = glm::vec<4, scalar_t>;
	
	using quat = glm::qua<scalar_t>;
	
	using mat1 = glm::mat<1, 1, scalar_t>;
	using mat2 = glm::mat<2, 2, scalar_t>;
	using mat3 = glm::mat<3, 3, scalar_t>;
	using mat4 = glm::mat<4, 4, scalar_t>;
	
}

namespace std {
	
    template<glm::length_t L, typename T, glm::qualifier Q>
	struct hash<glm::vec<L, T, Q>> {
		
        size_t operator()(glm::vec<L, T, Q> _value) const {
	       
			size_t result = 0U;
			
			hash<T> hashing_func;
			
	        for (glm::length_t i = 0; i < L; ++i) {
                result ^= hashing_func(_value[static_cast<size_t>(i)]) << i;
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