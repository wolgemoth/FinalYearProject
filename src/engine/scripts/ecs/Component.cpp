#include "Component.h"

#include "GameObject.h"

#include <iostream>
#include <memory>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP or GEP.

namespace LouiEriksson::ECS {
	
	Component::Component(const std::weak_ptr<GameObject>& _parent) noexcept :
			m_GameObject(_parent),
			m_Index(0u)
	{
		if (_parent.expired()) {
			std::cout << "Warning: Component initialised with no valid parent!\n";
		}
	}
	
	Component::~Component() {
		
		if (const auto go = m_GameObject.lock()) {
			go->RemoveComponent<Component>(m_Index);
		}
	}
	
	const std::weak_ptr<GameObject>& Component::Parent() const noexcept {
		return m_GameObject;
	}
	
} // LouiEriksson::ECS