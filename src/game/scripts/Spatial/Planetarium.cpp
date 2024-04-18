#include "Planetarium.h"

namespace LouiEriksson::Game::Scripts::Spatial {
	
	Planetarium::Planetarium(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {}
	Planetarium::~Planetarium() = default;
	
	void Planetarium::Begin() {
	
		if (const auto p =      Parent().lock()) {
		if (const auto s = p->GetScene().lock()) {
		
			// Get Transform.
			m_Transform = p->GetComponent<Transform>();
			
			// Create GameObjects to represent the different planets in the VSOP87 model...
			auto default_mesh     = Graphics::Mesh::Primitives::Sphere::Instance();
			auto default_material = Resources::Get<Graphics::Material>("sphere");
		
			// Prewarm collection:
			m_Planets.Reserve(m_Positions_From.Names().size());
			
			// Spawn objects for planets;
			for (const auto& item : m_Positions_From.Names()) {
			
				const auto go = ECS::GameObject::Create(s, item);
				
				if (const auto transform = go->AddComponent<Transform>().lock()         ) {
				if (const auto renderer  = go->AddComponent<Graphics::Renderer>().lock()) {
				
					auto mesh     = Resources::Get<Graphics::Mesh>    (item, false);
					auto material = Resources::Get<Graphics::Material>(item, false);
					
					if (    mesh.expired()) {     mesh = default_mesh;     }
					if (material.expired()) { material = default_material; }
					
					if (mesh.lock() && material.lock()) {
						renderer->SetMesh(mesh);
						renderer->SetMaterial(material);
						renderer->SetTransform(transform);
						
						m_Planets.Assign(item, go);
					}
				}}
			}
		}}
		
		// Prevent the sun from casting shadows:
		if (const auto sol = m_Planets.Get("Sol")) {
			
			if (const auto go = sol->lock()                                  ) {
			if (const auto t  = go->GetComponent<Transform>().lock()         ) {
			if (const auto r  = go->GetComponent<Graphics::Renderer>().lock()) {
				r->Shadows(false);
			}}}
		}
	}
	
	void Planetarium::Tick() {
		
		// Conversion from seconds to centuries.
		static constexpr auto s_to_c = 1.0 / 31557600000.0;
		
		/*
		 * Current UNIX time from the system clock.
		 * ...Probably UTC but not guaranteed?
		 *
		 * This may not be accurate to the 'actual' UNIX time
		 * -especially if the user doesn't have an internet connection
		 * from which their system is syncing the time automatically.
		 */
		const auto unix_time_utc = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::system_clock::now().time_since_epoch()
			).count() / 1000000.0;
		
		/*
		 * Convert from UTC to TAI using an offset of 37 seconds.
		 *
		 * This solution is imprecise as it doesn't take into account
		 * historical UTC-TAI conversions.
		 *
		 * A better solution is to use a lookup table and update leap
		 * seconds with an internet connection.
		 *
		 * FTP servers for downloading leap second information (Courtesy of: https://data.iana.org/time-zones/tzdb-2019c/leapseconds):
		 * <ftp://ftp.nist.gov/pub/time/leap-seconds.list>
         * <ftp://ftp.boulder.nist.gov/pub/time/leap-seconds.list>
		 */
		const auto TAI = unix_time_utc + 37.0;
		
		// Convert TAI to TT using the established conversion of 32.184.
		const auto TT = TAI + 32.184;
		
		// Get the current TT in the Julian calendar.
		const auto JD = ((TT) / 86400.0) + 2440587.5;

		// Get the days since the J2000 epoch.
		const auto j2000_days = JD - 2451545.0;
		
		// Convert from julian days to julian centuries.
		const auto curr = j2000_days / 365250.0;
		
		const auto update_interval = 120.0;
		
		if (curr > m_Positions_To.Time()) {
			m_Positions_From.Time(curr);
			  m_Positions_To.Time(curr + ((update_interval * s_to_c)));
		}
		else if (curr < m_Positions_From.Time()) {
			m_Positions_From.Time(curr - ((update_interval * s_to_c)));
			  m_Positions_To.Time(curr);
		}
		
		InterpolatePlanets(m_Positions_From, m_Positions_To, Utils::Remap(curr, m_Positions_From.Time(), m_Positions_To.Time(), 0.0, 1.0));
	}
	
} // LouiEriksson::Game::Scripts::Spatial