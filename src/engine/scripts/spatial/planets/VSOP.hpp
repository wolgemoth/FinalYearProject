#ifndef FINALYEARPROJECT_VSOP_HPP
#define FINALYEARPROJECT_VSOP_HPP

#include "../../core/utils/Utils.hpp"

#include <glm/detail/qualifier.hpp>
#include <vsop87a_full.h>

namespace LouiEriksson::Engine::Spatial {
	
	template <typename T, glm::precision Q>
	struct VSOP final {
	
		struct Position final {
			
			glm::vec<3, T, Q> m_Spherical;
			glm::vec<3, T, Q> m_Cartesian;
			glm::qua<   T, Q> m_Rotation;
			
			constexpr Position(const glm::vec<3, T, Q>& _spherical, const glm::vec<3, T, Q>& _cartesian, const glm::vec<3, T, Q>& _rotation) noexcept :
				m_Spherical(_spherical),
				m_Cartesian(_cartesian),
				m_Rotation (_rotation ) {}
			
			constexpr Position() noexcept :
				m_Spherical{},
				m_Cartesian{},
				m_Rotation {} {}
		};
		
		struct V87 final {
			
			struct A final {
				
				static Position GetSol() {
					return {
						{},
						{ 0.0, 0.0, 0.0 },
						{}
					};
				}
				
				static Position GetMercury(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getMercury(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
				
				static Position GetVenus(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getVenus(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
				
				static Position GetEarth(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getEarth(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
				
				static Position GetEMB(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getEmb(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
				
				static Position GetMoon(const Position& _earth, const Position& _emb) {
					
					auto e1 = Utils::ChangeHandedness<double, Q>(_earth.m_Cartesian);
					auto e2 = Utils::ChangeHandedness<double, Q>(  _emb.m_Cartesian);
					
					double tmp[3];
					
				    vsop87a_full::getMoon(&e1[0], &e2[0], tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{},
					};
				}
				
				static Position GetMars(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getMars(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
				
				static Position GetJupiter(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getJupiter(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
				
				static Position GetSaturn(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getSaturn(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
				
				static Position GetUranus(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getUranus(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
				
				static Position GetNeptune(const double& _time) {
					
					double tmp[3];
					
				    vsop87a_full::getNeptune(_time, tmp);
					
					return {
						{},
						{ Utils::ChangeHandedness<T, Q>({ tmp[0], tmp[1], tmp[2] }) },
						{}
					};
				}
			};
		};
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_VSOP_HPP