#ifndef FINALYEARPROJECT_PLANETARIUM_H
#define FINALYEARPROJECT_PLANETARIUM_H

#include "../../include/engine_core.h"
#include "../../include/engine_spatial.h"

using namespace LouiEriksson::Engine;
using namespace LouiEriksson::Engine::Spatial;

namespace LouiEriksson::Game::Scripts::Spatial {
	
	/**
	 * @class Planetarium
	 * @brief Represents a 3D planetarium.
	 *
	 * The Planetarium class inherits from the Script class and provides functionality for managing the positions and transformations of the planets in the solar system.
	 */
	class Planetarium final : public Script {
	
		using highp_time = long double;
		using scalar     = long double;
		
	public:
	
		explicit Planetarium(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {};
		
		/** @inheritdoc */
		[[nodiscard]] inline std::type_index TypeID() const noexcept override { return typeid(Planetarium); };
		
		/**
		 * @brief Conversion from seconds to centuries.
		 *
		 * @warning This function assumes the 'julian' definition of a century.
		 *     The conversion used is: _seconds / 31557600000.0
		 *
		 * @tparam T The type of the value to convert centuries.
		 * @param _seconds The value to convert.
		 * @return Centuries in given value.
		 */
		template <typename T = highp_time>
		static constexpr T SecondsToCenturies(const T& _seconds) {
			return _seconds / 31557600000.0;
		}
		
		template <typename T = highp_time>
		static T J2000_Centuries() {
			
			// Get the current TT in the Julian calendar.
			const T JD = ((TT<T>()) / 86400.0) + 2440587.5;
	
			// Get the days since the J2000 epoch.
			const T j2000_days = JD - 2451545.0;
			
			// Convert from julian days to julian centuries.
			return j2000_days / 365250.0;
		}
		
		/**
		 * @brief Get the current terrestrial time.
		 * @tparam T Type to return TT.
		 * @return Current TT
		 */
		template <typename T = highp_time>
		static T TT() {
			
			// Convert TAI (atomic time) to TT (terrestrial time) using the established conversion of 32.184.
			return TAI<T>() + 32.184;
		}
		/**
		 * @brief Get the current total atomic time.
		 * @tparam T Type to return TAI as. Must be floating-point.
		 *
		 * @warning (Apr 20, 2024) This solution is not future-proof!
		 *     Future leap seconds will cause this to go out of sync.
		 *
		 * @return Current total atomic time.
		 */
		template <typename T = highp_time>
		static T TAI() {
			
		    static_assert(std::is_floating_point<T>::value, "T must be a floating point type");

			// Yes, I know about std::chrono::tai_clock.
			// No, it doesn't compile on my system.
			
			/*
			 * Current UNIX time from the system clock.
			 * ...Probably UTC but not guaranteed?
			 *
			 * This may not be accurate to the 'actual' UNIX time
			 * -especially if the user doesn't have an internet connection
			 * from which their system is syncing the time automatically.
			 */
			const T unix_time_utc = std::chrono::duration_cast<std::chrono::duration<T>>(
				std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			
			/**
			 * Convert from UTC to TAI using an offset of 37 seconds.
			 *
			 *     This solution is not future-proof!
			 *     Future leap seconds will cause this to go out of sync.
			 *
			 * @todo A better solution is to use a lookup table and update leap seconds with an internet connection.
			 *      FTP servers for downloading leap second information (Courtesy of: https://data.iana.org/T-zones/tzdb-2019c/leapseconds):
			 *      <ftp://ftp.nist.gov/pub/T/leap-seconds.list>
			 *      <ftp://ftp.boulder.nist.gov/pub/T/leap-seconds.list>
			 */
			return unix_time_utc + 37.0;
		}
		
	protected:
		
		template<typename T = scalar, glm::precision P = glm::highp>
		class Planets final {
		
		public:
			
			/**
			 * @struct Transform
			 * @brief Represents the transform of a astronomical object.
			 */
			struct Transform final {
				
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
				static const Transform InterpolateTransform(const Transform& _a, const Transform& _b, const T& _t) {
		
					typename Planets<T, P>::Transform result{};
					
					result.m_Position.m_Cartesian = glm::mix(
						_a.m_Position.m_Cartesian,
						_b.m_Position.m_Cartesian,
						_t
					);
					
					result.m_Rotation = glm::slerp(_a.m_Rotation, _b.m_Rotation, _t);
					
					return result;
				}
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
			void Time(const highp_time& _tt) {
					
				m_Time = _tt;
		
#define POS VSOP<T, P>::V87::A
#define ROT glm::quat(glm::radians(WGCCRE::GetOrientationVSOP87<T, P>
				
				auto earth = POS::GetEarth(m_Time);
				auto moon  = POS::GetMoon(earth, POS::GetEMB(m_Time));
				
				m_Transforms.Emplace("Sol"    , { POS::GetSol    (      ), ROT("Sol"    , m_Time))) });
				m_Transforms.Emplace("Mercury", { POS::GetMercury(m_Time), ROT("Mercury", m_Time))) });
				m_Transforms.Emplace("Venus"  , { POS::GetVenus  (m_Time), ROT("Venus"  , m_Time))) });
				m_Transforms.Emplace("Earth"  , { earth                  , ROT("Earth"  , m_Time))) });
				m_Transforms.Emplace("Moon"   , { moon                   , ROT("Moon"   , m_Time))) });
				m_Transforms.Emplace("Mars"   , { POS::GetMars   (m_Time), ROT("Mars"   , m_Time))) });
				m_Transforms.Emplace("Jupiter", { POS::GetJupiter(m_Time), ROT("Jupiter", m_Time))) });
				m_Transforms.Emplace("Saturn" , { POS::GetSaturn (m_Time), ROT("Saturn" , m_Time))) });
				m_Transforms.Emplace("Uranus" , { POS::GetUranus (m_Time), ROT("Uranus" , m_Time))) });
				m_Transforms.Emplace("Neptune", { POS::GetNeptune(m_Time), ROT("Neptune", m_Time))) });
				
#undef POS
#undef ROT
		
			}
			
			/**
			 * @brief Get the current time of the planetarium.
			 * @return The current time of the planetarium in terrestrial time.
			 */
			[[nodiscard]] const highp_time& Time() const noexcept {
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
			[[nodiscard]] typename Hashmap<std::string, Transform>::optional_ref TryGetTransform(const std::string& _name) const {
				return m_Transforms.Get(_name);
			}
			
			/**
			 * @brief Retrieves the transforms of the stored astronomical objects.
			 *
			 * @return A vector containing the transformations of astronomical objects.
			 */
			[[nodiscard]] const std::vector<typename Hashmap<std::string, typename Planetarium::Planets<T, P>::Transform>::KeyValuePair> Transforms() const {
				return m_Transforms.GetAll();
			}
			
			/**
			 * @brief Retrieves the names of the stored astronomical objects.
			 *
			 * This function returns a vector containing the names of the stored astronomical objects.
			 *
			 * @return A vector of strings representing the names of the astronomical objects.
			 */
			[[nodiscard]] const std::vector<std::string> Names() const {
				return m_Transforms.Keys();
			}
			
			Planets() : m_Time(0) {}
			
		private:
			
			/**
			 * @brief Terrestrial time.
			 * @see https://en.wikipedia.org/wiki/Terrestrial_Time
			 */
			highp_time m_Time;
			
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
		Planets<scalar, glm::highp> m_Positions_From;
		Planets<scalar, glm::highp> m_Positions_To;
		
		/** @brief Hashmap containing planet GameObjects. */
		Hashmap<std::string, std::weak_ptr<ECS::GameObject>> m_Planets;
		
		/** @inheritdoc */
		void Begin() override {
		
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
				
					const auto go = s->Create(item).lock();
					
					const auto transform = go->AddComponent<Transform>();
					const auto renderer  = go->AddComponent<Graphics::Renderer>();
					
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
				}
			}}
			
			// Prevent the model of the sun from casting shadows:
			if (const auto sol = m_Planets.Get("Sol")) {
				
				if (const auto go = sol->lock()                                  ) {
				if (const auto t  = go->GetComponent<Transform>().lock()         ) {
				if (const auto r  = go->GetComponent<Graphics::Renderer>().lock()) {
					r->Shadows(false);
				}}}
			}
		}
		
		/** @inheritdoc */
		void Tick() override {
			
			const highp_time update_interval = 120.0;
			
			const highp_time curr = J2000_Centuries();
			
			if (curr > m_Positions_To.Time()) {
				m_Positions_From.Time(curr);
				  m_Positions_To.Time(curr + (SecondsToCenturies(update_interval)));
			}
			else if (curr < m_Positions_From.Time()) {
				m_Positions_From.Time(curr - (SecondsToCenturies(update_interval)));
				  m_Positions_To.Time(curr);
			}
			
			InterpolatePlanets(m_Positions_From, m_Positions_To, Utils::Remap(curr, m_Positions_From.Time(), m_Positions_To.Time(), static_cast<highp_time>(0.0), static_cast<highp_time>(1.0)));
		}
		
		/**
		 * @brief Interpolates the transforms of planets from a starting state to an
		 * ending state at a given interpolation factor.
		 *
		 * @tparam T The type used for calculations (float, scalar, etc.)
		 * @tparam P The precision qualifier of the type.
		 * @param[in] _from The starting state of the planets
		 * @param[in] _to The ending state of the planets
		 * @param[in] _t The interpolation factor (0.0 - starting state, 1.0 - ending state)
		 * @param[in] _origin The point-of-origin for the interpolation (default: "Earth")
		 */
		template<typename T = scalar, glm::precision P = glm::highp>
		void InterpolatePlanets(const Planetarium::Planets<T, P>& _from, const Planetarium::Planets<T, P>& _to, const highp_time& _t, const std::string& _origin = "Earth")  {
			
			using DISTANCE = Maths::Conversions::Distance;
			
			// Get the number of metres in an astronomical unit, to convert to 1:1 scale.
			const scalar au_to_m = DISTANCE::Convert(1.0, DISTANCE::Unit::AstronomicalUnit, DISTANCE::Unit::Metre);
			
			// Establish scale multipliers in AU:
			const auto distance_multiplier_au = (1.0 / au_to_m) * 1000.0;
			const auto     size_multiplier_au = (1.0 / au_to_m) * 1000.0;
			
			// Get position of earth to use as a point-of-origin.
			typename Planets<T, P>::Transform origin{};
			{
				auto origin_from = _from.TryGetTransform(_origin).value_or({});
				auto origin_to   =   _to.TryGetTransform(_origin).value_or({});
				
				origin = Planets<T, P>::Transform::InterpolateTransform(origin_from, origin_to, _t);
			}
			
			auto positions_from = _from.Transforms();
			auto positions_to   =   _to.Transforms();
			
			for (size_t i = 0; i < positions_from.size(); ++i) {
				
				auto name = positions_from[i].first;
				auto from = positions_from[i].second;
				auto   to =   positions_to[i].second;
				
				if (const auto item = m_Planets.Get(name)) {
					
					if (const auto go = item.value().lock()) {
					
						if (const auto t = go->template GetComponent<Transform>().lock()) {
							
							auto interpolated = Planets<T, P>::Transform::InterpolateTransform(from, to, _t);
							
							// Adjust the position and rotation of the planet.
							t->Position((interpolated.m_Position.m_Cartesian - origin.m_Position.m_Cartesian) * au_to_m * distance_multiplier_au);
							t->Rotation( interpolated.m_Rotation);
							
							// Adjust the scale of the planet.
							if (const auto scale = Planets<T, P>::s_Scales_AU.Get(name)) {
								t->Scale(glm::vec<3, T, P>(au_to_m * size_multiplier_au * scale.value()));
							}
						}
					}
				}
			}
			
			// Put the light where the sun is.
			// TODO: Use a 'Light' component instead!
			const auto sol = m_Planets["Sol"].lock()->GetComponent<Transform>().lock();
			Settings::Graphics::Material::s_LightPosition = sol->Position();
			Settings::Graphics::Material::s_LightRange = 4000.0 * 1000.0;
			Settings::Graphics::Material::s_LightIntensity = 1.0;
		}
		
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_PLANETARIUM_H