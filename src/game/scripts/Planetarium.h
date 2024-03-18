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
			
			const double& Time() const noexcept;
			void Time(const double& _ephemeris);
			
			const bool TryGetPosition(const std::string& _name, const Position& _out) const;
			
			const std::vector<typename Hashmap<std::string, Position>::KeyValuePair> Positions() const;
			
			const std::vector<std::string> Names() const;
			
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
			
			static Position GetSol(const double& _ephemeris);
			
			static Position GetMercury(const double& _ephemeris);
			
			static Position GetVenus(const double& _ephemeris);
			
			std::pair<Position, Position> GetEarthAndMoon(const double& _ephemeris);
			
			static Position GetMars(const double& _ephemeris);
			
			static Position GetJupiter(const double& _ephemeris);
			
			static Position GetSaturn(const double& _ephemeris);
			
			static Position GetUranus(const double& _ephemeris);
			
			static Position GetNeptune(const double& _ephemeris);
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
	
} // LouiEriksson::Game::Scripts

#endif //FINALYEARPROJECT_PLANETARIUM_H