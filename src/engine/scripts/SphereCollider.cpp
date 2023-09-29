#include "stdafx.h"

#include "SphereCollider.h"

SphereCollider::SphereCollider(std::weak_ptr<GameObject> _parent) : Collider(_parent) {
	
}

void SphereCollider::Radius(const float& _radius) {
	m_Radius = _radius;
}
float SphereCollider::Radius() {
	return m_Radius;
}