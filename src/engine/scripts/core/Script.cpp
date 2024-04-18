#include "Script.h"

#include "../ecs/GameObject.h"

#include <memory>

namespace LouiEriksson::Engine {
	
	Script::Script(const std::weak_ptr<ECS::GameObject>& _parent) : ECS::Component(_parent) {}
	
	void Script::Begin() {}
	void Script::Tick() {}
	void Script::FixedTick() {}
	void Script::OnCollision(const Physics::Collision& _collision) {}
	
} // LouiEriksson::Engine