#include "Camera.h"

#include "../core/File.h"
#include "../core/Resources.h"
#include "../core/Settings.h"
#include "../core/Time.h"
#include "../core/Transform.h"
#include "../core/utils/Utils.h"
#include "../core/Window.h"
#include "../ecs/GameObject.h"
#include "../graphics/Light.h"
#include "../graphics/Material.h"
#include "../graphics/Mesh.h"
#include "../graphics/Renderer.h"
#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../graphics/textures/Cubemap.h"

#include <GL/glew.h>
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_int4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <exception>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::Graphics {

	Camera::Camera(const std::weak_ptr<ECS::GameObject>& _parent) : Component(_parent),
	
		// Initialise default values for perspective matrix:
		m_FOV(90.0f),
		m_NearClip(0.1f),
		m_FarClip(60.0f),
		
		// Initialise the projection matrix to an identity matrix and raise the "isDirty" flag:
		m_Projection(1.0f),
		m_IsDirty(true),
	
		// Set exposure level from settings.
		m_Exposure(Settings::PostProcessing::ToneMapping::s_Exposure),
		
		// Init g-buffer:
		              m_RT(1, 1, Texture::Parameters::Format(GL_RGB16F,  false), Texture::Parameters::FilterMode(GL_LINEAR,  GL_LINEAR ), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::NONE),
		  m_Albedo_gBuffer(1, 1, Texture::Parameters::Format(GL_RGB,     false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
		m_Emission_gBuffer(1, 1, Texture::Parameters::Format(GL_RGB16F,  false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
		m_Material_gBuffer(1, 1, Texture::Parameters::Format(GL_RGBA16F, false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
		m_Position_gBuffer(1, 1, Texture::Parameters::Format(GL_RGB16F,  false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode:: FRAME_BUFFER),
		  m_Normal_gBuffer(1, 1, Texture::Parameters::Format(GL_RGB16F,  false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
		m_TexCoord_gBuffer(1, 1, Texture::Parameters::Format(GL_RG32F,   false), Texture::Parameters::FilterMode(GL_NEAREST, GL_NEAREST), Texture::Parameters::WrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE), RenderTexture::Parameters::DepthMode::RENDER_BUFFER) {}
	
	Camera::~Camera() {
	
		// Unlink the camera from the window. Catch and log any errors.
		try {
			if (const auto w = m_Window.lock()) {
				w->Unlink(*this);
			}
		}
		catch (const std::exception& e) {
			Debug::Log(e);
		}
	}
	
	void Camera::PreRender(const RenderFlags& _flags) {
		
		if (const auto w = GetWindow().lock()) {
		
			if ((static_cast<unsigned int>(_flags) & static_cast<unsigned int>(RenderFlags::REINITIALISE)) != 0u) {
				
				// Reinitialise the g-buffer:
				const auto& dimensions = w->Dimensions();
				
				              m_RT.Reinitialise(dimensions.x, dimensions.y);
				  m_Albedo_gBuffer.Reinitialise(dimensions.x, dimensions.y);
				m_Emission_gBuffer.Reinitialise(dimensions.x, dimensions.y);
				m_Material_gBuffer.Reinitialise(dimensions.x, dimensions.y);
				m_Position_gBuffer.Reinitialise(dimensions.x, dimensions.y);
				  m_Normal_gBuffer.Reinitialise(dimensions.x, dimensions.y);
				m_TexCoord_gBuffer.Reinitialise(dimensions.x, dimensions.y);
				
				// Clear the mip chain:
				m_MipChain.clear();
			}
		}
		else {
			Debug::Log("Camera is not bound to a valid Window!", LogType::Error);
		}
	}
	
	void Camera::GeometryPass(const std::vector<std::weak_ptr<Renderer>>& _renderers) {
		
		// Get the current culling and depth modes and cache them here.
		GLint cullMode, depthMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &cullMode);
		glGetIntegerv(GL_DEPTH_FUNC,     &depthMode);
		
		// Texture Coordinates:
		if (const auto p = Resources::Get<Shader>("pass_texcoords").lock()) {
			
			// Bind program.
			Shader::Bind(p->ID());
			
			p->Assign(p->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			p->Assign(p->AttributeID("u_View"),             View()); /* VIEW       */
			
			p->Assign(p->AttributeID("u_ST"), Settings::Graphics::Material::s_TextureScaleTranslate);
			
			{
				const auto t = GetTransform().lock();
				
				p->Assign(p->AttributeID("u_CameraPosition"),
					t != nullptr ?
						t->m_Position :
						glm::vec3(0.0f)
				);
			}
			
			RenderTexture::Bind(m_TexCoord_gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (const auto& renderer : _renderers) {
				
				if (const auto  r = renderer.lock()         ) {
				if (const auto  t = r->GetTransform().lock()) {
				if (const auto ma = r->GetMaterial().lock() ) {
				if (const auto me = r->GetMesh().lock()     ) {
					
					p->Assign(p->AttributeID("u_Displacement_Amount"), ma->GetDisplacementAmount());
					
					// Assign textures:
					if (const auto d = ma->GetDisplacementTexture().lock()) {
						p->Assign(p->AttributeID("u_Displacement"), *d, 0);
					}
					
					// Assign matrices.
					p->Assign(p->AttributeID("u_Model"), t->TRS());
					
					/* DRAW */
					Draw(*me);
					
				}}}}
			}
		}
		
		// Positions:
		if (const auto p = Resources::Get<Shader>("pass_positions").lock()) {
			
			// Bind program.
			Shader::Bind(p->ID());
			p->Assign(p->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			p->Assign(p->AttributeID("u_View"),             View()); /* VIEW       */
			
			RenderTexture::Bind(m_Position_gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (const auto& renderer : _renderers) {
				
				if (const auto  r = renderer.lock()         ) {
				if (const auto  t = r->GetTransform().lock()) {
				if (const auto me = r->GetMesh().lock()     ) {
					
					// Assign matrices.
					p->Assign(p->AttributeID("u_Model"), t->TRS()); /* MODEL      */
					
					/* DRAW */
					Draw(*me);
					
				}}}
			}
		}
		
		// Albedo:
		if (const auto p = Resources::Get<Shader>("pass_albedo").lock()) {
			
			// Bind program.
			Shader::Bind(p->ID());
			p->Assign(p->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			p->Assign(p->AttributeID("u_View"),             View()); /* VIEW       */
			
			{
				const auto w = m_Window.lock();
				
				p->Assign(p->AttributeID("u_ScreenDimensions"),
					w != nullptr ?
						(glm::vec2)w->Dimensions() :
						 glm::vec2(1.0f)
				);
			}
			
			p->Assign(
				p->AttributeID("u_TexCoord_gBuffer"),
				m_TexCoord_gBuffer,
				1
			);
			
			RenderTexture::Bind(m_Albedo_gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (const auto& renderer : _renderers) {
				
				if (const auto  r = renderer.lock()         ) {
				if (const auto  t = r->GetTransform().lock()) {
				if (const auto ma = r->GetMaterial().lock() ) {
				if (const auto me = r->GetMesh().lock()     ) {
				
					// Assign matrices.
					p->Assign(p->AttributeID("u_Model"), t->TRS()); /* MODEL      */
					
					p->Assign(p->AttributeID("u_AlbedoColor"), ma->GetAlbedoColor());
			
					// Assign textures:
					if (const auto a = ma->GetAlbedoTexture().lock()) {
						p->Assign(p->AttributeID("u_Albedo"), *a, 0);
					}
					
					/* DRAW */
					Draw(*me);
					
				}}}}
			}
		}
		
		// Emission:
		if (const auto p = Resources::Get<Shader>("pass_emission").lock()) {
			
			// Bind program.
			Shader::Bind(p->ID());
			p->Assign(p->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			p->Assign(p->AttributeID("u_View"),             View()); /* VIEW       */
			
			p->Assign(
				p->AttributeID("u_TexCoord_gBuffer"),
				m_TexCoord_gBuffer,
				1
			);
			
			{
				const auto w = m_Window.lock();
				
				p->Assign(p->AttributeID("u_ScreenDimensions"),
					w != nullptr ?
						(glm::vec2)w->Dimensions() :
						 glm::vec2(1.0f)
				);
			}
			
			RenderTexture::Bind(m_Emission_gBuffer);
			glClear(GL_DEPTH_BUFFER_BIT);
			
			for (const auto& renderer : _renderers) {
				
				if (const auto  r = renderer.lock()         ) {
				if (const auto  t = r->GetTransform().lock()) {
				if (const auto ma = r->GetMaterial().lock() ) {
				if (const auto me = r->GetMesh().lock()     ) {
				
					// Assign matrices.
					p->Assign(p->AttributeID("u_Model"), t->TRS()); /* MODEL      */
					
					p->Assign(p->AttributeID("u_EmissionColor"), ma->GetEmissionColor());
			
					// Assign textures:
					if (const auto e = ma->GetEmissionTexture().lock()) {
						p->Assign(p->AttributeID("u_Emission"), *e, 0);
					}
					
					/* DRAW */
					Draw(*me);
					
				}}}}
			}
			
			/* DRAW SKY */
			if (const auto s = Resources::Get<Shader>("skybox").lock()) {
				
				// Change culling and depth options for skybox rendering.
				glCullFace (GL_FRONT);
				glDepthFunc(GL_LEQUAL);
	
				Shader::Bind(s->ID());
	
				const static auto trs = glm::scale(
					glm::mat4(1.0),
					glm::vec3(2.0)
				);
	
				// Assign matrices.
				s->Assign(s->AttributeID("u_Projection"),           Projection()); /* PROJECTION */
				s->Assign(s->AttributeID("u_View"), glm::mat4(glm::mat3(View()))); /* VIEW       */
				s->Assign(s->AttributeID("u_Model"),                         trs); /* MODEL      */
				
				// Assign texture:
				if (const auto sky = Settings::Graphics::Skybox::s_Skybox.lock()) {
					s->Assign(
						s->AttributeID("u_Texture"),
						*sky,
						0
					);
				}
				
				s->Assign(s->AttributeID("u_Exposure"), Settings::Graphics::Skybox::s_Exposure);
				s->Assign(s->AttributeID("u_Blur"    ), Settings::Graphics::Skybox::s_Blur);
	
				// Bind VAO.
				if (const auto c = Mesh::Primitives::Cube::Instance().lock()) {
					Draw(*c);
				}
				
				// Restore culling and depth options.
				glCullFace ( cullMode);
				glDepthFunc(depthMode);
			}
		}
		
		// Surface properties Roughness, Metallic, AO, Parallax Shadows:
		if (const auto p = Resources::Get<Shader>("pass_material").lock()) {
			
			// Bind program.
			Shader::Bind(p->ID());
			p->Assign(p->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			p->Assign(p->AttributeID("u_View"),             View()); /* VIEW       */
			
			p->Assign(
				p->AttributeID("u_TexCoord_gBuffer"),
				m_TexCoord_gBuffer,
				4
			);

			p->Assign(p->AttributeID("u_ParallaxShadows"), Settings::Graphics::Material::s_ParallaxShadows);
			
			if (const auto t = GetTransform().lock()) {
				
				const auto lightType = (Light::Parameters::Type)Settings::Graphics::Material::s_CurrentLightType;
				
				auto lightPos = lightType == Light::Parameters::Type::Directional ?
						t->m_Position + (VEC_FORWARD * glm::quat(glm::radians(Settings::Graphics::Material::s_LightRotation)) * 65535.0f) :
						Settings::Graphics::Material::s_LightPosition;
				
				p->Assign(p->AttributeID("u_LightPosition"), lightPos);
			}
			
			{
				const auto w = m_Window.lock();
				
				p->Assign(p->AttributeID("u_ScreenDimensions"),
					w != nullptr ?
						(glm::vec2)w->Dimensions() :
						 glm::vec2(1.0f)
				);
			}
			
			RenderTexture::Bind(m_Material_gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			for (const auto& renderer : _renderers) {
				
				if (const auto  r = renderer.lock()         ) {
				if (const auto  t = r->GetTransform().lock()) {
				if (const auto ma = r->GetMaterial().lock() ) {
				if (const auto me = r->GetMesh().lock()     ) {
				
					// Assign matrices.
					p->Assign(p->AttributeID("u_Model"), t->TRS()); /* MODEL */
					
					p->Assign(p->AttributeID(   "u_Roughness_Amount"), ma->GetRoughnessAmount()   );
					p->Assign(p->AttributeID("u_Displacement_Amount"), ma->GetDisplacementAmount());
					p->Assign(p->AttributeID(          "u_AO_Amount"), ma->GetAOAmount()          );
					
					// Assign textures:
					if (const auto ro = ma->GetRoughnessTexture().lock()) {
						p->Assign(p->AttributeID("u_Roughness"), *ro, 0);
					}
					
					if (const auto met = ma->GetMetallicTexture().lock()) {
						p->Assign(p->AttributeID("u_Metallic"), *met, 1);
					}
					
					if (const auto a = ma->GetAOTexture().lock()) {
						p->Assign(p->AttributeID("u_AO"), *a, 2);
					}
					
					if (const auto d = ma->GetDisplacementTexture().lock()) {
						p->Assign(p->AttributeID("u_Displacement"), *d, 3);
					}
					
					/* DRAW */
					Draw(*me);
					
				}}}}
			}
		}
		
		// Normals:
		if (const auto p = Resources::Get<Shader>("pass_normals").lock()) {
			
			// Bind program.
			Shader::Bind(p->ID());
			p->Assign(p->AttributeID("u_Projection"), Projection()); /* PROJECTION */
			p->Assign(p->AttributeID("u_View"),             View()); /* VIEW       */
			
			p->Assign(
				p->AttributeID("u_TexCoord_gBuffer"),
				m_TexCoord_gBuffer,
				1
			);
			
			{
				const auto w = m_Window.lock();
				
				p->Assign(p->AttributeID("u_ScreenDimensions"),
					w != nullptr ?
						(glm::vec2)w->Dimensions() :
						 glm::vec2(1.0f)
				);
			}
			
			RenderTexture::Bind(m_Normal_gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (const auto& renderer : _renderers) {
				
				if (const auto  r = renderer.lock()         ) {
				if (const auto  t = r->GetTransform().lock()) {
				if (const auto ma = r->GetMaterial().lock() ) {
				if (const auto me = r->GetMesh().lock()     ) {
				
					// Assign matrices.
					p->Assign(p->AttributeID("u_Model"), t->TRS()); /* MODEL      */
					
					p->Assign(p->AttributeID("u_NormalAmount"), ma->GetNormalAmount());
			
					p->Assign(
						p->AttributeID("u_Normals"),
							*ma->GetNormalTexture().lock(),
						0
					);
					
					/* DRAW */
					Draw(*me);
					
				}}}}
			}
		}
	}
	
	void Camera::ShadowPass(const std::vector<std::weak_ptr<Renderer>>& _renderers, const std::vector<std::weak_ptr<Light>>& _lights) const {
		
		// Shadow implementation is very heavily modified derivative of implementations by Learn OpenGL:
        //  - de Vries, J. (n.d.). LearnOpenGL - Shadow Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping [Accessed 15 Dec. 2023].
        //  - de Vries, J. (n.d.). LearnOpenGL - Point Shadows. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows [Accessed 15 Dec. 2023].
		
		// Perform these computations for every light in the scene.
		for (const auto& light : _lights) {
	
			if (const auto l = light.lock()) {
			
				// Initialise / reinitialise the buffers used for the shadow map.
				l->m_Shadow.UpdateShadowMap(l->m_Type);
				
				// Check if the light has shadows enabled:
				if (l->m_Shadow.m_Target != GL_NONE &&
				    l->m_Shadow.m_Resolution > 0
				) {
					// Set the viewport resolution to that of the shadow map.
					glViewport(0, 0, l->m_Shadow.m_Resolution, l->m_Shadow.m_Resolution);
					
					RenderTexture::Bind(l->m_Shadow.m_ShadowMap_FBO);
					
					glClear(GL_DEPTH_BUFFER_BIT);
					
					/* CONFIGURE CULLING */
					
					// Get current culling settings.
					const bool cullEnabled = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
					int cullMode;
					glGetIntegerv(GL_CULL_FACE_MODE, &cullMode);
					
					// Configure culling based on whether or not the light uses two-sided shadows.
					if (l->m_Shadow.m_TwoSided) {
						glDisable(GL_CULL_FACE);
					}
					else {
						glEnable(GL_CULL_FACE);
						glCullFace(GL_FRONT);
					}
					
					// Get the correct shader program for the type of light:
					std::shared_ptr<Shader> p;
					switch(l->Type()) {
						case Light::Parameters::Point:       { p = Resources::Get<Shader>("shadowDepthCube").lock(); break; }
						case Light::Parameters::Directional: { p = Resources::Get<Shader>("shadowDepth"    ).lock(); break; }
						case Light::Parameters::Spot:        { p = Resources::Get<Shader>("shadowDepthSpot").lock(); break; }
						default: {
							Debug::Log("Unknown Light type!", LogType::Error);
						}
					}
					
					Shader::Bind(p->ID());
					
					// Compute the size of a texel in world space.
					// We can round the light's position to these coordinates
					// to reduce an artifact known as "shimmering".
					const float texelSize = l->m_Range / static_cast<float>((l->m_Shadow.m_Resolution / 4));
					
					const auto lightDir = VEC_FORWARD * l->m_Transform.lock()->m_Rotation;
					
					glm::vec3 lightPos;
					{
						const auto t = GetTransform().lock();
						
						if (l->Type() == Light::Parameters::Type::Directional &&
						    t != nullptr
						) {
							
							// Compute the position of the light.
							const glm::vec3 truncatedCamPos = glm::floor(
								t->m_Position / texelSize) * texelSize;
							
							lightPos = truncatedCamPos + (lightDir * (l->m_Range / 2.0f));
						}
						else {
							lightPos = l->m_Transform.lock()->m_Position;
						}
					}
					
					if (l->Type() == Light::Parameters::Type::Point) {
						
						// Collection of shadow transforms for each face of the cubemap.
						const std::array<glm::mat4, 6> shadowTransforms {
							l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)),
							l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)),
							l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
							l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)),
							l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)),
							l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0))
						};
						
						// Assign the transforms to the shader.
						glUniformMatrix4fv(
							p->AttributeID("u_Matrices"),
							shadowTransforms.size(),
							GL_FALSE,
							glm::value_ptr(shadowTransforms[0])
						);
						
						p->Assign(p->AttributeID("u_LightPosition"), lightPos  );
						p->Assign(p->AttributeID("u_FarPlane"     ), l->m_Range);
						
						l->m_Shadow.m_ViewProjection = glm::mat4(1.0f);
					}
					else {
						
						glm::mat4 lightView;
						
						lightView = glm::lookAt(
							lightPos,
							lightPos + -lightDir,
							VEC_UP
						);
						
						l->m_Shadow.m_ViewProjection = l->m_Shadow.m_Projection * lightView;
					}
					
					if (l->Type() != Light::Parameters::Type::Point) {
						p->Assign(p->AttributeID("u_LightSpaceMatrix"), l->m_Shadow.m_ViewProjection);
					}
					
					// We need to render the scene from the light's perspective.
					for (const auto& renderer : _renderers) {
						
						if (const auto  r = renderer.lock()         ) {
						if (const auto  t = r->GetTransform().lock()) {
						if (const auto me = r->GetMesh().lock()     ) {
							
							if (r->Shadows() &&
							    me->Format() == GL_TRIANGLES
							) {
								p->Assign(p->AttributeID("u_Model"), t->TRS());
								
								/* DRAW */
								Draw(*me);
							}
						}}}
					}
					
					// Restore original culling settings:
					if (cullEnabled) {
						glEnable(GL_CULL_FACE);
					}
					else {
						glDisable(GL_CULL_FACE);
					}
					
					glCullFace(cullMode);
				}
			}
		}
	}
	
	void Camera::Render(const std::vector<std::weak_ptr<Renderer>>& _renderers, const std::vector<std::weak_ptr<Light>>& _lights) {
		
		if (const auto w = m_Window.lock()) {
			
			// Enable culling and depth.
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			
			// Set the preferred culling and depth modes:
			const auto  cullMode = GL_BACK;
			const auto depthMode = GL_LEQUAL;
			
			glCullFace(cullMode);
			glDepthFunc(depthMode);
			
			glEnable(GL_POINT_SPRITE);       // Enable the rendering of points as sprites.
			glEnable(GL_PROGRAM_POINT_SIZE); // Allow variable point sizes from within shader programs.
			
			// Set point sprite to use texture coordinates.
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			
			/* GEOMETRY PASS */
			GeometryPass(_renderers);
		
			/* SHADOW PASS */
			ShadowPass(_renderers, _lights);

			// Reset resolution after shadow pass.
			auto dimensions = w->Dimensions();
			glViewport(0, 0, dimensions[0], dimensions[1]);

			/* SHADING */
			if (const auto m = Mesh::Primitives::Quad::Instance().lock()) {
				
				// Bind the main FBO.
				RenderTexture::Bind(m_RT);
				
				/* DRAW OBJECTS */
				if (const auto p = Settings::Graphics::Material::s_Shader.lock()) {
			
					// Bind program.
					Shader::Bind(p->ID());
		
					// Assign g-buffers:
					p->Assign(
						p->AttributeID("u_Albedo_gBuffer"),
						m_Albedo_gBuffer,
						0
					);
					
					p->Assign(
						p->AttributeID("u_Emission_gBuffer"),
						m_Emission_gBuffer,
						1
					);
					
					p->Assign(
						p->AttributeID("u_Material_gBuffer"),
						m_Material_gBuffer,
						2
					);
					
					p->Assign(
						p->AttributeID("u_Position_gBuffer"),
						m_Position_gBuffer,
						3
					);
					
					p->Assign(
						p->AttributeID("u_Normal_gBuffer"),
						m_Normal_gBuffer,
						4
					);
					
					p->AssignDepth(
						p->AttributeID("u_Depth_gBuffer"),
						m_Position_gBuffer,
						5
					);
					
					// Assign other material parameters:
					p->Assign(p->AttributeID("u_Time"), Time::Elapsed());
					
					{
						const auto t = GetTransform().lock();
						
						p->Assign(p->AttributeID("u_CameraPosition"),
							t != nullptr ?
								t->m_Position :
								glm::vec3(0.0f)
						);
					}
					
					{
						const auto w = GetWindow().lock();
						
						p->Assign(p->AttributeID("u_ScreenDimensions"),
							w != nullptr ?
								(glm::vec2)w->Dimensions() :
								 glm::vec2(1.0f)
						);
					}
					
					// Assign ambient texture:
					if (const auto s =  Settings::Graphics::Skybox::s_Skybox.lock()) {
						p->Assign(
							p->AttributeID("u_Ambient"),
							*s,
							98
						);
					}
		
					p->Assign(p->AttributeID("u_AmbientExposure"), Settings::Graphics::Skybox::s_Exposure);
		
					// For the time being, there is only one light. TODO: Introduce a lighting buffer.
					for (const auto& light : _lights) {
						
						using target_light = Settings::Graphics::Material;
						
						if (const auto l = light.lock()) {
							
							// TODO: Placeholder code for settings.
							{
								bool isDirty;
								
								{
									auto newResolution = std::stoi(target_light::s_ShadowResolutions.at(target_light::s_CurrentShadowResolutionSelection));
									
									auto newBias       = target_light::s_ShadowBias;
									auto newNormalBias = target_light::s_ShadowNormalBias;
									
									isDirty = newResolution != l->m_Shadow.m_Resolution             ||
											  newBias       != l->m_Shadow.m_Bias                   ||
											  newNormalBias != l->m_Shadow.m_NormalBias             ||
											  target_light::s_LightRange              != l->m_Range ||
											  target_light::s_LightAngle              != l->m_Angle ||
											  target_light::s_CurrentLightType        != l->m_Type;
									
									l->m_Shadow.m_Resolution = newResolution;
									l->m_Shadow.m_Bias       = newBias;
									l->m_Shadow.m_NormalBias = newNormalBias;
								}
								
								if (const auto t = l->m_Transform.lock()) {
									t->m_Position = target_light::s_LightPosition;
									t->m_Rotation = glm::quat(glm::radians(target_light::s_LightRotation));
								}
								
								l->m_Range     = target_light::s_LightRange;
								l->m_Intensity = target_light::s_LightIntensity;
								l->m_Color     = target_light::s_LightColor;
								l->m_Size      = target_light::s_LightSize;
								l->m_Angle     = target_light::s_LightAngle;
								l->m_Type      = (Light::Parameters::Type)target_light::s_CurrentLightType;
								
								if (isDirty) {
									l->m_Shadow.UpdateShadowMap(l->Type());
								}
							}
							
							if (l->m_Shadow.m_Target != GL_NONE) {
								
								if (l->Type() == Light::Parameters::Type::Point) {
				
									p->Assign(
										p->AttributeID("u_ShadowMap3D"),
										l->m_Shadow,
										100
									);
								}
								else {
				
									p->Assign(
										p->AttributeID("u_ShadowMap2D"),
										l->m_Shadow,
										99
									);
								}
							}
							
							p->Assign(p->AttributeID("u_LightSpaceMatrix"), l->m_Shadow.m_ViewProjection);
							
							p->Assign(p->AttributeID("u_ShadowBias"      ), l->m_Shadow.m_Bias      );
							p->Assign(p->AttributeID("u_ShadowNormalBias"), l->m_Shadow.m_NormalBias);
							
							p->Assign(p->AttributeID("u_ShadowSamples"   ), target_light::s_ShadowSamples);
							p->Assign(p->AttributeID("u_ShadowTechnique" ),
								l->m_Shadow.m_Target != GL_NONE ?
								target_light::s_CurrentShadowTechnique :
								-1
							);
							
							p->Assign(p->AttributeID("u_LightType"), l->m_Type);
							p->Assign(p->AttributeID("u_LightSize"), l->m_Size);
							
							p->Assign(p->AttributeID("u_LightAngle"),
								glm::cos(glm::radians(
									l->Type() == Light::Parameters::Type::Spot ?
										l->m_Angle * 0.5f :
										180.0f
									)
								)
							);
			
							p->Assign(p->AttributeID("u_NearPlane"), l->m_Shadow.m_NearPlane);
			
							if (const auto t = l->m_Transform.lock()) {
								p->Assign(p->AttributeID("u_LightPosition" ), t->m_Position);
								p->Assign(p->AttributeID("u_LightDirection"), t->FORWARD   );
							}
							
							p->Assign(p->AttributeID("u_LightRange"    ), l->m_Range    );
							p->Assign(p->AttributeID("u_LightIntensity"), l->m_Intensity);
							p->Assign(p->AttributeID("u_LightColor"    ), l->m_Color    );
			
							/* DRAW */
							if (const auto q = Mesh::Primitives::Quad::Instance().lock()) {
								Draw(*q);
							}
						}
					}
				}
			}
		}
		else {
			Debug::Log("Camera is not bound to a valid Window!", LogType::Error);
		}
	}
	
	void Camera::PostRender() {
		
		glDisable(GL_DEPTH_TEST);
		
		/* POST PROCESSING */
		if (Settings::PostProcessing::s_Enabled) {
			
			// Do ambient occlusion first.
			if (Settings::PostProcessing::AmbientOcclusion::IsActiveAndEnabled()) {
				AmbientOcclusion();
			}
		
			// Auto exposure after ambient occlusion.
			if (Settings::PostProcessing::ToneMapping::AutoExposure::IsActiveAndEnabled()) {
				AutoExposure();
			}
			
			// Bloom after auto exposure.
			if (Settings::PostProcessing::Bloom::IsActiveAndEnabled()) {
				Bloom();
			}
			
			/* EFFECTS STACK */
			// TODO: Allow modification of effect order and parameters outside of function.
		
			std::queue<std::weak_ptr<Shader>> effects;
			
			if (Settings::PostProcessing::ToneMapping::IsActiveAndEnabled()) {
				
				if (const auto t = Resources::Get<Shader>("aces").lock()) {
					
					Shader::Bind(t->ID());
					t->Assign(t->AttributeID("u_Gain"), Settings::PostProcessing::ToneMapping::s_Gain);
					
					t->Assign(t->AttributeID("u_Exposure"),
						Settings::PostProcessing::ToneMapping::AutoExposure::IsActiveAndEnabled() ?
						m_Exposure :
						m_Exposure = Settings::PostProcessing::ToneMapping::s_Exposure
					);
				
					effects.push(t);
				}
			}
			
			// Perform anti-aliasing after tonemapping as FXAA suffers in the HDR range.
			if (Settings::PostProcessing::AntiAliasing::IsActiveAndEnabled()) {
				
				if (const auto aa = Resources::Get<Shader>("fxaa").lock()) {
					
					Shader::Bind(aa->ID());
					aa->Assign(aa->AttributeID("u_Texture"), m_RT, 0);
					
					aa->Assign(aa->AttributeID("u_ContrastThreshold"    ), Settings::PostProcessing::AntiAliasing::s_ContrastThreshold    );
					aa->Assign(aa->AttributeID("u_RelativeThreshold"    ), Settings::PostProcessing::AntiAliasing::s_RelativeThreshold    );
					aa->Assign(aa->AttributeID("u_SubpixelBlending"     ), Settings::PostProcessing::AntiAliasing::s_SubpixelBlending     );
					aa->Assign(aa->AttributeID("u_EdgeBlending"         ), Settings::PostProcessing::AntiAliasing::s_EdgeBlending         );
					aa->Assign(aa->AttributeID("u_LocalContrastModifier"), Settings::PostProcessing::AntiAliasing::s_LocalContrastModifier);
					
					effects.push(aa);
				}
			}
			
			if (Settings::PostProcessing::Grain::IsActiveAndEnabled()) {
				
				if (const auto g = Resources::Get<Shader>("grain").lock()) {
					
					Shader::Bind(g->ID());
					g->Assign(g->AttributeID("u_Amount"), Settings::PostProcessing::Grain::s_Intensity);
					g->Assign(g->AttributeID("u_Time"), Time::Elapsed());
					
					effects.push(g);
				}
			}
			
			if (Settings::PostProcessing::Vignette::IsActiveAndEnabled()) {
				
				if (const auto v = Resources::Get<Shader>("vignette").lock()) {
					
					Shader::Bind(v->ID());
					v->Assign(v->AttributeID("u_Amount"    ), Settings::PostProcessing::Vignette::s_Intensity );
					v->Assign(v->AttributeID("u_Smoothness"), Settings::PostProcessing::Vignette::s_Smoothness);
					
					effects.push(v);
				}
			}
			
			// Check if there are effects in the stack...
			if (!effects.empty()) {
				
				// Render post-processing effects stack:
				while (!effects.empty()) {
					
					const auto shader = effects.front();
					effects.pop();
					
					Blit(m_RT, m_RT, shader);
					
					if (!effects.empty()) {
						RenderTexture::Unbind();
					}
				}
			}
			else {
				Copy(m_RT, m_RT); // Passthrough main render target.
			}
		}
		else {
			Copy(m_RT, m_RT); // Passthrough main render target.
		}
		
		//Copy(m_Albedo_gBuffer, m_RT);
		
		/* RENDER TO SCREEN */
		RenderTexture::Unbind();
		
		// Check if gamma correction is enabled in settings.
		if (Settings::Graphics::s_GammaCorrection) {
			
			// de Vries, J. (n.d.). LearnOpenGL - Gamma Correction. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Gamma-Correction [Accessed 15 Dec. 2023].
			glEnable(GL_FRAMEBUFFER_SRGB); // Enable gamma correction.
		}
	 
		// DRAW:
		Shader::Bind(Resources::Get<Shader>("passthrough").lock()->ID());
		if (const auto q = Mesh::Primitives::Quad::Instance().lock()) {
			Draw(*q);
		}
		
		// Reset gamma correction.
		glDisable(GL_FRAMEBUFFER_SRGB);
	}
	
	void Camera::Blur(const RenderTexture& _rt, const float& _intensity, const int& _passes, const bool& _highQuality, const bool& _consistentDPI) {
		
		// Get dimensions of screen.
		const auto dimensions = glm::vec2(_rt.Width(), _rt.Height());
		
		// Horizontal and vertical blur passes.
		if (const auto h = Resources::Get<Shader>("blur_horizontal").lock()) {
		if (const auto v = Resources::Get<Shader>("blur_vertical"  ).lock()) {
			
			// Assign target texture to both shader programs:
			h->Assign(h->AttributeID("u_Texture"), _rt, 0);
			v->Assign(v->AttributeID("u_Texture"), _rt, 0);
			  
			// Optionally, use a multiplier proportional to the screen's resolution to
			// keep the blur effect consistent across a range of different resolutions.
	        const float dpiFactor = _consistentDPI ?
	            glm::sqrt(dimensions.x * dimensions.y) * (1.0f / 3000.0f) : // (1.0f / 3000.0f) is an arbitrary value I chose by eye.
	            1.0f;
	
	        float rootIntensity, scalar, aspectCorrection;
	
			// Init rootIntensity, scalar, and aspectCorrection.
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
	
			// Perform a blur pass:
	        for (auto i = 0; i < _passes; ++i) {
	            
	            int width, height;
	            
	            float size;
	            
				// Init width, height and size.
	            if (_highQuality) {
	                width  = (int)dimensions.x;
	                height = (int)dimensions.y;
	                
	                size = (float)glm::pow(1.618f, i + 1) * _intensity;
	            }
	            else {
	                width  = (int)glm::ceil(dimensions.x / glm::pow(2.0f, (float)i / 2.0f * scalar));
	                height = (int)glm::ceil(dimensions.y / glm::pow(2.0f, (float)i / 2.0f * scalar) * aspectCorrection);
	                
	                size = (float)i * rootIntensity;
	            }
	            
				// Assign step value to both shader programs.
				h->Assign(h->AttributeID("u_Step"), dpiFactor * size);
				v->Assign(v->AttributeID("u_Step"), dpiFactor * size);
				  
				// Blit into a temporary texture, and blur that once on the x, and once on the y.
				const RenderTexture tmp(width, height, _rt.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), _rt.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
		        Blit(_rt, tmp, h);
		        Blit(tmp, tmp, v);
				
				// Copy the temp texture back to the original target.
				Copy(tmp, _rt);
			
				// Break early if the width or height are 1, as additional blurring is practically pointless.
	            if (width == 1 && height == 1) {
	                break;
	            }
	        }
		}}
	}
	
	void Camera::AutoExposure() {
	
		using target = Settings::PostProcessing::ToneMapping::AutoExposure;
		
		if (const auto w = GetWindow().lock()) {
			
			// Load shader program:
			if (const auto s = Resources::Get<Shader>("auto_exposure").lock()) {
				
				// Create a 32 by 32 render texture for the luminosity calculations.
				const glm::ivec2 luma_res(32, 32);
				
				const RenderTexture luma_out(luma_res.x, luma_res.y, Texture::Parameters::Format(m_RT.Format().PixelFormat(), false), Texture::Parameters::FilterMode(GL_LINEAR, GL_NEAREST), m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
				
				// Load a mask for the average luminosity calculation.
				if (const auto mask = Resources::Get<Texture>("exposure_weights").lock()) {
					
					// Generate the luminosity texture:
					Shader::Bind(s->ID());
					s->Assign(s->AttributeID("u_Weights"), *mask, 1);
				}
				
				Blit(m_RT, luma_out, s);
				
				// Create a buffer for the luminosity samples:
				std::vector<float> pixels(static_cast<size_t>(luma_res.x * luma_res.y * luma_out.Format().Channels()));
				
				// Load the luminosity samples into the buffer.
				RenderTexture::Bind(luma_out);
				glReadPixels(0, 0, luma_res.x, luma_res.y, luma_out.Format().TextureFormat(), GL_HALF_FLOAT, pixels.data());
				
				// Compute the average luminosity:
				int avg_count = 0;
				auto avg = 0.0f;
				
				for (auto y = 0; y < luma_res.y; y++) {
				for (auto x = 0; x < luma_res.x; x++) {
					
					const auto l = pixels.at((y * luma_res.x) + x);
					
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
				
				// Get difference between current exposure and average luma.
				const float diff = glm::clamp(
					(Settings::PostProcessing::ToneMapping::s_Exposure + target::s_Compensation) - avg,
					-1.0f,
					1.0f
				);
				
				// Determine the speed to change exposure by:
				const float speed = (diff - m_Exposure) >= 0 ?
						target::s_SpeedUp :
						target::s_SpeedDown;
				
				// Set a new exposure value:
				m_Exposure = glm::mix(
					m_Exposure,
					glm::clamp(Settings::PostProcessing::ToneMapping::s_Exposure + diff, target::s_MinEV, target::s_MaxEV),
					Time::DeltaTime() * speed
				);
			}
		}
		else {
			Debug::Log("Camera is not bound to a valid Window!", LogType::Error);
		}
	}
	
	void Camera::AmbientOcclusion() const {
		
		// Get shader program:
		if (const auto ao = Resources::Get<Shader>("ao").lock()) {
			
			// Get viewport dimensions:
			glm::ivec4 viewport;
			glGetIntegerv(GL_VIEWPORT, &viewport[0]);
			
			Shader::Bind(ao->ID());
			
			// Assign program values:
			ao->Assign(ao->AttributeID("u_Samples"), Settings::PostProcessing::AmbientOcclusion::s_Samples);
			
			ao->Assign(ao->AttributeID("u_Strength"),           Settings::PostProcessing::AmbientOcclusion::s_Intensity    );
			ao->Assign(ao->AttributeID("u_Bias"    ), -glm::min(Settings::PostProcessing::AmbientOcclusion::s_Radius, 0.2f));
			ao->Assign(ao->AttributeID("u_Radius"  ),           Settings::PostProcessing::AmbientOcclusion::s_Radius       );
			
			ao->Assign(ao->AttributeID("u_NearClip"), m_NearClip     );
			ao->Assign(ao->AttributeID("u_FarClip" ), m_FarClip      );
			ao->Assign(ao->AttributeID("u_Time"    ), Time::Elapsed());
			
			ao->Assign(ao->AttributeID("u_VP"  ), m_Projection * View());
			ao->Assign(ao->AttributeID("u_View"),                View());
			
			// Create a render texture for the AO. Optionally, downscale the texture
			// to save performance by computing AO at a lower resolution.
			const auto downscale = Settings::PostProcessing::AmbientOcclusion::s_Downscale;
			
			const RenderTexture ao_rt(
				glm::max(viewport[2] / (downscale + 1), 1),
				glm::max(viewport[3] / (downscale + 1), 1),
				Texture::Parameters::Format(GL_RGB, false),
				Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR),
				m_RT.WrapMode(),
				RenderTexture::Parameters::DepthMode::NONE
			);
			
			/* ASSIGN G-BUFFERS */
			ao->AssignDepth(
				ao->AttributeID("u_Position_gBuffer"),
				m_Position_gBuffer,
				0
			);
			
			ao->Assign(
				ao->AttributeID("u_Position_gBuffer"),
				m_Position_gBuffer,
				1
			);
			
			ao->Assign(
				ao->AttributeID("u_Normal_gBuffer"),
				m_Normal_gBuffer,
				2
			);
			
			// Set the viewport resolution to the scale of the AO render target.
			glViewport(viewport[0], viewport[1], ao_rt.Width(), ao_rt.Height());
			
			// Draw
			RenderTexture::Bind(ao_rt);
			if (const auto q = Mesh::Primitives::Quad::Instance().lock()) {
				Draw(*q);
			}
			
			// Blur the AO.
			Blur(ao_rt, 1.0f, 1, true, false);
	
			// Reset the viewport.
			glViewport(viewport[0], viewport[1], m_RT.Width(), m_RT.Height());
			
			// Apply the colors of the AO image to the main image using multiply blending:
		    glEnable(GL_BLEND);
		    glBlendFunc(GL_DST_COLOR, GL_ZERO);
	
			Copy(ao_rt, m_RT);
			
			glDisable(GL_BLEND);
		}
	}
	
	void Camera::Bloom() {
		
		using target = Settings::PostProcessing::Bloom;
		
		if (const auto w = GetWindow().lock()) {
			
			// Get screen dimensions.
			const auto dimensions = w->Dimensions();
			
			// Get each shader used for rendering the effect.
			if (const auto threshold_shader = Resources::Get<Shader>("threshold").lock()) {
			if (const auto downscale_shader = Resources::Get<Shader>("downscale").lock()) {
			if (const auto   upscale_shader = Resources::Get<Shader>("upscale"  ).lock()) {
			if (const auto   combine_shader = Resources::Get<Shader>("combine"  ).lock()) {
			if (const auto lens_dirt_shader = Resources::Get<Shader>("lens_dirt").lock()) {
				
				const auto target_length = static_cast<size_t>(std::max(
					static_cast<size_t>(target::s_Diffusion * 2),
					static_cast<size_t>(1)
				)) + 1;
				
				if (m_MipChain.empty() || m_MipChain.size() < target_length) {
					
					const auto offset = std::max(m_MipChain.size(), static_cast<size_t>(1)) - 1;
					
					for (auto i = 0; i < target_length; ++i) {
						
						const auto size = dimensions / static_cast<int>(std::pow(2.0, i + offset));
						
						if (size.x > 1 && size.y > 1) {
							
							m_MipChain.emplace_back(
								size.x,
								size.y,
								m_RT.Format(),
								Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR),
								m_RT.WrapMode(),
								RenderTexture::Parameters::DepthMode::NONE
							);
						}
						else {
							break;
						}
					}
				}
				else if (m_MipChain.size() > target_length) {
					m_MipChain.erase(m_MipChain.begin() + target_length, m_MipChain.end());
				}
				
				if (m_MipChain.size() > 1) {
				
					/* THRESHOLD PASS */
					Shader::Bind(threshold_shader->ID());
					threshold_shader->Assign(threshold_shader->AttributeID("u_Threshold"), target::s_Threshold);
					threshold_shader->Assign(threshold_shader->AttributeID("u_Clamp"    ), target::s_Clamp    );
					
					Blit(m_RT, m_MipChain[0], threshold_shader);
					
					/* DOWNSCALING */
					Shader::Bind(downscale_shader->ID());
					downscale_shader->Assign(downscale_shader->AttributeID("u_Resolution"), glm::vec2(dimensions[0], dimensions[1]));
					
					for (auto i = 1; i < m_MipChain.size(); ++i) {
						Blit(m_MipChain[i - 1], m_MipChain[i], downscale_shader);
					}
					
					/* UPSCALING */
					Shader::Bind(upscale_shader->ID());
					
					// Create the diffusion vector for the bloom algorithm:
					{
						const auto t = Utils::Remap(target::s_Anamorphism, -1.0f, 1.0f, 0.0f, 1.0f);
						
						// Imitate anamorphic artifacts by morphing the shape of the diffusion vector:
						const glm::vec2 diffusionVec(
								glm::mix(0.0, 3.0,       t),
								glm::mix(0.0, 3.0, 1.0 - t)
						);
						
						upscale_shader->Assign(
							upscale_shader->AttributeID("u_Diffusion"),
							diffusionVec
						);
					}
					
					// Enable additive blending
					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE);
					glBlendEquation(GL_FUNC_ADD);
					
					for (auto i = m_MipChain.size() - 1; i > 0; --i) {
						Blit(m_MipChain[i], m_MipChain[i - 1], upscale_shader);
					}
					
					// Disable additive blending
					glDisable(GL_BLEND);
					
					/* COMBINE */
					Shader::Bind(combine_shader->ID());
					combine_shader->Assign(combine_shader->AttributeID("u_Strength"), target::s_Intensity / glm::max((float)m_MipChain.size() - 1, 1.0f));
					combine_shader->Assign(combine_shader->AttributeID("u_Texture0"), m_RT,          0);
					combine_shader->Assign(combine_shader->AttributeID("u_Texture1"), m_MipChain[0], 1);
					
					// Blit to main render target:
					RenderTexture::Bind(m_RT);
					
					if (const auto q = Mesh::Primitives::Quad::Instance().lock()) {
						Draw(*q);
					}
					
					RenderTexture::Unbind();
					
					/* LENS DIRT */
					if (Settings::PostProcessing::Bloom::s_LensDirt > 0.0) {
						
						if (const auto t = Resources::Get<Texture>("Bokeh__Lens_Dirt_65").lock()) {
							
							Shader::Bind(lens_dirt_shader->ID());
							lens_dirt_shader->Assign(lens_dirt_shader->AttributeID("u_Strength"), target::s_LensDirt * target::s_Intensity);
							lens_dirt_shader->Assign(lens_dirt_shader->AttributeID("u_Texture0"), m_RT,          0);
							lens_dirt_shader->Assign(lens_dirt_shader->AttributeID("u_Bloom"),    m_MipChain[0], 1);
							lens_dirt_shader->Assign(lens_dirt_shader->AttributeID("u_Dirt"),                *t, 2);
							
							// Blit to main render target:
							RenderTexture::Bind(m_RT);
							if (const auto q = Mesh::Primitives::Quad::Instance().lock()) {
								Draw(*q);
							}
						}
					}
				}
				else {
					Debug::Log("Error creating mip chain!", LogType::Error);
				}
			}}}}}
		}
		else {
			Debug::Log("Camera is not bound to a valid Window!", LogType::Error);
		}
	}
	
	void Camera::Copy(const RenderTexture& _src, const RenderTexture& _dest) {
		
		// Simply blit _src to _dest using a passthrough shader.
		if (const auto p = Resources::Get<Shader>("passthrough").lock()) {
			Blit(_src, _dest, p);
		}
		else {
			Debug::Log("Couldn't get passthrough shader!", LogType::Error);
		}
	}
	
	void Camera::Blit(const RenderTexture& _src, const RenderTexture& _dest, const std::weak_ptr<Shader>& _shader) {
		
		if (const auto s = _shader.lock()) {
			
			// Cache the viewport dimensions.
			glm::ivec4 viewport;
			glGetIntegerv(GL_VIEWPORT, &viewport[0]);
			
			// Determine if the viewport dimensions need to change.
			const bool dimensionsDirty = viewport[2] != _dest.Width() ||
			                             viewport[3] != _dest.Height();
			
			if (dimensionsDirty) {
				
				// Set new viewport dimensions.
				glViewport(viewport[0], viewport[1], _dest.Width(), _dest.Height());
			}
	
			// Bind program.
			Shader::Bind(s->ID());
	
			// Enable texture channel.
			glActiveTexture(GL_TEXTURE0);
	
			if (Texture::s_CurrentTexture != _src.ID()) {
				glBindTexture(GL_TEXTURE_2D, Texture::s_CurrentTexture = static_cast<GLint>(_src.ID()));
			}
			
			// Bind destination target:
			RenderTexture::Bind(_dest);
			
			if (const auto q = Mesh::Primitives::Quad::Instance().lock()) {
				Draw(*q);
			}

			if (dimensionsDirty) {
				
				// Reset viewport dimensions:
				glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			}
		}
		else {
			Debug::Log("Couldn't lock _shader!", LogType::Error);
		}
	}
	
	void Camera::SetWindow(const std::weak_ptr<Window>& _window) {
		
		if (const auto w = _window.lock()) {
			w->Link(*this);
		}
		else {
			Debug::Log("Provided Window is invalid!", LogType::Error);
		}
	}
	const std::weak_ptr<Window>& Camera::GetWindow() const noexcept {
		return m_Window;
	}
	
	void Camera::SetTransform(const std::weak_ptr<Transform>& _transform) noexcept {
		m_Transform = _transform;
	}
	const std::weak_ptr<Transform>& Camera::GetTransform() const noexcept {
		return m_Transform;
	}
	
	float Camera::Aspect() const {
		
		float result;
		
		if (const auto w = m_Window.lock()) {
			result = w->Aspect();
		}
		else {
			result = 1.0f;
			
			Debug::Log("Camera is not bound to a valid Window!", LogType::Error);
		}
		
		return result;
	}
	
	void Camera::FOV(const float& _fov) noexcept {
		
		// Check if the value differs from the current one.
		if (m_FOV != _fov) {
			m_FOV = _fov;
			
			m_IsDirty = true; // Set dirty.
		}
	}
	const float& Camera::FOV() const noexcept {
		return m_FOV;
	}
	
	void Camera::NearClip(const float& _nearClip) noexcept {
		
		// Check if the value differs from the current one.
		if (m_NearClip != _nearClip) {
			m_NearClip = _nearClip;
			
			m_IsDirty = true; // Set dirty.
		}
	}
	const float& Camera::NearClip() const noexcept {
		return m_NearClip;
	}
	
	void Camera::FarClip(const float& _farClip) noexcept {
		
		// Check if the value differs from the current one.
		if (m_FarClip != _farClip) {
			m_FarClip = _farClip;
			
			m_IsDirty = true; // Set dirty.
		}
	}
	const float& Camera::FarClip() const noexcept {
		return m_FarClip;
	}
	
	void Camera::ClearColor(glm::vec4 _color) {
		
		// Set the clear color to the provided.
		glClearColor(_color[0], _color[1], _color[2], _color[3]);
	}
	glm::vec4 Camera::ClearColor() {
		
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
		
		glm::mat4 result;
		
		if (const auto transform = GetTransform().lock()) {
			result = glm::lookAt(
				transform->m_Position,
				transform->m_Position + transform->FORWARD,
				transform->UP
			);
		}
		else {
			result = glm::mat4(1.0f);
			
			Debug::Log("No valid Transform Component on Camera!", LogType::Error);
		}
		
		return result;
	}
	
	void Camera::SetDirty() noexcept {
		m_IsDirty = true;
	}
	
	void Camera::Draw(const Mesh& _mesh) {
		
		// Bind VAO.
		Mesh::Bind(_mesh);
		
		if (_mesh.IndexFormat() == GL_NONE) {
			glDrawArrays(_mesh.Format(), 0, static_cast<GLsizei>(_mesh.VertexCount()));
		}
		else {
			glDrawElements(_mesh.Format(), static_cast<GLsizei>(_mesh.IndexCount()), _mesh.IndexFormat(), nullptr);
		}
		
	}
	
} // LouiEriksson::Engine::Graphics