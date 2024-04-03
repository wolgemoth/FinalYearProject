#include "Light.h"

#include "../core/Transform.h"
#include "../ecs/Component.h"
#include "../ecs/GameObject.h"

#include "../core/Debug.h"
#include "Texture.h"
#include "textures/RenderTexture.h"

#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>

#include <cmath>
#include <memory>
#include <stdexcept>

namespace LouiEriksson::Engine::Graphics {
	
	Light::Light(const std::weak_ptr<ECS::GameObject>& _parent) : ECS::Component(_parent),
			m_Intensity(  3.0f),
			m_Range    (200.0f),
			m_Angle    (120.0f),
			m_Size     (  0.2f),
			m_Color    (glm::vec3(1, 1, 1)),
			m_Type     (Light::Parameters::Type::Point)
	{
		if (const auto p = Parent().lock()) {
			m_Transform = p->GetComponent<Transform>();
		}
		else {
			Debug::Log("Light has no Transform!", LogType::Error);
		}
		
		Type(m_Type);
	}
	
	Light::~Light() = default;
	
	void Light::Type(const Light::Parameters::Type& _type) {
		
		// Configure the light projection matrix.
		switch (_type) {
			
			case Light::Parameters::Type::Point: {
				
				m_Shadow.m_Projection = glm::perspective(
					glm::radians(90.0f),
					1.0f,
					m_Shadow.m_NearPlane,
					m_Range
				);
				
				break;
			}
			case Light::Parameters::Type::Directional: {
				
				m_Shadow.m_Projection = glm::ortho(
				   -m_Range / 2.0f,
				    m_Range / 2.0f,
				   -m_Range / 2.0f,
				    m_Range / 2.0f,
					m_Shadow.m_NearPlane,
					m_Range
				);
				
				break;
			}
			case Light::Parameters::Type::Spot: {
				
				m_Shadow.m_Projection = glm::perspective(
					glm::radians(m_Angle),
					1.0f,
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
	const Light::Parameters::Type& Light::Type() const noexcept {
		return m_Type;
	}
	
	Light::Parameters::Shadow::Shadow() noexcept :
			m_ShadowMap_Texture(  0    ),
			m_ShadowMap_FBO    (  0    ),
			m_Resolution       (128    ),
			m_Bias             (  0.01f),
			m_NormalBias       (  0.02f),
			m_NearPlane        (  0.2f ),
			m_TwoSided         (  true ),
			m_Projection       (  1.0f ),
			m_ViewProjection   (  1.0f ) {}
	
	Light::Parameters::Shadow::~Shadow() {
	
		RenderTexture::Unbind(); // Unbind FBO as a precaution before deleting.
		      Texture::Unbind(); // Unbind the texture as a precaution before deletion.
		
		// Delete the FBO and texture.
		if (m_ShadowMap_FBO     > 0) { glDeleteFramebuffers (1, &m_ShadowMap_FBO);     }
		if (m_ShadowMap_Texture > 0) { glDeleteTextures     (1, &m_ShadowMap_Texture); }
	}
	
	void Light::Parameters::Shadow::UpdateShadowMap(const Light::Parameters::Type& _type) {
		
		// Shadow implementation is very heavily modified derivative of implementations by Learn OpenGL:
        //  - de Vries, J. (n.d.). LearnOpenGL - Shadow Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping [Accessed 15 Dec. 2023].
        //  - de Vries, J. (n.d.). LearnOpenGL - Point Shadows. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows [Accessed 15 Dec. 2023].
		
		// Check if shadows are enabled.
		if (m_Resolution > 0) {
		
			const GLenum target = _type == Light::Parameters::Type::Point ?
					GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
			
			// Check to see if the shadow map is already initialised.
			if (m_ShadowMap_Texture != 0) {
				
				glBindTexture(target, Texture::s_CurrentTexture = static_cast<GLint>(m_ShadowMap_Texture));
				
				// Check the texture's resolution.
				int curr_resolution;
				glGetTexLevelParameteriv(_type == Light::Parameters::Type::Point ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &curr_resolution);
				
				Texture::Unbind();
				
				// Check if the texture's parameters have changed.
				if (curr_resolution != m_Resolution) {
					
					RenderTexture::Unbind(); // Unbind FBO as a precaution before deleting.
					
					if (m_ShadowMap_FBO != GL_NONE) {
						glDeleteFramebuffers(1, &m_ShadowMap_FBO); // Delete the FBO.
						
						m_ShadowMap_FBO = GL_NONE;
					}
					
					if (m_ShadowMap_Texture != GL_NONE) {
						glDeleteTextures(1, &m_ShadowMap_Texture); // Delete the texture.
						
						m_ShadowMap_Texture = GL_NONE;
					}
				}
			}
			
			// Check to see if the shadow map doesn't exist.
			if (m_ShadowMap_Texture == 0) {
				
				// Initialise the texture.
				glGenTextures(1, &m_ShadowMap_Texture);
			
				// Generate texture for shadow map (will bind it to the FBO).
				glBindTexture(target, Texture::s_CurrentTexture = static_cast<GLint>(m_ShadowMap_Texture));
				
				if (_type == Light::Parameters::Type::Point) {
					
					// Generate all six faces of a cubemap (for omnidirectional shadows).
					for (auto i = 0; i < 6; ++i) {
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_Resolution, m_Resolution, 0, GL_DEPTH_COMPONENT, GL_HALF_FLOAT, nullptr);
					}
				}
				else {
					
					// Generate a single face (for directional shadows).
					glTexImage2D(target, 0, GL_DEPTH_COMPONENT, m_Resolution, m_Resolution, 0, GL_DEPTH_COMPONENT, GL_HALF_FLOAT, nullptr);
				}
				
				// Set the texture's parameters.
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
				
				if (_type != Light::Parameters::Type::Point) {
					
					float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
					glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);
				}
				
				Texture::Unbind();
				
				// Create FBO for depth.
				glGenFramebuffers(1, &m_ShadowMap_FBO);
					
				// Bind shadow texture to FBO.
				glBindFramebuffer(GL_FRAMEBUFFER, RenderTexture::s_CurrentFBO = m_ShadowMap_FBO);
				
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowMap_Texture, 0);
				
				// Explicitly tell opengl that we're not rendering any color data in this FBO.
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				
				RenderTexture::Unbind();
			}
		}
		else {
			
			// If shadows are not enabled, dispose of the shadow map.
			if (m_ShadowMap_Texture != 0) {
				
				RenderTexture::Unbind(); // Unbind FBO as a precaution before deleting.
				      Texture::Unbind(); // Unbind the texture as a precaution before deletion.
				
				if (m_ShadowMap_FBO != GL_NONE) {
					glDeleteFramebuffers(1, &m_ShadowMap_FBO); // Delete the FBO.
					
					m_ShadowMap_FBO = GL_NONE;
				}
				
				if (m_ShadowMap_Texture != GL_NONE) {
					glDeleteTextures(1, &m_ShadowMap_Texture); // Delete the texture.
					
					m_ShadowMap_Texture = GL_NONE;
				}
			}
		}
	}

} // LouiEriksson::Engine::Graphics