#pragma once

#ifndef FINALYEARPROJECT_LIGHT_H
#define FINALYEARPROJECT_LIGHT_H

#include "Component.h"
#include "Transform.h"
#include "Camera.h"

namespace LouiEriksson {
	
	class Light : public Component {
	
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
				
				constexpr const static float SHADOW_DISTANCE = 100.0f;

				struct Resolution {
					
					/// <summary> 128x128. </summary>
					static const int ExtremelyLow = 128;
					
					/// <summary> 256x256. </summary>
					static const int VeryLow = 256;
					
					/// <summary> 512x512. </summary>
					static const int Low = 512;
					
					/// <summary> 1024x1024. </summary>
					static const int Medium = 1024;
					
					/// <summary> 2048x2048. </summary>
					static const int High = 2048;
					
					/// <summary> 4096x4096. </summary>
					static const int VeryHigh = 4096;
					
					/// <summary> 8192x8192. </summary>
					static const int ExtremelyHigh = 8192;
				};
				
				GLuint m_ShadowMap;
				
				int m_Resolution;
				
				float m_Bias;
				float m_NormalBias;
				
				bool m_TwoSided;
				
				glm::mat4 m_Projection;
				
				Shadow();
				
				void UpdateShadowMap();
			};
		};
		
		explicit Light(const std::shared_ptr<GameObject>& _parent);
		
		~Light() override = default;
		
		void Type(Light::Parameters::Type _type);
		
		Light::Parameters::Type Type();
		
		void Draw(const Camera& _camera, const std::vector<std::shared_ptr<Renderer>>& _renderers, const std::vector<std::shared_ptr<Light>>& _lights);
		
	private:
		
		std::weak_ptr<Transform> m_Transform;
		
		Light::Parameters::Type   m_Type;
		Light::Parameters::Shadow m_Shadow;
		
		float m_Range;
	};
}

#endif //FINALYEARPROJECT_LIGHT_H