#include "stdafx.h"

#include "Light.h"

namespace LouiEriksson {
	
	Light::Light(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
		
		Type(Light::Parameters::Type::Point);
	}
	
	void Light::Type(Light::Parameters::Type _type) {
		m_Type = _type;
	}
	Light::Parameters::Type Light::Type() {
		return m_Type;
	}
	
	void Light::Parameters::Shadow::Draw() {
	
	}
}