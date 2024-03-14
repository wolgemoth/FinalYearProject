#ifndef FINALYEARPROJECT_COORDS_H
#define FINALYEARPROJECT_COORDS_H

#include <glm/ext/vector_float3.hpp>

#include <cmath>

namespace LouiEriksson::Engine::Spatial::Maths {
				
	struct Coords {
	
        static glm::vec3 s_Origin;
		
		/// <summary>
        /// Globally-average Earth radius in metres.
        /// <see>https://en.wikipedia.org/wiki/Earth_radius</see>
        /// </summary>
        inline static constexpr auto s_AverageEarthRadius = 6371000.0f;

        /// <summary>
        /// Courtesy of OpenStreetMap. (See https://wiki.openstreetmap.org/wiki/Mercator#JavaScript_.28or_ActionScript.29)
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
		
        static auto GeoToGlobe(const glm::vec3& _coord, const float& _lat = 45.0f);
        
        /// <summary>
        /// Converts from latitude, longitude, and altitude to cartesian coordinates.
        /// Courtesy of "Dan S." (https://gis.stackexchange.com/questions/4147/lat-lon-alt-to-spherical-or-cartesian-coordinates)
        /// </summary>
        static auto GeoToSphere(const glm::vec3& _coord);
		
	};
	
} // LouiEriksson::Engine::Spatial::Maths

#endif //FINALYEARPROJECT_COORDS_H