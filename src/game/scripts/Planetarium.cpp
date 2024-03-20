#include "Planetarium.h"

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
				auto default_mesh     = Resources::GetMesh    ("sphere" );
				auto default_material = Resources::GetMaterial("Mercury");
			
				for (const auto& item : m_Positions.Names()) {
				
					const auto go = ECS::GameObject::Create(s, item);
					
					if (const auto transform = go->AddComponent<Transform>().lock()) {
					if (const auto renderer =  go->AddComponent<Graphics::Renderer>().lock()) {
					
						auto mesh     = Resources::GetMesh    (item, false);
						auto material = Resources::GetMaterial(item, false);
						
						if (    mesh.expired()) {     mesh = default_mesh;     }
						if (material.expired()) { material = default_material; }
						
						if (mesh.lock() && material.lock()) {
							renderer->SetMesh(mesh);
							renderer->SetMaterial(material);
							renderer->SetTransform(transform);
							
							m_Planets.Add(item, go);
						}
					}}
				}
			}
		}}
		
		// Prevent the sun from casting shadows:
		std::weak_ptr<ECS::GameObject> sol;
		if (m_Planets.Get("Sol", sol)) {
			
			if (const auto go = sol.lock()) {
			
				if (const auto t = go->GetComponent<Transform>().lock()) {
				if (const auto r = go->GetComponent<Graphics::Renderer>().lock()) {
					
					Settings::Graphics::Material::s_LightRange = 2000.0f;
					
					r->Shadows(false);
				}}
			}
		}
	}
	
	void Planetarium::Tick() {
		
		double j2000_days;
		
		{
			/*
			 * Current UNIX time from the system clock.
			 * ...Probably UTC but not guaranteed?
			 *
			 * This may not be accurate to the 'actual' UNIX time
			 * -especially if the user doesn't have an internet connection
			 * from which their system is syncing the time automatically.
			 */
			auto unix_time_utc = std::chrono::duration_cast<std::chrono::microseconds>(
					std::chrono::system_clock::now().time_since_epoch()
				).count() / 1000000.0;
			
			/*
			 * Convert from UTC to TAI using an offset of 37 seconds.
			 *
			 * This solution is imprecise and doesn't take into account
			 * historical UTC-TAI conversions.
			 *
			 * A better solution is to use a lookup table and update leap
			 * seconds with an internet connection.
			 *
			 * FTP servers for downloading leap second information (Courtesy of: https://data.iana.org/time-zones/tzdb-2019c/leapseconds):
			 * <ftp://ftp.nist.gov/pub/time/leap-seconds.list>
             * <ftp://ftp.boulder.nist.gov/pub/time/leap-seconds.list>
			 */
			const auto TAI = unix_time_utc + 37;
			
			// Convert TAI to TT using the established conversion of 32.184.
			const auto TT = TAI + 32.184;
			
			// Get the current TT in the Julian calendar.
			const auto JD = ((TT) / 86400.0) + 2440587.5;
	
			// Get the days since the J2000 epoch.
			j2000_days = JD - 2451545.0;
		}
		
		// DEBUG ONLY: Speedy time.
//		j2000_days += Time::Elapsed();
		
		const auto julian_centuries = j2000_days / 365250.0;
		
		m_Positions.Time(julian_centuries);
		
		{
			using DISTANCE = Spatial::Maths::Conversions::Distance;
			
			// Get the number of metres in an astronomical unit, to convert to 1:1 scale.
			const double au_to_m = DISTANCE::Convert(1.0f, DISTANCE::Unit::AstronomicalUnit, DISTANCE::Unit::Metre);
			
			// Establish scale multipliers in AU:
			const auto distance_multiplier_au = (1.0 / au_to_m) *  20.0;
			const auto     size_multiplier_au = (1.0 / au_to_m) * 400.0;
			
			// Get position of earth to use as a point-of-origin.
			const Spatial::VSOP<double, glm::highp>::Position origin{};
			
			m_Positions.TryGetPosition("Earth", origin);
			
			for (const auto& kvp : m_Positions.Positions()) {
				
				std::weak_ptr<ECS::GameObject> item;
				if (m_Planets.Get(kvp.first, item)) {
					
					if (const auto go = item.lock()) {
					
						if (const auto t = go->GetComponent<Transform>().lock()) {
							
							// Adjust the position of the planet.
							t->m_Position = (kvp.second.m_Cartesian - origin.m_Cartesian) * au_to_m * distance_multiplier_au;
							
							// Adjust the size of the planet.
							double scale;
							if (Planets<double, glm::highp>::s_Scales_AU.Get(kvp.first, scale)) {
								t->m_Scale = glm::vec3(au_to_m * scale * size_multiplier_au);
							}
							
							// Adjust the rotation of the planet.
							t->m_Rotation = glm::quat(glm::radians(Spatial::WGCCRE::GetOrientationVSOP87<double, glm::highp>(kvp.first, julian_centuries)));
						}
					}
				}
			}
		}
		
		// Put the light where the sun is.
		// TODO: Use a 'Light' component instead!
		const auto sol = m_Planets.Return("Sol").lock()->GetComponent<Transform>().lock();
		Settings::Graphics::Material::s_LightPosition = sol->m_Position;
	}
	
} // LouiEriksson::Game::Scripts