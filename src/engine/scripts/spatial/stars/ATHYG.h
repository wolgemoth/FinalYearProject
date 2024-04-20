#ifndef FINALYEARPROJECT_ATHYG_H
#define FINALYEARPROJECT_ATHYG_H

#include <array>
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
		struct [[maybe_unused]] V1 final {
			
			[[maybe_unused]] const std::optional<size_t>      id;
			[[maybe_unused]] const std::optional<std::string> tyc;
			[[maybe_unused]] const std::optional<size_t>      gaia;
			[[maybe_unused]] const std::optional<size_t>      hyg;
			[[maybe_unused]] const std::optional<size_t>      hip;
			[[maybe_unused]] const std::optional<size_t>      hd;
			[[maybe_unused]] const std::optional<size_t>      hr;
			[[maybe_unused]] const std::optional<std::string> gl;
			[[maybe_unused]] const std::optional<std::string> bayer;
			[[maybe_unused]] const std::optional<std::string> flam;
			[[maybe_unused]] const std::optional<std::string> con;
			[[maybe_unused]] const std::optional<std::string> proper;
			[[maybe_unused]] const std::optional<double>      ra;
			[[maybe_unused]] const std::optional<double>      dec;
			[[maybe_unused]] const std::optional<std::string> pos_src;
			[[maybe_unused]] const std::optional<double>      dist;
			[[maybe_unused]] const std::optional<double>      x0;
			[[maybe_unused]] const std::optional<double>      y0;
			[[maybe_unused]] const std::optional<double>      z0;
			[[maybe_unused]] const std::optional<std::string> dist_src;
			[[maybe_unused]] const std::optional<double>      mag;
			[[maybe_unused]] const std::optional<double>      absmag;
			[[maybe_unused]] const std::optional<std::string> mag_src;
			
			[[maybe_unused]] static constexpr auto s_ElementCount = 23;
			
			explicit V1(const std::array<std::string,      s_ElementCount>& _values) noexcept;
			explicit V1(const std::array<std::string_view, s_ElementCount>& _values) noexcept;
		};
		
		/**
		 * @struct V2
		 * @brief Utility for deserialising version 2 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct [[maybe_unused]] V2 final {
		
			[[maybe_unused]] const std::optional<size_t>      id;
			[[maybe_unused]] const std::optional<std::string> tyc;
			[[maybe_unused]] const std::optional<size_t>      gaia;
			[[maybe_unused]] const std::optional<size_t>      hyg;
			[[maybe_unused]] const std::optional<size_t>      hip;
			[[maybe_unused]] const std::optional<size_t>      hd;
			[[maybe_unused]] const std::optional<size_t>      hr;
			[[maybe_unused]] const std::optional<std::string> gl;
			[[maybe_unused]] const std::optional<std::string> bayer;
			[[maybe_unused]] const std::optional<std::string> flam;
			[[maybe_unused]] const std::optional<std::string> con;
			[[maybe_unused]] const std::optional<std::string> proper;
			[[maybe_unused]] const std::optional<double>      ra;
			[[maybe_unused]] const std::optional<double>      dec;
			[[maybe_unused]] const std::optional<std::string> pos_src;
			[[maybe_unused]] const std::optional<double>      dist;
			[[maybe_unused]] const std::optional<double>      x0;
			[[maybe_unused]] const std::optional<double>      y0;
			[[maybe_unused]] const std::optional<double>      z0;
			[[maybe_unused]] const std::optional<std::string> dist_src;
			[[maybe_unused]] const std::optional<double>      mag;
			[[maybe_unused]] const std::optional<double>      absmag;
			[[maybe_unused]] const std::optional<std::string> mag_src;
			[[maybe_unused]] const std::optional<double>      rv;
			[[maybe_unused]] const std::optional<std::string> rv_src;
			[[maybe_unused]] const std::optional<double>      pm_ra;
			[[maybe_unused]] const std::optional<double>      pm_dec;
			[[maybe_unused]] const std::optional<double>      pm_src;
			[[maybe_unused]] const std::optional<double>      vx;
			[[maybe_unused]] const std::optional<double>      vy;
			[[maybe_unused]] const std::optional<double>      vz;
			[[maybe_unused]] const std::optional<double>      spect;
			[[maybe_unused]] const std::optional<std::string> spect_src;
			
			[[maybe_unused]] static constexpr auto s_ElementCount = 33;
			
			explicit V2(const std::array<std::string,      s_ElementCount>& _values) noexcept;
			explicit V2(const std::array<std::string_view, s_ElementCount>& _values) noexcept;
		};
		
		/**
		 * @struct V3
		 * @brief Utility for deserialising version 3 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct [[maybe_unused]] V3 final {
			
			[[maybe_unused]] const std::optional<size_t>      id;
			[[maybe_unused]] const std::optional<std::string> tyc;
			[[maybe_unused]] const std::optional<size_t>      gaia;
			[[maybe_unused]] const std::optional<size_t>      hyg;
			[[maybe_unused]] const std::optional<size_t>      hip;
			[[maybe_unused]] const std::optional<size_t>      hd;
			[[maybe_unused]] const std::optional<size_t>      hr;
			[[maybe_unused]] const std::optional<std::string> gl;
			[[maybe_unused]] const std::optional<std::string> bayer;
			[[maybe_unused]] const std::optional<std::string> flam;
			[[maybe_unused]] const std::optional<std::string> con;
			[[maybe_unused]] const std::optional<std::string> proper;
			[[maybe_unused]] const std::optional<double>      ra;
			[[maybe_unused]] const std::optional<double>      dec;
			[[maybe_unused]] const std::optional<std::string> pos_src;
			[[maybe_unused]] const std::optional<double>      dist;
			[[maybe_unused]] const std::optional<double>      x0;
			[[maybe_unused]] const std::optional<double>      y0;
			[[maybe_unused]] const std::optional<double>      z0;
			[[maybe_unused]] const std::optional<std::string> dist_src;
			[[maybe_unused]] const std::optional<double>      mag;
			[[maybe_unused]] const std::optional<double>      absmag;
			[[maybe_unused]] const std::optional<double>      ci;
			[[maybe_unused]] const std::optional<std::string> mag_src;
			[[maybe_unused]] const std::optional<double>      rv;
			[[maybe_unused]] const std::optional<std::string> rv_src;
			[[maybe_unused]] const std::optional<double>      pm_ra;
			[[maybe_unused]] const std::optional<double>      pm_dec;
			[[maybe_unused]] const std::optional<double>      pm_src;
			[[maybe_unused]] const std::optional<double>      vx;
			[[maybe_unused]] const std::optional<double>      vy;
			[[maybe_unused]] const std::optional<double>      vz;
			[[maybe_unused]] const std::optional<double>      spect;
			[[maybe_unused]] const std::optional<std::string> spect_src;
			
			[[maybe_unused]] static constexpr auto s_ElementCount = 34;
			
			explicit V3(const std::array<std::string,      s_ElementCount>& _values) noexcept;
			explicit V3(const std::array<std::string_view, s_ElementCount>& _values) noexcept;
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ATHYG_H