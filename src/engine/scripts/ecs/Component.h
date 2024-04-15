#ifndef FINALYEARPROJECT_COMPONENT_H
#define FINALYEARPROJECT_COMPONENT_H

#include <cstddef>
#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
	/**
	 * @class Component
	 * @brief Represents a Component which can be attached to a GameObject.
	 *
	 * The Component class represents a component that can be attached to a GameObject.
	 * It provides functionality for getting the parent GameObject and the derived type ID of the component.
	 */
	class Component {
		
		friend GameObject;
	
	private:
	
		/** @brief Index of the component within its category in the GameObject. Used for tidying up during destruction. */
		size_t m_Index;
	
		/** @brief Parent GameObject that contains the component. */
		std::weak_ptr<GameObject> m_GameObject;
		
	protected:
	
		explicit Component(const std::weak_ptr<GameObject>& _parent) noexcept;
		
		/** @brief Invoked on finalisation of the Component. */
		virtual ~Component();
	
	public:
		
		/**
		 * @brief Return the type_index of this Component's type.
		 *
		 * This function returns a `std::type_index` representing the type of the component.
		 *
		 * @return A `std::type_index` representing the type of the class.
		 */
		[[nodiscard]] virtual std::type_index TypeID() const noexcept = 0;
		
		/**
		 * @brief Get the Component's parent GameObject.
		 *
		 * This function returns a `std::weak_ptr` referencing the parent GameObject that contains the component.
		 *
		 * @return A `std::weak_ptr<GameObject>` referencing the parent GameObject.
		 */
		[[nodiscard]] virtual const std::weak_ptr<GameObject>& Parent() const noexcept;
		
	};
	
} // LouiEriksson::Engine::ECS

#endif //FINALYEARPROJECT_COMPONENT_H