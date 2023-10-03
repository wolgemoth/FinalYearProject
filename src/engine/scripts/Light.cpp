#include "stdafx.h"

#include "Light.h"

namespace LouiEriksson {
	
	Light::Light(std::weak_ptr<GameObject> _parent) : Component(std::move(_parent)) {
	
	}
	
	void Light::SetTransform(const std::weak_ptr<Transform>& _transform) {
		m_Transform = _transform.lock();
	}
	std::weak_ptr<Transform> Light::GetTransform() {
		return m_Transform;
	}
}