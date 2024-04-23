#ifndef FINALYEARPROJECT_LIGHT_HPP
#define FINALYEARPROJECT_LIGHT_HPP

#include "../core/Debug.hpp"
#include "../core/Transform.hpp"
#include "../ecs/Component.hpp"
#include "../ecs/GameObject.hpp"

#include "Texture.hpp"
#include "textures/Cubemap.hpp"
#include "textures/RenderTexture.hpp"

#include <GL/glew.h>

#include <cmath>
#include <memory>
#include <string>

namespace LouiEriksson::Engine::Graphics {
	
	class Camera;
	class Shader;
	
	class Light final : public ECS::Component {
	
		friend Camera;
		
	public:
		
		struct Parameters final {
			
			friend Camera;
			friend Light;
			friend Shader;
			
		public:
			
			/**
			 * @enum Type
			 * @brief Enum representing different types of lights.
			 */
			enum Type : unsigned char {
				      Point = 0U,       /**< @brief Light is a Point Light. */
				Directional = 1U << 0U, /**< @brief Light is a Directional Light. */
				       Spot = 1U << 1U, /**< @brief Light is a Spot Light.  */
			};
		
		private:
			
			class ShadowMap final {

			public:
				
				GLuint m_ShadowMap_Texture;
				GLuint m_ShadowMap_FBO;
				GLenum m_Target;
				
				int m_Resolution;
				
				scalar_t m_Bias;
				scalar_t m_NormalBias;
				scalar_t m_NearPlane;
				
				bool m_TwoSided;
				
				glm::mat4 m_Projection;
				glm::mat4 m_ViewProjection;
				
				ShadowMap() noexcept :
					m_ShadowMap_Texture(GL_NONE),
					m_ShadowMap_FBO    (GL_NONE),
					m_Target           (GL_NONE),
					m_Resolution       (128    ),
					m_Bias             (  0.01),
					m_NormalBias       (  0.02),
					m_NearPlane        (  0.2 ),
					m_TwoSided         (  true),
					m_Projection       (  1.0 ),
					m_ViewProjection   (  1.0 ) {}
				 
				~ShadowMap() {
					Dispose();
				}
				
				void UpdateShadowMap(const Light::Parameters::Type& _type) {
					
					// Shadow implementation is very heavily modified derivative of implementations by Learn OpenGL:
			        //  - de Vries, J. (n.d.). LearnOpenGL - Shadow Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping [Accessed 15 Dec. 2023].
			        //  - de Vries, J. (n.d.). LearnOpenGL - Point Shadows. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows [Accessed 15 Dec. 2023].
					
					// Check if shadows are enabled.
					if (m_Resolution > 0) {
					
						const GLenum new_target = _type == Light::Parameters::Type::Point ?
								GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
						
						/*
						 * Check for differences between the desired parameters
						 * and the shadow map's current state.
						 */
						if (m_Target != new_target) {
							Dispose();
							m_Target = new_target;
						}
						else if (m_ShadowMap_Texture != GL_NONE) {
						
							glBindTexture(m_Target, Texture::s_CurrentTexture = static_cast<GLint>(m_ShadowMap_Texture));
							
							// Check the texture's resolution.
							int curr_resolution;
							glGetTexLevelParameteriv(_type == Light::Parameters::Type::Point ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &curr_resolution);
							
							if (curr_resolution != m_Resolution) {
								Dispose();
								m_Target = new_target;
							}
						}
						
						// Check to see if the shadow map doesn't exist.
						if (m_ShadowMap_Texture == GL_NONE) {
							
							// Initialise the texture.
							glGenTextures(1, &m_ShadowMap_Texture);
						
							// Generate texture for shadow map (will bind it to the FBO).
							glBindTexture(m_Target, Texture::s_CurrentTexture = static_cast<GLint>(m_ShadowMap_Texture));
							
							if (m_Target == GL_TEXTURE_CUBE_MAP) {
								
								// Generate all six faces of a cubemap (for omnidirectional shadows).
								for (size_t i = 0U; i < 6U; ++i) {
			                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_Resolution, m_Resolution, 0, GL_DEPTH_COMPONENT, GL_HALF_FLOAT, nullptr);
								}
							}
							else {
								
								// Generate a single face (for directional shadows).
								glTexImage2D(m_Target, 0, GL_DEPTH_COMPONENT, m_Resolution, m_Resolution, 0, GL_DEPTH_COMPONENT, GL_HALF_FLOAT, nullptr);
							}
							
							// Set the texture's parameters.
							glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
							glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
							glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
							glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
							glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
							
							if (_type != Light::Parameters::Type::Point) {
								
								const float borderColor[] { 1.0, 1.0, 1.0, 1.0 };
								glTexParameterfv(m_Target, GL_TEXTURE_BORDER_COLOR, borderColor);
							}
							
							// Create FBO for depth.
							glGenFramebuffers(1, &m_ShadowMap_FBO);
							
							// Bind shadow texture to FBO.
							RenderTexture::Bind(m_ShadowMap_FBO);
							
							glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMap_Texture, 0);
							
							// Explicitly tell opengl that we're not rendering any color data in this FBO.
							glDrawBuffer(GL_NONE);
							glReadBuffer(GL_NONE);
						}
					}
					else {
						
						// If shadows are not enabled, dispose of the shadow map.
						Dispose();
					}
				}
				
			private:
				
				void Dispose() {
					
					/* Delete FBO and texture. */
					
					if (m_ShadowMap_FBO != GL_NONE) {
						RenderTexture::Unbind(); // Unbind FBO as a precaution before deleting.
						
						glDeleteFramebuffers(1, &m_ShadowMap_FBO);
						m_ShadowMap_FBO = GL_NONE;
					}
					if (m_ShadowMap_Texture != GL_NONE) {
						
						switch (m_Target) {
							case GL_TEXTURE_2D:       { Texture::Unbind(); break; }
							case GL_TEXTURE_CUBE_MAP: { Cubemap::Unbind(); break; }
							default: {
								Debug::Log("Unknown target \"" + std::to_string(m_Target) + "\"", LogType::Error);
							}
						}
						
						glDeleteTextures(1, &m_ShadowMap_Texture);
						
						m_ShadowMap_Texture = GL_NONE;
						m_Target            = GL_NONE;
					}
				}
			};
		};
		
		explicit Light(const std::weak_ptr<ECS::GameObject>& _parent) : ECS::Component(_parent),
			m_Type     (Light::Parameters::Type::Point),
			m_Intensity(  3.0),
			m_Range    (200.0),
			m_Angle    (120.0),
			m_Size     (  0.2),
			m_Color    (glm::vec3(1, 1, 1))
		{
			if (const auto p = Parent().lock()) {
				m_Transform = p->GetComponent<Transform>();
			}
			else {
				Debug::Log("Light has no Transform!", LogType::Error);
			}
			
			Type(m_Type);
		}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Light); };
		
		void Type(const Light::Parameters::Type& _type) {
			
			// Configure the light projection matrix.
			switch (_type) {
				
				case Light::Parameters::Type::Point: {
					
					m_Shadow.m_Projection = glm::perspective(
						static_cast<GLfloat>(glm::radians(90.0)),
						static_cast<GLfloat>(1.0),
						m_Shadow.m_NearPlane,
						m_Range
					);
					
					break;
				}
				case Light::Parameters::Type::Directional: {
					
					m_Shadow.m_Projection = glm::ortho(
					   -m_Range / static_cast<GLfloat>(2.0),
					    m_Range / static_cast<GLfloat>(2.0),
					   -m_Range / static_cast<GLfloat>(2.0),
					    m_Range / static_cast<GLfloat>(2.0),
						m_Shadow.m_NearPlane,
						m_Range
					);
					
					break;
				}
				case Light::Parameters::Type::Spot: {
					
					m_Shadow.m_Projection = glm::perspective(
						glm::radians(m_Angle),
						static_cast<GLfloat>(1.0),
						m_Shadow.m_NearPlane,
						m_Range
					);
					
					break;
				}
				default: {
					
					m_Shadow.m_Projection = glm::mat4(1.0);
					
					Debug::Log("Not implemented!", LogType::Error);
				}
			}
			
			m_Type = _type;
		}
		
		[[nodiscard]] const Parameters::Type& Type() const noexcept {
			return m_Type;
		}
		
	private:
		
		std::weak_ptr<Transform> m_Transform;
		
		Light::Parameters::Type      m_Type;
		Light::Parameters::ShadowMap m_Shadow;
		
		GLfloat m_Intensity;
		GLfloat m_Range;
		GLfloat m_Angle;
		GLfloat m_Size;
		
		glm::vec3 m_Color;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_LIGHT_HPP