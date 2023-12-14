#pragma once

#ifndef FINALYEARPROJECT_LIGHT_H
#define FINALYEARPROJECT_LIGHT_H

#include "stdafx.h"

#include "Component.h"
#include "Transform.h"

#include "RenderTexture.h"

class Camera;

namespace LouiEriksson {
	
	class Light : public Component {
	
		friend Camera;
		
	public:
		
		struct Parameters {
			
			friend Light;
			friend Camera;
			
		public:
			
			/// <summary> Type of the Light. </summary>
			enum Type {
				/// <summary> Light is a Point Light. </summary>
				Point,
				/// <summary> Light is a Directional Light. </summary>
				Directional,
				/// <summary> Light is a Spot Light. </summary>
				Spot,
			};
		
		private:
			
			struct Shadow {
				
				GLuint m_ShadowMap_Texture;
				GLuint m_ShadowMap_FBO;
				
				int m_Resolution;
				
				float m_Bias;
				float m_NormalBias;
				
				float m_NearPlane;
				
				bool m_TwoSided;
				
				glm::mat4 m_Projection;
				glm::mat4 m_ViewProjection;
				
				 Shadow();
				~Shadow();
				
				void UpdateShadowMap(const Light::Parameters::Type& _type);
				
			};
		};
		
		explicit Light(const std::shared_ptr<GameObject>& _parent);
		
		~Light() override;
		
		void Type(Light::Parameters::Type _type);
		
		Light::Parameters::Type Type();
		
	private:
		
		std::weak_ptr<Transform> m_Transform;
		
		Light::Parameters::Type   m_Type;
		Light::Parameters::Shadow m_Shadow;
		
		float m_Intensity;
		float m_Range;
		float m_Angle;
		float m_Size;
		
		glm::vec3 m_Color;
	};
}

#endif //FINALYEARPROJECT_LIGHT_H