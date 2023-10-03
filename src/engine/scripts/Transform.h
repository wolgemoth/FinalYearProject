#pragma once

#ifndef FINALYEARPROJECT_TRANSFORM_H
#define FINALYEARPROJECT_TRANSFORM_H

#include "Component.h"

namespace LouiEriksson {
	
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
	
		explicit Transform(std::weak_ptr<GameObject> _parent);
	
		/// <summary> Get this Transform as a Transform, Rotation, Scale matrix.</summary>
		[[nodiscard]] glm::mat4 TRS() const;
	};
}

#endif //FINALYEARPROJECT_TRANSFORM_H