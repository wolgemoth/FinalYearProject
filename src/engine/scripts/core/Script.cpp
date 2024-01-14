#include "Script.h"

#include "../ecs/GameObject.h"

#include <memory>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	Script::Script(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : ECS::Component(_parent) {}
	
	void Script::Begin() {}
	void Script::Tick() {}
	void Script::FixedTick() {}
	void Script::OnCollision(const Physics::Collision& _collision) {}
	
} // LouiEriksson