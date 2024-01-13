#include "GameObject.h"

#include "../utils/Hashmap.h"

#include <any>
#include <memory>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	GameObject::GameObject(const std::weak_ptr<Scene>& _scene, std::string _name) noexcept :
			m_Scene(_scene),
			m_Name (std::move(_name)) {}
	
	std::shared_ptr<GameObject> GameObject::Create(const std::shared_ptr<Scene>& _scene, const std::string& _name) {
		
		// NOTE: GameObject has private destructor as scene manages it. Lambda here is needed for smart pointer.
		return { new GameObject(_scene, _name), [](GameObject* _ptr) { delete _ptr; } };
	}
	
	const Hashmap<std::type_index, std::vector<std::any>>& GameObject::Components() const noexcept {
		return m_Components;
	}
	
	void GameObject::Name(const std::string& _name) noexcept {
		m_Name = _name;
	}
	const std::string& GameObject::Name() const noexcept {
		return m_Name;
	}
	
	const std::weak_ptr<Scene>& GameObject::GetScene() const noexcept {
		return m_Scene;
	}
	
} // LouiEriksson