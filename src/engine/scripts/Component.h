#pragma once

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "GameObject.h"

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

	Component(std::weak_ptr<GameObject> _parent);
	virtual ~Component();

public:

	/// <summary> Get the Component's parent GameObject. </summary>
	virtual std::weak_ptr<GameObject> GetGameObject();
};

#endif // !_COMPONENT_H
