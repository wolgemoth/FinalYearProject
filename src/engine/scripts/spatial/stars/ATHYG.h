#ifndef FINALYEARPROJECT_ATHYG_H
#define FINALYEARPROJECT_ATHYG_H

#include <string>
#include <array>

namespace LouiEriksson::Engine::Spatial {

	struct ATHYG {
		
		struct V1 {
		
			  unsigned long id;
			  std::string   tyc;
			  unsigned long gaia;
			  unsigned long hyg;
			  unsigned long hip;
			  unsigned long hd;
			  unsigned long hr;
			  std::string   gl;
			  std::string   bayer;
			  std::string   flam;
			  std::string   con;
			  std::string   proper;
			  double        ra;
			  double        dec;
			  std::string   pos_src;
			  double        dist;
			  double        x0;
			  double        y0;
			  double        z0;
			  std::string   dist_src;
			  double        mag;
			  double        absmag;
			  std::string   mag_src;
			  
			  explicit V1(const std::array<std::string, 23>& _values);
		};
		
		struct V2 {
		
			  unsigned long id;
			  std::string   tyc;
			  unsigned long gaia;
			  unsigned long hyg;
			  unsigned long hip;
			  unsigned long hd;
			  unsigned long hr;
			  std::string   gl;
			  std::string   bayer;
			  std::string   flam;
			  std::string   con;
			  std::string   proper;
			  double        ra;
			  double        dec;
			  std::string   pos_src;
			  double        dist;
			  double        x0;
			  double        y0;
			  double        z0;
			  std::string   dist_src;
			  double        mag;
			  double        absmag;
			  std::string   mag_src;
			  double        rv;
			  std::string   rv_src;
			  double        pm_ra;
			  double        pm_dec;
			  double        pm_src;
			  double        vx;
			  double        vy;
			  double        vz;
			  double        spect;
			  std::string   spect_src;
			  
			  explicit V2(const std::array<std::string, 33>& _values);
		};
		
		struct V3 {
		
			  unsigned long id;
			  std::string   tyc;
			  unsigned long gaia;
			  unsigned long hyg;
			  unsigned long hip;
			  unsigned long hd;
			  unsigned long hr;
			  std::string   gl;
			  std::string   bayer;
			  std::string   flam;
			  std::string   con;
			  std::string   proper;
			  double        ra;
			  double        dec;
			  std::string   pos_src;
			  double        dist;
			  double        x0;
			  double        y0;
			  double        z0;
			  std::string   dist_src;
			  double        mag;
			  double        absmag;
			  double        ci;
			  std::string   mag_src;
			  double        rv;
			  std::string   rv_src;
			  double        pm_ra;
			  double        pm_dec;
			  double        pm_src;
			  double        vx;
			  double        vy;
			  double        vz;
			  double        spect;
			  std::string   spect_src;
			  
			  explicit V3(const std::array<std::string, 34>& _values);
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ATHYG_H