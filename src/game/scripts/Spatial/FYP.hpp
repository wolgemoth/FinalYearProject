#ifndef FINALYEARPROJECT_FYP_HPP
#define FINALYEARPROJECT_FYP_HPP

#include "../../include/engine_core.hpp"
#include "../../include/engine_graphics.hpp"
#include "../../include/engine_spatial.hpp"

#include "Map.hpp"
#include "Planetarium.hpp"
#include "Stars.hpp"

namespace LouiEriksson::Game::Scripts::Spatial {
	
	class FYP final : public Script {
	
		/* VARIABLES */
		
	private:
	
		std::weak_ptr<Stars>       m_Stars;
		std::weak_ptr<Planetarium> m_Planets;
		std::weak_ptr<Map>         m_Map;
		std::weak_ptr<FlyCam>      m_Camera;
		
	public:
	
		/* METHODS */
		
		explicit FYP(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(FYP); };
		
	protected:
	
		/** @inheritdoc */
		void Begin() override {
			
			if (const auto p = Parent()) {
			if (const auto s = p->GetScene()) {
				
				auto flyCam_object = s->Create("Player");
				flyCam_object->AddComponent<Transform>();
				m_Camera = flyCam_object->AddComponent<FlyCam>();
				
				auto stars_object = s->Create("Stars");
				m_Stars = stars_object->AddComponent<Stars>();
				
				auto map_object = s->Create("Map");
				auto map_script = map_object->AddComponent<Map>();
				m_Map = map_script;
				
				auto planets_object = s->Create("Planets");
				auto planets_script = planets_object->AddComponent<Planetarium>();
				planets_script->m_DistanceMultiplier = 0.0000001;
				planets_script->m_ScaleMultiplier    = 0.0000001;
				planets_script->m_SunLight           = false;
				planets_script->m_PlanetShadows      = false;
				m_Planets = planets_script;
				
				// Change AO scale to reflect the size of the map.
				Settings::PostProcessing::AmbientOcclusion::s_Radius = map_script->m_Scale * 20.0;
				
				// Set default lighting settings:
				Settings::Graphics::Material::s_CurrentShadowResolutionSelection = std::max(static_cast<int>(Settings::Graphics::Material::s_ShadowResolutions.size()) - 1, 1);
				Settings::Graphics::Material::s_CurrentShadowTechnique           =  1; // (0 = Hard, 1 = PCF, 2 = Disk, 3 = PCSS)
				Settings::Graphics::Material::s_LightSize                        =  0.005;
				Settings::Graphics::Material::s_LightRange                       = 40.0;
				Settings::Graphics::Material::s_ShadowBias                       =  0.04;
				Settings::Graphics::Material::s_ShadowNormalBias                 =  0.02;
			}}
	    }
		
		/** @inheritdoc */
		void LateTick() override {
			
			using Distance = Maths::Conversions::Distance;
			
			if (const auto camera = m_Camera.lock()) {                                            // null safety-check
			if (const auto camera_gameobject = camera->Parent()) {                                // null safety-check
			if (const auto camera_transform = camera_gameobject->GetComponent<Transform>()) {     // null safety-check
				
				if (auto planets = m_Planets.lock()) {                                            // null safety-check
				if (auto planets_gameobject = planets->Parent()) {                                // null safety-check
				if (auto planets_transform = planets_gameobject->GetComponent<Transform>()) {     // null safety-check
					
					if (auto stars = m_Stars.lock()) {                                            // null safety-check
					if (auto stars_gameobject = stars->Parent()) {                                // null safety-check
					if (auto stars_transform = stars_gameobject->GetComponent<Transform>()) {     // null safety-check
						
						if (auto earth = planets->m_Planets.Get("Earth")) {                       // null safety-check
						if (auto earth_gameobject = earth.value().lock()) {                       // null safety-check
						if (auto earth_transform = earth_gameobject->GetComponent<Transform>()) { // null safety-check
							
							if (auto sol = planets->m_Planets.Get("Sol")) {                       // null safety-check
							if (auto sol_gameobject = sol.value().lock()) {                       // null safety-check
							if (auto sol_transform = sol_gameobject->GetComponent<Transform>()) { // null safety-check
								
								if (Input::Input::Key::GetDown(SDL_SCANCODE_R)) {
									Application::ReloadScene();
								}
								
								// Get the progress of the current solar day.
								auto day_elapsed = planets->SolarDayElapsed<scalar_t>();
								
								// Retrieve the current geo position.
								auto geoPosition = Settings::Spatial::s_Coord;
								
								// Disable the planetarium rendering of earth:
								earth_gameobject->Active(false);
								
								/* Update the transform of the planets */
								
								auto earth_rotation = earth_transform->Rotation();
								
								// Calculate the amount by which to rotate the sun and planets from VSOP into cartesian space.
								auto geo_rotation = glm::angleAxis(glm::radians(        90.0F), vec3(1.0, 0.0, 0.0)) *
										            glm::angleAxis(glm::radians(geoPosition.y), vec3(0.0, 0.0, 1.0)) *
										            glm::angleAxis(glm::radians(geoPosition.x), vec3(1.0, 0.0, 0.0));
								
								// Get earth radius at position:
								auto earth_radius = Maths::Coords::WGS84::EarthRadius(geoPosition.x);
								
								// Offset to the position of the planets using the user's position:
								auto positionOffset = (
									camera_transform->Position() +
									glm::vec<3, scalar_t>(
										0.0,
										earth_radius + geoPosition.z,
										0.0
									)
								) * static_cast<scalar_t>(planets->m_DistanceMultiplier);
								
								// Apply rotation and position adjustments to planets.
								planets_transform->Rotation(glm::inverse(earth_rotation) * geo_rotation);
								planets_transform->Position(camera_transform->Position() - earth_transform->Position() + positionOffset);
								
								/* Set up a single directional light using the sun as an illumination source */
								
								Settings::Graphics::Material::s_CurrentLightType = Light::Parameters::Type::Directional;
								
								// Get direction from sun to the camera:
								auto sun_to_camera = camera_transform->Position() - static_cast<vec3>(sol_transform->World()[3]);
								
								bool nighttime = glm::dot(glm::normalize(sun_to_camera), glm::vec<3, scalar_t>(0.0, 1.0, 0.0)) > static_cast<scalar_t>(0.0);
								
								// Parameters for sun brightness:
								auto intensity = 5.0;
								auto distance_au = Distance::Convert(glm::length(sun_to_camera) / static_cast<scalar_t>(planets->m_DistanceMultiplier), Distance::Metre, Distance::AstronomicalUnit);
								auto toggle_light = 1.0;//nighttime ? 0.0 : 1.0;
								
								// Set position and rotation of light source:
								Settings::Graphics::Material::s_LightPosition = camera_transform->Position() + vec3(0.0, 1.0, 0.0);
								Settings::Graphics::Material::s_LightRotation = glm::degrees(glm::eulerAngles(glm::quatLookAtRH(glm::normalize(sun_to_camera), camera_transform->UP)));
								
								// Adjust the brightness of the sun light according to the inverse square distance from the camera and time of day.
								Settings::Graphics::Material::s_LightIntensity = (intensity / (distance_au * distance_au)) * toggle_light;
								
								/* Adjust the skybox for the time of day */
								
								Settings::Graphics::Skybox::s_Blur = 1.0; // Blur 100% to avoid being distracting.
								if (nighttime) {
									Settings::Graphics::Skybox::UpdateSkybox(0U); // (0 = Night Sky)
									Settings::Graphics::Skybox::s_Exposure = 5.0;
								}
								else {
									Settings::Graphics::Skybox::UpdateSkybox(1U); // (2 = Daylight)
									Settings::Graphics::Skybox::s_Exposure = 2.5;
								}
								
								/* Update the transform of the stars */
								
								// Calculate the amount by which to rotate the stars, using the latitude and solar time to calculate offsets for right ascension and declination.
								auto star_rotation = glm::angleAxis(glm::radians(fmod(geoPosition.y + (day_elapsed * 360.0F) + 90.0F, 360.0F)), vec3(0.0, 0.0, 1.0)) *
										             glm::angleAxis(glm::radians(geoPosition.x), vec3(1.0, 0.0, 0.0));
								
								scalar_t m_to_parsec = Distance::Convert(1.0, Distance::Metre, Distance::Parsec);
								
								auto sol_position_parsec =
										(sol_transform->Position() / static_cast<scalar_t>(planets->m_DistanceMultiplier)) * m_to_parsec;
								
								stars_transform->Position(camera_transform->Position() + sol_position_parsec);
								stars_transform->Rotation(star_rotation);
								
								// Set camera far clip far enough to see distant stars and planets.
								Settings::Graphics::Perspective::s_FarClip = 40000;
							}}}
						}}}
					}}}
				}}}
			}}}
	    }
		
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_FYP_HPP