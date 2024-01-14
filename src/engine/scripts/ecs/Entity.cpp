#include "Entity.h"

#include <memory>
#include <string>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson::ECS {
	
	void Entity::Name(const std::string& _name) {
		m_Name = _name;
	}
	const std::string& Entity::Name() {
		return m_Name;
	}
	
	std::shared_ptr<Scene> Entity::GetScene() {
		return m_Scene;
	}
	
} // LouiEriksson::ECS