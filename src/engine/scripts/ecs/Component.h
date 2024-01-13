#ifndef FINALYEARPROJECT_COMPONENT_H
#define FINALYEARPROJECT_COMPONENT_H

#include "GameObject.h"

#include <cstddef>
#include <memory>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class GameObject;

	/// <summary> Represents a Component which can be attached to a GameObject. </summary>
	class Component {
		
		friend class GameObject;
	
	private:
	
		/// <summary>
		/// Index of the component within its category in the GameObject. Used for tidying up during destruction.
		/// </summary>
		size_t m_Index;
	
		/// <summary>
		/// Parent GameObject which contains the component.
		/// </summary>
		std::weak_ptr<GameObject> m_GameObject;
		
	protected:
	
		explicit Component(const std::shared_ptr<GameObject>& _parent) noexcept;
		virtual ~Component();
	
	public:
	
		/// <summary> Get the Component's parent GameObject. </summary>
		[[nodiscard]] virtual std::shared_ptr<GameObject> Parent() const;
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_COMPONENT_H