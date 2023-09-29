#pragma once

#ifndef _BALL_H
#define _BALL_H

#include "Scene.h"

/// <summary>
/// Ball object with Rigidbody physics.
/// </summary>
class Ball final : public Script {

friend class Scene;

protected:

	/// <summary> Material of the Ball. </summary>
	std::shared_ptr<Material> m_Material;

	/// <summary> Called at the beginning of the first frame. </summary>
	void Begin();

	/// <summary> Called every frame. </summary>
	void Tick();

public:

	 Ball(std::weak_ptr<GameObject> _parent);
	~Ball();

};

#endif // !_BALL_H
