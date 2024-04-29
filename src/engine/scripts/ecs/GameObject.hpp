#ifndef FINALYEARPROJECT_GAMEOBJECT_HPP
#define FINALYEARPROJECT_GAMEOBJECT_HPP

#include "../core/Script.hpp"
#include "../core/utils/Hashmap.hpp"

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
	 * @class Parent
	 * @brief An Entity which exists in a Scene and can contain Components.
	 *
	 * The Parent class represents an entity that exists in a Scene and can contain Components.
	 */
	class GameObject final : public std::enable_shared_from_this<GameObject> {
		
		friend class Component;
		friend class Scene;
	
	private:
	
		/* @brief Whether or not the GameObject is active. */
		bool m_Active;
		
		/** @brief Scene the Parent belongs to. */
		std::weak_ptr<Scene> m_Scene;
	
		/** @brief Name of the Parent. */
		std::string m_Name;
	
		/** @brief Components attached to the Parent. */
		Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>> m_Components;
	
		/**
		 * @brief Attaches a component to the Parent.
		 *
		 * This function attaches a component of type T to the Parent. The provided type T must derive from the Component class.
		 *
		 * @tparam T The type of the component to attach.
		 * @param[in] _type The type index of the component.
		 * @param _component A shared pointer to the component to attach.
		 */
		template <typename T>
		void Attach(std::shared_ptr<T>&& _component) {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			const auto* category = std::is_base_of<Script, T>::value ? &typeid(Script) : &typeid(T);
			
			const auto existing = m_Components.Get(*category);
			
			if (existing.has_value()) {
			
				auto bucket = existing.value();
				bucket.emplace_back(_component);
				
				_component->m_Index = bucket.size() - 1U;
				
				m_Components.Assign(*category, std::move(bucket));
			}
			else {
				m_Components.Assign(*category, { _component });
			}
		}
		
		template <typename T>
		void Detach(const size_t& _index = 0U) {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			const auto* category = std::is_base_of<Script, T>::value ? &typeid(Script) : &typeid(T);
			
			if (const auto entries = m_Components.Get(*category)) {
	
				// Bounds check.
				if (_index < entries->size()) {
					
					auto itr = entries->begin() + static_cast<std::vector<std::shared_ptr<Component>>::difference_type>(_index);
					
					// Remove component from collection.
					const auto unsafe_collection = const_cast<std::vector<std::reference_wrapper<T>>>(entries);
					unsafe_collection.get().erase(itr);
				}
			}
			else {
				throw std::runtime_error("Component T with index \"" + std::to_string(_index) + "\" not found.");
			}
		}
		
		GameObject(const std::weak_ptr<Scene>& _scene, std::string _name) noexcept :
			m_Active(true),
			m_Scene(_scene),
			m_Name (std::move(_name)) {}
		
	public:
	
		GameObject(GameObject&& _other) noexcept :
			m_Active    (std::move(_other.m_Active    )),
			m_Scene     (std::move(_other.m_Scene     )),
			m_Name      (std::move(_other.m_Name      )),
			m_Components(std::move(_other.m_Components))
		{
			if (&_other != this) {
				_other.m_Scene.reset();
				_other.m_Components.Clear();
			}
		}
		
		GameObject             (const GameObject& _other) = delete;
		GameObject& operator = (const GameObject& _other) = delete;
		
		/**
		 * @brief Sets the active state of the GameObject.
		 *
		 * @param[in] _value The active state to set. True for active, false for inactive.
		 */
		void Active(const bool& _value) {
			m_Active = _value;
		}
		
		/**
		 * @brief Get the active state of the GameObject.
		 *
		 * @return A reference to the active state of the GameObject.
		 */
		constexpr const bool& Active() const noexcept {
			return m_Active;
		}
		
		/**
		 * @brief Set the name of the Parent.
		 *
		 * This function sets the name of the Parent.
		 *
		 * @param[in] _name The name to set.
		 */
		void Name(const std::string_view& _name) noexcept { m_Name = _name; }
	
		/** @brief Get the name of the Parent. */
		std::string_view Name() const noexcept {
			return m_Name;
		}
	
		/**
		 * @brief Get the scene to which the Parent belongs.
		 *
		 * This function returns a weak pointer to the scene to which the Parent belongs.
		 *
		 * @return A weak pointer to the scene.
		 */
		const std::shared_ptr<Scene> GetScene() const noexcept {
			
			std::shared_ptr<Scene> result;
			
			if (const auto s = m_Scene.lock()) {
				result = s;
			}
			
			return result;
		}
		
		/**
		 * @brief Get the Components attached to the Parent.
		 * @return The Components attached to the Parent.
		 */
		constexpr const Hashmap<std::type_index, std::vector<std::shared_ptr<Component>>>& Components() const noexcept {
			return m_Components;
		}
		
		/**
		 * @brief Get Components of type attached to Parent.
		 *
		 * This function returns a vector of weak_ptr objects of type T that are attached to the Parent.
		 * The provided type T must derive from the Component class.
		 *
		 * @tparam T The type of components to be searched.
		 * @return A vector of weak_ptr<T> objects.
		 */
		template <typename T>
		std::vector<std::weak_ptr<T>> GetComponents() const {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
	
			std::vector<std::weak_ptr<T>> result;
			
			if (auto category = m_Components.Get(typeid(T))) {
				result = dynamic_cast<std::vector<std::weak_ptr<T>>>(*category);
			}
	
			return result;
		}
		
		/**
		 * @fn template<typename T> std::weak_ptr<T> Parent::GetComponent(size_t _index) const
		 * @brief Get a Component of type in the Parent by index.
		 *
		 * This function returns a const std::weak_ptr<T> referencing the Component of type T in the Parent, specified by the index. If the Component is not found, it returns a std::weak_ptr<T> referencing a nullptr.
		 * The template parameter T must be a type derived from the Component class.
		 *
		 * @tparam T TypeID to be searched.
		 * @param[in] _index Index of the Component.
		 * @return const std::weak_ptr<T> Referencing the Component if successful, std::weak_ptr<T> referencing a nullptr if unsuccessful.
		 */
		template<typename T>
		std::shared_ptr<T> GetComponent(const size_t& _index = 0U) const {
			
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			std::shared_ptr<T> result;
			
			if (const auto category = m_Components.Get(typeid(T))) {
				result = std::dynamic_pointer_cast<T>((*category).at(_index));
			}
			
			return result;
		}
		
		/**
		 * @brief Add a Component of type to the Parent.
		 *
		 * This function adds a Component of type T to the Parent. The type T must derive from the Component class.
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
			std::shared_ptr<T> result(new T(weak_from_this()));
			
			try {
				Attach(std::move(result)); // Attach the component to the GameObject.
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
			
			// Return a weak reference to the component.
			return result;
		}
		
		/**
		 * @brief Remove a Component from the Parent by type and index.
		 *
		 * @tparam T The type of the Component to remove.
		 * @param[in] _index The index of the Component to remove if there are multiple instances of the same type. The default value is 0.
		 */
		template <typename T>
		void RemoveComponent(const size_t& _index = 0U) {
	
			static_assert(std::is_base_of<Component, T>::value, "Provided type must derive from \"Component\".");
			
			try {
				Detach<T>(_index); // Detach the component from the GameObject.
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
	};
	
} // LouiEriksson::Engine::ECS

#endif //FINALYEARPROJECT_GAMEOBJECT_HPP