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
				m_Planets = planets_object->AddComponent<Planetarium>();
			}}
	    }
		
		/** @inheritdoc */
		void LateTick() override {
			
			if (auto s = m_Stars.lock()) {
			if (auto p1 = s->Parent()) {
			if (auto t1 = p1->GetComponent<Transform>()) {
				
				if (auto c = m_Camera.lock()) {
				if (auto p2 = c->Parent()) {
				if (auto t2 = p2->GetComponent<Transform>()) {
					
					const auto time = Planetarium::J2000_Centuries();

					auto geoPosition = Meshing::Builder::s_Origin;
					
					auto now = std::chrono::system_clock::now();
				    std::time_t tt = std::chrono::system_clock::to_time_t(now);
				
				    // Get the current time
				    tm local_tm = *std::localtime(&tt);
				
				    // Calculate seconds passed since start of the day
				    auto seconds_today = local_tm.tm_hour * 3600 + local_tm.tm_min * 60 + local_tm.tm_sec;
				
				    scalar_t dayElapsed = seconds_today / 86400.0;
					
					// Convert the current geoposition into a rotation.
					auto geoRotation = glm::angleAxis(glm::radians(fmod(geoPosition.y + (dayElapsed * 360.0f) - 90.0f, 360.0f)), glm::vec3(0.0, 0.0, 1.0)) *
							           glm::angleAxis(glm::radians(geoPosition.x), glm::vec3(1.0, 0.0, 0.0));
					
					t1->Position(t2->Position());
					t1->Rotation(geoRotation);
				}}}
			}}}
	    }
		
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_FYP_HPP