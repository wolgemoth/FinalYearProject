#include "stdafx.h"

#include "Component.h"

namespace LouiEriksson {
	
	Component::Component(const std::shared_ptr<GameObject>& _parent) {
		m_GameObject = _parent;
		m_Index = 0;
	}
	
	Component::~Component() {
		m_GameObject.lock()->RemoveComponent<Component>(m_Index);
	}
	
	std::shared_ptr<GameObject> Component::Parent() const {
		return m_GameObject.lock();
	}
}