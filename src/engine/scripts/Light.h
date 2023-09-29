#pragma once

#ifndef _LIGHT_H
#define _LIGHT_H

#include "Component.h"
#include "Transform.h"

class Light : public Component {

private:
	
	/// <summary> Transform of the Light. </summary>
	std::shared_ptr<Transform> m_Transform;

public:
	
	 Light(std::weak_ptr<GameObject> _parent);
	~Light() = default;

	/// <summary> Type of the Light. </summary>
	enum Type {
		/// <summary> Light is a Point Light. </summary>
		Point,
		/// <summary> Light is a Directional Light. </summary>
		Directional
	};

	/// <summary> Set the Transform of the Light. </summary>
	void SetTransform(std::weak_ptr<Transform> _transform);

	/// <summary> Get the Transform of the Light. </summary>
	std::weak_ptr<Transform> GetTransform();

};

#endif // !_LIGHT_H
