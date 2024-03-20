#include "Component.h"

#include "GameObject.h"

#include <iostream>
#include <memory>

namespace LouiEriksson::Engine::ECS {
	
	Component::Component(const std::weak_ptr<GameObject>& _parent) noexcept :
			m_GameObject(_parent),
			m_Index(0u)
	{
		if (_parent.expired()) {
			std::cout << "Warning: Component initialised with no valid parent!\n";
		}
	}
	
	Component::~Component() {}
	
	const std::weak_ptr<GameObject>& Component::Parent() const noexcept {
		return m_GameObject;
	}
	
} // LouiEriksson::Engine::ECS