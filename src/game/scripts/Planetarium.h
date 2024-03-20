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
		
		template<typename T, glm::precision P>
		class Planets {
		
			using Position = typename Engine::Spatial::VSOP<T, P>::Position;
			
		public:
			
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
			
			const bool TryGetPosition(const std::string& _name, const Position& _out) const;
			
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
	void Planetarium::Planets<T, P>::Time(const double& _time) {
		
		auto earth = Spatial::VSOP<T, P>::V87::A::GetEarth(_time);
		
		auto moon = Spatial::VSOP<T, P>::V87::A::GetMoon(
			earth,
			Spatial::VSOP<T, P>::V87::A::GetEMB(_time)
		);
		
		m_Positions.Assign("Sol"    , Spatial::VSOP<T, P>::V87::A::GetSol    (_time));
		m_Positions.Assign("Mercury", Spatial::VSOP<T, P>::V87::A::GetMercury(_time));
		m_Positions.Assign("Venus"  , Spatial::VSOP<T, P>::V87::A::GetVenus  (_time));
		m_Positions.Assign("Earth"  , earth                                         );
		m_Positions.Assign("Moon"   , moon                                          );
		m_Positions.Assign("Mars"   , Spatial::VSOP<T, P>::V87::A::GetMars   (_time));
		m_Positions.Assign("Jupiter", Spatial::VSOP<T, P>::V87::A::GetJupiter(_time));
		m_Positions.Assign("Saturn" , Spatial::VSOP<T, P>::V87::A::GetSaturn (_time));
		m_Positions.Assign("Uranus" , Spatial::VSOP<T, P>::V87::A::GetUranus (_time));
		m_Positions.Assign("Neptune", Spatial::VSOP<T, P>::V87::A::GetNeptune(_time));
		
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
	
} // LouiEriksson::Game::Scripts

#endif //FINALYEARPROJECT_PLANETARIUM_H