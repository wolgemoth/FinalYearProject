#include "GameObject.h"

#include "../core/Debug.h"
#include "../core/utils/Hashmap.h"

#include "Scene.h"

#include <exception>
#include <memory>
#include <string>
#include <string_view>
#include <typeindex>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::ECS {
	
	GameObject::GameObject(const std::weak_ptr<Scene>& _scene, std::string _name) noexcept :
			m_Scene(_scene),
			m_Name (std::move(_name)) {}
	
	std::shared_ptr<GameObject> GameObject::Create(const std::weak_ptr<Scene>& _scene, const std::string_view& _name) {
		
		std::shared_ptr<GameObject> result;
		
		if (const auto s = _scene.lock()) {
			
			// NOTE: GameObject has private destructor as scene manages it. Lambda here is needed for smart pointer.
			result = s->Attach<ECS::GameObject>({
				new GameObject(_scene, _name.data()), [](GameObject* _ptr) { delete _ptr; }
			});
		}
		
		return result;
	}
	
	const Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>>& GameObject::Components() const noexcept {
		return m_Components;
	}
	
	void GameObject::Name(const std::string_view& _name) noexcept {
		m_Name = _name;
	}
	std::string_view GameObject::Name() const noexcept {
		return m_Name;
	}
	
	const std::weak_ptr<Scene>& GameObject::GetScene() const noexcept {
		return m_Scene;
	}
	
	void GameObject::Destroy() {
		
		try {
			
			if (const auto scene = m_Scene.lock()) {
				
				for (const auto& bucket : Components()) {
					
					for (const auto& item : bucket.second) {
						scene->Detach<Component>(item);
					}
				}
				
				m_Components.Clear();
				
				scene->Detach<GameObject>(shared_from_this());
			}
			
		}
		catch (const std::exception& e) {
			Debug::Log(e);
		}
	}
	
} // LouiEriksson::Engine::ECS