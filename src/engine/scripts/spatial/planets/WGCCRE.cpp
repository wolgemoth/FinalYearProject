#include "WGCCRE.h"

#include <cmath>

namespace LouiEriksson::Engine::Spatial {

	double WGCCRE::sin_d(const double& _x) {
		
		static constexpr auto D2R = (M_PI / 180.0);
		static constexpr auto R2D = (180.0 / M_PI);
		
		return std::sin(std::fmod(_x, 360.0) * D2R) * R2D;
	}
	
	double WGCCRE::cos_d(const double& _x) {
		
		static constexpr auto D2R = (M_PI / 180.0);
		static constexpr auto R2D = (180.0 / M_PI);
		
		return std::cos(std::fmod(_x, 360.0) * D2R) * R2D;
	}
	
} // LouiEriksson::Engine::Spatial