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
	const std::weak_ptr<Transform>& Collider::GetTransform() const noexcept {
		return m_Transform;
	}
	
	void Collider::SetRigidbody(const std::weak_ptr<Rigidbody>& _transform) noexcept {
		m_Rigidbody = _transform;
	}
	const std::weak_ptr<Rigidbody>& Collider::GetRigidbody() const noexcept {
		return m_Rigidbody;
	}
	
	void Collider::SetType(const Type& _type) noexcept {
		m_Type = _type;
	}
	const Collider::Type& Collider::GetType() const noexcept {
		return m_Type;
	}
	
} // LouiEriksson::Engine::Physics