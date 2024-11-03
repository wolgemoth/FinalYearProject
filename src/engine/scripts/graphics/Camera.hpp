#ifndef FINALYEARPROJECT_CAMERA_HPP
#define FINALYEARPROJECT_CAMERA_HPP

#include "../core/Debug.hpp"
#include "../core/IViewport.hpp"
#include "../core/Resources.hpp"
#include "../core/Settings.hpp"
#include "../core/Time.hpp"
#include "../core/Transform.hpp"
#include "../core/Types.hpp"
#include "../core/utils/Utils.hpp"
#include "../ecs/GameObject.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <GL/glew.h>

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace LouiEriksson::Engine {
	
	class Window;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Light;
	class Mesh;
	class Renderer;
	class Shader;
	class Texture;
	
	/**
	 * @class Camera
	 * @brief Represents a Camera component that can be attached to a Parent.
	 */
	class Camera final : public ECS::Component {
	
		friend LouiEriksson::Engine::Window;
	
	private:
		
		inline static std::weak_ptr<Shader> s_Passthrough{};
		inline static std::weak_ptr<Mesh>   s_Cube{};
		
		/** @brief Window of the camera. */
		std::weak_ptr<IViewport<scalar_t, size_t>> m_Viewport;
	
		/** @brief Camera Transform. */
		std::weak_ptr<Transform> m_Transform;
		
		/* PERSPECTIVE */
		
		/** @brief Projection matrix. */
		mat4 m_Projection;
		
		/** @brief Flag that indicates if the projection matrix needs to be recalculated or not. */
		bool m_IsDirty;
		
		float m_FOV;        // Field of view.
		float m_NearClip;   // Near clipping plane.
		float m_FarClip;    // Far clipping plane.
		
		/** @brief Main render target. */
		RenderTexture m_RT;
		
		/* G-BUFFER: */
		
		RenderTexture   m_Albedo_gBuffer; // Albedo channel.
		RenderTexture m_Emission_gBuffer; // Emission channel.
		RenderTexture m_Material_gBuffer; // Material properties (Roughness, Metallic).
		RenderTexture m_Position_gBuffer; // World-space surface positions.
		RenderTexture   m_Normal_gBuffer; // Surface normals.
		RenderTexture m_TexCoord_gBuffer; // Texture coordinates.
		
		/* EFFECTS */
		
		float m_Exposure; // Camera exposure for tonemapping.
		
		/** @brief Mip chain for bloom effect. */
		std::vector<RenderTexture> m_Bloom_MipChain;
		
		/** @brief RenderTexture for AO effect. */
		RenderTexture m_AO_RT;
		
		/** @brief RenderTexture for Auto-Exposure luminance detection. */
		RenderTexture m_AutoExposure_Luma;
		
		/* METHODS */
		
		/**
		 * \brief Deferred-rendering geometry pass.
		 *
		 * \param[in] _renderers The list of renderers to perform the geometry pass for.
		 */
		void GeometryPass(const std::vector<std::weak_ptr<Renderer>>& _renderers) {
			
			if (const auto v = m_Viewport.lock()) {
				
				if (const auto t = GetTransform().lock()) {
				
					// Get the current culling and depth modes and cache them here.
					GLint cullMode, depthMode;
					glGetIntegerv(GL_CULL_FACE_MODE, &cullMode);
					glGetIntegerv(GL_DEPTH_FUNC,     &depthMode);
					
					// Texture Coordinates:
					{
						static const auto shader = Resources::Get<Shader>("pass_texcoords");
						if (const auto p = shader) {
							
							// Bind program.
							Shader::Bind(p->ID());
							
							static const auto          u_Projection = p->AttributeID("u_Projection");
							static const auto                u_View = p->AttributeID("u_View");
							static const auto               u_Model = p->AttributeID("u_Model");
							static const auto      u_CameraPosition = p->AttributeID("u_CameraPosition");
							static const auto        u_Displacement = p->AttributeID("u_Displacement");
							static const auto u_Displacement_Amount = p->AttributeID("u_Displacement_Amount");
							static const auto                  u_ST = p->AttributeID("u_ST");
							
							p->Assign(u_Projection, Projection()); /* PROJECTION */
							p->Assign(u_View,             View()); /* VIEW       */
							
							p->Assign(u_ST, Settings::Graphics::Material::s_TextureScaleTranslate);
							
							p->Assign(u_CameraPosition,
								t != nullptr ?
										t->Position() :
									vec3(0.0)
							);
							
							RenderTexture::Bind(m_TexCoord_gBuffer);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
							for (const auto& renderer : _renderers) {
								
								if (const auto  r = renderer.lock()         ) {
								if (const auto tr = r->GetTransform().lock()) {
								if (const auto ma = r->GetMaterial().lock() ) {
								if (const auto me = r->GetMesh().lock()     ) {
									
									p->Assign(u_Displacement_Amount, ma->GetDisplacementAmount());
									
									// Assign textures:
									if (const auto d = ma->GetDisplacementTexture().lock()) {
										p->Assign(u_Displacement, *d, 0);
									}
									
									// Assign matrices.
									p->Assign(u_Model, tr->World());
									
									/* DRAW */
									Draw(*me);
								}}}}
							}
						}
					}
					
					// Positions:
					{
						static const auto shader = Resources::Get<Shader>("pass_positions");
						if (const auto p = shader) {
							
							// Bind program.
							Shader::Bind(p->ID());
							
							static const auto u_Projection = p->AttributeID("u_Projection");
							static const auto       u_View = p->AttributeID("u_View");
							static const auto      u_Model = p->AttributeID("u_Model");
							
							p->Assign(u_Projection, Projection()); /* PROJECTION */
							p->Assign(u_View,             View()); /* VIEW       */
							
							RenderTexture::Bind(m_Position_gBuffer);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
							for (const auto& renderer : _renderers) {
								
								if (const auto  r = renderer.lock()         ) {
								if (const auto tr = r->GetTransform().lock()) {
								if (const auto me = r->GetMesh().lock()     ) {
									
									// Assign matrices.
									p->Assign(u_Model, tr->World()); /* MODEL      */
									
									/* DRAW */
									Draw(*me);
								}}}
							}
						}
					}
					
					// Albedo:
					{
						static const auto shader = Resources::Get<Shader>("pass_albedo");
						if (const auto p = shader) {
							
							// Bind program.
							Shader::Bind(p->ID());
							
							static const auto       u_Projection = p->AttributeID("u_Projection");
							static const auto             u_View = p->AttributeID("u_View");
							static const auto            u_Model = p->AttributeID("u_Model");
							static const auto           u_Albedo = p->AttributeID("u_Albedo");
							static const auto u_TexCoord_gBuffer = p->AttributeID("u_TexCoord_gBuffer");
							static const auto u_ScreenDimensions = p->AttributeID("u_ScreenDimensions");
							static const auto      u_AlbedoColor = p->AttributeID("u_AlbedoColor");
							
							p->Assign(u_Projection, Projection()); /* PROJECTION */
							p->Assign(u_View,             View()); /* VIEW       */
							
							p->Assign(u_ScreenDimensions,
								v != nullptr ?
									(vec2)v->Dimensions() :
									 vec2(1.0)
							);
							
							p->Assign(
								u_TexCoord_gBuffer,
								m_TexCoord_gBuffer,
								1
							);
							
							RenderTexture::Bind(m_Albedo_gBuffer);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
							for (const auto& renderer : _renderers) {
								
								if (const auto  r = renderer.lock()         ) {
								if (const auto tr = r->GetTransform().lock()) {
								if (const auto ma = r->GetMaterial().lock() ) {
								if (const auto me = r->GetMesh().lock()     ) {
								
									// Assign matrices.
									p->Assign(u_Model, tr->World()); /* MODEL */
									
									p->Assign(u_AlbedoColor, ma->GetAlbedoColor());
							
									// Assign textures:
									if (const auto a = ma->GetAlbedoTexture().lock()) {
										p->Assign(u_Albedo, *a, 0);
									}
									
									/* DRAW */
									Draw(*me);
								}}}}
							}
						}
					}
					
					// Emission:
					{
						static const auto shader = Resources::Get<Shader>("pass_emission");
						if (const auto p = shader) {
							
							// Bind program.
							Shader::Bind(p->ID());
							
							static const auto       u_Projection = p->AttributeID("u_Projection");
							static const auto             u_View = p->AttributeID("u_View");
							static const auto            u_Model = p->AttributeID("u_Model");
							static const auto         u_Emission = p->AttributeID("u_Emission");
							static const auto u_TexCoord_gBuffer = p->AttributeID("u_TexCoord_gBuffer");
							static const auto u_ScreenDimensions = p->AttributeID("u_ScreenDimensions");
							static const auto    u_EmissionColor = p->AttributeID("u_EmissionColor");
							
							p->Assign(u_Projection, Projection()); /* PROJECTION */
							p->Assign(u_View,             View()); /* VIEW       */
							
							p->Assign(
								u_TexCoord_gBuffer,
								m_TexCoord_gBuffer,
								1
							);
							
							p->Assign(u_ScreenDimensions,
								v != nullptr ?
									(vec2)v->Dimensions() :
									 vec2(1.0)
							);
							
							RenderTexture::Bind(m_Emission_gBuffer);
							glClear(GL_DEPTH_BUFFER_BIT);
							
							for (const auto& renderer : _renderers) {
								
								if (const auto  r = renderer.lock()         ) {
								if (const auto tr = r->GetTransform().lock()) {
								if (const auto ma = r->GetMaterial().lock() ) {
								if (const auto me = r->GetMesh().lock()     ) {
								
									// Assign matrices.
									p->Assign(u_Model, tr->World()); /* MODEL      */
									
									p->Assign(u_EmissionColor, ma->GetEmissionColor());
							
									// Assign textures:
									if (const auto e = ma->GetEmissionTexture().lock()) {
										p->Assign(u_Emission, *e, 0);
									}
									
									/* DRAW */
									Draw(*me);
								}}}}
							}
							
							/* DRAW SKY */
							static const auto sky_shader = Resources::Get<Shader>("skybox");
							if (const auto s = sky_shader) {
								
								// Change culling and depth options for skybox rendering.
								glCullFace (GL_FRONT);
								glDepthFunc(GL_LEQUAL);
					
								Shader::Bind(s->ID());
					
								static const auto sky_u_Projection = s->AttributeID("u_Projection");
								static const auto sky_u_View       = s->AttributeID("u_View");
								static const auto sky_u_Model      = s->AttributeID("u_Model");
								static const auto sky_u_Texture    = s->AttributeID("u_Texture");
								static const auto sky_u_Exposure   = s->AttributeID("u_Exposure");
								static const auto sky_u_Blur       = s->AttributeID("u_Blur");
								
								static const auto sky_TRS = glm::scale(
									mat4(1.0),
									vec3(2.0)
								);
					
								// Assign matrices.
								s->Assign(sky_u_Projection, Projection()      ); /* PROJECTION */
								s->Assign(sky_u_View,       mat4(mat3(View()))); /* VIEW       */
								s->Assign(sky_u_Model,      sky_TRS           ); /* MODEL      */
								
								// Assign texture:
								if (const auto sky = Settings::Graphics::Skybox::s_Skybox.lock()) {
									s->Assign(sky_u_Texture, *sky, 0);
								}
								
								s->Assign(sky_u_Exposure, Settings::Graphics::Skybox::s_Exposure);
								s->Assign(sky_u_Blur,     Settings::Graphics::Skybox::s_Blur);
					
								// Bind VAO.
								if (const auto c = s_Cube.lock()) {
									Draw(*c);
								}
								
								// Restore culling and depth options.
								glCullFace ( cullMode);
								glDepthFunc(depthMode);
							}
						}
					}
					
					// Surface properties Roughness, Metallic, AO, Parallax Shadows:
					{
						static const auto shader = Resources::Get<Shader>("pass_material");
						if (const auto p = shader) {
							
							// Bind program.
							Shader::Bind(p->ID());
							
							static const auto          u_Projection = p->AttributeID("u_Projection");
							static const auto                u_View = p->AttributeID("u_View");
							static const auto               u_Model = p->AttributeID("u_Model");
							static const auto       u_LightPosition = p->AttributeID("u_LightPosition");
							static const auto           u_Roughness = p->AttributeID("u_Roughness");
							static const auto            u_Metallic = p->AttributeID("u_Metallic");
							static const auto                  u_AO = p->AttributeID("u_AO");
							static const auto        u_Displacement = p->AttributeID("u_Displacement");
							static const auto           u_TexCoords = p->AttributeID("u_TexCoord_gBuffer");
							static const auto    u_ScreenDimensions = p->AttributeID("u_ScreenDimensions");
							static const auto     u_ParallaxShadows = p->AttributeID("u_ParallaxShadows");
							static const auto    u_Roughness_Amount = p->AttributeID("u_Roughness_Amount");
							static const auto           u_AO_Amount = p->AttributeID("u_AO_Amount");
							static const auto u_Displacement_Amount = p->AttributeID("u_Displacement_Amount");
							
							p->Assign(u_Projection, Projection()); /* PROJECTION */
							p->Assign(u_View,             View()); /* VIEW       */
							
							p->Assign(u_TexCoords, m_TexCoord_gBuffer, 4);
				
							p->Assign(u_ParallaxShadows, Settings::Graphics::Material::s_ParallaxShadows);
							
							const auto lightType = (Light::Parameters::Type)Settings::Graphics::Material::s_CurrentLightType;
							
							const auto lightPos = lightType == Light::Parameters::Type::Directional ?
									t->Position() + (VEC_FORWARD * glm::inverse(glm::quat(glm::radians(Settings::Graphics::Material::s_LightRotation))) * static_cast<scalar_t>(65535.0)) :
									Settings::Graphics::Material::s_LightPosition;
							
							p->Assign(u_LightPosition, lightPos);
							
							p->Assign(u_ScreenDimensions,
								v != nullptr ?
									(vec2)v->Dimensions() :
									 vec2(1.0)
							);
							
							RenderTexture::Bind(m_Material_gBuffer);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
							
							for (const auto& renderer : _renderers) {
								
								if (const auto  r = renderer.lock()         ) {
								if (const auto tr = r->GetTransform().lock()) {
								if (const auto ma = r->GetMaterial().lock() ) {
								if (const auto me = r->GetMesh().lock()     ) {
								
									// Assign matrices.
									p->Assign(u_Model, tr->World()); /* MODEL */
									
									p->Assign(u_Roughness_Amount, ma->GetRoughnessAmount()   );
									p->Assign(u_Displacement_Amount, ma->GetDisplacementAmount());
									p->Assign(u_AO_Amount, ma->GetAOAmount()          );
									
									// Assign textures:
									if (const auto ro = ma->GetRoughnessTexture().lock()) {
										p->Assign(u_Roughness, *ro, 0);
									}
									
									if (const auto met = ma->GetMetallicTexture().lock()) {
										p->Assign(u_Metallic, *met, 1);
									}
									
									if (const auto a = ma->GetAOTexture().lock()) {
										p->Assign(u_AO, *a, 2);
									}
									
									if (const auto d = ma->GetDisplacementTexture().lock()) {
										p->Assign(u_Displacement, *d, 3);
									}
									
									/* DRAW */
									Draw(*me);
								}}}}
							}
						}
					}
					
					// Normals:
					{
						static const auto shader = Resources::Get<Shader>("pass_normals");
						if (const auto p = shader) {
							
							// Bind program.
							Shader::Bind(p->ID());
							
							static const auto       u_Projection = p->AttributeID("u_Projection");
							static const auto             u_View = p->AttributeID("u_View");
							static const auto            u_Model = p->AttributeID("u_Model");
							static const auto          u_Normals = p->AttributeID("u_Normals");
							static const auto u_TexCoord_gBuffer = p->AttributeID("u_TexCoord_gBuffer");
							static const auto     u_NormalAmount = p->AttributeID("u_NormalAmount");
							static const auto u_ScreenDimensions = p->AttributeID("u_ScreenDimensions");
							
							p->Assign(u_Projection, Projection()); /* PROJECTION */
							p->Assign(u_View,             View()); /* VIEW       */
							
							p->Assign(u_TexCoord_gBuffer, m_TexCoord_gBuffer, 1);
							
							p->Assign(u_ScreenDimensions,
								v != nullptr ?
									(vec2)v->Dimensions() :
									 vec2(1.0)
							);
							
							RenderTexture::Bind(m_Normal_gBuffer);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
							for (const auto& renderer : _renderers) {
								
								if (const auto  r = renderer.lock()         ) {
								if (const auto tr = r->GetTransform().lock()) {
								if (const auto ma = r->GetMaterial().lock() ) {
								if (const auto me = r->GetMesh().lock()     ) {
								
									// Assign matrices.
									p->Assign(u_Model, tr->World()); /* MODEL */
									
									p->Assign(u_NormalAmount, ma->GetNormalAmount());
									p->Assign(u_Normals, *ma->GetNormalTexture().lock(), 0);
									
									/* DRAW */
									Draw(*me);
								}}}}
							}
						}
					}
				}
				else {
					Debug::Log("Camera has no valid transform!", Error);
				}
			}
			else {
				Debug::Log("Camera is not bound to a valid viewport!", Error);
			}
		}
		
		/**
		 * \brief Deferred-rendering shadow pass.
		 *
		 * \param[in] _renderers The list of renderers to perform the geometry pass for.
		 */
		void ShadowPass(const std::vector<std::weak_ptr<Renderer>>& _renderers, const std::vector<std::weak_ptr<Light>>& _lights) const  {
		
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
							case Light::Parameters::Point:       { p = Resources::Get<Shader>("shadowDepthCube"); break; }
							case Light::Parameters::Directional: { p = Resources::Get<Shader>("shadowDepth"    ); break; }
							case Light::Parameters::Spot:        { p = Resources::Get<Shader>("shadowDepthSpot"); break; }
							default: {
								Debug::Log("Unknown Light type!", Error);
							}
						}
						
						if (p != nullptr) {
						
							Shader::Bind(p->ID());
							
							// Compute the size of a texel in world space.
							// We can round the light's position to these coordinates
							// to reduce an artifact known as "shimmering".
							const float texelSize = l->m_Range / static_cast<float>((l->m_Shadow.m_Resolution / 4));
							
							const auto lightDir = VEC_FORWARD * l->m_Transform.lock()->Rotation();
							
							vec3 lightPos;
							{
								const auto t = GetTransform().lock();
								
								if (l->Type() == Light::Parameters::Type::Directional &&
								    t != nullptr
								) {
									
									// Compute the position of the light.
									const vec3 truncatedCamPos = glm::floor(
											t->Position() / texelSize) * texelSize;
									
									lightPos = truncatedCamPos + (lightDir * (l->m_Range / static_cast<GLfloat>(2.0)));
								}
								else {
									lightPos = l->m_Transform.lock()->Position();
								}
							}
							
							if (l->Type() == Light::Parameters::Type::Point) {
								
								// Collection of shadow transforms for each face of the cubemap.
								const std::array<mat4, 6U> shadowTransforms {
									l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + vec3( 1.0, 0.0, 0.0), vec3(0.0,-1.0, 0.0)),
									l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + vec3(-1.0, 0.0, 0.0), vec3(0.0,-1.0, 0.0)),
									l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + vec3( 0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0)),
									l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + vec3( 0.0,-1.0, 0.0), vec3(0.0, 0.0,-1.0)),
									l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + vec3( 0.0, 0.0, 1.0), vec3(0.0,-1.0, 0.0)),
									l->m_Shadow.m_Projection * glm::lookAt(lightPos, lightPos + vec3( 0.0, 0.0,-1.0), vec3(0.0,-1.0, 0.0))
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
								
								l->m_Shadow.m_ViewProjection = mat4(1.0);
							}
							else {
								
								mat4 lightView;
								
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
										p->Assign(p->AttributeID("u_Model"), t->World());
										
										/* DRAW */
										Draw(*me);
									}
								}}}
							}
						}
						else {
							Debug::Log("Couldn't lock shadow shader program!", Error);
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
		
		/**
		 * @brief Copies the contents of one RenderTexture to another.
		 *
		 * This function uses a passthrough shader to perform the copy operation.
		 *
		 * @param[in] _src The source RenderTexture to copy from.
		 * @param[in] _dest The destination RenderTexture to copy to.
		 */
		static void Copy(const RenderTexture& _src, const RenderTexture& _dest) {
			
			// Simply blit _src to _dest using a passthrough shader.
			if (const auto p = s_Passthrough.lock()) {
				Blit(_src, _dest, p);
			}
			else {
				Debug::Log("Couldn't value passthrough shader!", Error);
			}
		}
		
		/**
		 * @brief Blits the contents of one RenderTexture to another using a specific shader.
		 *
		 * @param[in] _src The source RenderTexture to blit from.
		 * @param[in] _dest The destination RenderTexture to blit to.
		 * @param[in] _shader The shader to use for the blit operation.
		 */
		static void Blit(const RenderTexture& _src, const RenderTexture& _dest, const std::weak_ptr<Shader>& _shader) {
			
			if (const auto s = _shader.lock()) {
				
				// Cache the viewport dimensions.
				ivec4 viewport;
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
		
				Texture::Bind(_src);
				
				// Bind destination target:
				RenderTexture::Bind(_dest);
				
				if (const auto q = Mesh::Primitives::Quad<GLfloat>::Instance().lock()) {
					Draw(*q);
				}
	
				if (dimensionsDirty) {
					
					// Reset viewport dimensions:
					glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
				}
			}
			else {
				Debug::Log("Couldn't lock _shader!", Error);
			}
		}
		
		/**
		 * @fn void Draw(const Graphics::Mesh& _mesh)
		 * @brief Draws the given mesh using the current camera.
		 *
		 * This function is a static member of the Camera struct and is used to draw
		 * the given mesh using the current camera. It determine whether to call
		 * glDrawArrays or glDrawElements by checking the index format of the mesh.
		 *
		 * @param[in] _mesh The mesh to draw.
		 */
		static void Draw(const Graphics::Mesh& _mesh) {

			// Bind VAO.
			Mesh::Bind(_mesh);
			
			if (_mesh.IndexFormat() == GL_NONE) {
				glDrawArrays(_mesh.Format(), 0, static_cast<GLsizei>(_mesh.VertexCount()));
			}
			else {
				glDrawElements(_mesh.Format(), static_cast<GLsizei>(_mesh.IndexCount()), _mesh.IndexFormat(), nullptr);
			}
		}
		
		[[nodiscard]] bool IsDirty() const {
			
			auto result = m_IsDirty;
			
			if (const auto v = m_Viewport.lock()) {
				result |= v->IsDirty();
			}
			
			return result;
		}
		
		/* POST PROCESSING */
		
		/**
		 * @brief Apply a blur effect to the given RenderTexture.
		 *
		 * @param[in] _rt The RenderTexture to apply the blur effect to.
		 * @param[in] _intensity The intensity of the blur effect. Higher values result in a stronger blur effect.
		 * @param[in] _passes The number of blur passes to perform.
		 * @param[in] _highQuality Flag indicating whether to use high-quality settings for the blur effect.
		 * @param[in] _consistentDPI Flag indicating whether to keep the blur effect consistent across different resolutions.
		 */
		static void Blur(const RenderTexture& _rt, const float& _intensity, const int& _passes, const bool& _highQuality, const bool& _consistentDPI) {
		
			// Get dimensions of screen.
			const auto dimensions = vec2(_rt.Width(), _rt.Height());
			
			// Horizontal and vertical blur passes.
			static const auto blur_h = Resources::Get<Shader>("blur_horizontal");
			static const auto blur_v = Resources::Get<Shader>("blur_vertical"  );
			
			if (const auto h = blur_h) {
			if (const auto v = blur_v) {
				
				static const auto h_u_Texture = h->AttributeID("u_Texture");
				static const auto v_u_Texture = v->AttributeID("u_Texture");
				static const auto h_u_Step    = h->AttributeID("u_Step"   );
				static const auto v_u_Step    = v->AttributeID("u_Step"   );
				
				// Assign target texture to both shader programs:
				h->Assign(h_u_Texture, _rt, 0);
				v->Assign(v_u_Texture, _rt, 0);
				
				// Optionally, use a multiplier proportional to the screen's resolution to
				// keep the blur effect consistent across a range of different resolutions.
		        const auto dpiFactor = static_cast<scalar_t>(
					_consistentDPI ?
		                glm::sqrt(dimensions.x * dimensions.y) * (1.0 / 3000.0) : // (1.0 / 3000.0) is an arbitrary value I chose by eye.
	                    1.0
				);
		
		        float rootIntensity, scalar, aspectCorrection;
		
				// Init rootIntensity, scalar, and aspectCorrection.
		        if (_highQuality) {
		               rootIntensity = 0.0;
		                      scalar = 0.0;
		            aspectCorrection = 0.0;
		        }
		        else {
		               rootIntensity = glm::sqrt(_intensity);
		                      scalar = glm::sqrt(rootIntensity);
		            aspectCorrection = glm::sqrt(dimensions.x / static_cast<float>(dimensions.y));
		        }
		
				static RenderTexture tmp(1, 1, _rt.Format(), Texture::Parameters::FilterMode(GL_LINEAR, GL_LINEAR), _rt.WrapMode(), RenderTexture::Parameters::DepthMode::NONE);
				
				if (_highQuality) {
					tmp.Reinitialise(static_cast<int>(dimensions.x), static_cast<int>(dimensions.y));
				}
				
				// Perform a blur pass:
		        for (auto i = 0; i < _passes; ++i) {
		            
		            int width, height;
		            
		            float size;
		            
					// Init width, height and size.
		            if (_highQuality) {
		                width  = static_cast<int>(dimensions.x);
		                height = static_cast<int>(dimensions.y);
		                
		                size = static_cast<float>(std::pow(1.618, i + 1) * _intensity);
		            }
		            else {
		                width  = static_cast<int>(glm::ceil(dimensions.x / std::pow(2.0, static_cast<float>(i) / 2.0 * scalar)));
		                height = static_cast<int>(glm::ceil(dimensions.y / std::pow(2.0, static_cast<float>(i) / 2.0 * scalar) * aspectCorrection));
		                
		                size = static_cast<float>(i) * rootIntensity;
						
						tmp.Reinitialise(width, height);
		            }
		            
					// Assign step value to both shader programs.
					h->Assign(h_u_Step, dpiFactor * size);
					v->Assign(v_u_Step, dpiFactor * size);
					
					// Blit into a temporary texture, and blur that once on the x, and once on the y.
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
		
		/**
		 * @brief Auto exposure effect for use in conjunction with tonemapping.
		 *
		 * This method calculates the average luminosity of the image and adjusts the exposure value accordingly to
		 * compensate for overexposure or underexposure. The calculation is done using a shader program and a mask texture.
		 */
		void AutoExposure()  {
		
			using target = Settings::PostProcessing::ToneMapping::AutoExposure;
			
			if (const auto v = m_Viewport.lock()) {
				
				static const auto autoExposure = Resources::Get<Shader>("auto_exposure");
				static const auto         mask = Resources::Get<Texture>("exposure_weights");
				
				// Load shader program:
				if (const auto s = autoExposure) {
					
					// Load a mask for the average luminosity calculation.
					if (const auto m = mask) {
						
						// Generate the luminosity texture:
						Shader::Bind(s->ID());
						
						static const auto u_Weights = s->AttributeID("u_Weights");
						
						s->Assign(u_Weights, *m, 1);
					}
					
					Blit(m_RT, m_AutoExposure_Luma, s);
					
					// Create a buffer for the luminosity samples:
					static std::vector<float> pixels(static_cast<size_t>(m_AutoExposure_Luma.Width() * m_AutoExposure_Luma.Height() * m_AutoExposure_Luma.Format().Channels()));
					
					// Load the luminosity samples into the buffer.
					RenderTexture::Bind(m_AutoExposure_Luma);
					glReadPixels(0, 0, m_AutoExposure_Luma.Width(), m_AutoExposure_Luma.Height(), m_AutoExposure_Luma.Format().TextureFormat(), GL_HALF_FLOAT, pixels.data());
					
					// Compute the average luminosity:
					int avg_count = 0;
					auto avg = 0.0;
					
					for (auto y = 0; y < m_AutoExposure_Luma.Height(); y++) {
					for (auto x = 0; x < m_AutoExposure_Luma.Width();  x++) {
						
						const auto& l = pixels.at(Utils::To1D({x, y}, m_AutoExposure_Luma.Width()));
						
						if (l > 0) {
							avg += l;
							
							avg_count++;
						}
					}}
					
					// Detect and prevent NaN propagation:
					if (std::isnan(avg)) {
						avg = 0.0;
					}
					
					avg = avg / static_cast<float>(std::max(avg_count, 1));
					
					// Get difference between current exposure and average luma.
					const auto diff = static_cast<scalar_t>(std::clamp(
						(Settings::PostProcessing::ToneMapping::s_Exposure + target::s_Compensation) - avg,
						-1.0,
						1.0
					));
					
					// Determine the speed to change exposure by:
					const float speed = (diff - m_Exposure) >= 0 ?
						target::s_SpeedUp :
						target::s_SpeedDown;
					
					// Set a new exposure value:
					m_Exposure = glm::mix(
						m_Exposure,
						std::clamp(Settings::PostProcessing::ToneMapping::s_Exposure + diff, target::s_MinEV, target::s_MaxEV),
						Time::DeltaTime() * speed
					);
				}
			}
			else {
				Debug::Log("Camera is not bound to a valid Window!", Error);
			}
		}
		
		/** @brief Ambient occlusion post-processing effect using SSAO technique. */
		void AmbientOcclusion()  {
			
			static const auto as = Resources::Get<Shader>("ao");
			
			// Get shader program:
			if (const auto ao = as) {
				
				// Get viewport dimensions:
				ivec4 viewport;
				glGetIntegerv(GL_VIEWPORT, &viewport[0]);
				
				{
					// Create a render texture for the AO. Optionally, downscale the texture
					// to save performance by computing AO at a lower resolution.
					const auto downscale = Settings::PostProcessing::AmbientOcclusion::s_Downscale;
					const auto  width = std::max(viewport[2] / (downscale + 1), 1);
					const auto height = std::max(viewport[3] / (downscale + 1), 1);
					
					if (m_AO_RT.m_FBO_ID == GL_NONE || width != m_AO_RT.m_Width || height != m_AO_RT.m_Height) {
						m_AO_RT.Reinitialise(width, height);
					}
				}
				
				if (m_AO_RT.m_FBO_ID != GL_NONE) {
					
					Shader::Bind(ao->ID());
					
					static const auto u_Position_gBuffer = ao->AttributeID("u_Position_gBuffer");
					static const auto   u_Normal_gBuffer = ao->AttributeID("u_Normal_gBuffer"  );
					static const auto          u_Samples = ao->AttributeID("u_Samples" );
					static const auto         u_Strength = ao->AttributeID("u_Strength");
					static const auto             u_Bias = ao->AttributeID("u_Bias"    );
					static const auto           u_Radius = ao->AttributeID("u_Radius"  );
					static const auto         u_NearClip = ao->AttributeID("u_NearClip");
					static const auto          u_FarClip = ao->AttributeID("u_FarClip" );
					static const auto             u_Time = ao->AttributeID("u_Time"    );
					static const auto               u_VP = ao->AttributeID("u_VP"      );
					
					// Assign program values:
					ao->Assign( u_Samples,           Settings::PostProcessing::AmbientOcclusion::s_Samples                         );
					ao->Assign(u_Strength,           Settings::PostProcessing::AmbientOcclusion::s_Intensity                       );
					ao->Assign(    u_Bias, -std::min(Settings::PostProcessing::AmbientOcclusion::s_Radius, static_cast<float>(0.2)));
					ao->Assign(  u_Radius,           Settings::PostProcessing::AmbientOcclusion::s_Radius                          );
					
					ao->Assign(u_NearClip, m_NearClip              );
					ao->Assign( u_FarClip, m_FarClip               );
					ao->Assign(    u_Time, Time::Elapsed<GLfloat>());
					ao->Assign(      u_VP, m_Projection * View()   );
					
					/* ASSIGN G-BUFFERS */
					ao->AssignDepth(u_Position_gBuffer, m_Position_gBuffer, 0);
					ao->Assign(     u_Position_gBuffer, m_Position_gBuffer, 1);
					ao->Assign(       u_Normal_gBuffer,   m_Normal_gBuffer, 2);
					
					// Set the viewport resolution to the scale of the AO render target.
					glViewport(viewport[0], viewport[1], m_AO_RT.Width(), m_AO_RT.Height());
					
					// Draw
					RenderTexture::Bind(m_AO_RT);
					if (const auto q = Mesh::Primitives::Quad<GLfloat>::Instance().lock()) {
						Draw(*q);
					}
					
					// Blur the AO.
					Blur(m_AO_RT, 1.0, 1, true, false);
			
					// Reset the viewport.
					glViewport(viewport[0], viewport[1], m_RT.Width(), m_RT.Height());
					
					// Apply the colors of the AO image to the main image using multiply blending:
				    glEnable(GL_BLEND);
				    glBlendFunc(GL_DST_COLOR, GL_ZERO);
			
					Copy(m_AO_RT, m_RT);
					
					glDisable(GL_BLEND);
				}
				else {
					Debug::Log("Error initialising the render target!", Error);
				}
			}
		}
		
		/** @brief Physically-based bloom effect using 13-tap sampling method. */
		void Bloom() {
		
			using target = Settings::PostProcessing::Bloom;
			
			if (const auto v = m_Viewport.lock()) {
				
				// Get screen dimensions.
				const auto dimensions = v->Dimensions();
				
				static const auto ts = Resources::Get<Shader>("threshold");
				static const auto ds = Resources::Get<Shader>("downscale");
				static const auto us = Resources::Get<Shader>("upscale"  );
				static const auto cs = Resources::Get<Shader>("combine"  );
				static const auto ls = Resources::Get<Shader>("lens_dirt");
				
				// Get each shader used for rendering the effect.
				if (const auto threshold_shader = ts) {
				if (const auto downscale_shader = ds) {
				if (const auto   upscale_shader = us) {
				if (const auto   combine_shader = cs) {
				if (const auto lens_dirt_shader = ls) {
					
					const auto target_length = static_cast<size_t>(std::max(
						static_cast<size_t>(target::s_Diffusion * 2),
						static_cast<size_t>(1)
					)) + 1;
					
					if (m_Bloom_MipChain.empty() || m_Bloom_MipChain.size() < target_length) {
						
						const auto offset = std::max(m_Bloom_MipChain.size(), static_cast<size_t>(1U)) - 1U;
						
						for (size_t i = 0U; i < target_length; ++i) {
							
							const auto size = dimensions / static_cast<size_t>(std::pow(2.0, i + offset));
							
							if (size.x > 1U && size.y > 1U) {
								
								m_Bloom_MipChain.emplace_back(
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
					else if (m_Bloom_MipChain.size() > target_length) {
						m_Bloom_MipChain.erase(m_Bloom_MipChain.begin() + static_cast<std::vector<RenderTexture>::difference_type>(target_length), m_Bloom_MipChain.end());
					}
					
					if (m_Bloom_MipChain.size() > 1) {
					
						{
							/* THRESHOLD PASS */
							Shader::Bind(threshold_shader->ID());
							
							static const auto u_Threshold = threshold_shader->AttributeID("u_Threshold");
							static const auto     u_Clamp = threshold_shader->AttributeID("u_Clamp"    );
							
							threshold_shader->Assign(u_Threshold, target::s_Threshold);
							threshold_shader->Assign(    u_Clamp, target::s_Clamp    );
							
							Blit(m_RT, m_Bloom_MipChain[0], threshold_shader);
						}
						
						{
							/* DOWNSCALING */
							Shader::Bind(downscale_shader->ID());
							
							for (size_t i = 1; i < m_Bloom_MipChain.size(); ++i) {
								Blit(m_Bloom_MipChain[i - 1], m_Bloom_MipChain[i], downscale_shader);
							}
						}
						
						{
							/* UPSCALING */
							Shader::Bind(upscale_shader->ID());
							
							// Create the diffusion vector for the bloom algorithm:
							static const auto u_Diffusion = upscale_shader->AttributeID("u_Diffusion");
							
							const auto t = Utils::Remap(target::s_Anamorphism, static_cast<scalar_t>(-1.0), static_cast<scalar_t>(1.0), static_cast<scalar_t>(0.0), static_cast<scalar_t>(1.0));
						
							const auto d = std::min((1.0 / std::log2(target::s_Diffusion + 1.0)), 1.0) * 6.0;
							
							// Imitate anamorphic artifacts by morphing the shape of the diffusion vector:
							const vec2 diffusionVec(
								glm::mix(0.0, d,       t),
								glm::mix(0.0, d, 1.0 - t)
							);
							
							upscale_shader->Assign(u_Diffusion, diffusionVec);
						}
						
						// Enable additive blending
						glEnable(GL_BLEND);
						glBlendFunc(GL_ONE, GL_ONE);
						glBlendEquation(GL_FUNC_ADD);
						
						for (auto i = m_Bloom_MipChain.size() - 1; i > 0; --i) {
							Blit(m_Bloom_MipChain[i], m_Bloom_MipChain[i - 1], upscale_shader);
						}
						
						// Disable additive blending
						glDisable(GL_BLEND);
						
						{
							/* COMBINE */
							Shader::Bind(combine_shader->ID());
							
							static const auto u_Strength = combine_shader->AttributeID("u_Strength");
							static const auto u_Texture0 = combine_shader->AttributeID("u_Texture0");
							static const auto u_Texture1 = combine_shader->AttributeID("u_Texture1");
							
							combine_shader->Assign(u_Strength, target::s_Intensity / std::max(static_cast<float>(m_Bloom_MipChain.size()) - 1, static_cast<float>(1.0)));
							combine_shader->Assign(u_Texture0, m_RT, 0);
							combine_shader->Assign(u_Texture1, m_Bloom_MipChain[0], 1);
						}
						
						// Blit to main render target:
						RenderTexture::Bind(m_RT);
						
						if (const auto q = Mesh::Primitives::Quad<GLfloat>::Instance().lock()) {
							Draw(*q);
						}
						
						RenderTexture::Unbind();
						
						/* LENS DIRT */
						if (Settings::PostProcessing::Bloom::s_LensDirt > 0.0) {
							
							if (const auto t = Resources::Get<Texture>("Bokeh__Lens_Dirt_65")) {
								
								Shader::Bind(lens_dirt_shader->ID());
								
								static const auto u_Strength = lens_dirt_shader->AttributeID("u_Strength");
								static const auto u_Texture0 = lens_dirt_shader->AttributeID("u_Texture0");
								static const auto    u_Bloom = lens_dirt_shader->AttributeID("u_Bloom");
								static const auto     u_Dirt = lens_dirt_shader->AttributeID("u_Dirt");
								
								lens_dirt_shader->Assign(u_Strength, target::s_LensDirt * target::s_Intensity);
								lens_dirt_shader->Assign(u_Texture0, m_RT,                0);
								lens_dirt_shader->Assign(   u_Bloom, m_Bloom_MipChain[0], 1);
								lens_dirt_shader->Assign(    u_Dirt, *t,                  2);
								
								// Blit to main render target:
								RenderTexture::Bind(m_RT);
								if (const auto q = Mesh::Primitives::Quad<GLfloat>::Instance().lock()) {
									Draw(*q);
								}
							}
						}
					}
					else {
						Debug::Log("Error creating mip chain!", Error);
					}
				}}}}}
			}
			else {
				Debug::Log("Camera is not bound to a valid Window!", Error);
			}
		}
		
		/** @brief Set the Camera as dirty, so that a new projection matrix is computed when requested. */
		constexpr void SetDirty() noexcept {
			m_IsDirty = true;
		}
		
	public:
		
		/** @brief Represents different actions that can be taken during the rendering process. */
		enum RenderFlags : unsigned char {
			        NONE = 0U,       /**< @brief No special action to be taken. */
			REINITIALISE = 1U << 0U, /**< @brief Reinitialise the g-buffer. */
		};
		
		explicit Camera(const std::weak_ptr<ECS::GameObject>& _parent) : Component(_parent),
		
			// Initialise default values for perspective matrix:
			m_Projection(1.0),
			m_IsDirty(true),
			m_FOV(90.0),
			
			// Initialise the projection matrix to an identity matrix and raise the "isDirty" flag:
			m_NearClip(0.1),
			m_FarClip(60.0),
		
			// Init g-buffer:
			               m_RT(1, 1, { GL_RGB16F,  false }, { GL_LINEAR,  GL_LINEAR  }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }, RenderTexture::Parameters::DepthMode::NONE         ),
			   m_Albedo_gBuffer(1, 1, { GL_RGB,     false }, { GL_NEAREST, GL_NEAREST }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }, RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
			 m_Emission_gBuffer(1, 1, { GL_RGB16F,  false }, { GL_NEAREST, GL_NEAREST }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }, RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
			 m_Material_gBuffer(1, 1, { GL_RGBA16F, false }, { GL_NEAREST, GL_NEAREST }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }, RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
			 m_Position_gBuffer(1, 1, { GL_RGB16F,  false }, { GL_NEAREST, GL_NEAREST }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }, RenderTexture::Parameters::DepthMode:: FRAME_BUFFER),
			   m_Normal_gBuffer(1, 1, { GL_RGB16F,  false }, { GL_NEAREST, GL_NEAREST }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }, RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
			 m_TexCoord_gBuffer(1, 1, { GL_RG32F,   false }, { GL_NEAREST, GL_NEAREST }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }, RenderTexture::Parameters::DepthMode::RENDER_BUFFER),
			 
			// Set exposure level from settings.
			m_Exposure(Settings::PostProcessing::ToneMapping::s_Exposure),
			
			// Effects buffers:
			            m_AO_RT( 1,  1, { GL_RGB, false }, { GL_LINEAR,  GL_LINEAR }, { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }, RenderTexture::Parameters::DepthMode::NONE),
			m_AutoExposure_Luma(32, 32, { m_RT.Format().PixelFormat(), false }, { GL_LINEAR, GL_NEAREST }, m_RT.WrapMode(), RenderTexture::Parameters::DepthMode::NONE)
		{
			
			if (s_Passthrough.expired()) {
				s_Passthrough = Resources::Get<Shader>("passthrough");
			}
			else {
				Debug::Log("Couldn't bind passthrough texture!", Error);
			}
			
			if (s_Cube.expired()) {
				s_Cube = Resources::Get<Mesh>("cube");
			}
			else {
				Debug::Log("Couldn't get cube mesh!", Error);
			}
		}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Camera); };
		
		void ReinitialiseBuffers(const std::shared_ptr<IViewport<scalar_t, size_t>>& _viewport) {
			
			const auto& d = _viewport->Dimensions();

			// Reinitialise the g-buffer:
			              m_RT.Reinitialise(d.x, d.y);
			  m_Albedo_gBuffer.Reinitialise(d.x, d.y);
			m_Emission_gBuffer.Reinitialise(d.x, d.y);
			m_Material_gBuffer.Reinitialise(d.x, d.y);
			m_Position_gBuffer.Reinitialise(d.x, d.y);
			  m_Normal_gBuffer.Reinitialise(d.x, d.y);
			m_TexCoord_gBuffer.Reinitialise(d.x, d.y);
			
			// Clear the mip chain:
			m_Bloom_MipChain.clear();
		}
		
		/**
		 * @brief Pre-render stage of the rendering pipeline.
		 *
		 * This function is called before rendering the frame. It performs tasks such as
		 * reinitialising the g-buffer and clearing the mip chain.
		 *
		 * @param[in] _flags The render flags indicating any which actions should be taken.
		 */
		void PreRender(const RenderFlags& _flags) {
			
			if (const auto v = m_Viewport.lock()) {
			
				if ((_flags & RenderFlags::REINITIALISE) != 0U) {
					ReinitialiseBuffers(v);
				}
			}
			else {
				Debug::Log("Camera is not bound to a valid Window!", Error);
			}
		}
		
		/**
		 * \brief Renders each Renderer using the Camera.
		 * \param[in] _renderers The list of Renderers to be rendered.
		 * \param[in] _lights The list of Lights to be used during rendering.
		 */
		void Render(const std::vector<std::weak_ptr<Renderer>>& _renderers, const std::vector<std::weak_ptr<Light>>& _lights)  {
		
			if (const auto v = m_Viewport.lock()) {
				
				// Enable culling and depth.
				glEnable(GL_CULL_FACE);
				glEnable(GL_DEPTH_TEST);
				
				// Set the preferred culling and depth modes:
				static const auto  cullMode = GL_BACK;
				static const auto depthMode = GL_LEQUAL;
				
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
				auto dimensions = v->Dimensions();
				glViewport(0, 0, dimensions[0], dimensions[1]);
				
				/* SHADING */
				if (const auto m = Mesh::Primitives::Quad<GLfloat>::Instance().lock()) {
					
					// Bind the main FBO.
					RenderTexture::Bind(m_RT);
					
					/* DRAW OBJECTS */
					if (const auto p = Settings::Graphics::Material::s_Shader.lock()) {
				
						// Bind program.
						Shader::Bind(p->ID());
			
						// Assign g-buffers:
						const auto u_Albedo_gBuffer = p->AttributeID("u_Albedo_gBuffer");
						if (u_Albedo_gBuffer != -1) {
							p->Assign(u_Albedo_gBuffer, m_Albedo_gBuffer, 0);
						}
						
						const auto u_Emission_gBuffer = p->AttributeID("u_Emission_gBuffer");
						if (u_Emission_gBuffer != -1) {
							p->Assign(u_Emission_gBuffer, m_Emission_gBuffer, 1);
						}
						
						const auto u_Material_gBuffer = p->AttributeID("u_Material_gBuffer");
						if (u_Material_gBuffer != -1) {
							p->Assign(u_Material_gBuffer, m_Material_gBuffer, 2);
						}
						
						const auto u_Position_gBuffer = p->AttributeID("u_Position_gBuffer");
						if (u_Position_gBuffer != -1) {
							p->Assign(u_Position_gBuffer, m_Position_gBuffer, 3);
						}
						
						const auto u_Normal_gBuffer = p->AttributeID("u_Normal_gBuffer");
						if (u_Normal_gBuffer != -1) {
							p->Assign(u_Normal_gBuffer, m_Normal_gBuffer, 4);
						}
						
						const auto u_Depth_gBuffer = p->AttributeID("u_Depth_gBuffer");
						if (u_Depth_gBuffer != -1) {
							p->AssignDepth(u_Depth_gBuffer, m_Position_gBuffer, 5);
						}
						
						// Assign other material parameters:
						const auto u_Time = p->AttributeID("u_Time");
						if (u_Time != -1) {
							p->Assign(u_Time, Time::Elapsed<GLfloat>());
						}
						
						{
							const auto t = GetTransform().lock();
							
							p->Assign(p->AttributeID("u_CameraPosition"),
								t != nullptr ?
										t->Position() :
									vec3(0.0)
							);
						}
						
						// Assign ambient texture:
						if (const auto s = Settings::Graphics::Skybox::s_Skybox.lock()) {
							p->Assign(p->AttributeID("u_Ambient"), *s, 98);
						}
						
						p->Assign(p->AttributeID("u_AmbientExposure"), Settings::Graphics::Skybox::s_Exposure);
			
						// For the time being, there is only one light. TODO: Introduce a lighting buffer.
						for (const auto& light : _lights) {
							
							using target_light = Settings::Graphics::Material;
							
							if (const auto l = light.lock()) {
								
								if (const auto t = l->m_Transform.lock()) {
								
									// TODO: Placeholder code for settings.
									{
										bool isDirty;
										
										{
											const auto newResolution = Utils::Parse<int>(target_light::s_ShadowResolutions.at(target_light::s_CurrentShadowResolutionSelection));
											
											const auto newBias       = target_light::s_ShadowBias;
											const auto newNormalBias = target_light::s_ShadowNormalBias;
											
											isDirty = newResolution != l->m_Shadow.m_Resolution             ||
													  newBias       != l->m_Shadow.m_Bias                   ||
													  newNormalBias != l->m_Shadow.m_NormalBias             ||
													  target_light::s_LightRange              != l->m_Range ||
													  target_light::s_LightAngle              != l->m_Angle ||
													  target_light::s_CurrentLightType        != l->m_Type;
											
											l->m_Shadow.m_Resolution = newResolution;
											l->m_Shadow.m_Bias       = newBias;
											l->m_Shadow.m_NormalBias = newNormalBias;
											
											t->Position(target_light::s_LightPosition);
											t->Rotation(glm::inverse(glm::quat(glm::radians(target_light::s_LightRotation))));
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
										static_cast<GLfloat>(
											glm::cos(glm::radians(
													l->Type() == Light::Parameters::Type::Spot ?
													l->m_Angle / 2 :
													180.0
												)
											)
										)
									);
					
									p->Assign(p->AttributeID("u_NearPlane"), l->m_Shadow.m_NearPlane);
									
									p->Assign(p->AttributeID("u_LightPosition" ), t->Position());
									p->Assign(p->AttributeID("u_LightDirection"), t->FORWARD   );
									
									p->Assign(p->AttributeID("u_LightRange"    ), l->m_Range    );
									p->Assign(p->AttributeID("u_LightIntensity"), l->m_Intensity);
									p->Assign(p->AttributeID("u_LightColor"    ), l->m_Color    );
					
									/* DRAW */
									if (const auto q = Mesh::Primitives::Quad<GLfloat>::Instance().lock()) {
										Draw(*q);
									}
								}
								else {
									Debug::Log("Light has no valid transform!", Error);
								}
							}
						}
					}
				}
			}
			else {
				Debug::Log("Camera is not bound to a valid Window!", Error);
			}
		}
		
		/**
		 * @brief Called after rendering.
		 *
		 * This function is called after the rendering process and performs any
		 * post-processing effects before the final image is presented.
		 *
		 * @see Settings::PostProcessing, Settings::Graphics, Draw(), Blit(), Shader::Bind(), RenderTexture::Unbind(), glEnable(), glDisable(), GL_FRAMEBUFFER_SRGB, Mesh::Primitives::Quad::Instance()
		 */
		void PostRender() {
			
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
					
					static const auto aces = Resources::Get<Shader>("aces");
					
					if (const auto t = aces) {
						
						Shader::Bind(t->ID());
						
						static const auto     u_Gain = t->AttributeID("u_Gain"    );
						static const auto u_Exposure = t->AttributeID("u_Exposure");
						
						t->Assign(u_Gain, Settings::PostProcessing::ToneMapping::s_Gain);
						
						t->Assign(u_Exposure,
							Settings::PostProcessing::ToneMapping::AutoExposure::IsActiveAndEnabled() ?
							m_Exposure :
							m_Exposure = Settings::PostProcessing::ToneMapping::s_Exposure
						);
					
						effects.push(t);
					}
				}
				
				// Perform anti-aliasing after tonemapping as FXAA suffers in the HDR range.
				if (Settings::PostProcessing::AntiAliasing::IsActiveAndEnabled()) {
					
					static const auto fxaa = Resources::Get<Shader>("fxaa");
					
					if (const auto aa = fxaa) {
						
						Shader::Bind(aa->ID());
						
						static const auto u_Texture               = aa->AttributeID("u_Texture");
						static const auto u_ContrastThreshold     = aa->AttributeID("u_ContrastThreshold"    );
						static const auto u_RelativeThreshold     = aa->AttributeID("u_RelativeThreshold"    );
						static const auto u_SubpixelBlending      = aa->AttributeID("u_SubpixelBlending"     );
						static const auto u_EdgeBlending          = aa->AttributeID("u_EdgeBlending"         );
						static const auto u_LocalContrastModifier = aa->AttributeID("u_LocalContrastModifier");
						
						aa->Assign(u_Texture, m_RT, 0);
						
						aa->Assign(    u_ContrastThreshold, Settings::PostProcessing::AntiAliasing::s_ContrastThreshold    );
						aa->Assign(    u_RelativeThreshold, Settings::PostProcessing::AntiAliasing::s_RelativeThreshold    );
						aa->Assign(     u_SubpixelBlending, Settings::PostProcessing::AntiAliasing::s_SubpixelBlending     );
						aa->Assign(         u_EdgeBlending, Settings::PostProcessing::AntiAliasing::s_EdgeBlending         );
						aa->Assign(u_LocalContrastModifier, Settings::PostProcessing::AntiAliasing::s_LocalContrastModifier);
						
						effects.push(aa);
					}
				}
				
				if (Settings::PostProcessing::Grain::IsActiveAndEnabled()) {
					
					static const auto grain = Resources::Get<Shader>("grain");
					
					if (const auto g = grain) {
						
						Shader::Bind(g->ID());
						
						static const auto u_Amount = g->AttributeID("u_Amount");
						static const auto   u_Time = g->AttributeID("u_Time");
						
						g->Assign(u_Amount, Settings::PostProcessing::Grain::s_Intensity);
						g->Assign(  u_Time, Time::Elapsed<GLfloat>());
						
						effects.push(g);
					}
				}
				
				if (Settings::PostProcessing::Vignette::IsActiveAndEnabled()) {
					
					static const auto vignette = Resources::Get<Shader>("vignette");
					
					if (const auto v = vignette) {
						
						Shader::Bind(v->ID());
						
						static const auto     u_Amount = v->AttributeID("u_Amount"    );
						static const auto u_Smoothness = v->AttributeID("u_Smoothness");
						
						v->Assign(    u_Amount, Settings::PostProcessing::Vignette::s_Intensity );
						v->Assign(u_Smoothness, Settings::PostProcessing::Vignette::s_Smoothness);
						
						effects.push(v);
					}
				}
				
				// Check if there are effects in the stack...
				if (!effects.empty()) {
					
					// Render post-processing effects stack:
					while (!effects.empty()) {
					
						const auto& shader = effects.front();
						effects.pop();
						
						RenderTexture::Unbind();
						Blit(m_RT, m_RT, shader);
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
			RenderTexture::Unbind(true);
			
			// Check if gamma correction is enabled in settings.
			if (Settings::Graphics::s_GammaCorrection) {
				
				// de Vries, J. (n.d.). LearnOpenGL - Gamma Correction. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Gamma-Correction [Accessed 15 Dec. 2023].
				glEnable(GL_FRAMEBUFFER_SRGB); // Enable gamma correction.
			}
			
			// DRAW:
			if (const auto p = s_Passthrough.lock()) {
				
				Shader::Bind(p->ID());
				if (const auto q = Mesh::Primitives::Quad<GLfloat>::Instance().lock()) {
					Draw(*q);
				}
			}
			
			// Reset gamma correction.
			glDisable(GL_FRAMEBUFFER_SRGB);
		}
		
		void SetViewport(const std::shared_ptr<IViewport<scalar_t, size_t>>& _viewport) {
			
			bool reinitialise;
			
			if (const auto old_v = m_Viewport.lock()) {
				reinitialise = _viewport->Dimensions() != old_v->Dimensions();
			}
			else {
				reinitialise = true;
			}
			
			m_Viewport = _viewport;
		
			if (reinitialise) {
				ReinitialiseBuffers(_viewport);
			}
		}
		
		[[nodiscard]] constexpr const std::weak_ptr<IViewport<scalar_t, size_t>>& GetViewport() const {
			return m_Viewport;
		}
		
		/**
		 * @brief Get the Camera's Transform.
		 * @param[in] _transform The weak pointer to the transform.
		 */
		void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept {
			m_Transform = _transform;
		}
		
		/**
		 * @brief Get the Camera's Transform.
		 * @return The weak pointer to the transform.
		 */
		[[nodiscard]] constexpr const std::weak_ptr<Transform>& GetTransform() const noexcept {
			return m_Transform;
		}
		
		/**
		 * @brief Get the Camera's aspect.
		 * @return The aspect of window the camera is bound to, or -1.0 if no valid window is bound.
		 */
		[[nodiscard]] float Aspect() const {
			
			float result;
			
			if (const auto v = m_Viewport.lock()) {
				result = v->Aspect();
			}
			else {
				result = -1.0;
				
				Debug::Log("Camera is not bound to a valid Window!", Error);
			}
			
			return result;
		}
		
		/**
		 * @brief Set the Camera's field of view.
		 * @param[in] _fov The new field of view.
		 */
		void FOV(const float& _fov) noexcept {
			
			// Check if the value differs from the current one.
			if (m_FOV != _fov) {
				m_FOV = _fov;
				
				m_IsDirty = true; // Set dirty.
			}
		}
		
		/**
		 * @brief Get the Camera's field of view.
		 * @return The Camera's field of view.
		 */
		[[nodiscard]] constexpr const float& FOV() const noexcept {
			return m_FOV;
		}
		
		/**
		 * @brief Set the Camera's near-clipping distance.
		 * @param[in] _nearClip The new near-clipping distance.
		 */
		void NearClip(const float& _nearClip) noexcept {
		
			// Check if the value differs from the current one.
			if (m_NearClip != _nearClip) {
				m_NearClip = _nearClip;
				
				m_IsDirty = true; // Set dirty.
			}
		}
		
		/**
		 * @brief Get the Camera's near-clipping distance.
		 * @return The Camera's near-clipping distance.
		 */
		[[nodiscard]] constexpr const float& NearClip() const noexcept {
			return m_NearClip;
		}
		
		/**
		 * @brief Set the Camera's far-clipping distance.
		 * @param[in] _farClip The new far-clipping distance.
		 */
		void FarClip(const float& _farClip) noexcept {
			
			// Check if the value differs from the current one.
			if (m_FarClip != _farClip) {
				m_FarClip = _farClip;
				
				m_IsDirty = true; // Set dirty.
			}
		}
		
		/**
		 * @brief Get the Camera's far-clipping distance.
		 * @return The Camera's far-clipping distance.
		 */
		[[nodiscard]] constexpr const float& FarClip() const noexcept {
			return m_FarClip;
		}
		
		/**
		 * @brief Set the Camera's clear color.
		 * @param[in] _color The new clear color.
		 */
		static void ClearColor(vec4 _color) {
			
			// Set the clear color to the provided.
			glClearColor(_color[0], _color[1], _color[2], _color[3]);
		}
	
		/**
		 * @brief Get the Camera's clear color.
		 * @return The Camera's clear color.
		 */
		[[nodiscard]] static vec4 ClearColor() {
			
			vec4 result;
			
			// Read the clear value from opengl into the result.
			glGetFloatv(GL_COLOR_CLEAR_VALUE, &result[0]);
			
			return result;
		}
		
		/**
		 * @brief Get the Camera's projection matrix.
		 * @return The Camera's projection matrix.
		 */
		const mat4& Projection() {
			
			// Recalculate the perspective matrix if the camera is dirtied.
			if (IsDirty()) {
				m_IsDirty = false;
				
				m_Projection = glm::perspective(glm::radians(m_FOV), Aspect(), m_NearClip, m_FarClip);
			}
			
			return m_Projection;
		}
		
		/**
		 * @brief Get the Camera's view matrix.
		 * @return The Camera's view matrix.
		 */
		[[nodiscard]] mat4 View() const {
		
			mat4 result;
			
			if (const auto transform = GetTransform().lock()) {
				result = glm::lookAt(
						transform->Position(),
						transform->Position() + transform->FORWARD,
					transform->UP
				);
			}
			else {
				result = mat4(1.0);
				
				Debug::Log("No valid Transform Component on Camera!", Error);
			}
			
			return result;
		}
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_CAMERA_HPP