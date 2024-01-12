#include "GameObject.h"

#include "../utils/Hashmap.h"

#include <any>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	std::shared_ptr<GameObject> GameObject::Create(const std::shared_ptr<Scene>& _scene, const std::string& _name) {
		
		// NOTE: GameObject has private destructor as scene manages it. Lambda here is needed for smart pointer.
		std::shared_ptr<GameObject> result(new GameObject(), [](GameObject* _ptr) { delete _ptr; });
		result->m_Scene = _scene;
		result->m_Name = _name;
		
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
	
	std::shared_ptr<Scene> GameObject::GetScene() {
		return m_Scene.lock();
	}
}