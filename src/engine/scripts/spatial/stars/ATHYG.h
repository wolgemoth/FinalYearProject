#ifndef FINALYEARPROJECT_ATHYG_H
#define FINALYEARPROJECT_ATHYG_H

#include "../../core/utils/Utils.h"

#include <array>
#include <cstddef>
#include <string>

namespace LouiEriksson::Engine::Spatial {
	
	/**
	 * @class ATHYG
	 * @brief Contains utilities for deserialising astronomical data from the <a href="https://github.com/astronexus/ATHYG-Database/tree/main">ATHYG dataset</a>.
	 */
	class ATHYG final {
	
	public:
		
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
			
			template <typename T>
			explicit V1(const std::array<T, s_ElementCount>& _values) noexcept :
				id      (Utils::TryParse<size_t>(_values[ 0])),
				tyc     (                        _values[ 1] ),
				gaia    (Utils::TryParse<size_t>(_values[ 2])),
				hyg     (Utils::TryParse<size_t>(_values[ 3])),
				hip     (Utils::TryParse<size_t>(_values[ 4])),
				hd      (Utils::TryParse<size_t>(_values[ 5])),
				hr      (Utils::TryParse<size_t>(_values[ 6])),
				gl      (                        _values[ 7] ),
				bayer   (                        _values[ 8] ),
				flam    (                        _values[ 9] ),
				con     (                        _values[10] ),
				proper  (                        _values[11] ),
				ra      (Utils::TryParse<double>(_values[12])),
				dec     (Utils::TryParse<double>(_values[13])),
				pos_src (                        _values[14] ),
				dist    (Utils::TryParse<double>(_values[15])),
				x0      (Utils::TryParse<double>(_values[16])),
				y0      (Utils::TryParse<double>(_values[17])),
				z0      (Utils::TryParse<double>(_values[18])),
				dist_src(                        _values[19] ),
				mag     (Utils::TryParse<double>(_values[20])),
				absmag  (Utils::TryParse<double>(_values[21])),
				mag_src (                        _values[22]) {}
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
			
			template <typename T>
			explicit V2(const std::array<T, s_ElementCount>& _values) noexcept :
				id       (Utils::TryParse<size_t>(_values[ 0])),
				tyc      (                        _values[ 1] ),
				gaia     (Utils::TryParse<size_t>(_values[ 2])),
				hyg      (Utils::TryParse<size_t>(_values[ 3])),
				hip      (Utils::TryParse<size_t>(_values[ 4])),
				hd       (Utils::TryParse<size_t>(_values[ 5])),
				hr       (Utils::TryParse<size_t>(_values[ 6])),
				gl       (                        _values[ 7] ),
				bayer    (                        _values[ 8] ),
				flam     (                        _values[ 9] ),
				con      (                        _values[10] ),
				proper   (                        _values[11] ),
				ra       (Utils::TryParse<double>(_values[12])),
				dec      (Utils::TryParse<double>(_values[13])),
				pos_src  (                         _values[14]),
				dist     (Utils::TryParse<double>(_values[15])),
				x0       (Utils::TryParse<double>(_values[16])),
				y0       (Utils::TryParse<double>(_values[17])),
				z0       (Utils::TryParse<double>(_values[18])),
				dist_src (                        _values[19] ),
				mag      (Utils::TryParse<double>(_values[20])),
				absmag   (Utils::TryParse<double>(_values[21])),
				mag_src  (                        _values[22] ),
				rv       (Utils::TryParse<double>(_values[23])),
				rv_src   (                        _values[24] ),
				pm_ra    (Utils::TryParse<double>(_values[25])),
				pm_dec   (Utils::TryParse<double>(_values[26])),
				pm_src   (Utils::TryParse<double>(_values[27])),
				vx       (Utils::TryParse<double>(_values[28])),
				vy       (Utils::TryParse<double>(_values[29])),
				vz       (Utils::TryParse<double>(_values[30])),
				spect    (Utils::TryParse<double>(_values[31])),
				spect_src(                        _values[32]) {}
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
				
			template <typename T>
			explicit V3(const std::array<T, s_ElementCount>& _values) noexcept :
				id       (Utils::TryParse<size_t>(_values[ 0])),
				tyc      (                        _values[ 1] ),
				gaia     (Utils::TryParse<size_t>(_values[ 2])),
				hyg      (Utils::TryParse<size_t>(_values[ 3])),
				hip      (Utils::TryParse<size_t>(_values[ 4])),
				hd       (Utils::TryParse<size_t>(_values[ 5])),
				hr       (Utils::TryParse<size_t>(_values[ 6])),
				gl       (                        _values[ 7] ),
				bayer    (                        _values[ 8] ),
				flam     (                        _values[ 9] ),
				con      (                        _values[10] ),
				proper   (                        _values[11] ),
				ra       (Utils::TryParse<double>(_values[12])),
				dec      (Utils::TryParse<double>(_values[13])),
				pos_src  (                        _values[14] ),
				dist     (Utils::TryParse<double>(_values[15])),
				x0       (Utils::TryParse<double>(_values[16])),
				y0       (Utils::TryParse<double>(_values[17])),
				z0       (Utils::TryParse<double>(_values[18])),
				dist_src (                        _values[19] ),
				mag      (Utils::TryParse<double>(_values[20])),
				absmag   (Utils::TryParse<double>(_values[21])),
				ci       (Utils::TryParse<double>(_values[22])),
				mag_src  (                        _values[23] ),
				rv       (Utils::TryParse<double>(_values[24])),
				rv_src   (                        _values[25] ),
				pm_ra    (Utils::TryParse<double>(_values[26])),
				pm_dec   (Utils::TryParse<double>(_values[27])),
				pm_src   (Utils::TryParse<double>(_values[28])),
				vx       (Utils::TryParse<double>(_values[29])),
				vy       (Utils::TryParse<double>(_values[30])),
				vz       (Utils::TryParse<double>(_values[31])),
				spect    (Utils::TryParse<double>(_values[32])),
				spect_src(                        _values[33] ) {}
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ATHYG_H