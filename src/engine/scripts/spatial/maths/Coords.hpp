#ifndef FINALYEARPROJECT_COORDS_HPP
#define FINALYEARPROJECT_COORDS_HPP

#include "../../core/Types.hpp"

#include "Conversions.hpp"

#include <glm/common.hpp>

#include <cmath>
#include <cstddef>

namespace LouiEriksson::Engine::Spatial::Maths {
	
	struct Coords final {
	
		class SI final {
	        
	        /** @brief Globally-average Earth radius in metres, according to the International System of Units. */
            static constexpr long double s_EarthRadius = 6371000.0;
			
		};
		
		/**
		 * @class WGS84
		 * @brief WGS84 Implementation, courtesy of OpenStreetMap.
		 *
		 * @see OpenStreetMap, 2024. Mercator - OpenStreetMap Wiki [online]. wiki.openstreetmap.org. Available from: https://wiki.openstreetmap.org/wiki/Mercator#Elliptical_(true)_Mercator_Projection [Accessed 17 Mar 2024].
		 */
		class WGS84 final {
		
		private:
			
			static constexpr long double s_RMajor = 6378137.0;
            static constexpr long double s_RMinor = 6356752.3142;
            static constexpr long double s_Ratio  = s_RMinor / s_RMajor;

            static constexpr long double s_Eccent = 0.081819197; //std::sqrt(1.0 - (s_Ratio * s_Ratio));
            static constexpr long double s_Com    = 0.5 * s_Eccent;

		public:
			
			template<typename T = scalar_t>
			static constexpr T LongitudeToX(const T& _lon) {
			    return s_RMajor * Conversions::Rotation::s_DegreesToRadians * _lon;
			}
			
			template<typename T = scalar_t>
			static constexpr T XToLongitude(const T& _x) {
			    return (Conversions::Rotation::s_DegreesToRadians * _x) / s_RMajor;
			}
			
			template<typename T = scalar_t>
			static constexpr T LatitudeToY(T _lat) {
			
			    _lat = std::min(static_cast<T>(89.5), std::max(_lat, static_cast<T>(-89.5)));
			
			    constexpr T phi    = Conversions::Rotation::s_DegreesToRadians * _lat;
			    constexpr T sinphi = std::sin(phi);
				
			    constexpr T con = s_Eccent * sinphi;
			
			    con = std::pow(((static_cast<T>(1.0) - con) / (static_cast<T>(1.0) + con)), s_Com);
			
			    constexpr T ts = std::tan(static_cast<T>(0.5) * ((static_cast<T>(M_PI) / static_cast<T>(2)) - phi)) / con;
			
			    return (static_cast<T>(0.0) - s_RMajor * std::log(ts));
			}
			
			template<typename T = scalar_t>
			static T YToLatitude(const T& _y) {
			
			    const T ts = std::exp(-_y / s_RMajor);
			    T phi  = (M_PI / 2.0) - 2.0 * std::atan(ts);
				
			    constexpr T dphi = 1.0;
			
			    size_t i = 0U;
			    while ((std::abs(dphi) > static_cast<T>(0.00001)) && (i < 15U)) {
			
			        const T con = s_Eccent * std::sin(phi);
			
			        dphi = (static_cast<T>(M_PI) / static_cast<T>(2)) - static_cast<T>(2.0) * std::atan(ts * std::pow((static_cast<T>(1.0) - con) / (static_cast<T>(1.0) + con), s_Com)) - phi;
			        phi += dphi;
			
			        ++i;
			    }
			
			    return Conversions::Rotation::s_RadiansToDegrees * phi;
			}
			
			// https://stackoverflow.com/questions/3269202/latitude-and-longitude-bounding-box-for-c
			
			template<typename T = scalar_t>
			static T EarthRadius(const T& _lat) {
			
			    // http://en.wikipedia.org/wiki/Earth_radius
			    const T An(s_RMajor * s_RMajor * std::cos(_lat));
			    const T Bn(s_RMinor * s_RMinor * std::sin(_lat));
			    const T Ad(s_RMajor * std::cos(_lat));
			    const T Bd(s_RMinor * std::sin(_lat));
			
			    return std::sqrt((An * An + Bn * Bn) / (Ad * Ad + Bd * Bd));
			}
			
			// Custom function:
			template<typename T = scalar_t>
			static T EquatorialStretchFactor(const T& _lat) {
			
			    const T rad = Conversions::Rotation::s_DegreesToRadians * _lat;
				
			    const T stretch = std::sqrt(static_cast<T>(1.0) - (s_Eccent * s_Eccent) * std::sin(std::sin(rad))) * (static_cast<T>(1.0) / std::cos(rad));
			
			    return stretch;
			}
			
			// Custom function:
			template<typename T = scalar_t>
			static T AltitudeCompensation(const T _lat) {
				return EarthRadius(0.0) - EarthRadius(_lat);
			}
		};
		
		struct GPS final {
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<4, T, Q> GPSToBounds(const glm::vec<3, T, Q>& _coord, const T& _sizeKm) {
		
		        glm::vec<4, T, Q> result;
		
		        if (_sizeKm != 0) {
		
		            T halfSide = static_cast<T>(500.0) * _sizeKm;
		
		            T  latitudeRadians = Conversions::Rotation::s_DegreesToRadians * (_coord.x);
		            T longitudeRadians = Conversions::Rotation::s_DegreesToRadians * (_coord.y);
		
		            T radius = WGS84::EarthRadius(_coord.x);
		            T pradius = radius * std::cos(latitudeRadians);
					
		            result = {
		                Conversions::Rotation::s_RadiansToDegrees * ( latitudeRadians - (halfSide /  radius)),
		                Conversions::Rotation::s_RadiansToDegrees * (longitudeRadians - (halfSide / pradius)),
		                Conversions::Rotation::s_RadiansToDegrees * ( latitudeRadians + (halfSide /  radius)),
		                Conversions::Rotation::s_RadiansToDegrees * (longitudeRadians + (halfSide / pradius))
		            };
		        }
		        else {
		            result = { _coord.x, _coord.y, _coord.x, _coord.y };
		        }
		
		        return result;
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<3, T, Q> GPSToCartesian(const glm::vec<3, T, Q>& _coord) {
				
				return SphereToCartesian({
					_coord.x * Conversions::Rotation::s_DegreesToRadians,
					_coord.y * Conversions::Rotation::s_DegreesToRadians,
					_coord.z + WGS84::EarthRadius(_coord.x),
				});
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<2, size_t, Q> GPSToPixel(const glm::vec<2, T, Q>& _coord) {
		        return {
					WGS84::LongitudeToX(_coord.y),
					WGS84:: LatitudeToY(_coord.x)
				};
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<2, T, Q> GPSToUV(const glm::vec<2, T, Q>& _coord) {
				
				return {
					 (WGS84::LongitudeToX(_coord.y) / static_cast<T>(180.0)),
					((WGS84:: LatitudeToY(_coord.x) / static_cast<T>( 90.0)) + static_cast<T>(1.0)) / static_cast<T>(2.0)
				};
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<2, T, Q> GPSToUV(const glm::vec<2, T, Q>& _coord, const glm::vec<4, T, Q>& _bounds) {
		
		        return {
		            Utils::Remap(_coord.y, _bounds.y, _bounds.w, static_cast<T>(1.0), static_cast<T>(0.0)),
		            Utils::Remap(_coord.x, _bounds.x, _bounds.z, static_cast<T>(0.0), static_cast<T>(1.0))
		        };
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<3, T, Q> GPSToSphere(const glm::vec<3, T, Q>& _coord) {
				
		        T phi(_coord.x * Conversions::Rotation::s_DegreesToRadians);
		        T the(_coord.y * Conversions::Rotation::s_DegreesToRadians);
		        T rho(_coord.z);
		
		        T cPhi(std::cos(phi));
		        T sPhi(std::sin(phi));
		        T cThe(std::cos(the));
		        T sThe(std::sin(the));
		        
		        return glm::vec<3, T, Q> {
		            cPhi * cThe * rho,
		            sPhi * rho,
		            cPhi * sThe * rho
		        };
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<3, T, Q> PixelToGPS(const glm::vec<2, size_t>& _coord) {
				
		        return {
					WGS84::XToLongitude(static_cast<T>(_coord.x)),
					WGS84::YToLatitude (static_cast<T>(_coord.y)),
					static_cast<T>(0.0)
				};
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<3, T, Q> PixelToGPS(const glm::vec<2, size_t>& _xy, const glm::vec<2, size_t>& _dimensions, const glm::vec<4, T, Q>& _bounds) {
				
		        return {
		            glm::mix(_bounds.x, _bounds.z, static_cast<T>(_xy.y) / static_cast<T>(_dimensions.y - 1U)),
		            glm::mix(_bounds.y, _bounds.w, static_cast<T>(_xy.x) / static_cast<T>(_dimensions.x - 1U)),
					static_cast<T>(0.0)
		        };
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<3, T, Q> SphereToCartesian(const glm::vec<3, T, Q>& _radians) {
				
		        T cPhi(std::cos(_radians.x));
		        T sPhi(std::sin(_radians.x));
		        T cThe(std::cos(_radians.y));
		        T sThe(std::sin(_radians.y));
		        
		        return glm::vec<3, T, Q> {
		            cPhi * cThe * _radians.z,
		            cPhi * sThe * _radians.z,
		            sPhi *        _radians.z
		        };
			}
			
			template<typename T = scalar_t, glm::qualifier Q = glm::defaultp>
			static constexpr glm::vec<3, T, Q> UVToGPS(const glm::vec<2, T, Q>& _uv) {
				
				return {
					WGS84::XToLongitude(_uv.x * static_cast<T>(180.0)),
					WGS84::YToLatitude (_uv.y * static_cast<T>( 90.0)),
					static_cast<T>(0.0)
				};
			}
		};
	};
	
} // LouiEriksson::Engine::Spatial::Maths

#endif //FINALYEARPROJECT_COORDS_HPP