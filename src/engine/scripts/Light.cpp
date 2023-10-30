#include "stdafx.h"

#include "Light.h"

namespace LouiEriksson {
	
	Light::Light(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
	
		m_Type = Type::Point;
		
		m_Resolution = MEDIUM;
	}
	
	void Light::SetTransform(const std::weak_ptr<Transform>& _transform) {
		m_Transform = _transform.lock();
	}
	std::weak_ptr<Transform> Light::GetTransform() {
		return m_Transform;
	}
	
	void Light::Resolution(int _resolution) {
		m_Resolution = _resolution;
	}
	
	int Light::Resolution() {
		return m_Resolution;
	}
	
	void Light::GetType(Light::Type _type) {
		m_Type = _type;
	}
	Light::Type Light::SetType() {
		return m_Type;
	}
	
}