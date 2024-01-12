#ifndef FINALYEARPROJECT_GAMEOBJECT_H
#define FINALYEARPROJECT_GAMEOBJECT_H

#include "../utils/Hashmap.h"

#include "Component.h"

#include <cstddef>
#include <exception>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class Scene;
	
	class Entity final {
	
		friend class Component;
		friend class Scene;
	
	private:
	
		std::shared_ptr<Scene> m_Scene;
	
		std::string m_Name;
	
		Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>> m_Components;
	
		 Entity() = default;
		~Entity() = default;
	
	public:
	
		void Name(const std::string& _name);
		const std::string& Name();
	
		std::shared_ptr<Scene> GetScene();
	
		static std::shared_ptr<Entity> Create(std::shared_ptr<Scene> _scene, std::string _name = "") {
	
			std::shared_ptr<Entity> result(new Entity(), [](Entity* _ptr) { delete _ptr; });
			result->m_Scene = std::move(_scene);
			result->m_Name  = std::move(_name);
	
			return result;
		}
	
		Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>> Components() {
			return m_Components;
		}
	
		template <typename T>
		std::vector<std::shared_ptr<T>> GetComponents() {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			std::vector<std::shared_ptr<T>> result;
	
			std::vector<std::shared_ptr<Component>> category;
			if (m_Components.Get(typeid(T), category)) {
				result = static_cast<std::vector<std::shared_ptr<T>>>(result);
			}
	
			return result;
		}
	
		template <typename T>
		std::shared_ptr<T> GetComponent(size_t _index = 0) {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			std::shared_ptr<T> result(nullptr);
	
			std::vector<std::shared_ptr<Component>> category;
			if (m_Components.Get(typeid(T), category)) {
				result = std::static_pointer_cast<T>(category.at(_index));
			}
	
			return result;
		}
	
		template <typename T>
		std::shared_ptr<T> AddComponent() {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			// Create a new instance of the component, taking a pointer to this entity.
			std::shared_ptr<T> result(new T(std::shared_ptr<Entity>(this, [](Entity* _ptr) { delete _ptr; })));
	
			std::vector<std::shared_ptr<Component>> category;
			bool exists = m_Components.Get(typeid(T), category);
			
			category.push_back(result);
	
			if (exists) {
				result->m_Index = category.size() - 1;
			}
			else if (!m_Components.Add(typeid(T), category)) {
				result.reset();
	
				throw std::runtime_error("ERROR (Entity.cpp [AddComponent<T>()]): Failed adding new Component to hashmap!");
			}
	
			return result;
		}
	
		template <typename T>
		void RemoveComponent(size_t _index = 0) {
	
			std::vector<std::shared_ptr<Component>> category;
			if (m_Components.Get(typeid(T), category)) {
	
				// Bounds check.
				if (_index < category.size()) {
	
					// Remove component from category.
					category.erase(category.begin() + (std::vector<std::shared_ptr<Component>>::difference_type)_index);
				}
			}
		}
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_GAMEOBJECT_H