#ifndef FINALYEARPROJECT_OSM_UTILS_H
#define FINALYEARPROJECT_OSM_UTILS_H

#include <glm/ext/vector_float3.hpp>

#include <string>
#include <vector>

namespace LouiEriksson::Engine::Spatial::OSM {
	
	struct Utils {
	
		static auto GetMinMaxElevation(const std::vector<glm::vec3>& _coords);
		
		static bool TryConvertToMetres(const std::string& _str, float& _output);
	};
	
} // LouiEriksson::Engine::Spatial::OSM::Utils


#endif //FINALYEARPROJECT_OSM_UTILS_H