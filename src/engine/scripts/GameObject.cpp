#include "stdafx.h"

#include "GameObject.h"

namespace LouiEriksson {
	
	std::shared_ptr<GameObject> GameObject::Create(std::weak_ptr<Scene> _scene, std::string _name) {
	
		// NOTE: GameObject has a private destructor as the scene manages it. Lambda used for smart pointer.
		std::shared_ptr<GameObject> result(new GameObject(), [](GameObject* _ptr) { delete _ptr; });
		result->m_Scene = std::move(_scene);
		result->m_Name = std::move(_name);
	
		return result;
	}
	
	Hashmap<std::type_index, std::vector<std::any>>& GameObject::Components() {
		return m_Components;
	}
	
	void GameObject::Name(const std::string& _name) {
		m_Name = _name;
	}
	const std::string& GameObject::Name() {
		return m_Name;
	}
	
	std::weak_ptr<Scene> GameObject::GetScene() {
		return m_Scene;
	}
}