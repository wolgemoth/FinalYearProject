#include "stdafx.h"

#include "Component.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	Component::Component(const std::shared_ptr<GameObject>& _parent) {
		
		if (_parent == nullptr) {
			std::cout << "Warning: Component initialised with nullptr.\n";
		}
		
		m_GameObject = _parent;
		
		m_Index = 0;
	}
	
	Component::~Component() {
		
		auto go = m_GameObject.lock();
		
		if (go != nullptr) {
			go->RemoveComponent<Component>(m_Index);
		}
	}
	
	std::shared_ptr<GameObject> Component::Parent() const {
		return m_GameObject.lock();
	}
}