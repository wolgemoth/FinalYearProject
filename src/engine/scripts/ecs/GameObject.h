#ifndef FINALYEARPROJECT_GAMEOBJECT_H
#define FINALYEARPROJECT_GAMEOBJECT_H

#include "../core/Debug.h"
#include "../core/Script.h"
#include "../core/utils/Hashmap.h"

#include "Scene.h"

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::ECS {
	
	/**
	 * @class GameObject
	 * @brief An Entity which exists in a Scene and can contain Components.
	 *
	 * The GameObject class represents an entity that exists in a Scene and can contain Components.
	 */
	class GameObject final : public std::enable_shared_from_this<GameObject> {
		
		friend class Component;
		friend Scene;
	
	private:
	
		/** @brief Scene the GameObject belongs to. */
		const std::weak_ptr<Scene> m_Scene;
	
		/** @brief Name of the GameObject. */
		std::string m_Name;
	
		/** @brief Components attached to the GameObject. */
		Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>> m_Components;
	
		/**
		 * @brief Attaches a component to the GameObject.
		 *
		 * This function attaches a component of type T to the GameObject. The provided type T must derive from the Component class.
		 *
		 * @tparam T The type of the component to attach.
		 * @param[in] _type The type index of the component.
		 * @param _component A shared pointer to the component to attach.
		 */
		template <typename T>
		void Attach(std::shared_ptr<T>&& _component) {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			const std::type_index category = typeid(T);
			
			const auto existing = m_Components.Get(category);
			
			if (existing.has_value()) {
			
				auto bucket = existing.value();
				bucket.emplace_back(_component);
				
				_component->m_Index = bucket.size() - 1;
				
				m_Components.Emplace(typeid(T), std::move(bucket));
			}
			else {
				m_Components.Emplace(typeid(T), { _component });
			}
			
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
		
		 GameObject(const std::weak_ptr<Scene>& _scene, std::string _name) noexcept :
			m_Scene(_scene),
			m_Name (std::move(_name)) {}
		
	public:
	
		/**
		 * @brief Set the name of the GameObject.
		 *
		 * This function sets the name of the GameObject.
		 *
		 * @param[in] _name The name to set.
		 */
		inline void Name(const std::string_view& _name) noexcept { m_Name = _name; }
	
		/** @brief Get the name of the GameObject. */
		inline std::string_view Name() const noexcept {
			return m_Name;
		}
	
		/**
		 * @brief Get the scene to which the GameObject belongs.
		 *
		 * This function returns a weak pointer to the scene to which the GameObject belongs.
		 *
		 * @return A weak pointer to the scene.
		 */
		constexpr const std::weak_ptr<Scene>& GetScene() const noexcept {
			return m_Scene;
		}
		
		/**
		 * @brief Creates a new GameObject and attaches it to the specified Scene.
		 *
		 * This function creates a new GameObject and attaches it to the specified Scene.
		 *
		 * @param[in] _scene The Scene to attach the GameObject to.
		 * @param[in] _name The optional name of the GameObject.
		 * @return A shared pointer to the newly created GameObject.
		 */
		[[nodiscard]] static std::shared_ptr<GameObject> Create(const std::weak_ptr<Scene>& _scene, const std::string_view& _name = "")  {
			
			std::shared_ptr<GameObject> result;
			
			if (const auto s = _scene.lock()) {
				
				// NOTE: GameObject has private destructor as scene manages it. Lambda here is needed for smart pointer.
				result = s->Attach<ECS::GameObject>({
					new GameObject(_scene, _name.data()), [](GameObject* _ptr) { delete _ptr; }
				});
			}
			
			return result;
		}
		
		/**
		 * @fn void GameObject::Destroy()
		 * @brief Destroys the GameObject.
		 */
		void Destroy()  {
			
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
		
		/**
		 * @brief Get the Components attached to the GameObject.
		 * @return The Components attached to the GameObject.
		 */
		constexpr const Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>>& Components() const noexcept {
			return m_Components;
		}
		
		/**
		 * @brief Get Components of type attached to GameObject.
		 *
		 * This function returns a vector of weak_ptr<T> objects containing shared_ptr<T> components of type T that are attached to the GameObject.
		 * The provided type T must derive from the Component class.
		 *
		 * @tparam T The type of components to be searched.
		 * @return A vector of weak_ptr<T> objects.
		 */
		template <typename T>
		std::vector<std::weak_ptr<T>> GetComponents() const {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			std::vector<std::shared_ptr<T>> result, category;
	
			if (m_Components.Get(typeid(T), category)) {
				result = dynamic_cast<std::vector<std::shared_ptr<T>>>(result);
			}
	
			return result;
		}
		
		/**
		 * @fn template<typename T> std::weak_ptr<T> GameObject::GetComponent(size_t _index) const
		 * @brief Get a Component of type in the GameObject by index.
		 *
		 * This function returns a const std::weak_ptr<T> referencing the Component of type T in the GameObject, specified by the index. If the Component is not found, it returns a std::weak_ptr<T> referencing a nullptr.
		 * The template parameter T must be a type derived from the Component class.
		 *
		 * @tparam T TypeID to be searched.
		 * @param[in] _index Index of the Component.
		 * @return const std::weak_ptr<T> Referencing the Component if successful, std::weak_ptr<T> referencing a nullptr if unsuccessful.
		 */
		template<typename T>
		std::weak_ptr<T> GetComponent(const size_t& _index = 0) const {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			std::shared_ptr<T> result;
			
			if (const auto category = m_Components.Get(typeid(T))) {
				result = std::dynamic_pointer_cast<T>((*category).at(_index));
			}
			
			return result;
		}
		
		/**
		 * @brief Add a Component of type to the GameObject.
		 *
		 * This function adds a Component of type T to the GameObject. The type T must derive from the Component class.
		 *
		 * @tparam T The type of the Component to add.
		 * @return A const std::weak_ptr<T>& referencing the created type.
		 *
		 * @note The provided type must derive from the Component class.
		 */
		template <typename T>
		std::shared_ptr<T> AddComponent() {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			// Create a new instance of the component, taking a pointer to this gameobject.
			std::shared_ptr<T> result(new T(shared_from_this()));
			
			// Attach the component to the GameObject.
			Attach(std::move(result));
			
			// Return a weak reference to the component.
			return result;
		}
		
		/**
		 * @brief Remove a Component from the GameObject by type and index.
		 *
		 * @tparam T The type of the Component to remove.
		 * @param[in] _index The index of the Component to remove if there are multiple instances of the same type. The default value is 0.
		 */
		template <typename T>
		void RemoveComponent(const size_t& _index = 0) {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			if (const auto entries = m_Components.Get(typeid(T))) {
	
				// Bounds check.
				if (_index < entries->size()) {
					
					auto itr = entries->begin() + (std::vector<std::shared_ptr<Component>>::difference_type)_index;
					
					// Detach from scene.
					if (const auto s = GetScene().lock()) {
						s->Detach<Component>(*itr);
					}
					else {
						Debug::Log("No scene to detach component from.", LogType::Error);
					}
					
					// Remove component from collection.
					const auto __unsafe_c_ = const_cast<std::vector<std::reference_wrapper<T>>>(entries);
					__unsafe_c_.get().erase(itr);
				}
			}
		}
	};
	
} // LouiEriksson::Engine::ECS

#endif //FINALYEARPROJECT_GAMEOBJECT_H