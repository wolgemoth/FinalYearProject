#include "stdafx.h"

#include "Camera.h"
#include "Settings.h"

namespace LouiEriksson {

	Camera::Camera(const std::shared_ptr<GameObject>& _parent) : Component(_parent),
		              m_RT(1, 1, Texture::Parameters::Format(GL_RGB16F,  false), Texture::Parameters::FilterMode(GL_LINEAR,  GL_LINEAR ), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::NONE),
		  m_Albedo_gBuffer(1, 1, Texture::Parameters::Format(GL_RGB,     false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
		m_Emission_gBuffer(1, 1, Texture::Parameters::Format(GL_RGB16F,  false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
		m_Material_gBuffer(1, 1, Texture::Parameters::Format(GL_RGBA16F, false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
		m_Position_gBuffer(1, 1, Texture::Parameters::Format(GL_RGB16F,  false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::FRAME_BUFFER),
		  m_Normal_gBuffer(1, 1, Texture::Parameters::Format(GL_RGB16F,  false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
		m_TexCoord_gBuffer(1, 1, Texture::Parameters::Format(GL_RG16F,   false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER)
	{
		m_Window    = std::shared_ptr<Window>   (nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
		m_Cube      = std::shared_ptr<Mesh>     (nullptr);
	
		m_FOV      = 90.0f;
		m_NearClip =  0.1f;
		m_FarClip  = 60.0f;
		
		m_Exposure = Settings::PostProcessing::ToneMapping::s_Exposure;
		
		m_Projection = glm::mat4(1.0f);
		
		m_IsDirty = true;
		
		File::TryLoad("models/cube/cube.obj", m_Cube);
		
		m_LensDirt = Resources::GetTexture("Bokeh__Lens_Dirt_65").lock();
	}
	
	Camera::~Camera() {
	
		// Unlink the camera from the window. Catch and log any errors.
		try {
			if (m_Window != nullptr) {
				m_Window->Unlink(*this);
			}
		}
		catch (std::exception& e) {
			std::cout << e.what() << "\n";
		}
	}
	
	void Camera::Clear() {
	
		// Clear the camera.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void Camera::PreRender() {
		
		// Resize the frame buffers.
		// TODO: Set up enum flags for dirtying instead of m_IsDirty so that this doesn't happen every frame.
		auto dimensions = GetWindow()->Dimensions();
		
		              m_RT.Reinitialise(dimensions[0], dimensions[1]);
		  m_Albedo_gBuffer.Reinitialise(dimensions[0], dimensions[1]);
		m_Emission_gBuffer.Reinitialise(dimensions[0], dimensions[1]);
		m_Material_gBuffer.Reinitialise(dimensions[0], dimensions[1]);
		m_Position_gBuffer.Reinitialise(dimensions[0], dimensions[1]);
		  m_Normal_gBuffer.Reinitialise(dimensions[0], dimensions[1]);
		m_TexCoord_gBuffer.Reinitialise(dimensions[0], dimensions[1]);
	}
	
	void Camera::GeometryPass(const std::vector<std::shared_ptr<Renderer>>& _renderers) {
		
		GLint cullMode, depthMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &cullMode);
		glGetIntegerv(GL_DEPTH_FUNC,     &depthMode);
		
		// Texture Coordinates:
		{
			auto program = Resources::GetShader("pass_texcoords");
			
			// Bind program.
			Shader::Bind(program.lock()->ID());
			
			program.lock()->Assign(program.lock()->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			program.lock()->Assign(program.lock()->AttributeID("u_View"),             View()); /* VIEW       */
			
			program.lock()->Assign(program.lock()->AttributeID("u_ST"), Settings::Graphics::Material::s_TextureScaleTranslate);
			program.lock()->Assign(program.lock()->AttributeID("u_Displacement_Amount"), Settings::Graphics::Material::s_DisplacementAmount);
			program.lock()->Assign(program.lock()->AttributeID("u_CameraPosition"), GetTransform()->m_Position);
			
			RenderTexture::Bind(m_TexCoord_gBuffer);

			for (const auto& renderer : _renderers) {
				
				const auto transform = renderer->GetTransform();
				const auto material  = renderer->GetMaterial();
				const auto mesh      = renderer->GetMesh();
				
				// Bind VAO.
				Mesh::Bind(*mesh);
				
				program.lock()->Assign(
					program.lock()->AttributeID("u_Displacement"),
						material.lock()->GetDisplacement().lock()->ID(),
					0,
					GL_TEXTURE_2D
				);
				
				// Assign matrices.
				program.lock()->Assign(program.lock()->AttributeID("u_Model"), transform->TRS());
				
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
			}
		}
		
		// Positions:
		{
			auto program = Resources::GetShader("pass_positions");
			
			// Bind program.
			Shader::Bind(program.lock()->ID());
			program.lock()->Assign(program.lock()->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			program.lock()->Assign(program.lock()->AttributeID("u_View"),             View()); /* VIEW       */
			
			RenderTexture::Bind(m_Position_gBuffer);

			for (const auto& renderer : _renderers) {
				
				const auto transform = renderer->GetTransform();
				const auto material  = renderer->GetMaterial();
				const auto mesh      = renderer->GetMesh();
				
				// Bind VAO.
				Mesh::Bind(*mesh);
				
				// Assign matrices.
				program.lock()->Assign(program.lock()->AttributeID("u_Model"), transform->TRS()); /* MODEL      */
				
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
			}
		}
		
		// Albedo:
		{
			auto program = Resources::GetShader("pass_albedo");
			
			// Bind program.
			Shader::Bind(program.lock()->ID());
			program.lock()->Assign(program.lock()->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			program.lock()->Assign(program.lock()->AttributeID("u_View"),             View()); /* VIEW       */
			
			program.lock()->Assign(program.lock()->AttributeID("u_ScreenDimensions"), (glm::vec2)GetWindow()->Dimensions());
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_TexCoord_gBuffer"),
				m_TexCoord_gBuffer.ID(),
				1,
				GL_TEXTURE_2D
			);
			
			RenderTexture::Bind(m_Albedo_gBuffer);

			for (const auto& renderer : _renderers) {
				
				const auto transform = renderer->GetTransform();
				const auto material  = renderer->GetMaterial();
				const auto mesh      = renderer->GetMesh();
				
				// Bind VAO.
				Mesh::Bind(*mesh);
				
				// Assign matrices.
				program.lock()->Assign(program.lock()->AttributeID("u_Model"), transform->TRS()); /* MODEL      */
				
				program.lock()->Assign(
					program.lock()->AttributeID("u_Albedo"),
						material.lock()->GetAlbedo().lock()->ID(),
					0,
					GL_TEXTURE_2D
				);
				
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
			}
		}
		
		// Emission:
		{
			auto program = Resources::GetShader("pass_emission");
			
			// Bind program.
			Shader::Bind(program.lock()->ID());
			program.lock()->Assign(program.lock()->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			program.lock()->Assign(program.lock()->AttributeID("u_View"),             View()); /* VIEW       */
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_TexCoord_gBuffer"),
				m_TexCoord_gBuffer.ID(),
				1,
				GL_TEXTURE_2D
			);
			
			program.lock()->Assign(program.lock()->AttributeID("u_ScreenDimensions"), (glm::vec2)GetWindow()->Dimensions());
			
			program.lock()->Assign(program.lock()->AttributeID("u_EmissionAmount"), Settings::Graphics::Material::s_EmissionAmount);
			
			RenderTexture::Bind(m_Emission_gBuffer);
			glClear(GL_DEPTH_BUFFER_BIT);

			for (const auto& renderer : _renderers) {
				
				const auto transform = renderer->GetTransform();
				const auto material  = renderer->GetMaterial();
				const auto mesh      = renderer->GetMesh();
				
				// Bind VAO.
				Mesh::Bind(*mesh);
				
				// Assign matrices.
				program.lock()->Assign(program.lock()->AttributeID("u_Model"), transform->TRS()); /* MODEL      */
				
				program.lock()->Assign(
					program.lock()->AttributeID("u_Emission"),
						material.lock()->GetEmission().lock()->ID(),
					0,
					GL_TEXTURE_2D
				);
				
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
			}
			
			/* DRAW SKY */
			{
				glCullFace (GL_FRONT );
				glDepthFunc(GL_LEQUAL);
	
				auto skybox = Resources::GetShader("skybox");
	
				Shader::Bind(skybox.lock()->ID());
	
				auto trs = glm::scale(
					glm::mat4(1.0),
					glm::vec3(2.0)
				);
	
				// Assign matrices.
				skybox.lock()->Assign(skybox.lock()->AttributeID("u_Projection"),           Projection()); /* PROJECTION */
				skybox.lock()->Assign(skybox.lock()->AttributeID("u_View"), glm::mat4(glm::mat3(View()))); /* VIEW       */
				skybox.lock()->Assign(skybox.lock()->AttributeID("u_Model"),                         trs); /* MODEL      */
	
				skybox.lock()->Assign(
					skybox.lock()->AttributeID("u_Texture"),
					Settings::Graphics::Skybox::s_Skybox.lock()->ID(),
					0,
					GL_TEXTURE_2D
				);
	
				program.lock()->Assign(
					program.lock()->AttributeID("u_TexCoord_gBuffer"),
					m_Normal_gBuffer.ID(),
					1,
					GL_TEXTURE_2D
				);
				
				skybox.lock()->Assign(skybox.lock()->AttributeID("u_Exposure"), Settings::Graphics::Skybox::s_Exposure);
				skybox.lock()->Assign(skybox.lock()->AttributeID("u_Blur"), Settings::Graphics::Skybox::s_Blur);
	
				// Bind VAO.
				Mesh::Bind(*m_Cube);
	
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, Mesh::Primitives::Quad::Instance().lock()->VertexCount());
	
				glCullFace ( cullMode);
				glDepthFunc(depthMode);
			}
		}
		
		// Surface properties Roughness, Metallic, AO, Parallax Shadows:
		{
			auto program = Resources::GetShader("pass_material");
			
			// Bind program.
			Shader::Bind(program.lock()->ID());
			program.lock()->Assign(program.lock()->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			program.lock()->Assign(program.lock()->AttributeID("u_View"),             View()); /* VIEW       */
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_TexCoord_gBuffer"),
				m_TexCoord_gBuffer.ID(),
				4,
				GL_TEXTURE_2D
			);

			program.lock()->Assign(program.lock()->AttributeID(    "u_ParallaxShadows"), Settings::Graphics::Material::s_ParallaxShadows);
			
			program.lock()->Assign(program.lock()->AttributeID(   "u_Roughness_Amount"), Settings::Graphics::Material::s_RoughnessAmount);
			program.lock()->Assign(program.lock()->AttributeID("u_Displacement_Amount"), Settings::Graphics::Material::s_DisplacementAmount);
			program.lock()->Assign(program.lock()->AttributeID(          "u_AO_Amount"), Settings::Graphics::Material::s_AOAmount);

			program.lock()->Assign(program.lock()->AttributeID("u_LightPosition"), glm::vec3(0, 0, 0));

			program.lock()->Assign(program.lock()->AttributeID("u_ScreenDimensions"), (glm::vec2)GetWindow()->Dimensions());
			
			RenderTexture::Bind(m_Material_gBuffer);

			for (const auto& renderer : _renderers) {
				
				const auto transform = renderer->GetTransform();
				const auto material  = renderer->GetMaterial();
				const auto mesh      = renderer->GetMesh();
				
				// Bind VAO.
				Mesh::Bind(*mesh);
				
				// Assign matrices.
				program.lock()->Assign(program.lock()->AttributeID("u_Model"), transform->TRS()); /* MODEL      */
				
				program.lock()->Assign(
					program.lock()->AttributeID("u_Roughness"),
						material.lock()->GetRoughness().lock()->ID(),
					0,
					GL_TEXTURE_2D
				);
				
				program.lock()->Assign(
					program.lock()->AttributeID("u_Metallic"),
						material.lock()->GetMetallic().lock()->ID(),
					1,
					GL_TEXTURE_2D
				);
				
				program.lock()->Assign(
					program.lock()->AttributeID("u_AO"),
						material.lock()->GetAO().lock()->ID(),
					2,
					GL_TEXTURE_2D
				);
				
				program.lock()->Assign(
					program.lock()->AttributeID("u_Displacement"),
						material.lock()->GetDisplacement().lock()->ID(),
					3,
					GL_TEXTURE_2D
				);
				
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
			}
		}
		
		// Normals:
		{
			auto program = Resources::GetShader("pass_normals");
			
			// Bind program.
			Shader::Bind(program.lock()->ID());
			program.lock()->Assign(program.lock()->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			program.lock()->Assign(program.lock()->AttributeID("u_View"),             View()); /* VIEW       */
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_TexCoord_gBuffer"),
				m_TexCoord_gBuffer.ID(),
				1,
				GL_TEXTURE_2D
			);
			
			program.lock()->Assign(program.lock()->AttributeID("u_NormalAmount"), Settings::Graphics::Material::s_NormalAmount);
			program.lock()->Assign(program.lock()->AttributeID("u_ScreenDimensions"), (glm::vec2)GetWindow()->Dimensions());
			
			RenderTexture::Bind(m_Normal_gBuffer);

			for (const auto& renderer : _renderers) {
				
				const auto transform = renderer->GetTransform();
				const auto material  = renderer->GetMaterial();
				const auto mesh      = renderer->GetMesh();
				
				// Bind VAO.
				Mesh::Bind(*mesh);
				
				// Assign matrices.
				program.lock()->Assign(program.lock()->AttributeID("u_Model"), transform->TRS()); /* MODEL      */
				
				program.lock()->Assign(
					program.lock()->AttributeID("u_Normals"),
						material.lock()->GetNormals().lock()->ID(),
					0,
					GL_TEXTURE_2D
				);
				
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
			}
		}
	}
	
	void Camera::ShadowPass(const std::vector<std::shared_ptr<Renderer>>& _renderers, const std::vector<std::shared_ptr<Light>>& _lights) const {
		
		for (const auto& light : _lights) {
	
			// Initialise / reinitialise the buffers used for the shadow map.
			light->m_Shadow.UpdateShadowMap(light->m_Type);
			
			if (light->m_Shadow.m_Resolution > 0) {
				
				// Set the viewport resolution to that of the shadow map.
				glViewport(0, 0, light->m_Shadow.m_Resolution, light->m_Shadow.m_Resolution);
				
				glBindFramebuffer(GL_FRAMEBUFFER, RenderTexture::s_CurrentFBO = light->m_Shadow.m_ShadowMap_FBO);
				glClear(GL_DEPTH_BUFFER_BIT);
				
				/* DRAW SHADOWS */
				glCullFace(light->m_Shadow.m_TwoSided ? GL_NONE : GL_FRONT);
				
				std::weak_ptr<Shader> program;
				switch(light->Type()) {
					case Light::Parameters::Point:       { program = Resources::GetShader("shadowDepthCube"); break; }
					case Light::Parameters::Directional: { program = Resources::GetShader("shadowDepth");     break; }
					case Light::Parameters::Spot:        { program = Resources::GetShader("shadowDepthSpot"); break; }
					default: {
						throw std::runtime_error("Not implemented!");
					}
				}
				
				Shader::Bind(program.lock()->ID());
				
				// Compute the size of a texel in world space.
				// We can round the light's position to these coordinates
				// to reduce an artifact known as "shimmering".
				const float texelSize = light->m_Range / (float)(light->m_Shadow.m_Resolution / 4);
				
				const auto lightDir = VEC_FORWARD * light->m_Transform.lock()->m_Rotation;
				
				glm::vec3 lightPos;
				if (light->Type() == Light::Parameters::Type::Directional) {
					
					// Compute the position of the light.
					const glm::vec3 truncatedCamPos = glm::floor(
						GetTransform()->m_Position / texelSize) * texelSize;
					
					lightPos = truncatedCamPos + (lightDir * (light->m_Range / 2.0f));
				}
				else {
					lightPos = light->m_Transform.lock()->m_Position;
				}
				
				if (light->Type() == Light::Parameters::Type::Point) {
					
					const std::array<glm::mat4, 6> shadowTransforms {
						light->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)),
						light->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)),
						light->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
						light->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)),
						light->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)),
						light->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0))
					};
					
					glUniformMatrix4fv(
						program.lock()->AttributeID("u_Matrices"),
						shadowTransforms.size(),
						GL_FALSE,
						glm::value_ptr(shadowTransforms[0])
					);
					
					program.lock()->Assign(
							program.lock()->AttributeID("u_LightPosition"), lightPos);
					
					program.lock()->Assign(program.lock()->AttributeID("u_FarPlane"), light->m_Range);
					
					light->m_Shadow.m_ViewProjection = glm::mat4(1.0f);
				}
				else {
					
					glm::mat4 lightView;
					
					lightView = glm::lookAt(
						lightPos,
						lightPos + -lightDir,
						VEC_UP
					);
					
					light->m_Shadow.m_ViewProjection = light->m_Shadow.m_Projection * lightView;
				}
				
				program.lock()->Assign(program.lock()->AttributeID("u_LightSpaceMatrix"), light->m_Shadow.m_ViewProjection);
				
				// We need to render the scene from the light's perspective.
				for (const auto& renderer : _renderers) {
					
					const auto transform = renderer->GetTransform();
					const auto mesh      = renderer->GetMesh();
					
					// Bind VAO.
					Mesh::Bind(*mesh);
					
					program.lock()->Assign(program.lock()->AttributeID("u_Model"), transform->TRS());
					
					/* DRAW */
					glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
				}
				
				       Shader::Unbind(); // Unbind the program.
				      Texture::Unbind(); // Unbind the texture.
				RenderTexture::Unbind(); // Unbind the FBO
			}
		}
	}
	
	void Camera::Render(const std::vector<std::shared_ptr<Renderer>>& _renderers, const std::vector<std::shared_ptr<Light>>& _lights) {
		
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
		const auto  cullMode = GL_BACK;
		const auto depthMode = GL_LESS;
		
		glDepthFunc(depthMode);
		glCullFace(cullMode);
		
		/* GEOMETRY PASS */
		{
			GeometryPass(_renderers);
		}
		
		/* SHADOW PASS */
		{
			ShadowPass(_renderers, _lights);
			
			// Reset resolution after shadow pass.
			auto dimensions = m_Window->Dimensions();
			glViewport(0, 0, dimensions[0], dimensions[1]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			// Set / reset culling mode (after shadow pass).
			glCullFace(cullMode);
		}
		
		Mesh::Bind(*Mesh::Primitives::Quad::Instance().lock());
		
		/* SHADING */
		{
			// Bind the main FBO.
			RenderTexture::Bind(m_RT);
			
			/* DRAW OBJECTS */
			const auto program = Settings::Graphics::Material::s_Shader;
	
			// Bind program.
			Shader::Bind(program.lock()->ID());

			program.lock()->Assign(
				program.lock()->AttributeID("u_Albedo_gBuffer"),
				m_Albedo_gBuffer.ID(),
				0,
				GL_TEXTURE_2D
			);
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_Emission_gBuffer"),
				m_Emission_gBuffer.ID(),
				1,
				GL_TEXTURE_2D
			);
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_Material_gBuffer"),
				m_Material_gBuffer.ID(),
				2,
				GL_TEXTURE_2D
			);
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_Position_gBuffer"),
				m_Position_gBuffer.ID(),
				3,
				GL_TEXTURE_2D
			);
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_Normal_gBuffer"),
				m_Normal_gBuffer.ID(),
				4,
				GL_TEXTURE_2D
			);
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_Depth_gBuffer"),
				m_Position_gBuffer.DepthID(),
				5,
				GL_TEXTURE_2D
			);
			
			program.lock()->Assign(program.lock()->AttributeID("u_Time"), Time::Elapsed());

			program.lock()->Assign(program.lock()->AttributeID("u_CameraPosition"), GetTransform()->m_Position);

			program.lock()->Assign(program.lock()->AttributeID("u_ScreenDimensions"), (glm::vec2)GetWindow()->Dimensions());
			
			program.lock()->Assign(
				program.lock()->AttributeID("u_Ambient"),
				Settings::Graphics::Skybox::s_Skybox.lock()->ID(),
				98,
				GL_TEXTURE_2D
			);

			program.lock()->Assign(program.lock()->AttributeID("u_AmbientExposure"), Settings::Graphics::Skybox::s_Exposure);

			if (_lights.empty()) {

				// Draw the scene with no lighting.
				program.lock()->Assign(
					program.lock()->AttributeID("u_ShadowMap2D"),
					Resources::GetTexture("white").lock()->ID(),
					99,
					GL_TEXTURE_2D
				);

				program.lock()->Assign(
					program.lock()->AttributeID("u_ShadowMap3D"),
					Resources::GetTexture("white").lock()->ID(),
					100,
					GL_TEXTURE_CUBE_MAP
				);

				program.lock()->Assign(program.lock()->AttributeID("u_LightSpaceMatrix"), glm::mat4(1.0));

				program.lock()->Assign(program.lock()->AttributeID("u_ShadowBias"      ), 0.0f);
				program.lock()->Assign(program.lock()->AttributeID("u_ShadowNormalBias"), 0.0f);

				program.lock()->Assign(program.lock()->AttributeID("u_LightSize"),    0.0f);
				program.lock()->Assign(program.lock()->AttributeID("u_LightAngle"),   0.0f);
				program.lock()->Assign(program.lock()->AttributeID("u_NearPlane"),    0.0f);

				program.lock()->Assign(program.lock()->AttributeID("u_LightPosition" ), glm::vec3(0.0f));
				program.lock()->Assign(program.lock()->AttributeID("u_LightDirection"), glm::vec3(0.0f));

				program.lock()->Assign(program.lock()->AttributeID("u_LightRange"    ), 0.0f);
				program.lock()->Assign(program.lock()->AttributeID("u_LightIntensity"), 0.0f);
				program.lock()->Assign(program.lock()->AttributeID("u_LightColor"    ), glm::vec3(0.0f));

				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, Mesh::Primitives::Quad::Instance().lock()->VertexCount());
			}
			else {

				// Draw the scene once for each light (forward rendering technique).
				for (const auto& light : _lights) {
					
					using target_light = Settings::Graphics::Material;
					
					// TODO: Replace.
					{
						bool isDirty;
						
						{
							auto newResolution = std::stoi(target_light::s_ShadowResolutions.at(target_light::s_CurrentShadowResolutionSelection));
							
							auto newBias       = target_light::s_ShadowBias;
							auto newNormalBias = target_light::s_ShadowNormalBias;
							
							isDirty = newResolution != light->m_Shadow.m_Resolution ||
									  newBias       != light->m_Shadow.m_Bias       ||
									  newNormalBias != light->m_Shadow.m_NormalBias ||
									  target_light::s_LightRange              != light->m_Range               ||
									  target_light::s_LightAngle              != light->m_Angle               ||
									  target_light::s_CurrentLightType        != light->m_Type;
							
							light->m_Shadow.m_Resolution = newResolution;
							light->m_Shadow.m_Bias       = newBias;
							light->m_Shadow.m_NormalBias = newNormalBias;
						}
						
						light->m_Transform.lock()->m_Position = target_light::s_LightPosition;
						light->m_Transform.lock()->m_Rotation = glm::quat(glm::radians(target_light::s_LightRotation));
						light->m_Range                        = target_light::s_LightRange;
						light->m_Intensity                    = target_light::s_LightIntensity;
						light->m_Color                        = target_light::s_LightColor;
						light->m_Size                         = target_light::s_LightSize;
						light->m_Angle                        = target_light::s_LightAngle;
						light->m_Type                         = (Light::Parameters::Type)target_light::s_CurrentLightType;
						
						if (isDirty) {
							light->m_Shadow.UpdateShadowMap(light->Type());
						}
					}
					
					if (light->Type() == Light::Parameters::Type::Point) {

						program.lock()->Assign(
							program.lock()->AttributeID("u_ShadowMap3D"),
							light->m_Shadow.m_ShadowMap_Texture,
							100,
							GL_TEXTURE_CUBE_MAP
						);
					}
					else {

						program.lock()->Assign(
							program.lock()->AttributeID("u_ShadowMap2D"),
							light->m_Shadow.m_ShadowMap_Texture,
							99,
							GL_TEXTURE_2D
						);
					}

					program.lock()->Assign(program.lock()->AttributeID("u_LightSpaceMatrix"),
						light->m_Shadow.m_ViewProjection);

					program.lock()->Assign(program.lock()->AttributeID("u_ShadowBias"),
						light->m_Shadow.m_Bias);

					program.lock()->Assign(program.lock()->AttributeID("u_ShadowNormalBias"),
						light->m_Shadow.m_NormalBias);

					program.lock()->Assign(program.lock()->AttributeID("u_ShadowSamples"),
						target_light::s_ShadowSamples);
					
					program.lock()->Assign(program.lock()->AttributeID("u_ShadowTechnique"),
						target_light::s_CurrentShadowTechnique);
					
					program.lock()->Assign(program.lock()->AttributeID("u_LightType"),
						light->m_Type);
					
					program.lock()->Assign(program.lock()->AttributeID("u_LightSize"),
						light->m_Size);

					program.lock()->Assign(program.lock()->AttributeID("u_LightAngle"),
						glm::cos(glm::radians(
							light->Type() == Light::Parameters::Type::Spot ?
								light->m_Angle * 0.5f :
								180.0f
							)
						)
					);

					program.lock()->Assign(program.lock()->AttributeID("u_NearPlane"),
						light->m_Shadow.m_NearPlane);

					program.lock()->Assign(program.lock()->AttributeID("u_LightPosition"),
						light->m_Transform.lock()->m_Position);

					program.lock()->Assign(program.lock()->AttributeID("u_LightDirection"),
						light->m_Transform.lock()->FORWARD);

					program.lock()->Assign(program.lock()->AttributeID("u_LightRange"),
						light->m_Range);

					program.lock()->Assign(program.lock()->AttributeID("u_LightIntensity"),
						light->m_Intensity);

					program.lock()->Assign(program.lock()->AttributeID("u_LightColor"),
						light->m_Color);

					/* DRAW */
					glDrawArrays(GL_TRIANGLES, 0, Mesh::Primitives::Quad::Instance().lock()->VertexCount());
				}
			}
		}
		
		glDisable(GL_DEPTH_TEST);
	}
	
	void Camera::PostRender() {
		
		/* POST PROCESSING */
		if (Settings::PostProcessing::s_Enabled) {
			
			if (Settings::PostProcessing::AmbientOcclusion::IsActiveAndEnabled()) {
				AmbientOcclusion();
			}
			
			if (Settings::PostProcessing::ToneMapping::AutoExposure::IsActiveAndEnabled()) {
				AutoExposure();
			}
			
			if (Settings::PostProcessing::Bloom::IsActiveAndEnabled()) {
				Bloom();
			}
			
			/* EFFECTS STACK */
			// TODO: Allow modification of effect order and parameters outside of function.
		
			std::queue<std::weak_ptr<Shader>> effects;
			
			if (Settings::PostProcessing::ToneMapping::IsActiveAndEnabled()) {
				
				auto tonemapping = Resources::GetShader("aces");
				Shader::Bind(tonemapping.lock()->ID());
				tonemapping.lock()->Assign(tonemapping.lock()->AttributeID("u_Gain"), Settings::PostProcessing::ToneMapping::s_Gain);
				tonemapping.lock()->Assign(tonemapping.lock()->AttributeID("u_Exposure"), Settings::PostProcessing::ToneMapping::AutoExposure::IsActiveAndEnabled() ? m_Exposure : m_Exposure = Settings::PostProcessing::ToneMapping::s_Exposure);
			
				effects.push(tonemapping);
			}
			
			if (Settings::PostProcessing::AntiAliasing::IsActiveAndEnabled()) {
				
				auto aa = Resources::GetShader("fxaa");
				Shader::Bind(aa.lock()->ID());
				aa.lock()->Assign(aa.lock()->AttributeID("u_Texture"), m_RT.ID(), 0, GL_TEXTURE_2D);
				aa.lock()->Assign(aa.lock()->AttributeID("u_ContrastThreshold"), Settings::PostProcessing::AntiAliasing::s_ContrastThreshold);
				aa.lock()->Assign(aa.lock()->AttributeID("u_RelativeThreshold"), Settings::PostProcessing::AntiAliasing::s_RelativeThreshold);
				aa.lock()->Assign(aa.lock()->AttributeID("u_SubpixelBlending"), Settings::PostProcessing::AntiAliasing::s_SubpixelBlending);
				aa.lock()->Assign(aa.lock()->AttributeID("u_EdgeBlending"), Settings::PostProcessing::AntiAliasing::s_EdgeBlending);
				aa.lock()->Assign(aa.lock()->AttributeID("u_LocalContrastModifier"), Settings::PostProcessing::AntiAliasing::s_LocalContrastModifier);
				
				effects.push(aa);
			}
			
			if (Settings::PostProcessing::Grain::IsActiveAndEnabled()) {
				
				auto grain = Resources::GetShader("grain");
				Shader::Bind(grain.lock()->ID());
				grain.lock()->Assign(grain.lock()->AttributeID("u_Amount"), Settings::PostProcessing::Grain::s_Intensity);
				grain.lock()->Assign(grain.lock()->AttributeID("u_Time"), Time::Elapsed());
				
				effects.push(grain);
			}
			
			if (Settings::PostProcessing::Vignette::IsActiveAndEnabled()) {
				
				auto vignette = Resources::GetShader("vignette");
				Shader::Bind(vignette.lock()->ID());
				vignette.lock()->Assign(vignette.lock()->AttributeID("u_Amount"), Settings::PostProcessing::Vignette::s_Intensity);
				
				effects.push(vignette);
			}
			
			if (!effects.empty()) {
				
				// Render post-processing effects stack:
				while (!effects.empty()) {
					
					const auto shader = effects.front();
					effects.pop();
					
					Blit(m_RT, m_RT, shader);
					
					RenderTexture::Unbind();
				}
			}
			else {
				Blit(m_RT, m_RT, Resources::GetShader("passthrough"));
			}
		}
		else {
			Blit(m_RT, m_RT, Resources::GetShader("passthrough"));
		}
		
		/* RENDER TO SCREEN */
		RenderTexture::Unbind();
		
		glEnable(GL_FRAMEBUFFER_SRGB);  // ENABLE GAMMA CORRECTION
	
		Shader::Bind(Resources::GetShader("passthrough").lock()->ID());
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Primitives::Quad::Instance().lock()->VertexCount());
		
		glDisable(GL_FRAMEBUFFER_SRGB); // DISABLE GAMMA CORRECTION
	}
	
	void Camera::Blur(const RenderTexture& _rt, const float& _intensity, const int& _passes, const bool& _highQuality, const bool& _consistentDPI) const {
		
		const auto dimensions = glm::vec2(_rt.Width(), _rt.Height());
		
		auto horizontal = Resources::GetShader("blur_horizontal");
		auto   vertical = Resources::GetShader("blur_vertical");
		
		horizontal.lock()->Assign(horizontal.lock()->AttributeID("u_Texture"), _rt.ID());
		  vertical.lock()->Assign(  vertical.lock()->AttributeID("u_Texture"), _rt.ID());
		
		const int w = (int)dimensions.x,
                  h = (int)dimensions.y;

        float const dpiFactor = _consistentDPI ?
            glm::sqrt(dimensions.x * dimensions.y) * (1.0f / 3000.0f) :
            1.0f;

        float rootIntensity, scalar, aspectCorrection;

        if (_highQuality) {
               rootIntensity = 0.0f;
                      scalar = 0.0f;
            aspectCorrection = 0.0f;
        }
        else {
               rootIntensity = glm::sqrt(_intensity);
                      scalar = glm::sqrt(rootIntensity);
            aspectCorrection = glm::sqrt(dimensions.x / (float)dimensions.y);
        }

        for (int i = 0; i < _passes; i++) {
            
            int width, height;
            
            float size;
            
            if (_highQuality) {
                width  = w;
                height = h;
                
                size = (float)glm::pow(1.618f, i + 1) * _intensity;
            }
            else {
                width  = (int)glm::ceil(float(w) / glm::pow(2.0f, (float)i / 2.0f * scalar));
                height = (int)glm::ceil(float(h) / glm::pow(2.0f, (float)i / 2.0f * scalar) * aspectCorrection);
                
                size = (float)i * rootIntensity;
            }
            
			horizontal.lock()->Assign(horizontal.lock()->AttributeID("u_Step"), dpiFactor * size);
			  vertical.lock()->Assign(  vertical.lock()->AttributeID("u_Step"), dpiFactor * size);

			RenderTexture tmp(width, height, _rt.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), _rt.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
	        Blit(_rt, tmp, horizontal);
	        Blit(tmp, tmp, vertical);
			
			Copy(tmp, _rt);
		
            if (width == 1 && height == 1) {
                break;
            }
        }
	}
	
	void Camera::AutoExposure() {
	
		using target = Settings::PostProcessing::ToneMapping::AutoExposure;
		
		const auto dimensions = GetWindow()->Dimensions();
		
		auto shader = Resources::GetShader("auto_exposure");
		
		const glm::ivec2 luma_res(32, 32);
		
		RenderTexture luma_out(luma_res.x, luma_res.y, Texture::Parameters::Format(m_RT.Format().PixelFormat(), false), Texture::Parameters::FilterMode(GL_LINEAR, GL_NEAREST), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		
		auto mask = Resources::GetTexture("exposure_weights");
		
		Shader::Bind(shader.lock()->ID());
		shader.lock()->Assign(shader.lock()->AttributeID("u_Weights"),  mask.lock()->ID(), 1, GL_TEXTURE_2D);
		
		Blit(m_RT, luma_out, shader);
		
		std::vector<float> pixels(luma_res.x * luma_res.y * luma_out.Format().Channels());
		
		RenderTexture::Bind(luma_out);
		glReadPixels(0, 0, luma_res.x, luma_res.y, luma_out.Format().TextureFormat(), GL_FLOAT, pixels.data());
		
		// Compute the average luminosity:
		int avg_count = 0;
		float avg = 0.0f;
		
		for (auto y = 0; y < luma_res.y; y++) {
		for (auto x = 0; x < luma_res.x; x++) {
			
			float l = pixels.at((y * luma_res.x) + x);
			
			if (l > 0) {
				avg += l;
				
				avg_count++;
			}
		}}
		
		// Detect and prevent NaN propagation by checking the value against itself:
		if (avg != avg) {
			avg = 0.0f;
		}
		
		avg = avg / (float)glm::max(avg_count, 1);
		
		const float curr = avg;
		
		const float diff = glm::clamp((Settings::PostProcessing::ToneMapping::s_Exposure + target::s_Compensation) - curr, -1.0f, 1.0f);
		const float speed = (diff - m_Exposure) >= 0 ? target::s_SpeedUp : target::s_SpeedDown;
		
		m_Exposure = glm::mix(
			m_Exposure,
			glm::clamp(Settings::PostProcessing::ToneMapping::s_Exposure + diff, target::s_MinEV, target::s_MaxEV),
			Time::DeltaTime() * speed
		);
	}
	
	void Camera::AmbientOcclusion() const {
		
		const auto dimensions = GetWindow()->Dimensions();
		
		auto ao = Resources::GetShader("ao");
		Shader::Bind(ao.lock()->ID());
		
		ao.lock()->Assign(ao.lock()->AttributeID("u_Samples"), Settings::PostProcessing::AmbientOcclusion::s_Samples);
		
		ao.lock()->Assign(ao.lock()->AttributeID("u_Strength"), Settings::PostProcessing::AmbientOcclusion::s_Intensity);
		ao.lock()->Assign(ao.lock()->AttributeID("u_Bias"), -0.2f);
		ao.lock()->Assign(ao.lock()->AttributeID("u_Radius"), Settings::PostProcessing::AmbientOcclusion::s_Radius);
		
		ao.lock()->Assign(ao.lock()->AttributeID("u_NearClip"), m_NearClip);
		ao.lock()->Assign(ao.lock()->AttributeID("u_FarClip"), m_FarClip);
		ao.lock()->Assign(ao.lock()->AttributeID("u_Time"), Time::Elapsed());
		
		ao.lock()->Assign(ao.lock()->AttributeID("u_VP"), m_Projection * View());
		ao.lock()->Assign(ao.lock()->AttributeID("u_View"), View());
		
		int downscale = Settings::PostProcessing::AmbientOcclusion::s_Downscale;
		
		RenderTexture ao_rt(
			dimensions.x / (downscale + 1),
			dimensions.y / (downscale + 1),
			Texture::Parameters::Format(GL_RGB, false),
			Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR),
			m_RT.WrapMode(),
			RenderTexture::Parameters::DepthMode::NONE
		);
		
		ao.lock()->Assign(
			ao.lock()->AttributeID("u_Position_gBuffer"),
			m_Position_gBuffer.DepthID(),
			0,
			GL_TEXTURE_2D
		);
		
		ao.lock()->Assign(
			ao.lock()->AttributeID("u_Position_gBuffer"),
			m_Position_gBuffer.ID(),
			1,
			GL_TEXTURE_2D
		);
		
		ao.lock()->Assign(
			ao.lock()->AttributeID("u_Normal_gBuffer"),
			m_Normal_gBuffer.ID(),
			2,
			GL_TEXTURE_2D
		);
		
		glViewport(0, 0, ao_rt.Width(), ao_rt.Height());
		
		RenderTexture::Bind(ao_rt);
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Primitives::Quad::Instance().lock()->VertexCount());
		
		Blur(ao_rt, 1.0f, 1, true, false);

		glViewport(0, 0, m_RT.Width(), m_RT.Height());
		
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_DST_COLOR, GL_ZERO);

		Copy(ao_rt, m_RT);
		
		glDisable(GL_BLEND);
	}
	
	void Camera::Bloom() const {
		
		using target = Settings::PostProcessing::Bloom;
		
		const auto dimensions = GetWindow()->Dimensions();
		
		// Get each shader used for rendering the effect.
		auto threshold_shader = Resources::GetShader("threshold");
		auto downscale_shader = Resources::GetShader("downscale");
		auto   upscale_shader = Resources::GetShader("upscale");
		auto   combine_shader = Resources::GetShader("combine");
		auto lens_dirt_shader = Resources::GetShader("lens_dirt");
		
		/* SET BLOOM PARAMETERS */
		const int scalingPasses = 6;
		
		Shader::Bind(threshold_shader.lock()->ID());
		threshold_shader.lock()->Assign(threshold_shader.lock()->AttributeID("u_Threshold"), target::s_Threshold);
		threshold_shader.lock()->Assign(threshold_shader.lock()->AttributeID("u_Clamp"), target::s_Clamp);
		
		RenderTexture tmp(dimensions.x / 2, dimensions.y / 2, m_RT.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		
		Blit(m_RT, tmp, threshold_shader);
		
		Shader::Bind(downscale_shader.lock()->ID());
		downscale_shader.lock()->Assign(downscale_shader.lock()->AttributeID("u_Resolution"), glm::vec2(dimensions[0], dimensions[1]));

		RenderTexture mip5(dimensions.x / 128, dimensions.y / 128, m_RT.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		RenderTexture mip4(dimensions.x /  64, dimensions.y /  64, m_RT.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		RenderTexture mip3(dimensions.x /  32, dimensions.y /  32, m_RT.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		RenderTexture mip2(dimensions.x /  16, dimensions.y /  16, m_RT.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		RenderTexture mip1(dimensions.x /   8, dimensions.y /   8, m_RT.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		RenderTexture mip0(dimensions.x /   4, dimensions.y /   4, m_RT.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		
		Blit(tmp,  mip0, downscale_shader);
		Blit(mip0, mip1, downscale_shader);
		Blit(mip1, mip2, downscale_shader);
		Blit(mip2, mip3, downscale_shader);
		Blit(mip3, mip4, downscale_shader);
		Blit(mip4, mip5, downscale_shader);

		Shader::Bind(upscale_shader.lock()->ID());

		// Create the diffusion vector for the bloom algorithm:
		{
			float t = Utils::Remap(target::s_Anamorphism, -1.0f, 1.0f, 0.0f, 1.0f);
		
			// Imitate anamorphic artifacts by morphing the shape of the diffusion vector:
			glm::vec2 diffusionVec(
				glm::mix(0.0f, target::s_Diffusion,        t),
				glm::mix(0.0f, target::s_Diffusion, 1.0f - t)
			);
			
			upscale_shader.lock()->Assign(
				upscale_shader.lock()->AttributeID("u_Diffusion"),
				diffusionVec
			);
		}
		
	    // Enable additive blending
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_ONE, GL_ONE);
	    glBlendEquation(GL_FUNC_ADD);
		
		Blit(mip5, mip4, upscale_shader);
		Blit(mip4, mip3, upscale_shader);
		Blit(mip3, mip2, upscale_shader);
		Blit(mip2, mip1, upscale_shader);
		Blit(mip1, mip0, upscale_shader);
		Blit(mip0, tmp,  upscale_shader);

	    // Disable additive blending
	    glDisable(GL_BLEND);

		/* COMBINE */
		Shader::Bind(combine_shader.lock()->ID());
		combine_shader.lock()->Assign(combine_shader.lock()->AttributeID("u_Strength"), target::s_Intensity / glm::max((float)scalingPasses, 1.0f));
		combine_shader.lock()->Assign(combine_shader.lock()->AttributeID("u_Texture0"), m_RT.ID(), 0, GL_TEXTURE_2D);
		combine_shader.lock()->Assign(combine_shader.lock()->AttributeID("u_Texture1"),  tmp.ID(), 1, GL_TEXTURE_2D);

		RenderTexture::Bind(m_RT);
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Primitives::Quad::Instance().lock()->VertexCount());
		RenderTexture::Unbind();
		
		/* LENS DIRT */
		if (Settings::PostProcessing::Bloom::s_LensDirt > 0.0f) {
			Shader::Bind(lens_dirt_shader.lock()->ID());
			lens_dirt_shader.lock()->Assign(lens_dirt_shader.lock()->AttributeID("u_Strength"), target::s_LensDirt * target::s_Intensity);
			lens_dirt_shader.lock()->Assign(lens_dirt_shader.lock()->AttributeID("u_Texture0"), m_RT.ID(), 0, GL_TEXTURE_2D);
			lens_dirt_shader.lock()->Assign(lens_dirt_shader.lock()->AttributeID("u_Bloom"),  tmp.ID(), 1, GL_TEXTURE_2D);
			lens_dirt_shader.lock()->Assign(lens_dirt_shader.lock()->AttributeID("u_Dirt"),  m_LensDirt.lock()->ID(), 2, GL_TEXTURE_2D);
	
			RenderTexture::Bind(m_RT);
			glDrawArrays(GL_TRIANGLES, 0, Mesh::Primitives::Quad::Instance().lock()->VertexCount());
		}
	}
	
	void Camera::Copy(const RenderTexture& _src, const RenderTexture& _dest) {
		Blit(_src, _dest, Resources::GetShader("passthrough").lock());
	}
	
	void Camera::Blit(const RenderTexture& _src, const RenderTexture& _dest, const std::weak_ptr<Shader>& _shader) {
		
		glm::ivec4 dimensions;
		glGetIntegerv(GL_VIEWPORT, &dimensions[0]);

		const bool dimensionsDirty = dimensions[2] != _dest.Width() ||
		                             dimensions[3] != _dest.Height();

		if (dimensionsDirty) {
			glViewport(dimensions[0], dimensions[1], _dest.Width(), _dest.Height());
		}

		Shader::Bind(_shader.lock()->ID());

		glActiveTexture(GL_TEXTURE0);

		if (Texture::s_CurrentTexture != _src.ID()) {
			glBindTexture(GL_TEXTURE_2D, Texture::s_CurrentTexture = _src.ID());
		}
		
		RenderTexture::Bind(_dest);
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Primitives::Quad::Instance().lock()->VertexCount());
		
		if (dimensionsDirty) {
			glViewport(dimensions[0], dimensions[1], dimensions[2], dimensions[3]);
		}
	}
	
	void Camera::SetWindow(std::shared_ptr<Window> _window) {
		_window->Link(*this);
	}
	
	std::shared_ptr<Window> Camera::GetWindow() const {
		return m_Window;
	}
	
	void Camera::SetTransform(std::shared_ptr<Transform> _transform) {
		m_Transform = _transform;
	}
	
	std::shared_ptr<Transform> Camera::GetTransform() const {
		return m_Transform;
	}
	
	float Camera::Aspect() const {
		return m_Window->Aspect();
	}
	
	void Camera::FOV(const float& _fov) {
		
		if (m_FOV != _fov) {
			m_FOV = _fov;
			
			m_IsDirty = true; // Set dirty.
		}
	}
	
	const float& Camera::FOV() const {
		return m_FOV;
	}
	
	void Camera::NearClip(const float& _nearClip) {
		
		if (m_NearClip != _nearClip) {
			m_NearClip = _nearClip;
			
			m_IsDirty = true; // Set dirty.
		}
	}
	
	const float& Camera::NearClip() const {
		return m_NearClip;
	}
	
	void Camera::FarClip(const float& _farClip) {
		
		if (m_FarClip != _farClip) {
			m_FarClip = _farClip;
			
			m_IsDirty = true; // Set dirty.
		}
	}
	
	const float& Camera::FarClip() const {
		return m_FarClip;
	}
	
	void Camera::ClearColor(glm::vec4 _color) {
		
		// Set the clear color to the provided.
		glClearColor(_color[0], _color[1], _color[2], _color[3]);
	}
	
	glm::vec4 Camera::ClearColor() const {
		
		glm::vec4 result;
		
		// Read the clear value from opengl into the result.
		glGetFloatv(GL_COLOR_CLEAR_VALUE, &result[0]);
		
		return result;
	}
	
	const glm::mat4& Camera::Projection() {
		
		// Recalculate the perspective matrix if the camera is dirtied.
		if (m_IsDirty) {
			m_IsDirty = false;
			
			m_Projection = glm::perspective(glm::radians(m_FOV), Aspect(), m_NearClip, m_FarClip);
		}
		
		return m_Projection;
	}
	
	glm::mat4 Camera::View() const {
		
		const auto transform = GetTransform();
		
		return glm::lookAt(
			transform->m_Position,
			transform->m_Position + transform->FORWARD,
			transform->UP
		);
	}
	
	void Camera::SetDirty() {
		m_IsDirty = true;
	}
}