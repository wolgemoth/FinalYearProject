#include "stdafx.h"

#include "Component.h"

namespace LouiEriksson {
	
	Component::Component(std::weak_ptr<GameObject> _parent) {
		m_GameObject = std::move(_parent);
		m_Index = 0;
	}
	
	Component::~Component() {
		m_GameObject.lock()->RemoveComponent<Component>(m_Index);
	}
	
	std::weak_ptr<GameObject> Component::GetGameObject() {
		return m_GameObject;
	}
}