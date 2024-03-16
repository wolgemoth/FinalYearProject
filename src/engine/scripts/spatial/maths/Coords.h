#ifndef FINALYEARPROJECT_COORDS_H
#define FINALYEARPROJECT_COORDS_H

#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include <cmath>

namespace LouiEriksson::Engine::Spatial::Maths {
				
	struct Coords {
	
        inline static glm::vec3 s_Origin = { 0.0f, 0.0f, 0.0f };
		
		/// <summary>
        /// Globally-average Earth radius in metres, according to the International System of Units.
        /// </summary>
        inline static constexpr auto SI_EarthRadius = 6371000.0f;

        /// <summary>
        /// WGS84 Implementation, courtesy of OpenStreetMap (https://wiki.openstreetmap.org/wiki/Mercator#Elliptical_(true)_Mercator_Projection).
        /// </summary>
		class WGS84 {
		
		private:
			
			inline static constexpr auto s_RMajor = 6378137.0f;
            inline static constexpr auto s_RMinor = 6356752.3142f;
            inline static constexpr auto s_Ratio  = s_RMinor / s_RMajor;

            inline static auto s_Eccent = std::sqrt(1.0f - (s_Ratio * s_Ratio));
            inline static auto s_Com    = 0.5f * s_Eccent;

		public:
			
            static auto LongitudeToX(const float& _lon);

            static auto XToLongitude(const float& _x);

            static auto LatitudeToY(float _lat);

            static auto YToLatitude(const float& _y);

            // https://stackoverflow.com/questions/3269202/latitude-and-longitude-bounding-box-for-c
            static auto WGS84EarthRadius(const float& _lat);

            static auto CalculateEquatorialStretchFactor(const float& _latitude);
		};
		
		class GPS {
			
		public:
			
			static glm::vec3 GPSToCartesian(const glm::vec3& _coord, const float& _lat = 45.0f);
			
			static glm::ivec2 GPSToPixel(const glm::vec2& _coord);
			
			static glm::vec2 GPSToUV(const glm::vec2& _coord);
			
			static glm::vec2 GPSToUV(const glm::vec2& _coord, const glm::ivec2& _dimensions, const glm::vec4& _bounds);
	  
			static glm::vec3 GPSToSphere(const glm::vec3& _coord);
			
		    static glm::vec3 PixelToGPS(const glm::ivec2& _coord);
	     
			static glm::vec3 PixelToGPS(const glm::vec2& _coord, const glm::ivec2& _dimensions, const glm::vec4& _bounds);
			
	        static glm::vec3 SphereToCartesian(const glm::vec3& _coord);
		
		    static glm::vec3 UVToGPS(const glm::vec2& _coord);
		};
		
	};
	
} // LouiEriksson::Engine::Spatial::Maths

#endif //FINALYEARPROJECT_COORDS_H