#include "stdafx.h"

#include "Light.h"

Light::Light(std::weak_ptr<GameObject> _parent) : Component(_parent) {

}

void Light::SetTransform(std::weak_ptr<Transform> _transform) {
	m_Transform = _transform.lock();
}
std::weak_ptr<Transform> Light::GetTransform() {
	return m_Transform;
}
