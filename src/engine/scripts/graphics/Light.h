#ifndef FINALYEARPROJECT_LIGHT_H
#define FINALYEARPROJECT_LIGHT_H

#include "../ecs/Component.h"

#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>

#include <memory>

// @Assessor: This class has been changed since its submission for 3DGP. Please mark it for GEP.

namespace LouiEriksson {
	
	class Transform;
	
} // LouiEriksson

namespace LouiEriksson::Graphics {
	
	class Camera;
	
	class Light final : public ECS::Component {
	
		friend Camera;
		
	public:
		
		struct Parameters {
			
			friend Camera;
			friend Light;
			
		public:
			
			/// <summary> TypeID of the Light. </summary>
			enum Type : char {
				
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
				
				 Shadow() noexcept;
				~Shadow();
				
				void UpdateShadowMap(const Light::Parameters::Type& _type);
				
			};
			
		};
		
		explicit Light(const std::weak_ptr<ECS::GameObject>& _parent);
		~Light() override;
		
		std::type_index TypeID() const noexcept override { return typeid(Light); };
		
		void Type(const Light::Parameters::Type& _type);
		
		[[nodiscard]] const Parameters::Type& Type() const noexcept;
		
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
	
} // LouiEriksson::Graphics

#endif //FINALYEARPROJECT_LIGHT_H