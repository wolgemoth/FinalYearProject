#ifndef FINALYEARPROJECT_ELEVATION_H
#define FINALYEARPROJECT_ELEVATION_H

#include "serialisation/ElevationDeserialiser.h"


#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>

#include <functional>
#include <future>
#include <vector>

namespace LouiEriksson::Engine::Spatial {

	class Elevation final {
	
	public:
		
        enum ElevationProvider : unsigned char {
            OpenElevation = 0u,       /**< @brief <a href="https://open-elevation.com/">OpenElevation</a> */
            OpenTopoData  = 1u << 1u  /**< @brief <a href="https://www.opentopodata.org/">OpenTopoData</a> */
        };
		
        static std::future<void> LoadElevationAsync(const glm::vec4& _bounds, const ElevationProvider& _provider, const glm::ivec2& _dimensions, const std::function<void(const std::vector<float>&)>& _callback);
	
	private:
	
        static void LoadElevation(const std::vector<glm::vec2>& _elevation_result, const glm::vec4& _bounds, const ElevationProvider& _provider, const std::function<void(const std::vector<float>&)>& _callback);
	
        static std::future<void> PostRequestOpenElevationAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OEJSON::Root&)>& _callback);

        static std::future<void> PostRequestOpenTopoDataAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OTDJSON::Root&)>& _callback);
		
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ELEVATION_H