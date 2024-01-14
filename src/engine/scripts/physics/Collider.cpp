#include "Collider.h"

#include "../core/Transform.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"
#include "../physics/Rigidbody.h"

#include <memory>

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson::Physics {
	
	Collider::Collider(const std::shared_ptr<ECS::GameObject>& _parent) noexcept : ECS::Component(_parent),
			m_CollisionShape(nullptr),
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
	
} // LouiEriksson::Physics