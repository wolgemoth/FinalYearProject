#include "Component.h"

#include "GameObject.h"
#include "../core/Debug.h"

#include <memory>

namespace LouiEriksson::Engine::ECS {
	
	Component::Component(const std::weak_ptr<GameObject>& _parent) noexcept :
		m_Index(0u),
		m_GameObject(_parent)
	{
		Debug::Assert(!_parent.expired(), "Component initialised with no valid parent!", LogType::Warning);
	}
	
} // LouiEriksson::Engine::ECS