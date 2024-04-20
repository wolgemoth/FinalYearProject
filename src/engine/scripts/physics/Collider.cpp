#include "Collider.h"

#include "../core/Transform.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"
#include "../physics/Rigidbody.h"

#include <memory>

namespace LouiEriksson::Engine::Physics {
	
	Collider::Collider(const std::weak_ptr<ECS::GameObject>& _parent) noexcept : ECS::Component(_parent),
			m_Type() {}
	
	void Collider::SetTransform(const std::weak_ptr<Transform>& _transform) noexcept {
		m_Transform = _transform;
	}
	
	void Collider::SetRigidbody(const std::weak_ptr<Rigidbody>& _rigidbody) noexcept {
		m_Rigidbody = _rigidbody;
	}
	
	void Collider::SetType(const Type& _type) noexcept {
		m_Type = _type;
	}
	
} // LouiEriksson::Engine::Physics