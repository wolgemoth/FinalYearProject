#pragma once

#ifndef FINALYEARPROJECT_LIGHT_H
#define FINALYEARPROJECT_LIGHT_H

#include "Component.h"
#include "Transform.h"

namespace LouiEriksson {
	
	class Light : public Component {
	
	private:
		
		/// <summary> Transform of the Light. </summary>
		std::shared_ptr<Transform> m_Transform;
	
	public:
		
		explicit Light(std::weak_ptr<GameObject> _parent);
		~Light() override = default;
	
		/// <summary> Type of the Light. </summary>
		enum Type {
			/// <summary> Light is a Point Light. </summary>
			Point,
			/// <summary> Light is a Directional Light. </summary>
			Directional
		};
	
		/// <summary> Set the Transform of the Light. </summary>
		void SetTransform(const std::weak_ptr<Transform>& _transform);
	
		/// <summary> Get the Transform of the Light. </summary>
		std::weak_ptr<Transform> GetTransform();
	
	};
}

#endif //FINALYEARPROJECT_LIGHT_H