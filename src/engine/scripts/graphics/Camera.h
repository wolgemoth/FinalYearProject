#ifndef FINALYEARPROJECT_CAMERA_H
#define FINALYEARPROJECT_CAMERA_H

#include "../ecs/Component.h"

#include "textures/RenderTexture.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>

#include <memory>
#include <vector>

// @Assessor: This class has been changed significantly since it was submitted for 3DGP. Please mark it for GACP.

namespace LouiEriksson::Engine {
	
	class Transform;
	class Window;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Light;
	class Mesh;
	class Renderer;
	class Shader;
	class Texture;
	
	/// <summary> Camera class for 3D rendering of a scene from a perspective. </summary>
	class Camera final : public ECS::Component {
	
		friend LouiEriksson::Engine::Window;
	
	private:
		
		// Main render target:
		RenderTexture m_RT;
		
		/// <summary> Window of the Camera. </summary>
		std::weak_ptr<Window> m_Window;
	
		/// <summary> Transform of the Camera. </summary>
		std::weak_ptr<Transform> m_Transform;
		
		/* PERSPECTIVE */
		
		/// <summary> Projection matrix. </summary>.
		glm::mat4 m_Projection;
		
		/// <summary> Flag that indicates if the projection matrix needs to be rebuilt or not. </summary>.
		bool m_IsDirty;
		
		float m_FOV;        // Field of view.
		float m_NearClip;   // Near clipping plane.
		float m_FarClip;    // Far clipping plane.
		
		/* EFFECTS */
		
		float m_Exposure;   // Camera exposure for tonemapping.
		
		/* G-BUFFER: */
		
		RenderTexture   m_Albedo_gBuffer; // Albedo channel.
		RenderTexture m_Emission_gBuffer; // Emission channel.
		RenderTexture m_Material_gBuffer; // Material properties (Roughness, Metallic).
		RenderTexture m_Position_gBuffer; // World-space surface positions.
		RenderTexture   m_Normal_gBuffer; // Surface normals.
		RenderTexture m_TexCoord_gBuffer; // Texture coordinates.
		
		/// <summary> Deferred rendering geometry pass. </summary>
		void GeometryPass(const std::vector<std::weak_ptr<Renderer>>& _renderers);
		
		/// <summary> Deferred rendering shadow pass. </summary>
		void ShadowPass(const std::vector<std::weak_ptr<Renderer>>& _renderers, const std::vector<std::weak_ptr<Light>>& _lights) const;
		
		/// <summary> Copies one RenderTexture into another. </summary>
		static void Copy(const RenderTexture& _src, const RenderTexture& _dest) ;
		
		/// <summary> Blit one RenderTexture into another with a specified shader. </summary>
		static void Blit(const RenderTexture& _src, const RenderTexture& _dest, const std::weak_ptr<Shader>& _shader) ;

		/* POST PROCESSING */
		
		/// <summary> Blur the contents of a RenderTexture. </summary>
		static void Blur(const RenderTexture& _rt, const float& _intensity, const int& _passes, const bool& _highQuality, const bool& _consistentDPI) ;

		/// <summary> Auto exposure effect for use in conjunction with tonemapping. </summary>
		void AutoExposure();
		
		/// <summary> Ambient occlusion effect. </summary>
		void AmbientOcclusion() const;
		
		/// <summary> Physically-based bloom effect using 13-tap sampling method. </summary>
		void Bloom() const;
		
	public:
	
		/// <summary> Represents different actions that can taken during the rendering process. </summary>
		enum RenderFlags : char {
			
			/// <summary> No special action to be taken. </summary>
			NONE,
			
			/// <summary> Reinitialise the g-buffer. </summary>
			REINITIALISE
		};
		
		 explicit Camera(const std::weak_ptr<ECS::GameObject>& _parent);
		~Camera() override;
	
		std::type_index TypeID() const noexcept override { return typeid(Camera); };
		
		/// <summary> Called before rendering. </summary>
		void PreRender(const RenderFlags& _flags);
		
		/// <summary> Renders each Renderer using the Camera. </summary>
		void Render(const std::vector<std::weak_ptr<Renderer>>& _renderers, const std::vector<std::weak_ptr<Light>>& _lights);
		
		/// <summary> Called after rendering. </summary>
		void PostRender();
		
		/// <summary> Set the Camera's Window. </summary>
		void SetWindow(const std::weak_ptr<Window>& _window);
		
		/// <summary> Get the Camera's Window. </summary>
		[[nodiscard]] const std::weak_ptr<Window> GetWindow() const noexcept;
		
		/// <summary> Set the Camera's Transform. </summary>
		void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept;
		
		/// <summary> Get the Camera's Transform. </summary>
		[[nodiscard]] const std::weak_ptr<Transform> GetTransform() const noexcept;
		
		/// <summary> Get the Camera's Aspect. </summary>
		[[nodiscard]] float Aspect() const;
		
		/// <summary> Set the Camera's field of view. </summary>
		void FOV(const float& _fov) noexcept;
		
		/// <summary> Get the Camera's field of view. </summary>
		[[nodiscard]] const float& FOV() const noexcept;
		
		/// <summary> Set the Camera's near clip plane. </summary>
		void NearClip(const float& _nearClip) noexcept;
		
		/// <summary> Get the Camera's near clip plane. </summary>
		[[nodiscard]] const float& NearClip() const noexcept;
		
		/// <summary> Set the Camera's far clip plane. </summary>
		void FarClip(const float& _farClip) noexcept;
		
		/// <summary> Get the Camera's far clip plane. </summary>
		[[nodiscard]] const float& FarClip() const noexcept;
		
		/// <summary> Set the Camera's clear color. </summary>
		static void ClearColor(glm::vec4 _color);
		
		/// <summary> Get the Camera's clear color. </summary>
		[[nodiscard]] static glm::vec4 ClearColor() ;
		
		/// <summary> Get the Camera's projection matrix. </summary>
		const glm::mat4& Projection();
		
		/// <summary> Get the Camera's view matrix. </summary>
		[[nodiscard]] glm::mat4 View() const;
		
		/// <summary> Set the Camera dirty, so that it computes a new projection matrix. </summary>
		void SetDirty() noexcept;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_CAMERA_H