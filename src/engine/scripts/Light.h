#pragma once

#ifndef FINALYEARPROJECT_LIGHT_H
#define FINALYEARPROJECT_LIGHT_H

#include "Component.h"
#include "Transform.h"

namespace LouiEriksson {
	
	class Light : public Component {
	
	public:
		
		explicit Light(const std::shared_ptr<GameObject>& _parent);
		
		~Light() override = default;
	
		/// <summary> Type of the Light. </summary>
		enum Type {
			/// <summary> Light is a Point Light. </summary>
			Point,
			/// <summary> Light is a Directional Light. </summary>
			Directional,
			/// <summary> Light is a Spot Light. </summary>
			Spot,
		};
	
		/// <summary> 128x128. </summary>
		#define EXTREMELY_LOW 128
		/// <summary> 256x256. </summary>
		#define VERY_LOW 256
		/// <summary> 512x512. </summary>
		#define LOW 512
		/// <summary> 1024x1024. </summary>
		#define MEDIUM 1024
		/// <summary> 2048x2048. </summary>
		#define HIGH 2048
		/// <summary> 4096x4096. </summary>
		#define VERY_HIGH 4096
		/// <summary> 8192x8192. </summary>
		#define EXTREMELY_HIGH 8192
		
		void Resolution(int _resolution);
		
		int Resolution();
		
		void GetType(Light::Type _type);
		
		Light::Type SetType();
		
		/// <summary> Set the Transform of the Light. </summary>
		void SetTransform(const std::weak_ptr<Transform>& _transform);
	
		/// <summary> Get the Transform of the Light. </summary>
		std::weak_ptr<Transform> GetTransform();
	
	private:
		
		Light::Type m_Type;
		
		int m_Resolution;
		
		/// <summary> Transform of the Light. </summary>
		std::shared_ptr<Transform> m_Transform;
		
	};
}

#endif //FINALYEARPROJECT_LIGHT_H