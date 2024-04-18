#ifndef FINALYEARPROJECT_ATHYG_H
#define FINALYEARPROJECT_ATHYG_H

#include <array>
#include <string>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

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
			
			const std::optional<size_t>      id;
			const std::optional<std::string> tyc;
			const std::optional<size_t>      gaia;
			const std::optional<size_t>      hyg;
			const std::optional<size_t>      hip;
			const std::optional<size_t>      hd;
			const std::optional<size_t>      hr;
			const std::optional<std::string> gl;
			const std::optional<std::string> bayer;
			const std::optional<std::string> flam;
			const std::optional<std::string> con;
			const std::optional<std::string> proper;
			const std::optional<double>      ra;
			const std::optional<double>      dec;
			const std::optional<std::string> pos_src;
			const std::optional<double>      dist;
			const std::optional<double>      x0;
			const std::optional<double>      y0;
			const std::optional<double>      z0;
			const std::optional<std::string> dist_src;
			const std::optional<double>      mag;
			const std::optional<double>      absmag;
			const std::optional<std::string> mag_src;
			
			static constexpr auto s_ElementCount = 23;
			
			explicit V1(const std::array<std::string,      s_ElementCount>& _values) noexcept;
			explicit V1(const std::array<std::string_view, s_ElementCount>& _values) noexcept;
		};
		
		/**
		 * @struct V2
		 * @brief Utility for deserialising version 2 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct V2 final {
		
			const std::optional<size_t>      id;
			const std::optional<std::string> tyc;
			const std::optional<size_t>      gaia;
			const std::optional<size_t>      hyg;
			const std::optional<size_t>      hip;
			const std::optional<size_t>      hd;
			const std::optional<size_t>      hr;
			const std::optional<std::string> gl;
			const std::optional<std::string> bayer;
			const std::optional<std::string> flam;
			const std::optional<std::string> con;
			const std::optional<std::string> proper;
			const std::optional<double>      ra;
			const std::optional<double>      dec;
			const std::optional<std::string> pos_src;
			const std::optional<double>      dist;
			const std::optional<double>      x0;
			const std::optional<double>      y0;
			const std::optional<double>      z0;
			const std::optional<std::string> dist_src;
			const std::optional<double>      mag;
			const std::optional<double>      absmag;
			const std::optional<std::string> mag_src;
			const std::optional<double>      rv;
			const std::optional<std::string> rv_src;
			const std::optional<double>      pm_ra;
			const std::optional<double>      pm_dec;
			const std::optional<double>      pm_src;
			const std::optional<double>      vx;
			const std::optional<double>      vy;
			const std::optional<double>      vz;
			const std::optional<double>      spect;
			const std::optional<std::string> spect_src;
			
			static constexpr auto s_ElementCount = 33;
			
			explicit V2(const std::array<std::string,      s_ElementCount>& _values) noexcept;
			explicit V2(const std::array<std::string_view, s_ElementCount>& _values) noexcept;
		};
		
		/**
		 * @struct V3
		 * @brief Utility for deserialising version 3 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct V3 final {
			
			const std::optional<size_t>      id;
			const std::optional<std::string> tyc;
			const std::optional<size_t>      gaia;
			const std::optional<size_t>      hyg;
			const std::optional<size_t>      hip;
			const std::optional<size_t>      hd;
			const std::optional<size_t>      hr;
			const std::optional<std::string> gl;
			const std::optional<std::string> bayer;
			const std::optional<std::string> flam;
			const std::optional<std::string> con;
			const std::optional<std::string> proper;
			const std::optional<double>      ra;
			const std::optional<double>      dec;
			const std::optional<std::string> pos_src;
			const std::optional<double>      dist;
			const std::optional<double>      x0;
			const std::optional<double>      y0;
			const std::optional<double>      z0;
			const std::optional<std::string> dist_src;
			const std::optional<double>      mag;
			const std::optional<double>      absmag;
			const std::optional<double>      ci;
			const std::optional<std::string> mag_src;
			const std::optional<double>      rv;
			const std::optional<std::string> rv_src;
			const std::optional<double>      pm_ra;
			const std::optional<double>      pm_dec;
			const std::optional<double>      pm_src;
			const std::optional<double>      vx;
			const std::optional<double>      vy;
			const std::optional<double>      vz;
			const std::optional<double>      spect;
			const std::optional<std::string> spect_src;
			
			static constexpr auto s_ElementCount = 34;
			
			explicit V3(const std::array<std::string,      s_ElementCount>& _values) noexcept;
			explicit V3(const std::array<std::string_view, s_ElementCount>& _values) noexcept;
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ATHYG_H

#pragma clang diagnostic pop