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
			auto default_mesh     = Resources::GetMesh    ("sphere" );
			auto default_material = Resources::GetMaterial("Mercury");
		
			for (const auto& item : m_Positions_From.Names()) {
			
				const auto go = ECS::GameObject::Create(s, item);
				
				if (const auto transform = go->AddComponent<Transform>().lock()         ) {
				if (const auto renderer  = go->AddComponent<Graphics::Renderer>().lock()) {
				
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
		}}
		
		// Prevent the sun from casting shadows:
		std::weak_ptr<ECS::GameObject> sol;
		if (m_Planets.Get("Sol", sol)) {
			
			if (const auto go = sol.lock()                                   ) {
			if (const auto t  = go->GetComponent<Transform>().lock()         ) {
			if (const auto r  = go->GetComponent<Graphics::Renderer>().lock()) {
				r->Shadows(false);
			}}}
		}
		
		Settings::Graphics::Perspective::s_FarClip = 40000.0f;
		
		LoadStars({
			"resources/ATHYG-Database-main/data/athyg_v31-1.csv",
			"resources/ATHYG-Database-main/data/athyg_v31-2.csv"
			},
			6.5
		);
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
		auto unix_time_utc = std::chrono::duration_cast<std::chrono::microseconds>(
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
	
	void Planetarium::LoadStars(std::vector<std::filesystem::path> _athyg_paths, const double& _threshold_magnitude) {
	
		std::vector<std::string> lines;
		
		for (const auto& path : _athyg_paths) {
			
			std::cout << "Loading \"" << path.string() << "\"... " << std::flush;
			
			try {
			
				if (exists(path)) {
					
					auto data = Utils::Split(File::ReadAllText(path), '\n');
					
					std::move(data.begin(), data.end(), std::back_inserter(lines));
					
					std::cout << "Done." << std::endl;
				}
				else {
					throw std::runtime_error("Path is not valid.");
				}
			}
			catch (const std::exception& e) {
				std::cout << "Failed.\n" << e.what() << std::endl;
			}
		}
		
		std::cout << "Parsing " << (lines.size() > 0 ? lines.size() - 1 : lines.size()) << " lines... " << std::flush;
		
		std::vector<glm::vec3> star_positions;
		
		for (auto line = lines.begin() + 1; line != lines.end(); ++line) {
			
			const auto columns = Utils::Split(*line, ',');
			
			std::array<std::string, 34> dat{};
			std::move(
				columns.begin(),
				columns.begin() + (columns.size() > dat.size() ? dat.size() : columns.size()),
				    dat.begin()
			);
			
			const auto star = Engine::Spatial::ATHYG::V3(dat);
			
			if (star.mag <= _threshold_magnitude) {
				star_positions.emplace_back(star.x0, star.y0, star.z0);
			}
		}
		
		std::cout << "Done.\nSpawning " << star_positions.size() << " stars... " << std::flush;
		
		try {
			
			if (const auto p =      Parent().lock()) {
			if (const auto s = p->GetScene().lock()) {
			
				const auto go = ECS::GameObject::Create(s, "Stars");
				
				if (const auto transform = go->AddComponent<Transform>().lock()         ) {
				if (const auto renderer  = go->AddComponent<Graphics::Renderer>().lock()) {
				
					m_Stars = Engine::Graphics::Mesh::Primitives::Points::CreateInstance(star_positions);
					
					// Set point size
					glPointSize(2.0f);
					
					auto material = Resources::GetMaterial("Stars");
					
					if (material.lock()) {
						renderer->SetMesh(m_Stars);
						renderer->SetMaterial(material);
						renderer->SetTransform(transform);
					}
				}}
			}}
			
			std::cout << "Done." << std::endl;
		}
		catch (const std::exception& e) {
			std::cout << "Failed.\n" << e.what() << std::endl;
		}
	}
	
} // LouiEriksson::Game::Scripts