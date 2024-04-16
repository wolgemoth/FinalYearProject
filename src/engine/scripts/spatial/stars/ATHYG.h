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
		
			  std::optional<size_t>      id;
			  std::optional<std::string> tyc;
			  std::optional<size_t>      gaia;
			  std::optional<size_t>      hyg;
			  std::optional<size_t>      hip;
			  std::optional<size_t>      hd;
			  std::optional<size_t>      hr;
			  std::optional<std::string> gl;
			  std::optional<std::string> bayer;
			  std::optional<std::string> flam;
			  std::optional<std::string> con;
			  std::optional<std::string> proper;
			  std::optional<double>      ra;
			  std::optional<double>      dec;
			  std::optional<std::string> pos_src;
			  std::optional<double>      dist;
			  std::optional<double>      x0;
			  std::optional<double>      y0;
			  std::optional<double>      z0;
			  std::optional<std::string> dist_src;
			  std::optional<double>      mag;
			  std::optional<double>      absmag;
			  std::optional<std::string> mag_src;
			  
			  explicit V1(const std::array<std::string, 23>& _values);
		};
		
		/**
		 * @struct V2
		 * @brief Utility for deserialising version 2 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct V2 final {
		
			  std::optional<size_t>      id;
			  std::optional<std::string> tyc;
			  std::optional<size_t>      gaia;
			  std::optional<size_t>      hyg;
			  std::optional<size_t>      hip;
			  std::optional<size_t>      hd;
			  std::optional<size_t>      hr;
			  std::optional<std::string> gl;
			  std::optional<std::string> bayer;
			  std::optional<std::string> flam;
			  std::optional<std::string> con;
			  std::optional<std::string> proper;
			  std::optional<double>      ra;
			  std::optional<double>      dec;
			  std::optional<std::string> pos_src;
			  std::optional<double>      dist;
			  std::optional<double>      x0;
			  std::optional<double>      y0;
			  std::optional<double>      z0;
			  std::optional<std::string> dist_src;
			  std::optional<double>      mag;
			  std::optional<double>      absmag;
			  std::optional<std::string> mag_src;
			  std::optional<double>      rv;
			  std::optional<std::string> rv_src;
			  std::optional<double>      pm_ra;
			  std::optional<double>      pm_dec;
			  std::optional<double>      pm_src;
			  std::optional<double>      vx;
			  std::optional<double>      vy;
			  std::optional<double>      vz;
			  std::optional<double>      spect;
			  std::optional<std::string> spect_src;
			  
			  explicit V2(const std::array<std::string, 33>& _values);
		};
		
		/**
		 * @struct V3
		 * @brief Utility for deserialising version 3 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct V3 final {
		
			  std::optional<size_t>      id;
			  std::optional<std::string> tyc;
			  std::optional<size_t>      gaia;
			  std::optional<size_t>      hyg;
			  std::optional<size_t>      hip;
			  std::optional<size_t>      hd;
			  std::optional<size_t>      hr;
			  std::optional<std::string> gl;
			  std::optional<std::string> bayer;
			  std::optional<std::string> flam;
			  std::optional<std::string> con;
			  std::optional<std::string> proper;
			  std::optional<double>      ra;
			  std::optional<double>      dec;
			  std::optional<std::string> pos_src;
			  std::optional<double>      dist;
			  std::optional<double>      x0;
			  std::optional<double>      y0;
			  std::optional<double>      z0;
			  std::optional<std::string> dist_src;
			  std::optional<double>      mag;
			  std::optional<double>      absmag;
			  std::optional<double>      ci;
			  std::optional<std::string> mag_src;
			  std::optional<double>      rv;
			  std::optional<std::string> rv_src;
			  std::optional<double>      pm_ra;
			  std::optional<double>      pm_dec;
			  std::optional<double>      pm_src;
			  std::optional<double>      vx;
			  std::optional<double>      vy;
			  std::optional<double>      vz;
			  std::optional<double>      spect;
			  std::optional<std::string> spect_src;
			  
			  explicit V3(const std::array<std::string, 34>& _values);
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ATHYG_H