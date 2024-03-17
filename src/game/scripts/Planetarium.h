#ifndef FINALYEARPROJECT_PLANETARIUM_H
#define FINALYEARPROJECT_PLANETARIUM_H

#include "../include/engine_core.h"
#include "../include/engine_spatial.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/// <summary>
	/// Planetarium.
	/// </summary>
	class Planetarium final : public Script {
	
	protected:
	
		struct Utils {
			
			struct Time {
				
				static auto EphemerisToJulian(const double& _julian);
				
				static auto JulianToEphemeris(const double& _ephemeris);
				
				static auto JulianToUNIX(const double& _julian);
				
				static auto UNIXToJulian(const double& _unix);
				
				static auto TT(const double& _tai);
			};
			
			struct Coord {
				
				static auto ChangeHandedness(const glm::vec3& _vec);
			};
		};
		
		/// <summary> Transform of the FlyCam. </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary>
		/// String-indexed Hashmap holding strong pointers to planets as represented in the VSOP87 model.
		/// </summary>
		Hashmap<std::string, std::shared_ptr<ECS::GameObject>> m_Planets;
		
		/// <summary>
		/// String-indexed Hashmap holding planetary positions.
		/// </summary>
		Hashmap<std::string, glm::dvec3> m_Positions {
			{ "Sol",     {} },
			{ "Mercury", {} },
			{ "Venus",   {} },
			{ "Earth",   {} },
			{ "Moon",    {} },
			{ "Mars",    {} },
			{ "Jupiter", {} },
			{ "Saturn",  {} },
			{ "Uranus",  {} },
			{ "Neptune", {} },
		};
		
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
		
		void UpdatePlanets();
		
		glm::dvec3 GetSol(const double& _ephemeris) const;
		
		glm::dvec3 GetMercury(const double& _ephemeris) const;
		
		glm::dvec3 GetVenus(const double& _ephemeris) const;
		
		std::pair<glm::dvec3, glm::dvec3> GetEarthAndMoon(const double& _ephemeris) const;
		
		glm::dvec3 GetMars(const double& _ephemeris) const;
		
		glm::dvec3 GetJupiter(const double& _ephemeris) const;
		
		glm::dvec3 GetSaturn(const double& _ephemeris) const;
		
		glm::dvec3 GetUranus(const double& _ephemeris) const;
		
		glm::dvec3 GetNeptune(const double& _ephemeris) const;
		
	public:
	
		explicit Planetarium(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Planetarium() override;
	
		[[nodiscard]] const std::type_index TypeID() const noexcept override { return typeid(Planetarium); };
	};
	
} // LouiEriksson::Game::Scripts

#endif //FINALYEARPROJECT_PLANETARIUM_H