#ifndef FINALYEARPROJECT_GAMEOBJECT_H
#define FINALYEARPROJECT_GAMEOBJECT_H

#include "../core/Script.h"
#include "../core/utils/Hashmap.h"

#include "Scene.h"

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <vector>

namespace LouiEriksson::Engine::ECS {
	
	class Component;
	
	/// <summary> An Entity which exists in a Scene and can contain Components. </summary>
	class GameObject final : public std::enable_shared_from_this<GameObject> {
		
		friend Component;
		friend Scene;
	
	private:
	
		/// <summary> Scene the GameObject belongs to. </summary>
		const std::weak_ptr<Scene> m_Scene;
	
		/// <summary> Name of the GameObject. </summary>
		std::string m_Name;
	
		/// <summary> Components attached to the GameObject. </summary>
		Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>> m_Components;
	
		/// <summary>
		/// Attach a component to the GameObject.
		/// </summary>
		template <typename T>
		void Attach(const std::type_index _type, const std::shared_ptr<T>& _component) {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			std::type_index category = typeid(T);
			
			std::vector<std::shared_ptr<Component>> entries;
			const bool exists = m_Components.Get(category, entries);
			
			entries.emplace_back(_component);
			
			m_Components.Assign(category, entries);
			
			if (exists) {
				_component->m_Index = entries.size() - 1;
			}
			else {
				
				// Attach to scene:
				if (const auto s = GetScene().lock()) {
					
					/*
					 * If type derives from script, attach it as a script.
					 * Otherwise, attach it as current type.
					 */
					if (std::is_base_of<Script, T>::value) {
						s->Attach<Script>(std::dynamic_pointer_cast<Script>(_component));
					}
					else {
						s->Attach(_component);
					}
				}
			}
		}
		
		 GameObject(const std::weak_ptr<Scene>& _scene, std::string _name) noexcept;
		~GameObject() = default;
		
	public:
	
		/// <summary> Set the name of the GameObject. </summary>
		void Name(const std::string& _name) noexcept;
	
		/// <summary> Get the name of the GameObject. </summary>
		const std::string& Name() const noexcept;
	
		/// <summary> Get the Scene the GameObject belongs to. </summary>
		const std::weak_ptr<Scene>& GetScene() const noexcept;
		
		/// <summary> Factory function which creates a GameObject within a Scene. </summary>
		[[nodiscard]] static std::shared_ptr<GameObject> Create(const std::shared_ptr<Scene>& _scene, const std::string& _name = "");
		
		void Destroy();
		
		/// <summary>
		/// Get the Components attached to the GameObject.
		/// </summary>
		const Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>>& Components() const noexcept;
	
		/// <summary>
		/// Get Components of type attached to GameObject.
		/// </summary>
		/// <typeparam name="T">TypeID to be searched.</typeparam>
		/// <returns>Vector of std::shared_ptr<T> wrapping a std::shared_ptr<T></returns>
		template <typename T>
		std::vector<const std::weak_ptr<T>> GetComponents() const {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			std::vector<std::shared_ptr<T>> result, category;
	
			if (m_Components.Get(typeid(T), category)) {
				result = dynamic_cast<std::vector<std::shared_ptr<T>>>(result);
			}
	
			return result;
		}
		
		/// <summary>
		/// Get a Component of type in the GameObject by index.
		/// </summary>
		/// <typeparam name="T">TypeID to be searched.</typeparam>
		/// <param name="_index">Index of the Component.</param>
		/// <returns>const std::weak_ptr<T> Referencing the Component if successful. std::weak_ptr<T> referencing a nullptr if unsuccessful.</returns>
		template<typename T>
		std::weak_ptr<T> GetComponent(size_t _index = 0) const {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			std::shared_ptr<T> result;
			
			std::vector<std::shared_ptr<Component>> category;
			if (m_Components.Get(typeid(T), category)) {
				result = std::dynamic_pointer_cast<T>(category.at(_index));
			}
			
			return result;
		}
	
		/// <summary>
		/// Add a Component of type to the GameObject.
		/// </summary>
		/// <typeparam name="T">TypeID to be added.</typeparam>
		/// <returns>const::weak_ptr<T>& referencing the created type.</returns>
		template <typename T>
		std::weak_ptr<T> AddComponent() {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			// Create a new instance of the component, taking a pointer to this gameobject.
			auto result = std::shared_ptr<T>(new T(shared_from_this()));
			
			// Attach the component to the GameObject.
			Attach(typeid(T), result);
			
			// Return a weak reference to the component.
			return result;
		}
		
		/// <summary>
		/// Remove a Component of type from the GameObject, using an index.
		/// </summary>
		/// <typeparam name="T">TypeID to be searched.</typeparam>
		/// <param name="_index">Index of the component.</param>
		template <typename T>
		void RemoveComponent(size_t _index = 0) {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			std::vector<std::shared_ptr<Component>> entries;
			if (m_Components.Get(typeid(T), entries)) {
	
				// Bounds check.
				if (_index < entries.size()) {
					
					auto itr = entries.begin() + (std::vector<std::shared_ptr<Component>>::difference_type)_index;
					
					// Detach from scene.
					if (const auto s = GetScene().lock()) {
						s->Detach<Component>(*itr);
					}
					else {
						std::cout << "No scene to detach component from.\n";
					}
					
					// Remove component from collection.
					entries.erase(itr);
				}
			}
		}
	};
	
} // LouiEriksson::Engine::ECS

#endif //FINALYEARPROJECT_GAMEOBJECT_H