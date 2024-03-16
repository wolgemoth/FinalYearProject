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
				m_Planets = {
					{ "Earth",   ECS::GameObject::Create(s, "Earth"  ) },
					{ "Jupiter", ECS::GameObject::Create(s, "Jupiter") },
					{ "Mars",    ECS::GameObject::Create(s, "Mars"   ) },
					{ "Mercury", ECS::GameObject::Create(s, "Mercury") },
					{ "Neptune", ECS::GameObject::Create(s, "Neptune") },
					{ "Saturn",  ECS::GameObject::Create(s, "Saturn" ) },
					{ "Uranus",  ECS::GameObject::Create(s, "Uranus" ) },
					{ "Venus",   ECS::GameObject::Create(s, "Venus"  ) },
				};
				
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
	
	auto Planetarium::UNIX2JD(const double& _val) {
		return (_val / 86400.0) + 2440587.5000000;
	}
	
	auto Planetarium::JD2UNIX(const double& _val) {
		return (_val - 2440587.5000000) * 86400.0;
	}
	
	auto Planetarium::ET2JD(const double& _val) {
		return (_val / 365250.0) + 2451545.0;
	}
	
	auto Planetarium::JD2ET(const double& _val) {
		return (_val - 2451545.0) / 365250.0;
	}
	
	auto Planetarium::TT(const double& _tai) {
		return _tai + 32.184;
	}
	
	auto Planetarium::Invert(const glm::vec3& _vec) {
		return glm::vec3 { -_vec.x, -_vec.y, -_vec.z };
	}
	
	void Planetarium::UpdatePlanets() {
		
		using COORDS = LouiEriksson::Engine::Spatial::Maths::Coords;
		
		glm::dvec3 coord;
		
		auto unix_time = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::system_clock::now().time_since_epoch()
			).count() / 1000000.0;
		
		auto curr_time = JD2ET(UNIX2JD(unix_time));
		
		std::cout << std::fixed << unix_time << ", " << curr_time << "\n";
		
		vsop87d_full::getEarth(curr_time, &coord[0]);
		m_Planets.Return("Earth")->GetComponent<Transform>().lock()->m_Position = COORDS::GPS::SphereToCartesian(Invert(coord));

	    vsop87d_full::getJupiter(curr_time, &coord[0]);
		m_Planets.Return("Jupiter")->GetComponent<Transform>().lock()->m_Position = COORDS::GPS::SphereToCartesian(Invert(coord));

	    vsop87d_full::getMars(curr_time, &coord[0]);
		m_Planets.Return("Mars")->GetComponent<Transform>().lock()->m_Position = COORDS::GPS::SphereToCartesian(Invert(coord));

	    vsop87d_full::getMercury(curr_time, &coord[0]);
		m_Planets.Return("Mercury")->GetComponent<Transform>().lock()->m_Position = COORDS::GPS::SphereToCartesian(Invert(coord));

	    vsop87d_full::getNeptune(curr_time, &coord[0]);
		m_Planets.Return("Neptune")->GetComponent<Transform>().lock()->m_Position = COORDS::GPS::SphereToCartesian(Invert(coord));

	    vsop87d_full::getSaturn(curr_time, &coord[0]);
		m_Planets.Return("Saturn")->GetComponent<Transform>().lock()->m_Position = COORDS::GPS::SphereToCartesian(Invert(coord));

	    vsop87d_full::getUranus(curr_time, &coord[0]);
		m_Planets.Return("Uranus")->GetComponent<Transform>().lock()->m_Position = COORDS::GPS::SphereToCartesian(Invert(coord));

	    vsop87d_full::getVenus(curr_time, &coord[0]);
		m_Planets.Return("Venus")->GetComponent<Transform>().lock()->m_Position = COORDS::GPS::SphereToCartesian(Invert(coord));
	}
	
} // LouiEriksson::Game::Scripts