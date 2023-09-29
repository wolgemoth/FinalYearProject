#pragma once

#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "Component.h"

/// <summary>
/// Component representing a Transform.
/// </summary>
struct Transform : public Component {

	/// <summary> Position of the Transform. </summary>
	glm::vec3 m_Position;

	/// <summary> Rotation of the Transform. </summary>
	glm::quat m_Rotation;

	/// <summary> Scale of the Transform. </summary>
	glm::vec3 m_Scale;

	Transform(std::weak_ptr<GameObject> _parent);

	/// <summary> Get this Transform as a Transform, Rotation, Scale matrix.</summary>
	glm::mat4 TRS();
};

#endif // !_TRANSFORM_H
