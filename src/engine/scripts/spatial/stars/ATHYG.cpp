#include "ATHYG.h"

#include <string>
#include <array>

namespace LouiEriksson::Engine::Spatial {
	
	ATHYG::V1::V1(const std::array<std::string, 23>& _values) {
		
		try { id        = std::stoul(_values[ 0]); } catch(...) { id        = {}; }
		try { tyc       =            _values[ 1] ; } catch(...) { tyc       = {}; }
		try { gaia      = std::stoul(_values[ 2]); } catch(...) { gaia      = {}; }
		try { hyg       = std::stoul(_values[ 3]); } catch(...) { hyg       = {}; }
		try { hip       = std::stoul(_values[ 4]); } catch(...) { hip       = {}; }
		try { hd        = std::stoul(_values[ 5]); } catch(...) { hd        = {}; }
		try { hr        = std::stoul(_values[ 6]); } catch(...) { hr        = {}; }
		try { gl        =            _values[ 7] ; } catch(...) { gl        = {}; }
		try { bayer     =            _values[ 8] ; } catch(...) { bayer     = {}; }
		try { flam      =            _values[ 9] ; } catch(...) { flam      = {}; }
		try { con       =            _values[10] ; } catch(...) { con       = {}; }
		try { proper    =            _values[11] ; } catch(...) { proper    = {}; }
		try { ra        = std::stof(_values[12]) ; } catch(...) { ra        = {}; }
		try { dec       = std::stof(_values[13]) ; } catch(...) { dec       = {}; }
		try { pos_src   =           _values[14]  ; } catch(...) { pos_src   = {}; }
		try { dist      = std::stof(_values[15]) ; } catch(...) { dist      = {}; }
		try { x0        = std::stof(_values[16]) ; } catch(...) { x0        = {}; }
		try { y0        = std::stof(_values[17]) ; } catch(...) { y0        = {}; }
		try { z0        = std::stof(_values[18]) ; } catch(...) { z0        = {}; }
		try { dist_src  =           _values[19]  ; } catch(...) { dist_src  = {}; }
		try { mag       = std::stof(_values[20]) ; } catch(...) { mag       = {}; }
		try { absmag    = std::stof(_values[21]) ; } catch(...) { absmag    = {}; }
		try { mag_src   =           _values[22]  ; } catch(...) { mag_src   = {}; }
	}
	
	ATHYG::V2::V2(const std::array<std::string, 33>& _values) {
		
		try { id        = std::stoul(_values[ 0]); } catch(...) { id        = {}; }
		try { tyc       =            _values[ 1] ; } catch(...) { tyc       = {}; }
		try { gaia      = std::stoul(_values[ 2]); } catch(...) { gaia      = {}; }
		try { hyg       = std::stoul(_values[ 3]); } catch(...) { hyg       = {}; }
		try { hip       = std::stoul(_values[ 4]); } catch(...) { hip       = {}; }
		try { hd        = std::stoul(_values[ 5]); } catch(...) { hd        = {}; }
		try { hr        = std::stoul(_values[ 6]); } catch(...) { hr        = {}; }
		try { gl        =            _values[ 7] ; } catch(...) { gl        = {}; }
		try { bayer     =            _values[ 8] ; } catch(...) { bayer     = {}; }
		try { flam      =            _values[ 9] ; } catch(...) { flam      = {}; }
		try { con       =            _values[10] ; } catch(...) { con       = {}; }
		try { proper    =            _values[11] ; } catch(...) { proper    = {}; }
		try { ra        = std::stof(_values[12]) ; } catch(...) { ra        = {}; }
		try { dec       = std::stof(_values[13]) ; } catch(...) { dec       = {}; }
		try { pos_src   =           _values[14]  ; } catch(...) { pos_src   = {}; }
		try { dist      = std::stof(_values[15]) ; } catch(...) { dist      = {}; }
		try { x0        = std::stof(_values[16]) ; } catch(...) { x0        = {}; }
		try { y0        = std::stof(_values[17]) ; } catch(...) { y0        = {}; }
		try { z0        = std::stof(_values[18]) ; } catch(...) { z0        = {}; }
		try { dist_src  =           _values[19]  ; } catch(...) { dist_src  = {}; }
		try { mag       = std::stof(_values[20]) ; } catch(...) { mag       = {}; }
		try { absmag    = std::stof(_values[21]) ; } catch(...) { absmag    = {}; }
		try { mag_src   =           _values[22]  ; } catch(...) { mag_src   = {}; }
		try { rv        = std::stof(_values[23]) ; } catch(...) { rv        = {}; }
		try { rv_src    =           _values[24]  ; } catch(...) { rv_src    = {}; }
		try { pm_ra     = std::stof(_values[25]) ; } catch(...) { pm_ra     = {}; }
		try { pm_dec    = std::stof(_values[26]) ; } catch(...) { pm_dec    = {}; }
		try { pm_src    = std::stof(_values[27]) ; } catch(...) { pm_src    = {}; }
		try { vx        = std::stof(_values[28]) ; } catch(...) { vx        = {}; }
		try { vy        = std::stof(_values[29]) ; } catch(...) { vy        = {}; }
		try { vz        = std::stof(_values[30]) ; } catch(...) { vz        = {}; }
		try { spect     = std::stof(_values[31]) ; } catch(...) { spect     = {}; }
		try { spect_src =           _values[32]  ; } catch(...) { spect_src = {}; }
	}
	
	ATHYG::V3::V3(const std::array<std::string, 34>& _values) {

		try { id        = std::stoul(_values[ 0]); } catch(...) { id        = {}; }
		try { tyc       =            _values[ 1] ; } catch(...) { tyc       = {}; }
		try { gaia      = std::stoul(_values[ 2]); } catch(...) { gaia      = {}; }
		try { hyg       = std::stoul(_values[ 3]); } catch(...) { hyg       = {}; }
		try { hip       = std::stoul(_values[ 4]); } catch(...) { hip       = {}; }
		try { hd        = std::stoul(_values[ 5]); } catch(...) { hd        = {}; }
		try { hr        = std::stoul(_values[ 6]); } catch(...) { hr        = {}; }
		try { gl        =            _values[ 7] ; } catch(...) { gl        = {}; }
		try { bayer     =            _values[ 8] ; } catch(...) { bayer     = {}; }
		try { flam      =            _values[ 9] ; } catch(...) { flam      = {}; }
		try { con       =            _values[10] ; } catch(...) { con       = {}; }
		try { proper    =            _values[11] ; } catch(...) { proper    = {}; }
		try { ra        = std::stof(_values[12]) ; } catch(...) { ra        = {}; }
		try { dec       = std::stof(_values[13]) ; } catch(...) { dec       = {}; }
		try { pos_src   =           _values[14]  ; } catch(...) { pos_src   = {}; }
		try { dist      = std::stof(_values[15]) ; } catch(...) { dist      = {}; }
		try { x0        = std::stof(_values[16]) ; } catch(...) { x0        = {}; }
		try { y0        = std::stof(_values[17]) ; } catch(...) { y0        = {}; }
		try { z0        = std::stof(_values[18]) ; } catch(...) { z0        = {}; }
		try { dist_src  =           _values[19]  ; } catch(...) { dist_src  = {}; }
		try { mag       = std::stof(_values[20]) ; } catch(...) { mag       = {}; }
		try { absmag    = std::stof(_values[21]) ; } catch(...) { absmag    = {}; }
		try { ci        = std::stof(_values[22]) ; } catch(...) { ci        = {}; }
		try { mag_src   =           _values[23]  ; } catch(...) { mag_src   = {}; }
		try { rv        = std::stof(_values[24]) ; } catch(...) { rv        = {}; }
		try { rv_src    =           _values[25]  ; } catch(...) { rv_src    = {}; }
		try { pm_ra     = std::stof(_values[26]) ; } catch(...) { pm_ra     = {}; }
		try { pm_dec    = std::stof(_values[27]) ; } catch(...) { pm_dec    = {}; }
		try { pm_src    = std::stof(_values[28]) ; } catch(...) { pm_src    = {}; }
		try { vx        = std::stof(_values[29]) ; } catch(...) { vx        = {}; }
		try { vy        = std::stof(_values[30]) ; } catch(...) { vy        = {}; }
		try { vz        = std::stof(_values[31]) ; } catch(...) { vz        = {}; }
		try { spect     = std::stof(_values[32]) ; } catch(...) { spect     = {}; }
		try { spect_src =           _values[33]  ; } catch(...) { spect_src = {}; }
    }
	
} // LouiEriksson::Engine::Spatial