#include "Component.h"

#include "GameObject.h"
#include "../core/Debug.h"

#include <memory>

namespace LouiEriksson::Engine::ECS {
	
	Component::Component(const std::weak_ptr<GameObject>& _parent) noexcept :
			m_GameObject(_parent),
			m_Index(0u)
	{
		Debug::Assert(!_parent.expired(), "Warning: Component initialised with no valid parent!", LogType::Error);
	}
	
	Component::~Component() = default;
	
	const std::weak_ptr<GameObject>& Component::Parent() const noexcept {
		return m_GameObject;
	}
	
} // LouiEriksson::Engine::ECS