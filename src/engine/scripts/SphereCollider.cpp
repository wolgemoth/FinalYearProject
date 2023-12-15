#include "stdafx.h"

#include "SphereCollider.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	SphereCollider::SphereCollider(const std::shared_ptr<GameObject>& _parent) : Collider(_parent) {
		m_Radius = 1.0f;
	}
	SphereCollider::~SphereCollider() = default;
	
	void SphereCollider::Radius(const float& _radius) {
		m_Radius = _radius;
	}
	float SphereCollider::Radius() const {
		return m_Radius;
	}
}