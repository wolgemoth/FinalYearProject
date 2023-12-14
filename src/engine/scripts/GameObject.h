#ifndef FINALYEARPROJECT_GAMEOBJECT_H
#define FINALYEARPROJECT_GAMEOBJECT_H

#include "stdafx.h"

#include "Component.h"

namespace LouiEriksson {
	
	class Scene;
	class Script;
	class Component;
	
	/// <summary> An Entity which exists in a Scene and can contain Components. </summary>
	class GameObject final : public std::enable_shared_from_this<GameObject> {
		
		friend class Component;
		friend class Scene;
	
	private:
	
		/// <summary> Scene the GameObject belongs to. </summary>
		std::weak_ptr<Scene> m_Scene;
	
		/// <summary> Name of the GameObject. </summary>
		std::string m_Name;
	
		/// <summary> Components attached to the GameObject. </summary>
		Hashmap<std::type_index, std::vector<std::any>> m_Components;
	
		 GameObject() = default;
		~GameObject() = default;
	
	public:
	
		/// <summary> Set the name of the GameObject. </summary>
		void Name(const std::string& _name);
	
		/// <summary> Get the name of the GameObject. </summary>
		const std::string& Name();
	
		/// <summary> Get the Scene the GameObject belongs to. </summary>
		std::shared_ptr<Scene> GetScene();
		
		/// <summary> Factory function which creates a GameObject within a Scene. </summary>
		[[nodiscard]] static std::shared_ptr<GameObject> Create(const std::shared_ptr<Scene>& _scene, const std::string& _name = "");
		
		/// <summary>
		/// Get the components attached to the GameObject.
		/// </summary>
		Hashmap<std::type_index, std::vector<std::any>>& Components();
	
		/// <summary>
		/// Get components of type attached to GameObject.
		/// </summary>
		/// <typeparam name="T">Type to be searched.</typeparam>
		/// <returns>Vector of std::any wrapping a std::shared_ptr<T></returns>
		template <typename T>
		std::vector<std::any> GetComponents() {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			std::vector<std::any> result, category;
	
			if (m_Components.Get(typeid(T), category)) {
				result = static_cast<std::vector<std::any>>(result);
			}
	
			return result;
		}
		
		/// <summary>
		/// Get a Component of type in the GameObject by index.
		/// </summary>
		/// <typeparam name="T">Type to be searched.</typeparam>
		/// <param name="_index">Index of the component.</param>
		/// <returns>std::shared_ptr<T> Referencing the component if successful. std::weak_ptr<T> referencing a nullptr if unsuccessful.</returns>
		template<typename T>
		std::shared_ptr<T> GetComponent(size_t _index = 0) {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			std::shared_ptr<T> result(nullptr);
			
			std::vector<std::any> category;
			if (m_Components.Get(typeid(T), category)) {
				result = std::any_cast<std::shared_ptr<T>>(category.at(_index));
			}
			
			return result;
		}
	
		/// <summary>
		/// Add a Component of type to the GameObject.
		/// </summary>
		/// <typeparam name="T">Type to be added.</typeparam>
		/// <returns>std::shared_ptr<T> referencing the created type.</returns>
		template <typename T>
		std::shared_ptr<T> AddComponent() {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			// Create a new instance of the component, taking a pointer to this gameobject.
			auto result = std::shared_ptr<T>(new T(shared_from_this()));
			
			std::type_index category = typeid(T);
			
			// If the provided type is of "Script", change the category to "Script".
			if (std::is_base_of<Script, T>::value) {
				category = typeid(Script);
			}
			
			std::vector<std::any> entries;
			const bool exists = m_Components.Get(category, entries);
			
			entries.push_back(result);
			
			if (exists) {
				result->m_Index = entries.size() - 1;
			}
			else if (!m_Components.Add(category, entries)) {
				result.reset();
				
				throw std::runtime_error("ERROR (GameObject.cpp [AddComponent<T>()]): Failed adding new Component to hashmap!");
			}
			
			return result;
		}
	
		/// <summary>
		/// Remove a Component of type from the GameObject, using an index.
		/// </summary>
		/// <typeparam name="T">Type to be searched.</typeparam>
		/// <param name="_index">Index of the component.</param>
		template <typename T>
		void RemoveComponent(size_t _index = 0) {
	
			std::vector<std::any> entries;
			if (m_Components.Get(typeid(T), entries)) {
	
				// Bounds check.
				if (_index < entries.size()) {
	
					// Remove component from category.
					entries.erase(entries.begin() + (std::vector<std::shared_ptr<Component>>::difference_type)_index);
				}
			}
		}
	};
}

#endif //FINALYEARPROJECT_GAMEOBJECT_H