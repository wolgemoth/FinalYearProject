#pragma once

#ifndef _SCRIPT_H
#define _SCRIPT_H

#include "Time.h"
#include "Component.h"

class Scene;

/// <summary>
/// Base class to be inherited by scriptable types.
/// </summary>
class Script : public Component {

friend class Scene;

protected:

	Script(std::weak_ptr<GameObject> _parent);

	virtual ~Script() = default;

	/// <summary> Called at the beginning of the first frame. </summary>
	virtual void Begin();

	/// <summary> Called every frame. </summary>
	virtual void Tick();

};

#endif // !_SCRIPT_H
