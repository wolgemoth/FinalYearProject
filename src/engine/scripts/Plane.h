#pragma once

#ifndef _PLANE_H
#define _PLANE_H

#include "Scene.h"

/// <summary>
/// A Plane with Collision.
/// </summary>
class Plane final : public Script {

	friend class Scene;

protected:

	/// <summary> Material of the Plane. </summary>
	std::shared_ptr<Material> m_Material;

	/// <summary> Called at the beginning of every frame. </summary>
	void Begin();

	/// <summary> Called every frame. </summary>
	void Tick();

public:

	Plane(std::weak_ptr<GameObject> _parent);
	~Plane();

};
#endif // !_PLANE_H
