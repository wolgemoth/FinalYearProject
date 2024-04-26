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
			if (const auto s = p->GetScene().lock()) {
				
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
					
					auto geoPosition = Meshing::Builder::s_Origin;

					auto geoRotation = glm::quat(glm::radians(glm::vec3(geoPosition.y, geoPosition.x, geoPosition.z)));
					
					auto earthRotation = glm::quat(glm::radians(WGCCRE::GetOrientationVSOP87("Earth", Planetarium::J2000_Centuries())));
					
					t1->Position(t2->Position());
					t1->Rotation(earthRotation * glm::inverse(geoRotation));
				}}}
			}}}
	    }
		
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_FYP_HPP