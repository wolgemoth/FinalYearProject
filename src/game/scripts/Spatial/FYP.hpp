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
		
	public:
	
		/* METHODS */
		
		explicit FYP(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent) {}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(FYP); };

	protected:
	
		/** @inheritdoc */
		void Begin() override  {
			
			if (const auto p =      Parent().lock()) {
			if (const auto s = p->GetScene().lock()) {
				
				auto stars_object = s->Create("Stars").lock();
				m_Stars = stars_object->AddComponent<Stars>();
				
				auto planets_object = s->Create("Planets").lock();
				m_Planets = planets_object->AddComponent<Planetarium>();
				
				auto map_object = s->Create("Map").lock();
				m_Map = map_object->AddComponent<Map>();
			}}
	    }
		
		/** @inheritdoc */
		void Tick() override {
			
	    }
		
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_FYP_HPP