#ifndef FINALYEARPROJECT_ATHYG_H
#define FINALYEARPROJECT_ATHYG_H

#include <string>
#include <array>

namespace LouiEriksson::Engine::Spatial {

	struct ATHYG {
		
		struct [[maybe_unused]] V1 {
		
			[[maybe_unused]] unsigned long id;
			[[maybe_unused]] std::string   tyc;
			[[maybe_unused]] unsigned long gaia;
			[[maybe_unused]] unsigned long hyg;
			[[maybe_unused]] unsigned long hip;
			[[maybe_unused]] unsigned long hd;
			[[maybe_unused]] unsigned long hr;
			[[maybe_unused]] std::string   gl;
			[[maybe_unused]] std::string   bayer;
			[[maybe_unused]] std::string   flam;
			[[maybe_unused]] std::string   con;
			[[maybe_unused]] std::string   proper;
			[[maybe_unused]] double        ra;
			[[maybe_unused]] double        dec;
			[[maybe_unused]] std::string   pos_src;
			[[maybe_unused]] double        dist;
			[[maybe_unused]] double        x0;
			[[maybe_unused]] double        y0;
			[[maybe_unused]] double        z0;
			[[maybe_unused]] std::string   dist_src;
			[[maybe_unused]] double        mag;
			[[maybe_unused]] double        absmag;
			[[maybe_unused]] std::string   mag_src;
	    
            [[maybe_unused]] explicit V1(const std::array<std::string, 23>& _values);
		};
		
		struct [[maybe_unused]] V2 {
		
			[[maybe_unused]] unsigned long id;
			[[maybe_unused]] std::string   tyc;
			[[maybe_unused]] unsigned long gaia;
			[[maybe_unused]] unsigned long hyg;
			[[maybe_unused]] unsigned long hip;
			[[maybe_unused]] unsigned long hd;
			[[maybe_unused]] unsigned long hr;
			[[maybe_unused]] std::string   gl;
			[[maybe_unused]] std::string   bayer;
			[[maybe_unused]] std::string   flam;
			[[maybe_unused]] std::string   con;
			[[maybe_unused]] std::string   proper;
			[[maybe_unused]] double        ra;
			[[maybe_unused]] double        dec;
			[[maybe_unused]] std::string   pos_src;
			[[maybe_unused]] double        dist;
			[[maybe_unused]] double        x0;
			[[maybe_unused]] double        y0;
			[[maybe_unused]] double        z0;
			[[maybe_unused]] std::string   dist_src;
			[[maybe_unused]] double        mag;
			[[maybe_unused]] double        absmag;
			[[maybe_unused]] std::string   mag_src;
			[[maybe_unused]] double        rv;
			[[maybe_unused]] std::string   rv_src;
			[[maybe_unused]] double        pm_ra;
			[[maybe_unused]] double        pm_dec;
			[[maybe_unused]] double        pm_src;
			[[maybe_unused]] double        vx;
			[[maybe_unused]] double        vy;
			[[maybe_unused]] double        vz;
			[[maybe_unused]] double        spect;
			[[maybe_unused]] std::string   spect_src;
	    
            [[maybe_unused]] explicit V2(const std::array<std::string, 33>& _values);
		};
		
		struct [[maybe_unused]] V3 {
		
			[[maybe_unused]] unsigned long id;
			[[maybe_unused]] std::string   tyc;
			[[maybe_unused]] unsigned long gaia;
			[[maybe_unused]] unsigned long hyg;
			[[maybe_unused]] unsigned long hip;
			[[maybe_unused]] unsigned long hd;
			[[maybe_unused]] unsigned long hr;
			[[maybe_unused]] std::string   gl;
			[[maybe_unused]] std::string   bayer;
			[[maybe_unused]] std::string   flam;
			[[maybe_unused]] std::string   con;
			[[maybe_unused]] std::string   proper;
			[[maybe_unused]] double        ra;
			[[maybe_unused]] double        dec;
			[[maybe_unused]] std::string   pos_src;
			[[maybe_unused]] double        dist;
			[[maybe_unused]] double        x0;
			[[maybe_unused]] double        y0;
			[[maybe_unused]] double        z0;
			[[maybe_unused]] std::string   dist_src;
			[[maybe_unused]] double        mag;
			[[maybe_unused]] double        absmag;
			[[maybe_unused]] double        ci;
			[[maybe_unused]] std::string   mag_src;
			[[maybe_unused]] double        rv;
			[[maybe_unused]] std::string   rv_src;
			[[maybe_unused]] double        pm_ra;
			[[maybe_unused]] double        pm_dec;
			[[maybe_unused]] double        pm_src;
			[[maybe_unused]] double        vx;
			[[maybe_unused]] double        vy;
			[[maybe_unused]] double        vz;
			[[maybe_unused]] double        spect;
			[[maybe_unused]] std::string   spect_src;
	    
            [[maybe_unused]] explicit V3(const std::array<std::string, 34>& _values);
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ATHYG_H