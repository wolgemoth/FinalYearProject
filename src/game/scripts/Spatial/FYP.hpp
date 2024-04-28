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
				m_Map = map_object->AddComponent<Map>();
				
				auto planets_object = s->Create("Planets");
				auto planets_script = planets_object->AddComponent<Planetarium>();
				planets_script->m_DistanceMultiplier = 0.00000001;
				planets_script->m_ScaleMultiplier    = 0.00000001;
				planets_script->m_SunLight = false;
				
				m_Planets = planets_script;
			}}
	    }
		
		/** @inheritdoc */
		void LateTick() override {
			
			if (auto camera = m_Camera.lock()) {                                                  // null safety-check
			if (auto camera_gameobject = camera->Parent()) {                                      // null safety-check
			if (auto camera_transform = camera_gameobject->GetComponent<Transform>()) {           // null safety-check
				
				if (auto planets = m_Planets.lock()) {                                            // null safety-check
				if (auto planets_gameobject = planets->Parent()) {                                // null safety-check
				if (auto planets_transform = planets_gameobject->GetComponent<Transform>()) {     // null safety-check
					
					if (auto stars = m_Stars.lock()) {                                            // null safety-check
					if (auto stars_gameobject = stars->Parent()) {                                // null safety-check
					if (auto stars_transform = stars_gameobject->GetComponent<Transform>()) {     // null safety-check
						
						if (auto earth = planets->m_Planets.Get("Earth")) {                       // null safety-check
						if (auto earth_gameobject = earth.value().lock()) {                       // null safety-check
						if (auto earth_transform = earth_gameobject->GetComponent<Transform>()) { // null safety-check
							
							// Get the progress of the current solar day.
							auto day_elapsed = planets->SolarDayElapsed<scalar_t>();
							
							// Retrieve the current geo position.
							auto geoPosition = Meshing::Builder::s_Origin;
							
							// Update the transform of the sun and planets:
							{
								auto earth_rotation = earth_transform->Rotation();
								
								auto geo_rotation = glm::angleAxis(glm::radians(90.0f + geoPosition.x + WGCCRE::EarthAxialTilt<scalar_t>()), glm::vec3(1.0, 0.0, 0.0)) *
										            glm::angleAxis(glm::radians(fmod(geoPosition.y, 360.0f)), glm::vec3(0.0, 0.0, 1.0));
								
								planets_transform->Rotation(glm::inverse(earth_rotation) * geo_rotation);
								planets_transform->Position(camera_transform->Position() - earth_transform->Position());
							}
							
							// Update the transform of the stars:
							{
								// Calculate the amount by which to rotate the stars to be correctly represented in the engine.
								auto star_rotation = glm::angleAxis(glm::radians(fmod(geoPosition.y + (day_elapsed * 360.0f) - 90.0f, 360.0f)), glm::vec3(0.0, 0.0, 1.0)) *
										             glm::angleAxis(glm::radians(geoPosition.x), glm::vec3(1.0, 0.0, 0.0));
								
								stars_transform->Position(camera_transform->Position());
								stars_transform->Rotation(star_rotation);
							}
						}}}
					}}}
				}}}
			}}}
	    }
		
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_FYP_HPP