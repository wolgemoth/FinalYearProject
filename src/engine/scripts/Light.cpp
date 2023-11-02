#include "stdafx.h"

#include "Light.h"

namespace LouiEriksson {
	
	Light::Light(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
		
		m_Intensity = 1.0f;
		m_Range     = 50.0f;
		m_Angle     = 90.0f;
		m_Size      = 0.2f;
		m_Color     = glm::vec3(1, 1, 1);
		
		Type(Light::Parameters::Type::Directional);
		
		m_Transform = Parent()->GetComponent<Transform>();
	}
	
	Light::~Light() {
	
	}
	
	void Light::Type(Light::Parameters::Type _type) {
		
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
				
				throw std::runtime_error("Not Implemented!");
			}
		}
		
		m_Type = _type;
	}
	Light::Parameters::Type Light::Type() {
		return m_Type;
	}
	
	Light::Parameters::Shadow::Shadow() {
		
		m_Resolution = Light::Parameters::Shadow::Resolution::High;
		
		m_Bias       = 0.002f;
		m_NormalBias = 0.005f;
		
		m_NearPlane = 0.2f;
		
		m_TwoSided   = false;
		
		m_Projection     = glm::mat4(1.0);
		m_ViewProjection = glm::mat4(1.0);
	}
	
	Light::Parameters::Shadow::~Shadow() {
	
		RenderTexture::Unbind(); // Unbind FBO as a precaution before deleting.
		      Texture::Unbind(); // Unbind the texture as a precaution before deletion.
		
		// Delete the FBO and texture.
		if (m_ShadowMap_FBO     != 0) { glDeleteFramebuffers (1, &m_ShadowMap_FBO);     }
		if (m_ShadowMap_Texture != 0) { glDeleteTextures     (1, &m_ShadowMap_Texture); }
	}
	
	void Light::Parameters::Shadow::UpdateShadowMap(const Light::Parameters::Type& _type) {
		
		// Check if shadows are enabled.
		if (m_Resolution > Light::Parameters::Shadow::Resolution::Disabled) {
		
				GLenum target = _type == Light::Parameters::Type::Directional ?
						GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
				
			// Check to see if the shadow map is already initialised.
			if (m_ShadowMap_Texture != 0) {
				
				glBindTexture(target, m_ShadowMap_Texture);
				
				// Check the texture's resolution.
				int curr_resolution;
				glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &curr_resolution);
				
				Texture::Unbind();
				
				// Check if the texture's parameters have changed.
				if (curr_resolution != m_Resolution) {
					
					RenderTexture::Unbind(); // Unbind FBO as a precaution before deleting.
					
					if (m_ShadowMap_FBO     != 0) { throw std::runtime_error("Something just went horribly wrong."); }
					if (m_ShadowMap_Texture != 0) { throw std::runtime_error("Something just went horribly wrong."); }
					
					glDeleteFramebuffers(1, &m_ShadowMap_FBO); // Delete the FBO.
					glDeleteTextures(1, &m_ShadowMap_Texture); // Delete the texture.
					
					m_ShadowMap_FBO     = 0;
					m_ShadowMap_Texture = 0;
				}
			}
			
			// Check to see if the shadow map doesn't exist.
			if (m_ShadowMap_Texture == 0) {
				
				// Initialise the texture.
				glGenTextures(1, &m_ShadowMap_Texture);
			
				// Generate texture for shadow map (will bind it to the FBO).
				glBindTexture(target, m_ShadowMap_Texture);
				
				// Set the texture's parameters.
				glTexImage2D(target, 0, GL_DEPTH_COMPONENT, m_Resolution, m_Resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				
				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);
					
				Texture::Unbind();
				
				// Create FBO for depth.
				glGenFramebuffers(1, &m_ShadowMap_FBO);
					
				// Bind shadow texture to FBO.
				glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMap_FBO);
				
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
				
				if (m_ShadowMap_FBO     != 0) { throw std::runtime_error("Something just went horribly wrong."); }
				if (m_ShadowMap_Texture != 0) { throw std::runtime_error("Something just went horribly wrong."); }
				
				glDeleteFramebuffers(1, &m_ShadowMap_FBO); // Delete the FBO.
				glDeleteTextures(1, &m_ShadowMap_Texture); // Delete the texture.
				
				m_ShadowMap_FBO     = 0;
				m_ShadowMap_Texture = 0;
			}
		}
	}
}