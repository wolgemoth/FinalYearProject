#ifndef FINALYEARPROJECT_ELEVATION_H
#define FINALYEARPROJECT_ELEVATION_H

#include "serialisation/ElevationDeserialiser.h"

#include "../../networking/Requests.h"

#include <cstddef>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>

#include <functional>
#include <future>
#include <vector>

namespace LouiEriksson::Engine::Spatial {

	class Elevation {
	
	public:
		
		struct Heightmap {
		
		private:
			
			std::vector<float> m_Pixels;
			
			size_t m_Width,
			       m_Height;
			
			[[nodiscard]] size_t GetIndex(const glm::ivec2& _pixel) const;
			
		public:
			
			[[nodiscard]] size_t  Width() const noexcept;
			[[nodiscard]] size_t Height() const noexcept;
			
			[[nodiscard]] float GetPixel        (const glm::ivec2& _pixel) const;
			[[nodiscard]] float GetPixelBilinear(const glm:: vec2& _uv   ) const;
			
			[[nodiscard]] const std::vector<float>& Pixels() const noexcept;
			
			Heightmap(const std::vector<float>& _pixels, const size_t& _width, const size_t& _height);
		};
		
        enum ElevationProvider : char {
            OpenElevation,
            OpenTopoData
        };
		
        static std::future<void> LoadElevationAsync(const glm::vec4& _bounds, const ElevationProvider& _provider, const glm::ivec2& _dimensions, const std::function<void(const std::vector<float>&)>& _callback);
	
	private:
	
        static void LoadElevation(const std::vector<glm::vec2>& _elevation_result, const glm::vec4& _bounds, const ElevationProvider& _provider, const std::function<void(const std::vector<float>&)>& _callback);
	
        static std::future<void> PostRequestOpenElevationAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OEJSON::Root&)>& _callback);

        static std::future<void> PostRequestOpenTopoDataAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OTDJSON::Root&)>& _callback);
		
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ELEVATION_H