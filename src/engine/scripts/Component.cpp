#include "stdafx.h"

#include "Component.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Component::Component(const std::shared_ptr<GameObject>& _parent) {
		m_GameObject = _parent;
		m_Index = 0;
	}
	
	Component::~Component() {
		
		if (!m_GameObject.expired()) {
			m_GameObject.lock()->RemoveComponent<Component>(m_Index);
		}
	}
	
	std::shared_ptr<GameObject> Component::Parent() const {
		return m_GameObject.lock();
	}
}