#include "Coords.h"

#include "Conversions.h"

#include "../../core/utils/Utils.h"

#include "glm/common.hpp"
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include <cmath>

namespace LouiEriksson::Engine::Spatial::Maths {
	
	auto Coords::WGS84::LongitudeToX(const float& _lon) {
	    return s_RMajor * Conversions::Rotation::s_DegreesToRadians * _lon;
	}
	
	auto Coords::WGS84::XToLongitude(const float& _x) {
	    return (Conversions::Rotation::s_DegreesToRadians * _x) / s_RMajor;
	}
	
	auto Coords::WGS84::LatitudeToY(float _lat) {
	
	    _lat = std::min(89.5f, std::max(_lat, -89.5f));
	
	    const auto phi    = Conversions::Rotation::s_DegreesToRadians * _lat;
	    const auto sinphi = std::sin(phi);
		
	    auto con = s_Eccent * sinphi;
	
	    con = std::pow(((1.0f - con) / (1.0f + con)), s_Com);
	
	    const auto ts = std::tan(0.5f * ((static_cast<float>(M_PI) / 2.0f) - phi)) / con;
	
	    return (0.0f - s_RMajor * std::log(ts));
	}
	
	auto Coords::WGS84::YToLatitude(const float& _y) {
	
	    const auto ts = std::exp(-_y / s_RMajor);
	    auto phi  = (M_PI / 2.0f) - 2.0f * std::atan(ts);
		
	    auto dphi = 1.0f;
	
	    int i = 0;
	    while ((std::abs(dphi) > 0.00001f) && (i < 15)) {
	
	        const auto con = s_Eccent * std::sin(phi);
	
	        dphi = (static_cast<float>(M_PI) / 2.0f) - 2.0f * std::atan(ts * std::pow((1.0f - con) / (1.0f + con), s_Com)) - phi;
	        phi += dphi;
	
	        i++;
	    }
	
	    return Conversions::Rotation::s_RadiansToDegrees * phi;
	}
	
	auto Coords::WGS84::WGS84EarthRadius(const float& _lat) {
	
	    // http://en.wikipedia.org/wiki/Earth_radius
	    const auto An = s_RMajor * s_RMajor * std::cos(_lat);
	    const auto Bn = s_RMinor * s_RMinor * std::sin(_lat);
	    const auto Ad = s_RMajor * std::cos(_lat);
	    const auto Bd = s_RMinor * std::sin(_lat);
	
	    return std::sqrt((An * An + Bn * Bn) / (Ad * Ad + Bd * Bd));
	}
	
	auto Coords::WGS84::CalculateEquatorialStretchFactor(const float& _latitude) {
	
	    const auto rad = Conversions::Rotation::s_DegreesToRadians * _latitude;
	    const auto stretch = std::sqrt(1.0f - (s_Eccent * s_Eccent) * std::sin(std::sin(rad))) * (1.0f / std::cos(rad));
	
	    return stretch;
	}
	
	glm::vec3 Coords::GPS::GPSToCartesian(const glm::vec3& _coord, const float& _lat) {
		
		return SphereToCartesian({
			_coord.x * Conversions::Rotation::s_DegreesToRadians,
			_coord.y * Conversions::Rotation::s_DegreesToRadians,
			_coord.z + WGS84::WGS84EarthRadius(_lat)
		});
	}
	
	glm::ivec2 Coords::GPS::GPSToPixel(const glm::vec2& _coord) {
		
        return {
			WGS84::LongitudeToX(_coord.y),
			WGS84:: LatitudeToY(_coord.x)
		};
	}
	
	glm::vec2 Coords::GPS::GPSToUV(const glm::vec2& _coord) {
		
		return {
			WGS84::LongitudeToX(_coord.y) / 180.0f,
			WGS84:: LatitudeToY(_coord.x) /  90.0f
		};
	}
	
	glm::vec2 Coords::GPS::GPSToUV(const glm::vec2& _coord, const glm::ivec2& _dimensions, const glm::vec4& _bounds) {

        return {
            Utils::Remap(_coord.y, _bounds.y, _bounds.w, 0.0f, 1.0f),
            Utils::Remap(_coord.x, _bounds.x, _bounds.z, 0.0f, 1.0f)
        };
	}
	
	glm::vec3 Coords::GPS::GPSToSphere(const glm::vec3& _coord) {
		
        auto phi = _coord.x * Conversions::Rotation::s_DegreesToRadians;
        auto the = _coord.y * Conversions::Rotation::s_DegreesToRadians;
        auto rho = _coord.z;

        auto cPhi = std::cos(phi);
        auto sPhi = std::sin(phi);
        auto cThe = std::cos(the);
        auto sThe = std::sin(the);
        
        return glm::vec3 {
            cPhi * cThe * rho,
            sPhi * rho,
            cPhi * sThe * rho
        };
	}
	
	glm::vec3 Coords::GPS::PixelToGPS(const glm::ivec2& _coord) {
		
        return {
			WGS84::XToLongitude(static_cast<float>(_coord.x)),
			WGS84::YToLatitude (static_cast<float>(_coord.y)),
			0.0f
		};
	}
	
	glm::vec3 Coords::GPS::PixelToGPS(const glm::vec2& _coord, const glm::ivec2& _dimensions, const glm::vec4& _bounds) {
		
        return {
            glm::mix(_bounds.x, _bounds.z, _coord.y / static_cast<float>(_dimensions.y - 1)),
            glm::mix(_bounds.y, _bounds.w, _coord.x / static_cast<float>(_dimensions.x - 1)),
			0.0f
        };
	}
	
	glm::vec3 Coords::GPS::SphereToCartesian(const glm::vec3& _coord) {
		
        auto cPhi = std::cos(_coord.x);
        auto sPhi = std::sin(_coord.x);
        auto cThe = std::cos(_coord.y);
        auto sThe = std::sin(_coord.y);
        
        return glm::vec3 {
            cPhi * cThe * _coord.z,
            cPhi * sThe * _coord.z,
            sPhi *        _coord.z
        };
	}
	
	glm::vec3 Coords::GPS::UVToGPS(const glm::vec2& _coord) {
		
		return {
			WGS84::XToLongitude(_coord.x * 180.0f),
			WGS84::YToLatitude (_coord.y *  90.0f),
			0.0f
		};
	}
	
} // LouiEriksson::Engine::Spatial::Maths