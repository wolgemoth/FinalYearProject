#include "../stdafx.h"

#include "Entity.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	void Entity::Name(const std::string& _name) {
		m_Name = _name;
	}
	const std::string& Entity::Name() {
		return m_Name;
	}
	
	std::shared_ptr<Scene> Entity::GetScene() {
		return m_Scene;
	}
}