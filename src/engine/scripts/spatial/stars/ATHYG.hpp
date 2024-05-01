#ifndef FINALYEARPROJECT_ATHYG_HPP
#define FINALYEARPROJECT_ATHYG_HPP

#include "../../core/utils/Utils.hpp"

#include <array>
#include <cstddef>
#include <optional>
#include <string>

namespace LouiEriksson::Engine::Spatial {
	
	/**
	 * @class ATHYG
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
			
			[[maybe_unused]] const std::optional<const size_t>      id;
			[[maybe_unused]] const std::optional<const std::string> tyc;
			[[maybe_unused]] const std::optional<const size_t>      gaia;
			[[maybe_unused]] const std::optional<const size_t>      hyg;
			[[maybe_unused]] const std::optional<const size_t>      hip;
			[[maybe_unused]] const std::optional<const size_t>      hd;
			[[maybe_unused]] const std::optional<const size_t>      hr;
			[[maybe_unused]] const std::optional<const std::string> gl;
			[[maybe_unused]] const std::optional<const std::string> bayer;
			[[maybe_unused]] const std::optional<const std::string> flam;
			[[maybe_unused]] const std::optional<const std::string> con;
			[[maybe_unused]] const std::optional<const std::string> proper;
			[[maybe_unused]] const std::optional<const double>      ra;
			[[maybe_unused]] const std::optional<const double>      dec;
			[[maybe_unused]] const std::optional<const std::string> pos_src;
			[[maybe_unused]] const std::optional<const double>      dist;
			[[maybe_unused]] const std::optional<const double>      x0;
			[[maybe_unused]] const std::optional<const double>      y0;
			[[maybe_unused]] const std::optional<const double>      z0;
			[[maybe_unused]] const std::optional<const std::string> dist_src;
			[[maybe_unused]] const std::optional<const double>      mag;
			[[maybe_unused]] const std::optional<const double>      absmag;
			[[maybe_unused]] const std::optional<const std::string> mag_src;
			
			[[maybe_unused]] static constexpr size_t s_ElementCount { 23U };
			
			template <typename T>
			explicit V1(const std::array<T, s_ElementCount>& _values) noexcept :
				id      (Utils::TryParse<size_t>(_values[ 0U])),
				tyc     (                        _values[ 1U] ),
				gaia    (Utils::TryParse<size_t>(_values[ 2U])),
				hyg     (Utils::TryParse<size_t>(_values[ 3U])),
				hip     (Utils::TryParse<size_t>(_values[ 4U])),
				hd      (Utils::TryParse<size_t>(_values[ 5U])),
				hr      (Utils::TryParse<size_t>(_values[ 6U])),
				gl      (                        _values[ 7U] ),
				bayer   (                        _values[ 8U] ),
				flam    (                        _values[ 9U] ),
				con     (                        _values[10U] ),
				proper  (                        _values[11U] ),
				ra      (Utils::TryParse<double>(_values[12U])),
				dec     (Utils::TryParse<double>(_values[13U])),
				pos_src (                        _values[14U] ),
				dist    (Utils::TryParse<double>(_values[15U])),
				x0      (Utils::TryParse<double>(_values[16U])),
				y0      (Utils::TryParse<double>(_values[17U])),
				z0      (Utils::TryParse<double>(_values[18U])),
				dist_src(                        _values[19U] ),
				mag     (Utils::TryParse<double>(_values[20U])),
				absmag  (Utils::TryParse<double>(_values[21U])),
				mag_src (                        _values[22U]) {}
		};
		
		/**
		 * @struct V2
		 * @brief Utility for deserialising version 2 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct [[maybe_unused]] V2 final {
		
			[[maybe_unused]] const std::optional<const size_t>      id;
			[[maybe_unused]] const std::optional<const std::string> tyc;
			[[maybe_unused]] const std::optional<const size_t>      gaia;
			[[maybe_unused]] const std::optional<const size_t>      hyg;
			[[maybe_unused]] const std::optional<const size_t>      hip;
			[[maybe_unused]] const std::optional<const size_t>      hd;
			[[maybe_unused]] const std::optional<const size_t>      hr;
			[[maybe_unused]] const std::optional<const std::string> gl;
			[[maybe_unused]] const std::optional<const std::string> bayer;
			[[maybe_unused]] const std::optional<const std::string> flam;
			[[maybe_unused]] const std::optional<const std::string> con;
			[[maybe_unused]] const std::optional<const std::string> proper;
			[[maybe_unused]] const std::optional<const double>      ra;
			[[maybe_unused]] const std::optional<const double>      dec;
			[[maybe_unused]] const std::optional<const std::string> pos_src;
			[[maybe_unused]] const std::optional<const double>      dist;
			[[maybe_unused]] const std::optional<const double>      x0;
			[[maybe_unused]] const std::optional<const double>      y0;
			[[maybe_unused]] const std::optional<const double>      z0;
			[[maybe_unused]] const std::optional<const std::string> dist_src;
			[[maybe_unused]] const std::optional<const double>      mag;
			[[maybe_unused]] const std::optional<const double>      absmag;
			[[maybe_unused]] const std::optional<const std::string> mag_src;
			[[maybe_unused]] const std::optional<const double>      rv;
			[[maybe_unused]] const std::optional<const std::string> rv_src;
			[[maybe_unused]] const std::optional<const double>      pm_ra;
			[[maybe_unused]] const std::optional<const double>      pm_dec;
			[[maybe_unused]] const std::optional<const double>      pm_src;
			[[maybe_unused]] const std::optional<const double>      vx;
			[[maybe_unused]] const std::optional<const double>      vy;
			[[maybe_unused]] const std::optional<const double>      vz;
			[[maybe_unused]] const std::optional<const double>      spect;
			[[maybe_unused]] const std::optional<const std::string> spect_src;
			
			[[maybe_unused]] static constexpr size_t s_ElementCount { 33U };
			
			template <typename T>
			explicit V2(const std::array<T, s_ElementCount>& _values) noexcept :
				id       (Utils::TryParse<size_t>(_values[ 0U])),
				tyc      (                        _values[ 1U] ),
				gaia     (Utils::TryParse<size_t>(_values[ 2U])),
				hyg      (Utils::TryParse<size_t>(_values[ 3U])),
				hip      (Utils::TryParse<size_t>(_values[ 4U])),
				hd       (Utils::TryParse<size_t>(_values[ 5U])),
				hr       (Utils::TryParse<size_t>(_values[ 6U])),
				gl       (                        _values[ 7U] ),
				bayer    (                        _values[ 8U] ),
				flam     (                        _values[ 9U] ),
				con      (                        _values[10U] ),
				proper   (                        _values[11U] ),
				ra       (Utils::TryParse<double>(_values[12U])),
				dec      (Utils::TryParse<double>(_values[13U])),
				pos_src  (                        _values[14U] ),
				dist     (Utils::TryParse<double>(_values[15U])),
				x0       (Utils::TryParse<double>(_values[16U])),
				y0       (Utils::TryParse<double>(_values[17U])),
				z0       (Utils::TryParse<double>(_values[18U])),
				dist_src (                        _values[19U] ),
				mag      (Utils::TryParse<double>(_values[20U])),
				absmag   (Utils::TryParse<double>(_values[21U])),
				mag_src  (                        _values[22U] ),
				rv       (Utils::TryParse<double>(_values[23U])),
				rv_src   (                        _values[24U] ),
				pm_ra    (Utils::TryParse<double>(_values[25U])),
				pm_dec   (Utils::TryParse<double>(_values[26U])),
				pm_src   (Utils::TryParse<double>(_values[27U])),
				vx       (Utils::TryParse<double>(_values[28U])),
				vy       (Utils::TryParse<double>(_values[29U])),
				vz       (Utils::TryParse<double>(_values[30U])),
				spect    (Utils::TryParse<double>(_values[31U])),
				spect_src(                        _values[32U]) {}
		};
		
		/**
		 * @struct V3
		 * @brief Utility for deserialising version 3 of the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
		 *
		 * @see <a href="https://github.com/astronexus/ATHYG-Database/blob/main/version-info.md">ATHYG version info.</a>
		 */
		struct [[maybe_unused]] V3 final {
			
			[[maybe_unused]] const std::optional<const size_t>      id;
			[[maybe_unused]] const std::optional<const std::string> tyc;
			[[maybe_unused]] const std::optional<const size_t>      gaia;
			[[maybe_unused]] const std::optional<const size_t>      hyg;
			[[maybe_unused]] const std::optional<const size_t>      hip;
			[[maybe_unused]] const std::optional<const size_t>      hd;
			[[maybe_unused]] const std::optional<const size_t>      hr;
			[[maybe_unused]] const std::optional<const std::string> gl;
			[[maybe_unused]] const std::optional<const std::string> bayer;
			[[maybe_unused]] const std::optional<const std::string> flam;
			[[maybe_unused]] const std::optional<const std::string> con;
			[[maybe_unused]] const std::optional<const std::string> proper;
			[[maybe_unused]] const std::optional<const double>      ra;
			[[maybe_unused]] const std::optional<const double>      dec;
			[[maybe_unused]] const std::optional<const std::string> pos_src;
			[[maybe_unused]] const std::optional<const double>      dist;
			[[maybe_unused]] const std::optional<const double>      x0;
			[[maybe_unused]] const std::optional<const double>      y0;
			[[maybe_unused]] const std::optional<const double>      z0;
			[[maybe_unused]] const std::optional<const std::string> dist_src;
			[[maybe_unused]] const std::optional<const double>      mag;
			[[maybe_unused]] const std::optional<const double>      absmag;
			[[maybe_unused]] const std::optional<const double>      ci;
			[[maybe_unused]] const std::optional<const std::string> mag_src;
			[[maybe_unused]] const std::optional<const double>      rv;
			[[maybe_unused]] const std::optional<const std::string> rv_src;
			[[maybe_unused]] const std::optional<const double>      pm_ra;
			[[maybe_unused]] const std::optional<const double>      pm_dec;
			[[maybe_unused]] const std::optional<const double>      pm_src;
			[[maybe_unused]] const std::optional<const double>      vx;
			[[maybe_unused]] const std::optional<const double>      vy;
			[[maybe_unused]] const std::optional<const double>      vz;
			[[maybe_unused]] const std::optional<const double>      spect;
			[[maybe_unused]] const std::optional<const std::string> spect_src;
			
			[[maybe_unused]] static constexpr size_t s_ElementCount { 34U };
				
			template <typename T>
			explicit V3(const std::array<T, s_ElementCount>& _values) noexcept :
				id       (Utils::TryParse<size_t>(_values[ 0U])),
				tyc      (                        _values[ 1U] ),
				gaia     (Utils::TryParse<size_t>(_values[ 2U])),
				hyg      (Utils::TryParse<size_t>(_values[ 3U])),
				hip      (Utils::TryParse<size_t>(_values[ 4U])),
				hd       (Utils::TryParse<size_t>(_values[ 5U])),
				hr       (Utils::TryParse<size_t>(_values[ 6U])),
				gl       (                        _values[ 7U] ),
				bayer    (                        _values[ 8U] ),
				flam     (                        _values[ 9U] ),
				con      (                        _values[10U] ),
				proper   (                        _values[11U] ),
				ra       (Utils::TryParse<double>(_values[12U])),
				dec      (Utils::TryParse<double>(_values[13U])),
				pos_src  (                        _values[14U] ),
				dist     (Utils::TryParse<double>(_values[15U])),
				x0       (Utils::TryParse<double>(_values[16U])),
				y0       (Utils::TryParse<double>(_values[17U])),
				z0       (Utils::TryParse<double>(_values[18U])),
				dist_src (                        _values[19U] ),
				mag      (Utils::TryParse<double>(_values[20U])),
				absmag   (Utils::TryParse<double>(_values[21U])),
				ci       (Utils::TryParse<double>(_values[22U])),
				mag_src  (                        _values[23U] ),
				rv       (Utils::TryParse<double>(_values[24U])),
				rv_src   (                        _values[25U] ),
				pm_ra    (Utils::TryParse<double>(_values[26U])),
				pm_dec   (Utils::TryParse<double>(_values[27U])),
				pm_src   (Utils::TryParse<double>(_values[28U])),
				vx       (Utils::TryParse<double>(_values[29U])),
				vy       (Utils::TryParse<double>(_values[30U])),
				vz       (Utils::TryParse<double>(_values[31U])),
				spect    (Utils::TryParse<double>(_values[32U])),
				spect_src(                        _values[33U] ) {}
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ATHYG_HPP