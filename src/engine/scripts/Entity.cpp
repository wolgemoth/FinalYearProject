#include "stdafx.h"

#include "Entity.h"

void Entity::Name(const std::string _name) {
	m_Name = _name;
}
const std::string& Entity::Name() {
	return m_Name;
}

std::shared_ptr<Scene> Entity::GetScene() {
	return m_Scene;
}
