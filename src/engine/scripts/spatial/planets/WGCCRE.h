#ifndef FINALYEARPROJECT_WGCCRE_H
#define FINALYEARPROJECT_WGCCRE_H

#include <glm/detail/qualifier.hpp>

#include <cstdio>
#include <string>

namespace LouiEriksson::Engine::Spatial {
	
	struct WGCCRE {
	
	private:
		
		static double sin_d(const double& _x);
		static double cos_d(const double& _x);
		
		template<typename T, glm::precision P>
		static glm::vec<3, T, P> ToVSOP87(const glm::vec<3, T, P>& _alpha_delta_W);
		
	public:
		
		template<typename T, glm::precision P>
		static glm::vec<3, T, P> GetOrientationVSOP87(const std::string& _name, const double& _t);
		
		/// <summary>
		/// https://astropedia.astrogeology.usgs.gov/download/Docs/WGCCRE/WGCCRE2015reprint.pdf
		/// </summary>
		struct Report_2015 {
		
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Sol(const double& _t);
		
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Mercury(const double& _t);
			
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Venus(const double& _t);
			
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Mars(const double& _t);
			
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Jupiter(const double& _t);
			
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Saturn(const double& _t);
			
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Uranus(const double& _t);
			
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Neptune(const double& _t);
			
		};
		
		/// <summary>
		/// https://astropedia.astrogeology.usgs.gov/download/Docs/WGCCRE/WGCCRE2009reprint.pdf
		/// </summary>
		struct Report_2009 {
			
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Earth(const double& _t);
			
			template<typename T, glm::precision P>
			static glm::vec<3, T, P> Moon(const double& _t);
		};
	};
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::ToVSOP87(const glm::vec<3, T, P>& _alpha_delta_W) {
		
		const auto ra = _alpha_delta_W[0];
		const auto de = _alpha_delta_W[1];
		
		const auto correction = _alpha_delta_W[2];
		
		// Values courtesy of stellarium: https://github.com/Stellarium/stellarium/blob/e57820ca6122fe4353d4d66dfa1104bd60e4deb5/src/core/StelCore.cpp#L59
		const auto x_offset = 90.0 - 23.4392803055555555556;
		const auto y_offset = 0.0000275;
		
		return {
			std::fmod(de + x_offset, 360.0),
			std::fmod(((ra + correction) * (360.0 / 365.0)) + y_offset, 360.0),
			0
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::GetOrientationVSOP87(const std::string& _name, const double& _t) {
		
		glm::dvec3 result;
		
		     if (_name == "Sol"    ) { result = Report_2015::Sol    <double, glm::highp>(_t); }
		else if (_name == "Mercury") { result = Report_2015::Mercury<double, glm::highp>(_t); }
		else if (_name == "Venus"  ) { result = Report_2015::Venus  <double, glm::highp>(_t); }
		else if (_name == "Earth"  ) { result = Report_2009::Earth  <double, glm::highp>(_t); }
		else if (_name == "Moon"   ) { result = Report_2009::Moon   <double, glm::highp>(_t); }
		else if (_name == "Mars"   ) { result = Report_2015::Mars   <double, glm::highp>(_t); }
		else if (_name == "Jupiter") { result = Report_2015::Jupiter<double, glm::highp>(_t); }
		else if (_name == "Saturn" ) { result = Report_2015::Saturn <double, glm::highp>(_t); }
		else if (_name == "Uranus" ) { result = Report_2015::Uranus <double, glm::highp>(_t); }
		else if (_name == "Neptune") { result = Report_2015::Neptune<double, glm::highp>(_t); }
		
		return ToVSOP87(result);
	}
	
	/* 2015 */
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2015::Sol(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		return {
			286.13,
			 63.87,
			84.176 + (14.1844000 * d)
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2015::Mercury(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		const auto M1 = 174.7910857 + ( 4.092335 * d),
		           M2 = 349.5821714 + ( 8.184670 * d),
		           M3 = 164.3732571 + (12.277005 * d),
		           M4 = 339.1643429 + (16.369340 * d),
		           M5 = 153.9554286 + (20.461675 * d);
		
		return {
			281.0103 - (0.0328 * _t),
			 61.4155 - (0.0049 * _t),
			329.5988 + (6.1385108 * d)     // (± 0.0037)
				+ 0.01067257 * sin_d(M1)
				- 0.00112309 * sin_d(M2)
				- 0.00011040 * sin_d(M3)
				- 0.00002539 * sin_d(M4)
				- 0.00000571 * sin_d(M5)
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2015::Venus(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		return {
			272.76,
			 67.16,
			160.20 - (1.4813688 * d)
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2015::Mars(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		return {
			317.269202 - (0.10927547 * _t)
				+ (0.000068 * sin_d(198.991226 + (19139.4819985 * _t)))
				+ (0.000238 * sin_d(226.292679 + (38280.8511281 * _t)))
				+ (0.000052 * sin_d(249.663391 + (57420.7251593 * _t)))
				+ (0.000009 * sin_d(266.183510 + (76560.6367950 * _t)))
				+ (0.419057 * sin_d( 79.398797 + (    0.5042615 * _t))),
			54.432516 - (0.05827105 * _t)
				+ (0.000051 * cos_d(122.433576 + (19139.9407476 * _t)))
				+ (0.000141 * cos_d( 43.058401 + (38280.8753272 * _t)))
				+ (0.000031 * cos_d( 57.663379 + (57420.7517205 * _t)))
				+ (0.000005 * cos_d( 79.476401 + (76560.6495004 * _t)))
				+ (1.591274 * cos_d(166.325722 + (    0.5042615 * _t))),
			176.049863 + (350.891982443297 * d)
				+ (0.000145 * sin_d(129.071773 + (19140.0328244 * _t)))
				+ (0.000157 * sin_d( 36.352167 + (38281.0473591 * _t)))
				+ (0.000040 * sin_d( 56.668646 + (57420.9295360 * _t)))
				+ (0.000001 * sin_d( 67.364003 + (76560.2552215 * _t)))
				+ (0.000001 * sin_d(104.792680 + (95700.4387578 * _t)))
				+ (0.584542 * sin_d( 95.391654 + (    0.5042615 * _t)))
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2015::Jupiter(const double& _t) {
	
		const auto d = _t * 365250.0;
		
		const auto Ja =  99.360714 + (4850.4046 * _t), Jb = 175.895369 + (1191.9605 * _t),
		           Jc = 300.323162 + ( 262.5475 * _t), Jd = 114.012305 + (6070.2476 * _t),
		           Je =  49.511251 + (  64.3000 * _t);
		
		return {
			268.056595 - (0.006499 * _t) + (0.000117 * sin_d(Ja)) + (0.000938 * sin_d(Jb))
				+ (0.001432 * sin_d(Jc)) + (0.000030 * sin_d(Jd)) + (0.002150 * sin_d(Je)),
			64.495303 + (0.002413 * _t)  + (0.000050 * cos_d(Ja)) + (0.000404 * cos_d(Jb))
				+ (0.000617 * cos_d(Jc)) - (0.000013 * cos_d(Jd)) + (0.000926 * cos_d(Je)),
			284.95 + (870.5360000 * d)
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2015::Saturn(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		return {
			40.589 - (0.036 * _t),
			83.537 - (0.004 * _t),
			38.90  + (810.7939024 * d)
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2015::Uranus(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		return {
			257.311,
			-15.175,
			203.81 - (501.1600928 * d)
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2015::Neptune(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		const auto N = 357.85 + (52.316 * _t);
		
		return {
			299.36  + (0.70 * sin_d(N)),
			 43.46  - (0.51 * cos_d(N)),
			249.978 + (541.1397757 * d) - (0.48 * sin_d(N)),
		};
	}
	
	/* 2009 */
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2009::Earth(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		return {
			  0.00  - (0.641 * _t),
			 90.00  - (0.557 * _t),
			190.147 + (360.9856235 * d)
		};
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P> WGCCRE::Report_2009::Moon(const double& _t) {
		
		const auto d = _t * 365250.0;
		
		const auto E1  = 125.045 - ( 0.0529921 * d), E2  = 250.089 - (0.1059842 * d), E3  = 260.008 + (13.0120009 * d),
		           E4  = 176.625 + (13.3407154 * d), E5  = 357.529 + (0.9856003 * d), E6  = 311.589 + (26.4057084 * d),
		           E7  = 134.963 + (13.0649930 * d), E8  = 276.617 + (0.3287146 * d), E9  =  34.226 + ( 1.7484877 * d),
		           E10 =  15.134 - ( 0.1589763 * d), E11 = 119.743 + (0.0036096 * d), E12 = 239.961 + ( 0.1643573 * d),
		           E13 =  25.053 + (12.9590088 * d);
		
		return {
			269.9949 + (0.0031 * _t)    - (3.8787 * sin_d(E1)) - (0.1204 * sin_d(E2))
				+ (0.0700 * sin_d(E3 )) - (0.0172 * sin_d(E4)) + (0.0072 * sin_d(E6))
				- (0.0052 * sin_d(E10)) + (0.0043 * sin_d(E13)),
				
			66.5392 + (0.0130 * _t)    + (1.5419 * cos_d(E1 )) + (0.0239 * cos_d(E2 ))
				- (0.0278 * cos_d(E3)) + (0.0068 * cos_d(E4 )) - (0.0029 * cos_d(E6 ))
				+ (0.0009 * cos_d(E7)) + (0.0008 * cos_d(E10)) - (0.0009 * cos_d(E13)),
				
			38.3213 + (13.17635815 * d) - (1.4 * std::pow(10.0, -12.0) * (d * d)) + (3.5610 * sin_d(E1))
				+ (0.1208 * sin_d(E2 )) - (0.0642 * sin_d(E3 )) + (0.0158 * sin_d(E4 ))
				+ (0.0252 * sin_d(E5 )) - (0.0066 * sin_d(E6 )) - (0.0047 * sin_d(E7 ))
				- (0.0046 * sin_d(E8 )) + (0.0028 * sin_d(E9 )) + (0.0052 * sin_d(E10))
				+ (0.0040 * sin_d(E11)) + (0.0019 * sin_d(E12)) - (0.0044 * sin_d(E13))
		};
		
	}
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_WGCCRE_H