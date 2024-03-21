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
		
		public:
			
			struct Transform {
				
				typename Engine::Spatial::VSOP<T, P>::Position m_Position;
				
				glm::qua<T, P> m_Rotation;
				
				static const Transform InterpolateTransform(Transform _a, Transform _b, const T& _t);
			};
			
			/// </summary>
			/// Radius of planets in solar system (in AU).
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
			
			void Time(const double& _TT);
			
			[[nodiscard]] const double& Time() const noexcept;
			
			const bool TryGetTransform(const std::string& _name, const Transform& _out) const;
			
			[[nodiscard]] const std::vector<typename Hashmap<std::string, Transform>::KeyValuePair> Transforms() const;
			
			[[nodiscard]] const std::vector<std::string> Names() const;
			
		private:
			
			double m_Time;
			
			/// <summary>
			/// String-indexed Hashmap holding planetary transform information.
			/// </summary>
			Hashmap<std::string, Transform> m_Transforms {
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
		Planets<double, glm::highp> m_Positions_A;
		
		/// <summary>
		/// Planetary positions computed using the VSOP87 model.
		/// </summary>
		Planets<double, glm::highp> m_Positions_B;
		
		/// <summary>
		/// Hashmap containing planet GameObjects.
		/// </summary>
		Hashmap<std::string, std::weak_ptr<ECS::GameObject>> m_Planets;
		
		double m_FromUpdate =  std::numeric_limits<double>().infinity();
		double   m_ToUpdate = -std::numeric_limits<double>().infinity();
		
		/// <inheritdoc/>
		void Begin() override;
		
		/// <inheritdoc/>
		void Tick() override;
		
		template<typename T, glm::precision P>
		void InterpolatePlanets(Planetarium::Planets<T, P> _from, Planetarium::Planets<T, P> _to, const double& _t, const std::string _origin = "Earth");
		
	public:
	
		explicit Planetarium(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		~Planetarium() override;
	
		[[nodiscard]] const std::type_index TypeID() const noexcept override { return typeid(Planetarium); };
	};
	
	template<typename T, glm::precision P>
	void Planetarium::Planets<T, P>::Time(const double& _time) {
		
		m_Time = _time;

#define POS Spatial::VSOP<T, P>::V87::A
#define ROT glm::quat(glm::radians(Spatial::WGCCRE::GetOrientationVSOP87<T, P>
		
		auto earth = POS::GetEarth(m_Time);
		auto moon  = POS::GetMoon(earth, POS::GetEMB(m_Time));
		
		m_Transforms.Assign("Sol"    , { POS::GetSol    (m_Time), ROT("Sol"    , m_Time))) });
		m_Transforms.Assign("Mercury", { POS::GetMercury(m_Time), ROT("Mercury", m_Time))) });
		m_Transforms.Assign("Venus"  , { POS::GetVenus  (m_Time), ROT("Venus"  , m_Time))) });
		m_Transforms.Assign("Earth"  , { earth                  , ROT("Earth"  , m_Time))) });
		m_Transforms.Assign("Moon"   , { moon                   , ROT("Moon"   , m_Time))) });
		m_Transforms.Assign("Mars"   , { POS::GetMars   (m_Time), ROT("Mars"   , m_Time))) });
		m_Transforms.Assign("Jupiter", { POS::GetJupiter(m_Time), ROT("Jupiter", m_Time))) });
		m_Transforms.Assign("Saturn" , { POS::GetSaturn (m_Time), ROT("Saturn" , m_Time))) });
		m_Transforms.Assign("Uranus" , { POS::GetUranus (m_Time), ROT("Uranus" , m_Time))) });
		m_Transforms.Assign("Neptune", { POS::GetNeptune(m_Time), ROT("Neptune", m_Time))) });
		
#undef POS
#undef ROT

	}
	
	template<typename T, glm::precision P>
	const double& Planetarium::Planets<T, P>::Time() const noexcept {
		return m_Time;
	}
	
	template<typename T, glm::precision P>
	const bool Planetarium::Planets<T, P>::TryGetTransform(const std::string& _name, const Planets<T, P>::Transform& _out) const {
		return m_Transforms.Get(_name, const_cast<Planets<T, P>::Transform&>(_out) );
	}
	
	template<typename T, glm::precision P>
	const std::vector<typename Hashmap<std::string, typename Planetarium::Planets<T, P>::Transform>::KeyValuePair> Planetarium::Planets<T, P>::Transforms() const {
		return m_Transforms.GetAll();
	}
	
	template<typename T, glm::precision P>
	const std::vector<std::string> Planetarium::Planets<T, P>::Names() const {
		return m_Transforms.Keys();
	}
	
	template<typename T, glm::precision P>
	const typename Planetarium::Planets<T, P>::Transform Planetarium::Planets<T, P>::Transform::InterpolateTransform(Planets<T, P>::Transform _a, Planets<T, P>::Transform _b, const T& _t) {
		
		typename Planets<T, P>::Transform result{};
		
		result.m_Position.m_Cartesian = glm::mix(
			_a.m_Position.m_Cartesian,
			_b.m_Position.m_Cartesian,
			_t
		);
		
		result.m_Rotation = glm::slerp(_a.m_Rotation, _b.m_Rotation, _t);
		
		return result;
	}
	
	template<typename T, glm::precision P>
	void Planetarium::InterpolatePlanets(Planets<T, P> _from, Planets<T, P> _to, const double& _t, const std::string _origin) {
		
		using DISTANCE = Spatial::Maths::Conversions::Distance;
		
		// Get the number of metres in an astronomical unit, to convert to 1:1 scale.
		const double au_to_m = DISTANCE::Convert(1.0f, DISTANCE::Unit::AstronomicalUnit, DISTANCE::Unit::Metre);
		
		// Establish scale multipliers in AU:
		const auto distance_multiplier_au = (1.0 / au_to_m) * 1000.0;
		const auto     size_multiplier_au = (1.0 / au_to_m) * 1000.0;
		
		// Get position of earth to use as a point-of-origin.
		typename Planets<T, P>::Transform origin{};
		
		{
			const typename Planets<T, P>::Transform origin_from{}, origin_to{};
			
			_from.TryGetTransform(_origin, origin_from);
			  _to.TryGetTransform(_origin, origin_to  );
			  
			  origin = Planets<T, P>::Transform::InterpolateTransform(origin_from, origin_to, _t);
		}
		
		auto positions_from = _from.Transforms();
		auto positions_to   =   _to.Transforms();
		
		for (auto i = 0; i < positions_from.size(); ++i) {
			
			auto name = positions_from[i].first;
			auto from = positions_from[i].second;
			auto   to =   positions_to[i].second;
			
			std::weak_ptr<ECS::GameObject> item;
			if (m_Planets.Get(name, item)) {
				
				if (const auto go = item.lock()) {
				
					if (const auto t = go->GetComponent<Transform>().lock()) {
						
						auto interpolated = Planets<T, P>::Transform::InterpolateTransform(from, to, _t);
						
						// Adjust the position and rotation of the planet.
						t->m_Position = (interpolated.m_Position.m_Cartesian - origin.m_Position.m_Cartesian) * au_to_m * distance_multiplier_au;
						t->m_Rotation =  interpolated.m_Rotation;
						
						// Adjust the scale of the planet.
						T scale;
						if (Planets<T, P>::s_Scales_AU.Get(name, scale)) {
							t->m_Scale = glm::vec<3, T, P>(au_to_m * scale * size_multiplier_au);
						}
					}
				}
			}
		}
		
		// Put the light where the sun is.
		// TODO: Use a 'Light' component instead!
		const auto sol = m_Planets.Return("Sol").lock()->GetComponent<Transform>().lock();
		Settings::Graphics::Material::s_LightPosition = sol->m_Position;
		Settings::Graphics::Material::s_LightRange = 40000.000 * 1000.0;
	}
	
} // LouiEriksson::Game::Scripts

#endif //FINALYEARPROJECT_PLANETARIUM_H