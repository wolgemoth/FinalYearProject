#pragma once

#ifndef FINALYEARPROJECT_COMPONENT_H
#define FINALYEARPROJECT_COMPONENT_H

#include "GameObject.h"

namespace LouiEriksson {
	
	/// <summary> Represents a Component which can be attached to a GameObject. </summary>
	class Component {
	
		friend class GameObject;
	
	private:
	
		/// <summary>
		/// Index of the component within it's category in the GameObject. Used for tidying up during destruction.
		/// </summary>
		size_t m_Index;
	
		/// <summary>
		/// Parent GameObject which contains the component.
		/// </summary>
		std::weak_ptr<GameObject> m_GameObject;
		
	protected:
	
		explicit Component(std::weak_ptr<GameObject> _parent);
		virtual ~Component();
	
	public:
	
		/// <summary> Get the Component's parent GameObject. </summary>
		virtual std::weak_ptr<GameObject> GetGameObject();
	};
}

#endif //FINALYEARPROJECT_COMPONENT_H