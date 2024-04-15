#ifndef FINALYEARPROJECT_PLANETARIUM_H
#define FINALYEARPROJECT_PLANETARIUM_H

#include "../include/engine_core.h"
#include "../include/engine_spatial.h"

using namespace LouiEriksson::Engine;

namespace LouiEriksson::Game::Scripts {
	
	/**
	 * @class Planetarium
	 * @brief Represents a 3D planetarium.
	 *
	 * The Planetarium class inherits from the Script class and provides functionality for managing the positions and transformations of the planets in the solar system.
	 */
	class Planetarium final : public Script {
	
	protected:
		
		template<typename T, glm::precision P>
		class Planets {
		
		public:
			
			/**
			 * @struct Transform
			 * @brief Represents the transform of a astronomical object.
			 */
			struct Transform {
				
				typename Engine::Spatial::VSOP<T, P>::Position m_Position;
				
				glm::qua<T, P> m_Rotation;
				
				/**
				 * @brief Interpolates between two transforms.
				 *
				 * This function takes two transforms (_a and _b) and a interpolation parameter (_t) and returns
				 * an interpolated transform between them. The interpolation is performed in a linear manner for
				 * the position and using spherical linear interpolation (slerp) for the rotation.
				 *
				 * @param[in] _a The first transform.
				 * @param[in] _b The second transform.
				 * @param[in] _t The interpolation parameter.
				 * @return The interpolated transform.
				 */
				static const Transform InterpolateTransform(const Transform& _a, const Transform& _b, const T& _t);
			};
			
			/**
			 * @brief Radius of planets in solar system (in AU).
			 * @note Converted from information provided by NASA.
			 * @see https://informal.jpl.nasa.gov/museum/sites/default/files/ResourceLibrary/Planets%20to%20Scale.pdf,
			 * @see https://nightsky.jpl.nasa.gov/club/attachments/Fun_Facts_About_the_Moon.pdf
			 */
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
			
			/**
			 * @brief Sets the time of the planetarium.
			 * @param[in] _tt The new time value in terrestrial time.
			 */
			void Time(const double& _tt);
			
			/**
			 * @brief Get the current time of the planetarium.
			 * @return The current time of the planetarium in terrestrial time.
			 */
			[[nodiscard]] const double& Time() const noexcept;
			
			/**
			 * @brief Tries to retrieve the transform of an astronomical object by name.
			 *
			 * This function attempts to retrieve the transform of an astronomical object specified by its name.
			 * The transform is returned through the reference parameter _out. If the transform is found, the function
			 * returns true. Otherwise, it returns false.
			 *
			 * @param[in] _name The name of the astronomical object.
			 * @param[in,out] _out The reference to the Transform object to store the result.
			 * @return True if the transform is found, false otherwise.
			 */
			[[nodiscard]] bool TryGetTransform(const std::string& _name, const Transform& _out) const;
			
			/**
			 * @brief Retrieves the transforms of the stored astronomical objects.
			 *
			 * @return A vector containing the transformations of astronomical objects.
			 */
			[[nodiscard]] const std::vector<typename Hashmap<std::string, Transform>::KeyValuePair> Transforms() const;
			
			/**
			 * @brief Retrieves the names of the stored astronomical objects.
			 *
			 * This function returns a vector containing the names of the stored astronomical objects.
			 *
			 * @return A vector of strings representing the names of the astronomical objects.
			 */
			[[nodiscard]] const std::vector<std::string> Names() const;
			
		private:
			
			/**
			 * @brief Terrestrial time.
			 * @see https://en.wikipedia.org/wiki/Terrestrial_Time
			 */
			double m_Time;
			
			/** @brief String-indexed Hashmap holding planetary transform information. */
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
		
		/** @brief String-indexed Transform of the planetarium. */
		std::weak_ptr<Transform> m_Transform;
		
		/** @brief Planetary positions computed using the VSOP87 model. */
		Planets<double, glm::highp> m_Positions_From;
		Planets<double, glm::highp> m_Positions_To;
		
		/** @brief Hashmap containing planet GameObjects. */
		Hashmap<std::string, std::weak_ptr<ECS::GameObject>> m_Planets;
		
		std::shared_ptr<Graphics::Mesh> m_Stars;
		
		/** @inheritdoc */
		void Begin() override;
		
		/** @inheritdoc */
		void Tick() override;
		
		void LoadStars(const std::vector<std::filesystem::path>& _athyg_paths, const double& _threshold_magnitude = std::numeric_limits<double>::infinity());
		
		/**
		 * @brief Interpolates the transforms of planets from a starting state to an
		 * ending state at a given interpolation factor.
		 *
		 * @tparam T The type used for calculations (float, double, etc.)
		 * @tparam P The precision used for calculations (lowp, mediump, highp)
		 * @param[in] _from The starting state of the planets
		 * @param[in] _to The ending state of the planets
		 * @param[in] _t The interpolation factor (0.0 - starting state, 1.0 - ending state)
		 * @param[in] _origin The point-of-origin for the interpolation (default: "Earth")
		 */
		template<typename T, glm::precision P>
		void InterpolatePlanets(const Planetarium::Planets<T, P>& _from, const Planetarium::Planets<T, P>& _to, const double& _t, const std::string& _origin = "Earth");
		
	public:
	
		explicit Planetarium(const std::weak_ptr<ECS::GameObject>& _parent) noexcept;
		
		/** @inheritdoc */
		~Planetarium() override;
	
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Planetarium); };
	};
	
	/**
	 * @brief Sets the time of the planetarium.
	 *
	 * This function sets the time of the planetarium to the specified value.
	 *
	 * @param[in] _tt The new time value in terrestrial time.
	 */
	template<typename T, glm::precision P>
	void Planetarium::Planets<T, P>::Time(const double& _tt) {
		
		m_Time = _tt;

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
	
	/**
	 * @brief Retrieves the current time of the Planets class.
	 *
	 * This function returns the current time of the Planets class in terrestrial time.
	 *
	 * @return A constant reference to the current time value.
	 */
	template<typename T, glm::precision P>
	const double& Planetarium::Planets<T, P>::Time() const noexcept {
		return m_Time;
	}
	
	/**
	 * @brief Tries to retrieve the transform of an astronomical object by name.
	 *
	 * This function attempts to retrieve the transform of an astronomical object specified by its name.
	 * The transform is returned through the reference parameter _out. If the transform is found, the function
	 * returns true. Otherwise, it returns false.
	 *
	 * @param[in] _name The name of the astronomical object.
	 * @param[in,out] _out The reference to the Transform object to store the result.
	 * @return True if the transform is found, false otherwise.
	 */
	template<typename T, glm::precision P>
	bool Planetarium::Planets<T, P>::TryGetTransform(const std::string& _name, const Planets<T, P>::Transform& _out) const {
		return m_Transforms.Get(_name, const_cast<Planets<T, P>::Transform&>(_out) );
	}
	
	/**
	 * @brief Retrieves the transforms of the stored astronomical objects.
	 *
	 * This function returns a vector containing the transformations of astronomical objects.
	 *
	 * @tparam T The type of the astronomical object.
	 * @tparam P The precision of the astronomical object.
	 * @return A vector containing the transformations of astronomical objects.
	 */
	template<typename T, glm::precision P>
	const std::vector<typename Hashmap<std::string, typename Planetarium::Planets<T, P>::Transform>::KeyValuePair> Planetarium::Planets<T, P>::Transforms() const {
		return m_Transforms.GetAll();
	}
	
	/**
	 * @brief Retrieves the names of the stored astronomical objects.
	 *
	 * This function returns a vector containing the names of the stored astronomical objects.
	 *
	 * @return A vector of strings representing the names of the astronomical objects.
	 */
	template<typename T, glm::precision P>
	const std::vector<std::string> Planetarium::Planets<T, P>::Names() const {
		return m_Transforms.Keys();
	}
	
	/**
	 * @brief Interpolates between two transforms.
	 *
	 * This function takes two transforms (_a and _b) and an interpolation parameter (_t) and returns
	 * an interpolated transform between them. The interpolation is performed in a linear manner for
	 * the position and using spherical linear interpolation (slerp) for the rotation.
	 *
	 * @tparam T The type of the elements in the transform.
	 * @tparam P The precision of the transform.
	 * @param[in] _a The first transform.
	 * @param[in] _b The second transform.
	 * @param[in] _t The interpolation parameter.
	 * @return The interpolated transform.
	 */
	template<typename T, glm::precision P>
	const typename Planetarium::Planets<T, P>::Transform Planetarium::Planets<T, P>::Transform::InterpolateTransform(const Planets<T, P>::Transform& _a, const Planets<T, P>::Transform& _b, const T& _t) {
		
		typename Planets<T, P>::Transform result{};
		
		result.m_Position.m_Cartesian = glm::mix(
			_a.m_Position.m_Cartesian,
			_b.m_Position.m_Cartesian,
			_t
		);
		
		result.m_Rotation = glm::slerp(_a.m_Rotation, _b.m_Rotation, _t);
		
		return result;
	}
	
	/**
	 * @brief Interpolates the transform of planets between two states.
	 *
	 * This function interpolates the position, rotation, and scale of planets between two sets of transforms.
	 * It adjusts the position and rotation of the planet based on the planet's original position, the interpolated transform,
	 * and the origin point. It also adjusts the scale of the planet based on the scale multiplier.
	 *
	 * @tparam T     The type for the planet's position, rotation, and scale components.
	 * @tparam P     The precision for the planet's position, rotation, and scale components.
	 * @param[in] _from  The set of transforms representing the initial state of the planets.
	 * @param[in] _to    The set of transforms representing the final state of the planets.
	 * @param[in] _t     The interpolation factor between the initial and final states of the planets.
	 * @param[in] _origin The name of the planet to be used as the point of origin for the interpolation.
	 */
	template<typename T, glm::precision P>
	void Planetarium::InterpolatePlanets(const Planets<T, P>& _from, const Planets<T, P>& _to, const double& _t, const std::string& _origin) {
		
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
		const auto sol = m_Planets["Sol"].lock()->GetComponent<Transform>().lock();
		Settings::Graphics::Material::s_LightPosition = sol->m_Position;
		Settings::Graphics::Material::s_LightRange = 4000.0 * 1000.0;
		Settings::Graphics::Material::s_LightIntensity = 1.0;
	}
	
} // LouiEriksson::Game::Scripts

#endif //FINALYEARPROJECT_PLANETARIUM_H