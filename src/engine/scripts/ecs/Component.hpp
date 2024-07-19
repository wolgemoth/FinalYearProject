#ifndef FINALYEARPROJECT_COMPONENT_HPP
#define FINALYEARPROJECT_COMPONENT_HPP

#include "../core/Debug.hpp"

#include <cstddef>
#include <memory>
#include <typeindex>

namespace LouiEriksson::Engine {
	
	class Transform;
}

namespace LouiEriksson::Engine::ECS {
	
	class GameObject;
	
	/**
	 * @class Component
	 * @brief Represents a Component which can be attached to a Parent.
	 *
	 * The Component class represents a component that can be attached to a Parent.
	 * It provides functionality for getting the parent Parent and the derived type ID of the component.
	 */
	class Component {
		
		friend GameObject;
	
	private:
	
		/** @brief Index of the component within its category in the Parent. Used for tidying up during destruction. */
		size_t m_Index;
	
		/** @brief Parent GameObject that contains the component. */
		std::weak_ptr<GameObject> m_GameObject;
		
	protected:
	
		explicit Component(const std::weak_ptr<GameObject>& _parent) noexcept :
			m_Index(0U),
			m_GameObject(_parent)
		{
			Debug::Assert(!_parent.expired(), "Component initialised with no valid parent!", Warning);
		}
		
		/** @brief Invoked on finalisation of the Component. */
		virtual ~Component() = default;
	
	public:
		
		/**
		 * @brief Return the type_index of this Component's type.
		 *
		 * This function returns a `std::type_index` representing the type of the component.
		 *
		 * @return A `std::type_index` representing the type of the class.
		 */
		[[nodiscard]] virtual inline std::type_index TypeID() const noexcept = 0;
		
		/**
		 * @brief Get the Component's parent Parent.
		 *
		 * This function returns a `std::weak_ptr` referencing the parent Parent that contains the component.
		 *
		 * @return A `std::weak_ptr<Parent>` referencing the parent GameObject.
		 */
		[[nodiscard]] std::shared_ptr<GameObject> Parent() const noexcept {
			return m_GameObject.lock();
		}
	};
	
} // LouiEriksson::Engine::ECS

#endif //FINALYEARPROJECT_COMPONENT_HPP