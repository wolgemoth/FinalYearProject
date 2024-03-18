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
				for (const auto& item : m_Positions.Names()) {
				
					auto go = ECS::GameObject::Create(s, item);
					
					const auto transform = go->AddComponent<Transform>().lock();
					const auto renderer  = go->AddComponent<Graphics::Renderer>().lock();
					renderer->SetMesh(Resources::GetMesh("sphere"));
					renderer->SetMaterial(Resources::GetMaterial("sphere"));
					renderer->SetTransform(transform);
					
					m_Planets.Add(item, go);
				}
			}
		}}
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
			 */
			const auto TAI = unix_time_utc + 37;
			
			// Convert TAI to TT using the established conversion of 32.184.
			const auto TT = TAI + 32.184;
			
			// Get the current TT in the Julian calendar.
			const auto JD = ((TT) / 86400.0) + 2440587.5;
	
			// Get the days since the J2000 epoch.
			j2000_days = JD - 2451545.0;
		}
		
		// DEBUG ONLY: Speedy time. 1s = 1d.
		j2000_days += Time::Elapsed();//Time::Elapsed() / 24.0;
		
		const auto ephemeris_time = j2000_days / 365250.0;
		
		m_Positions.Time(ephemeris_time);
		
		{
			using DISTANCE = Spatial::Maths::Conversions::Distance;
			using ROTATION = Spatial::Maths::Conversions::Rotation;
			
			// Get the number of metres in an astronomical unit, to convert to 1:1 scale.
			const double au_to_m = DISTANCE::Convert(1.0f, DISTANCE::Unit::AstronomicalUnit, DISTANCE::Unit::Metre);
			
			// Establish scale multipliers in AU:
			const auto distance_multiplier_au = (1.0 / au_to_m) * 20.0;
			const auto     size_multiplier_au = (1.0 / au_to_m) * 400.0;
			
			/*
			 * Scales of planets in solar system (in AU).
			 * Converted from information provided by NASA:
			 * https://informal.jpl.nasa.gov/museum/sites/default/files/ResourceLibrary/Planets%20to%20Scale.pdf,
			 * https://nightsky.jpl.nasa.gov/club/attachments/Fun_Facts_About_the_Moon.pdf
			 */
			const Hashmap<std::string, double> scales_au {
				{ "Sol",     0.0092982607 },
				{ "Mercury", 0.0000327545 },
				{ "Venus",   0.0000808835 },
				{ "Earth",   0.0000855627 },
				{ "Moon",    0.0000232289 },
				{ "Mars",    0.0000454552 },
				{ "Jupiter", 0.000955896  },
				{ "Saturn",  0.0008054927 },
				{ "Uranus",  0.0003415824 },
				{ "Neptune", 0.0003308871 },
			};
			
			// https://www.wikiwand.com/en/Rotation_period_(astronomy)
			// https://www.wikiwand.com/en/Axial_tilt
			const Hashmap<std::string, glm::vec<2, double, glm::highp>> rotations {
				{ "Sol",     {  7.25,   25.379995  } },
				{ "Mercury", {  0.03,   58.6462    } },
				{ "Venus",   {  2.64, -243.0226    } },
				{ "Earth",   { 23.44,    0.99726968} },
				{ "Moon",    {  6.68,    0.0       } },
				{ "Mars",    { 25.19,    1.02595675} },
				{ "Jupiter", {  3.13,    0.41007   } },
				{ "Saturn",  { 26.73,    0.4264    } },
				{ "Uranus",  { 82.23,   -0.71833   } },
				{ "Neptune", { 28.32,    0.67125   } },
			};
			
			// Get position of earth to use as a point-of-origin.
			const Planets<double, glm::highp>::Position origin{};
			
			m_Positions.TryGetPosition("Earth", origin);
			
			for (const auto& kvp : m_Positions.Positions()) {
				
				std::weak_ptr<ECS::GameObject> item;
				if (m_Planets.Get(kvp.first, item)) {
					
					if (auto go = item.lock()) {
					
					auto transform = go->GetComponent<Transform>();
					
						if (const auto t = transform.lock()) {
							
							// Adjust the position of the planet.
							t->m_Position = (kvp.second.m_Cartesian - origin.m_Cartesian) * au_to_m * distance_multiplier_au;
							
							// Adjust the size of the planet.
							double scale;
							if (scales_au.Get(kvp.first, scale)) {
								t->m_Scale = glm::vec3(au_to_m * scale * size_multiplier_au);
							}
							
							glm::vec<2, double, glm::highp> tilt_period;
							if (rotations.Get(kvp.first, tilt_period)) {
	
								glm::vec<3, double, glm::highp> rotation = {
									tilt_period.x,
									0.0,
									0.0
								};
								
								if (tilt_period.y != 0.0) {
									rotation.y = (1.0 / tilt_period.y) * (j2000_days) * 360.0 * 360.0;
								}
								
								t->m_Rotation = glm::quat(glm::radians(rotation));
							}
						}
					}
				}
			}
			
			// Set rotation of moon separately since it is tidally-locked to the earth..
			{
				std::weak_ptr<ECS::GameObject> moon, earth;
				
				if (m_Planets.Get("Moon",   moon) &&
				    m_Planets.Get("Earth", earth)) {
					
					if (const auto m =  moon.lock()) {
					if (const auto e = earth.lock()) {
						
						auto  moon_transform = m->GetComponent<Transform>();
						auto earth_transform = e->GetComponent<Transform>();
						
						if (const auto m_t =  moon_transform.lock()) {
						if (const auto e_t = earth_transform.lock()) {
							
							auto dir_to_earth = glm::normalize(e_t->m_Position - m_t->m_Position);
							
							// To correctly represent the rotation, the z-axis must be reversed.
							// See: https://stackoverflow.com/questions/72706695/how-works-glmquatlookat
							dir_to_earth.z = -dir_to_earth.z;
							
							m_t->m_Rotation = glm::quatLookAt(dir_to_earth, e_t->UP);
						}}
					}}
				}
			}
		}
	}
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P>  Planetarium::Utils::Coord::ChangeHandedness(const glm::vec<3, T, P>& _vec) {
		return glm::vec3 { -_vec.x, _vec.z, _vec.y };
	}
	
	template<typename T, glm::precision P>
	const double& Planetarium::Planets<T, P>::Time() const noexcept {
		return m_Time;
	}
	
	template<typename T, glm::precision P>
	void Planetarium::Planets<T, P>::Time(const double& _ephemeris) {
		
		const auto earthMoon = GetEarthAndMoon(_ephemeris);
		
		m_Positions.Assign("Sol"    , GetSol    (_ephemeris));
		m_Positions.Assign("Mercury", GetMercury(_ephemeris));
		m_Positions.Assign("Venus"  , GetVenus  (_ephemeris));
		m_Positions.Assign("Earth"  , earthMoon.first       );
		m_Positions.Assign("Moon"   , earthMoon.second      );
		m_Positions.Assign("Mars"   , GetMars   (_ephemeris));
		m_Positions.Assign("Jupiter", GetJupiter(_ephemeris));
		m_Positions.Assign("Saturn" , GetSaturn (_ephemeris));
		m_Positions.Assign("Uranus" , GetUranus (_ephemeris));
		m_Positions.Assign("Neptune", GetNeptune(_ephemeris));
	}
	
	template<typename T, glm::precision P>
	const bool Planetarium::Planets<T, P>::TryGetPosition(const std::string& _name, const Position& _out) const {
		return m_Positions.Get(_name, const_cast<Position&>(_out) );
	}
	
	template<typename T, glm::precision P>
	const std::vector<typename Hashmap<std::string, typename Planetarium::Planets<T, P>::Position>::KeyValuePair> Planetarium::Planets<T, P>::Positions() const {
		return m_Positions.GetAll();
	}
	
	template<typename T, glm::precision P>
	const std::vector<std::string> Planetarium::Planets<T, P>::Names() const {
		return m_Positions.Keys();
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetSol(const double& _ephemeris) {
		
		return {
			{},
			{ 0.0, 0.0, 0.0 },
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetMercury(const double& _ephemeris) {
		
		double tmp[3];
		
	    vsop87a_full::getMercury(_ephemeris, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetVenus(const double& _ephemeris) {
		
		double tmp[3];
		
	    vsop87a_full::getVenus(_ephemeris, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	std::pair<typename Planetarium::Planets<T, P>::Position, typename Planetarium::Planets<T, P>::Position> Planetarium::Planets<T, P>::GetEarthAndMoon(const double& _ephemeris) {
		
		double tmp_earth[3], tmp_emb[3], tmp_moon[3];
		
	    vsop87a_full::getEarth(_ephemeris, tmp_earth);
		vsop87a_full::getEmb  (_ephemeris, tmp_emb);
		vsop87a_full::getMoon (tmp_earth, tmp_emb, tmp_moon);
		
		return {
			{
				{},
				{ Utils::Coord::ChangeHandedness<T, P>({ tmp_earth[0], tmp_earth[1], tmp_earth[2] }) },
				{}
			},
			{
				{},
				{Utils::Coord::ChangeHandedness<T, P>({ tmp_moon[0], tmp_moon[1], tmp_moon[2] }) },
				{}
			}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetMars(const double& _ephemeris) {
		
		double tmp[3];
		
	    vsop87a_full::getMars(_ephemeris, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetJupiter(const double& _ephemeris) {
		
		double tmp[3];
		
	    vsop87a_full::getJupiter(_ephemeris, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetSaturn(const double& _ephemeris) {
		
		double tmp[3];
		
	    vsop87a_full::getSaturn(_ephemeris, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetUranus(const double& _ephemeris) {
		
		double tmp[3];
		
	    vsop87a_full::getUranus(_ephemeris, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetNeptune(const double& _ephemeris) {
		
		double tmp[3];
		
	    vsop87a_full::getNeptune(_ephemeris, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
} // LouiEriksson::Game::Scripts