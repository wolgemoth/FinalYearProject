#ifndef FINALYEARPROJECT_ATHYG_H
#define FINALYEARPROJECT_ATHYG_H

#include <array>
#include <cstddef>
#include <string>

namespace LouiEriksson::Engine::Spatial {
	
	/**
	 * @struct ATHYG
	 * @brief Contains utilities for deserialising astronomical data from the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
	 */
	struct ATHYG final {
		
		/**
		 * @struct V1
		 * @brief Utility for deserialising version 1 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct V1 final {
		
			  size_t      id;
			  std::string tyc;
			  size_t      gaia;
			  size_t      hyg;
			  size_t      hip;
			  size_t      hd;
			  size_t      hr;
			  std::string gl;
			  std::string bayer;
			  std::string flam;
			  std::string con;
			  std::string proper;
			  double      ra;
			  double      dec;
			  std::string pos_src;
			  double      dist;
			  double      x0;
			  double      y0;
			  double      z0;
			  std::string dist_src;
			  double      mag;
			  double      absmag;
			  std::string mag_src;
			  
			  explicit V1(const std::array<std::string, 23>& _values);
		};
		
		/**
		 * @struct V2
		 * @brief Utility for deserialising version 2 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct V2 final {
		
			  size_t      id;
			  std::string tyc;
			  size_t      gaia;
			  size_t      hyg;
			  size_t      hip;
			  size_t      hd;
			  size_t      hr;
			  std::string gl;
			  std::string bayer;
			  std::string flam;
			  std::string con;
			  std::string proper;
			  double      ra;
			  double      dec;
			  std::string pos_src;
			  double      dist;
			  double      x0;
			  double      y0;
			  double      z0;
			  std::string dist_src;
			  double      mag;
			  double      absmag;
			  std::string mag_src;
			  double      rv;
			  std::string rv_src;
			  double      pm_ra;
			  double      pm_dec;
			  double      pm_src;
			  double      vx;
			  double      vy;
			  double      vz;
			  double      spect;
			  std::string spect_src;
			  
			  explicit V2(const std::array<std::string, 33>& _values);
		};
		
		/**
		 * @struct V3
		 * @brief Utility for deserialising version 3 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct V3 final {
		
			  size_t      id;
			  std::string tyc;
			  size_t      gaia;
			  size_t      hyg;
			  size_t      hip;
			  size_t      hd;
			  size_t      hr;
			  std::string gl;
			  std::string bayer;
			  std::string flam;
			  std::string con;
			  std::string proper;
			  double      ra;
			  double      dec;
			  std::string pos_src;
			  double      dist;
			  double      x0;
			  double      y0;
			  double      z0;
			  std::string dist_src;
			  double      mag;
			  double      absmag;
			  double      ci;
			  std::string mag_src;
			  double      rv;
			  std::string rv_src;
			  double      pm_ra;
			  double      pm_dec;
			  double      pm_src;
			  double      vx;
			  double      vy;
			  double      vz;
			  double      spect;
			  std::string spect_src;
			  
			  explicit V3(const std::array<std::string, 34>& _values);
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ATHYG_H