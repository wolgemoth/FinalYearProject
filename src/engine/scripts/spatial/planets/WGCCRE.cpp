#include "WGCCRE.h"

#include <cmath>

namespace LouiEriksson::Engine::Spatial {

	double WGCCRE::sin_d(const double& _x) {
		
		static constexpr auto Deg2Rad = (M_PI / 180.0);
		static constexpr auto Rad2Deg = (180.0 / M_PI);
		
		return std::sin(std::fmod(_x, 360.0) * Deg2Rad) * Rad2Deg;
	}
	
	double WGCCRE::cos_d(const double& _x) {
		
		static constexpr auto Deg2Rad = (M_PI / 180.0);
		static constexpr auto Rad2Deg = (180.0 / M_PI);
		
		return std::cos(std::fmod(_x, 360.0) * Deg2Rad) * Rad2Deg;
	}
	
} // LouiEriksson::Engine::Spatial