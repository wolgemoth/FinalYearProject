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
			
		private:
			
			struct Shadow {
				
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
				
				int m_Resolution;
				
				float m_Bias;
				float m_NormalBias;
				
				bool m_TwoSided;
				
				glm::mat4 m_View;
				
				Shadow() {
					m_Resolution = Light::Parameters::Shadow::Resolution::Medium;
					m_Bias       = 0.005f;
					m_NormalBias = 0.05f;
					m_TwoSided   = false;
					m_View       = glm::mat4(1.0);
				}
				
				void Draw();
			};
			
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
		};
		
		explicit Light(const std::shared_ptr<GameObject>& _parent);
		
		~Light() override = default;
		
		void Type(Light::Parameters::Type _type);
		
		Light::Parameters::Type Type();
		
	private:
		
		Light::Parameters::Type   m_Type;
		Light::Parameters::Shadow m_ShadowSettings;
		
		float m_Range;
	};
}

#endif //FINALYEARPROJECT_LIGHT_H