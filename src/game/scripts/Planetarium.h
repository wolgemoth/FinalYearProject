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
			
			struct Coord {
				
				template<typename T, glm::precision P>
				static glm::vec<3, T, P> ChangeHandedness(const glm::vec<3, T, P>& _vec);
				
				template<typename T, glm::precision P>
				static double SignedAngle(glm::vec<3, T, P> _a, glm::vec<3, T, P> _b, glm::vec<3, T, P> _axis);
			};
		};
		
		template<typename T, glm::precision P>
		class Planets {
		
		public:
			
			struct Position {
				
				glm::vec<3, T, P> m_Spherical;
				glm::vec<3, T, P> m_Cartesian;
				glm::qua<   T, P> m_Rotation;
			};
			
			/// </summary>
			/// Scales of planets in solar system (in AU).
			/// Converted from information provided by NASA:
			/// https://informal.jpl.nasa.gov/museum/sites/default/files/ResourceLibrary/Planets%20to%20Scale.pdf,
			/// https://nightsky.jpl.nasa.gov/club/attachments/Fun_Facts_About_the_Moon.pdf
			/// </summary>
			inline static const Hashmap<std::string, T> s_Scales_AU {
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
			
			void Time(const double& _time);
			
			[[nodiscard]] const double& Time() const noexcept;
			
			[[nodiscard]] const bool TryGetPosition(const std::string& _name, const Position& _out) const;
			
			[[nodiscard]] const std::vector<typename Hashmap<std::string, Position>::KeyValuePair> Positions() const;
			
			[[nodiscard]] const std::vector<std::string> Names() const;
			
		private:
			
			double m_Time;
			
			/// <summary>
			/// String-indexed Hashmap holding planetary positions.
			/// </summary>
			Hashmap<std::string, Position> m_Positions {
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
			
			static Position GetSol(const double& _time);
			
			static Position GetMercury(const double& _time);
			
			static Position GetVenus(const double& _time);
			
			std::pair<Position, Position> GetEarthAndMoon(const double& _time);
			
			static Position GetMars(const double& _time);
			
			static Position GetJupiter(const double& _time);
			
			static Position GetSaturn(const double& _time);
			
			static Position GetUranus(const double& _time);
			
			static Position GetNeptune(const double& _time);
		};
		
		/// <summary> Transform of the FlyCam. </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/// <summary>
		/// Planetary positions computed using the VSOP87 model.
		/// </summary>
		Planets<double, glm::highp> m_Positions;
		
		/// <summary>
		/// Hashmap containing planet GameObjects.
		/// </summary>
		Hashmap<std::string, std::weak_ptr<ECS::GameObject>> m_Planets;
		
		/// <inheritdoc/>
		void Begin() override;
	
		/// <inheritdoc/>
		void Tick() override;
		
	public:
	
		explicit Planetarium(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Planetarium() override;
	
		[[nodiscard]] const std::type_index TypeID() const noexcept override { return typeid(Planetarium); };
	};
	
	template<typename T, glm::precision P>
	glm::vec<3, T, P>  Planetarium::Utils::Coord::ChangeHandedness(const glm::vec<3, T, P>& _vec) {
		return glm::vec3 { -_vec.x, _vec.z, _vec.y };
	}
	
	template<typename T, glm::precision P>
	double Planetarium::Utils::Coord::SignedAngle(glm::vec<3, T, P> _a, glm::vec<3, T, P> _b, glm::vec<3, T, P> _axis) {
		
	    auto d = glm::dot  (_a, _b);
	    auto p = glm::cross(_a, _b);
	    
	    auto angle = glm::atan(glm::length(p), d);
	    
	    return angle * glm::sign(dot(p, _axis));
	}
	
	template<typename T, glm::precision P>
	void Planetarium::Planets<T, P>::Time(const double& _time) {
		
		const auto earthMoon = GetEarthAndMoon(_time);
		
		m_Positions.Assign("Sol"    , GetSol    (_time));
		m_Positions.Assign("Mercury", GetMercury(_time));
		m_Positions.Assign("Venus"  , GetVenus  (_time));
		m_Positions.Assign("Earth"  , earthMoon.first  );
		m_Positions.Assign("Moon"   , earthMoon.second );
		m_Positions.Assign("Mars"   , GetMars   (_time));
		m_Positions.Assign("Jupiter", GetJupiter(_time));
		m_Positions.Assign("Saturn" , GetSaturn (_time));
		m_Positions.Assign("Uranus" , GetUranus (_time));
		m_Positions.Assign("Neptune", GetNeptune(_time));
	}
	
	template<typename T, glm::precision P>
	const double& Planetarium::Planets<T, P>::Time() const noexcept {
		return m_Time;
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
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetSol(const double& _time) {
		
		return {
			{},
			{ 0.0, 0.0, 0.0 },
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetMercury(const double& _time) {
		
		double tmp[3];
		
	    vsop87a_full::getMercury(_time, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetVenus(const double& _time) {
		
		double tmp[3];
		
	    vsop87a_full::getVenus(_time, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	std::pair<typename Planetarium::Planets<T, P>::Position, typename Planetarium::Planets<T, P>::Position> Planetarium::Planets<T, P>::GetEarthAndMoon(const double& _time) {
		
		double tmp_earth[3], tmp_emb[3], tmp_moon[3];
		
	    vsop87a_full::getEarth(_time, tmp_earth);
		vsop87a_full::getEmb  (_time, tmp_emb);
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
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetMars(const double& _time) {
		
		double tmp[3];
		
	    vsop87a_full::getMars(_time, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetJupiter(const double& _time) {
		
		double tmp[3];
		
	    vsop87a_full::getJupiter(_time, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetSaturn(const double& _time) {
		
		double tmp[3];
		
	    vsop87a_full::getSaturn(_time, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetUranus(const double& _time) {
		
		double tmp[3];
		
	    vsop87a_full::getUranus(_time, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
	template<typename T, glm::precision P>
	typename Planetarium::Planets<T, P>::Position Planetarium::Planets<T, P>::GetNeptune(const double& _time) {
		
		double tmp[3];
		
	    vsop87a_full::getNeptune(_time, tmp);
		
		return {
			{},
			Utils::Coord::ChangeHandedness<T, P>({ tmp[0], tmp[1], tmp[2] }),
			{}
		};
	}
	
} // LouiEriksson::Game::Scripts

#endif //FINALYEARPROJECT_PLANETARIUM_H