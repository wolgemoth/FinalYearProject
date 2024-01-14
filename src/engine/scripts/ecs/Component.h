#ifndef FINALYEARPROJECT_COMPONENT_H
#define FINALYEARPROJECT_COMPONENT_H

#include <cstddef>
#include <memory>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson::ECS {
	
	class GameObject;
	
	/// <summary> Represents a Component which can be attached to a GameObject. </summary>
	class Component {
		
		friend GameObject;
	
	private:
	
		/// <summary>
		/// Index of the component within its category in the GameObject. Used for tidying up during destruction.
		/// </summary>
		size_t m_Index;
	
		/// <summary>
		/// Parent GameObject which contains the component.
		/// </summary>
		const std::weak_ptr<GameObject> m_GameObject;
		
	protected:
	
		explicit Component(const std::weak_ptr<GameObject>& _parent) noexcept;
		virtual ~Component();
	
	public:
	
		/// <summary> Get the Component's parent GameObject. </summary>
		[[nodiscard]] virtual const std::weak_ptr<GameObject>& Parent() const noexcept;
		
	};
	
} // LouiEriksson::ECS

#endif //FINALYEARPROJECT_COMPONENT_H