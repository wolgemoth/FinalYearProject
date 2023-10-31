#include "stdafx.h"

#include "Light.h"

namespace LouiEriksson {
	
	Light::Light(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {
		
		Type(Light::Parameters::Type::Point);
		
		m_Transform = Parent()->GetComponent<Transform>();
	}
	
	void Light::Type(Light::Parameters::Type _type) {
		
		// Configure the light projection matrix.
		switch (_type) {
			
			case Light::Parameters::Type::Directional: {
				
				m_Shadow.m_Projection = glm::ortho(
				   -Light::Parameters::Shadow::SHADOW_DISTANCE / 2.0f,
				    Light::Parameters::Shadow::SHADOW_DISTANCE / 2.0f,
				   -Light::Parameters::Shadow::SHADOW_DISTANCE / 2.0f,
				    Light::Parameters::Shadow::SHADOW_DISTANCE / 2.0f,
					0.0f,           // Near plane.
					Light::Parameters::Shadow::SHADOW_DISTANCE // Far plane.
				);
				
				break;
			}
			default: {
				
				m_Shadow.m_Projection = glm::mat4(0.0);
				
				throw std::runtime_error("Not Implemented!");
			}
		}
		
		m_Type = _type;
	}
	Light::Parameters::Type Light::Type() {
		return m_Type;
	}
	
	Light::Parameters::Shadow::Shadow() {
		m_Resolution = Light::Parameters::Shadow::Resolution::Medium;
		m_Bias       = 0.005f;
		m_NormalBias = 0.05f;
		m_TwoSided   = false;
		m_Projection       = glm::mat4(1.0);
	}
	
	void Light::Draw(const Camera& _camera, const std::vector<std::shared_ptr<Renderer>>& _renderers, const std::vector<std::shared_ptr<Light>>& _lights) {
		
		/* DRAW SHADOWS */
		{
			// Create FBO for depth.
			GLuint depthMapFBO;
			glGenFramebuffers(1, &depthMapFBO);
			
			glViewport(0, 0, m_Shadow.m_Resolution, m_Shadow.m_Resolution);
			
			// Bind shadow texture to FBO.
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Shadow.m_ShadowMap, 0);
			
			// Explicitly tell opengl that we're not rendering any color data in this FBO.
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			
			glCullFace(m_Shadow.m_TwoSided ? GL_NONE : GL_FRONT);
			
			auto shadowShader = Shader::m_Cache.Return("shadowDepth");
			
			Shader::Bind(shadowShader->ID());
			
			const auto lightRot = glm::quat(glm::radians(glm::vec3(-45.0f, 135.0f, 0.0f)));
			const auto lightDir = VEC_FORWARD * lightRot;
			
			// Compute the size of a texel in world space.
			// We can round the light's position to these coordinates
			// to reduce an artifact known as "shimmering".
			const float texelSize = m_Shadow.SHADOW_DISTANCE / (m_Shadow.m_Resolution / 2);
			
			// Artificially add
			const glm::vec3 truncatedCamPos = glm::floor(
					_camera.GetTransform()->m_Position / texelSize) * texelSize;
			
			const glm::vec3 lightPos = truncatedCamPos + (lightDir * (m_Shadow.SHADOW_DISTANCE / 2.0f));
			
			const glm::mat4 lightView = glm::lookAt(
				lightPos,
				lightPos - lightDir,
				VEC_UP
			);
			
			const auto lightSpaceMatrix = m_Shadow.m_Projection * lightView;
			
			shadowShader->Assign(shadowShader->AttributeID("u_LightSpaceMatrix"), lightSpaceMatrix);
			
			// We need to render the scene from the light's perspective.
			for (const auto& renderer : _renderers) {
				
				const auto transform = renderer->GetTransform();
				const auto mesh      = renderer->GetMesh();
				
				// Bind VAO.
				glBindVertexArray(mesh->VAO_ID());
				
				shadowShader->Assign(shadowShader->AttributeID("u_Model"), transform->TRS());
				
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
				
				// Unbind VAO.
				glBindVertexArray(0);
			}
			
			// Unbind the program.
			Shader::Unbind();
			
			// Unbind the texture.
			Texture::Unbind();
			
			// Unbind the FBO
			RenderTexture::Unbind();
			
			glDeleteFramebuffers(1, &depthMapFBO);
		}
	}
	
	void Light::Parameters::Shadow::UpdateShadowMap() {
		
		// Generate texture for shadow map (will bind it to the FBO).
		glGenTextures(1, &m_ShadowMap);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Resolution, m_Resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	}
	
}