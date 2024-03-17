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
					
					if (item == "Moon") {
						transform->m_Scale = glm::vec3{ 0.25f };
					}
					else {
						transform->m_Scale = glm::vec3{ 1.0f };
					}
					
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
		
		// Update planetary positions using current UNIX time.
		{
			const auto unix_time = std::chrono::duration_cast<std::chrono::microseconds>(
					std::chrono::system_clock::now().time_since_epoch()
				).count() / 1000000.0;
			
			const auto DEBUG_SpeedyTime = 0.0f;// Time::Elapsed() * 1000000.0;
			
			const auto ephemeris_time = Utils::Time::JulianToEphemeris(Utils::Time::UNIXToJulian(unix_time + DEBUG_SpeedyTime));
			
			m_Positions.Time(ephemeris_time);
		}
		
		{
			using DISTANCE = Spatial::Maths::Conversions::Distance;
			
			// Get position of earth to use as a point-of-origin.
			const Planets<double, glm::highp>::Position origin{};
			m_Positions.TryGetPosition("Earth", origin);
			
			// Get the number of metres in an astronomical unit, to convert to 1:1 scale.
			const double au_to_m = DISTANCE::Convert(1.0f, DISTANCE::Unit::AstronomicalUnit, DISTANCE::Unit::Metre);
			
			// Establish scale multipliers in AU:
			const auto distance_multiplier_au = (1.0 / au_to_m) * 200.0;
			const auto     size_multiplier_au = (1.0 / au_to_m) * 200.0;
			
			/*
			 * Scales of planets in solar system (in AU).
			 * Converted from information provided by NASA:
			 * https://informal.jpl.nasa.gov/museum/sites/default/files/ResourceLibrary/Planets%20to%20Scale.pdf,
			 * https://nightsky.jpl.nasa.gov/club/attachments/Fun_Facts_About_the_Moon.pdf
			 */
			const Hashmap<std::string, double> scales {
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
			
			for (const auto& kvp : m_Positions.Positions()) {
				
				std::shared_ptr<ECS::GameObject> item;
				if (m_Planets.Get(kvp.first, item)) {
					
					auto transform = item->GetComponent<Transform>();
					
					if (const auto t = transform.lock()) {
						
						// Adjust the position of the planet.
						t->m_Position = (kvp.second.m_Cartesian - origin.m_Cartesian) * au_to_m * distance_multiplier_au;
						
						// Adjust the size of the planet.
						double scale;
						if (scales.Get(kvp.first, scale)) {
							t->m_Scale = glm::vec3(au_to_m * scale * size_multiplier_au);
						}
					}
				}
			}
		}
	}
	
	template<typename T>
	T Planetarium::Utils::Time::EphemerisToJulian(const T& _julian) {
		return (_julian / 365250.0) + 2451545.0;
	}
	
	template<typename T>
	T Planetarium::Utils::Time::JulianToEphemeris(const T& _ephemeris) {
		return (_ephemeris - 2451545.0) / 365250.0;
	}
	
	template<typename T>
	T Planetarium::Utils::Time::JulianToUNIX(const T& _julian) {
		return (_julian - 2440587.5) * 86400.0;
	}
	
	template<typename T>
	T Planetarium::Utils::Time::UNIXToJulian(const T& _unix) {
		return (_unix / 86400.0) + 2440587.5;
	}
	
	template<typename T>
	T Planetarium::Utils::Time::TT(const T& _tai) {
		return _tai + 32.184;
	}
	
	template<typename T, glm::precision P>
	auto Planetarium::Utils::Coord::ChangeHandedness(const glm::vec<3, T, P>& _vec) {
		return glm::vec3 { -_vec.x, _vec.z, _vec.y };
	}
	
	template<typename T, glm::precision P>
	const double& Planetarium::Planets<T, P>::Time() {
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