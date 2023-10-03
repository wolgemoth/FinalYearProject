#include "stdafx.h"

#include "SphereCollider.h"

namespace LouiEriksson {
	
	SphereCollider::SphereCollider(std::weak_ptr<GameObject> _parent) : Collider(std::move(_parent)) {}
	SphereCollider::~SphereCollider() = default;
	
	void SphereCollider::Radius(const float& _radius) {
		m_Radius = _radius;
	}
	float SphereCollider::Radius() {
		return m_Radius;
	}
}