#include "Planetarium.h"

#include "glm/ext/vector_double3.hpp"

namespace LouiEriksson::Game::Scripts {
	
	Planetarium::Planetarium(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : Script(_parent) {}
	
	Planetarium::~Planetarium() = default;
	
	void Planetarium::Begin() {
	
		if (const auto p =      Parent().lock()) {
		if (const auto s = p->GetScene().lock()) {
		
			// Get Transform.
			m_Transform = p->GetComponent<Transform>();
			
			// Create GameObjects to represent the different planets in the VSOP87 model...
			{
				for (const auto& item : m_Positions.Keys()) {
					m_Planets.Add(item, ECS::GameObject::Create(s, item));
				}
				
				for (auto& kvp : m_Planets.GetAll()) {
				
					const auto transform = kvp.second->AddComponent<Transform>().lock();
					const auto renderer  = kvp.second->AddComponent<Graphics::Renderer>().lock();
					
					renderer->SetMesh(Resources::GetMesh("sphere"));
					renderer->SetMaterial(Resources::GetMaterial("sphere"));
					renderer->SetTransform(transform);
				}
			}
			
			UpdatePlanets();
		}}
	}
	
	void Planetarium::Tick() {
		UpdatePlanets();
	}
	
	auto Planetarium::Utils::Time::EphemerisToJulian(const double& _julian) {
		return (_julian / 365250.0) + 2451545.0;
	}
	
	auto  Planetarium::Utils::Time::JulianToEphemeris(const double& _ephemeris) {
		return (_ephemeris - 2451545.0) / 365250.0;
	}
	
	auto  Planetarium::Utils::Time::JulianToUNIX(const double& _julian) {
		return (_julian - 2440587.5000000) * 86400.0;
	}
	
	auto  Planetarium::Utils::Time::UNIXToJulian(const double& _unix) {
		return (_unix / 86400.0) + 2440587.5000000;
	}
	
	auto  Planetarium::Utils::Time::TT(const double& _tai) {
		return _tai + 32.184;
	}
	
	auto Planetarium::Utils::Coord::ChangeHandedness(const glm::vec3& _vec) {
		return glm::vec3 { -_vec.x, _vec.z, _vec.y };
	}
	
	void Planetarium::UpdatePlanets() {
		
		glm::dvec3 tmp;
		
		auto unix_time = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::system_clock::now().time_since_epoch()
			).count() / 1000000.0;
		
		auto ephemeris_time = Utils::Time::JulianToEphemeris(Utils::Time::UNIXToJulian(unix_time));
		
		{
			const auto earthMoon = GetEarthAndMoon(ephemeris_time);
			
			// Origin as Earth for Earth-centric planetarium.
			const auto& origin = earthMoon.first;
			
			m_Positions.Assign("Sol"    , GetSol    (ephemeris_time) - origin);
			m_Positions.Assign("Mercury", GetMercury(ephemeris_time) - origin);
			m_Positions.Assign("Venus"  , GetVenus  (ephemeris_time) - origin);
			m_Positions.Assign("Earth"  , earthMoon.first            - origin);
			m_Positions.Assign("Moon"   , earthMoon.second           - origin);
			m_Positions.Assign("Mars"   , GetMars   (ephemeris_time) - origin);
			m_Positions.Assign("Jupiter", GetJupiter(ephemeris_time) - origin);
			m_Positions.Assign("Saturn" , GetSaturn (ephemeris_time) - origin);
			m_Positions.Assign("Uranus" , GetUranus (ephemeris_time) - origin);
			m_Positions.Assign("Neptune", GetNeptune(ephemeris_time) - origin);
			
			for (const auto& kvp : m_Positions.GetAll()) {
				
				std::shared_ptr<ECS::GameObject> item;
				if (m_Planets.Get(kvp.first, item)) {
					
					auto transform = item->GetComponent<Transform>();
					
					if (const auto t = transform.lock()) {
						t->m_Position = kvp.second;
					}
				}
			}
		}
	}
	
	glm::dvec3 Planetarium::GetSol(const double& _ephemeris) const {
		return Utils::Coord::ChangeHandedness({ 0.0f, 0.0f, 0.0f });
	}
	
	glm::dvec3 Planetarium::GetMercury(const double& _ephemeris) const {
		
		glm::dvec3 result;
		
	    vsop87a_full::getMercury(_ephemeris, &result[0]);
		
		return Utils::Coord::ChangeHandedness(result);
	}
	
	glm::dvec3 Planetarium::GetVenus(const double& _ephemeris) const {
		
		glm::dvec3 result;
		
	    vsop87a_full::getVenus(_ephemeris, &result[0]);
		
		return Utils::Coord::ChangeHandedness(result);
	}
	
	std::pair<glm::dvec3, glm::dvec3> Planetarium::GetEarthAndMoon(const double& _ephemeris) const {
		
		std::pair<glm::dvec3, glm::dvec3> result;
		
		glm::dvec3 emb;
		
	    vsop87a_full::getEarth(_ephemeris, &result.first[0]);
		vsop87a_full::getEmb  (_ephemeris, &emb[0]);
		vsop87a_full::getMoon (&result.first[0], &emb[0], &result.second[0]);
		
		return { Utils::Coord::ChangeHandedness(result.first), Utils::Coord::ChangeHandedness(result.first) };
	}
	
	glm::dvec3 Planetarium::GetMars(const double& _ephemeris) const {
		
		glm::dvec3 result;
		
	    vsop87a_full::getMars(_ephemeris, &result[0]);
		
		return Utils::Coord::ChangeHandedness(result);
	}
	
	glm::dvec3 Planetarium::GetJupiter(const double& _ephemeris) const {
		
		glm::dvec3 result;
		
	    vsop87a_full::getJupiter(_ephemeris, &result[0]);
		
		return Utils::Coord::ChangeHandedness(result);
	}
	
	glm::dvec3 Planetarium::GetSaturn(const double& _ephemeris) const {
		
		glm::dvec3 result;
		
	    vsop87a_full::getSaturn(_ephemeris, &result[0]);
		
		return Utils::Coord::ChangeHandedness(result);
	}
	
	glm::dvec3 Planetarium::GetUranus(const double& _ephemeris) const {
		
		glm::dvec3 result;
		
	    vsop87a_full::getUranus(_ephemeris, &result[0]);
		
		return Utils::Coord::ChangeHandedness(result);
	}
	
	glm::dvec3 Planetarium::GetNeptune(const double& _ephemeris) const {
		
		glm::dvec3 result;
		
	    vsop87a_full::getNeptune(_ephemeris, &result[0]);
		
		return Utils::Coord::ChangeHandedness(result);
	}
	
} // LouiEriksson::Game::Scripts