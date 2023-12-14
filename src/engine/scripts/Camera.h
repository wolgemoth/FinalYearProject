#pragma once

#ifndef FINALYEARPROJECT_CAMERA_H
#define FINALYEARPROJECT_CAMERA_H

#include "stdafx.h"

#include "RenderTexture.h"
#include "Renderer.h"
#include "Window.h"
#include "Light.h"
#include "Resources.h"

namespace LouiEriksson {
	
	class Window;
	
	/// <summary> Camera class for 3D rendering of a scene from a perspective. </summary>
	class Camera : public Component {
	
		friend class Window;
	
	private:
		
		// Main render target:
		RenderTexture m_RT;
		
		/// <summary> Skybox cube Mesh. </summary>.
		inline static std::shared_ptr<Mesh> s_Cube = nullptr;
		
		/// <summary> Lens-dirt Texture. </summary>
		std::weak_ptr<Texture> m_LensDirt;
		
		/// <summary> Window of the Camera. </summary>
		std::shared_ptr<Window> m_Window;
	
		/// <summary> Transform of the Camera. </summary>
		std::shared_ptr<Transform> m_Transform;
		
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
		
		RenderTexture   m_Albedo_gBuffer; // Albedo channel
		RenderTexture m_Emission_gBuffer; // Emission channel
		RenderTexture m_Material_gBuffer; // Material properties (Roughness, Metallic)
		RenderTexture m_Position_gBuffer; // World-space surface positions
		RenderTexture   m_Normal_gBuffer; // Surface normals
		RenderTexture m_TexCoord_gBuffer; // Texture coordinates
		
		/// <summary> Deferred rendering geometry pass. </summary>
		void GeometryPass(const std::vector<std::shared_ptr<Renderer>>& _renderers);
		
		/// <summary> Deferred rendering shadow pass. </summary>
		void ShadowPass(const std::vector<std::shared_ptr<Renderer>>& _renderers, const std::vector<std::shared_ptr<Light>>& _lights) const;
		
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
	
		 explicit Camera(const std::shared_ptr<GameObject>& _parent);
		~Camera() override;
	
		/// <summary> Render clear the m_Camera. </summary>
		static void Clear();
		
		/// <summary> Called before rendering. </summary>
		void PreRender();
		
		/// <summary> Renders each Renderer using the Camera. </summary>
		void Render(const std::vector<std::shared_ptr<Renderer>>& _renderers, const std::vector<std::shared_ptr<Light>>& _lights);
		
		/// <summary> Called after rendering. </summary>
		void PostRender();
		
		/// <summary> Set the Camera's Window. </summary>
		void SetWindow(const std::shared_ptr<Window>& _window);
		
		/// <summary> Get the Camera's Window. </summary>
		[[nodiscard]] std::shared_ptr<Window> GetWindow() const;
		
		/// <summary> Set the Camera's Transform. </summary>
		void SetTransform(const std::shared_ptr<Transform>& _transform);
		
		/// <summary> Get the Camera's Transform. </summary>
		[[nodiscard]] std::shared_ptr<Transform> GetTransform() const;
		
		/// <summary> Get the Camera's Aspect. </summary>
		[[nodiscard]] float Aspect() const;
		
		/// <summary> Set the Camera's field of view. </summary>
		void FOV(const float& _fov);
		
		/// <summary> Get the Camera's field of view. </summary>
		[[nodiscard]] const float& FOV() const;
		
		/// <summary> Set the Camera's near clip plane. </summary>
		void NearClip(const float& _nearClip);
		
		/// <summary> Get the Camera's near clip plane. </summary>
		[[nodiscard]] const float& NearClip() const;
		
		/// <summary> Set the Camera's far clip plane. </summary>
		void FarClip(const float& _farClip);
		
		/// <summary> Get the Camera's far clip plane. </summary>
		[[nodiscard]] const float& FarClip() const;
		
		/// <summary> Set the Camera's clear color. </summary>
		static void ClearColor(glm::vec4 _color);
		
		/// <summary> Get the Camera's clear color. </summary>
		[[nodiscard]] static glm::vec4 ClearColor() ;
		
		/// <summary> Get the Camera's projection matrix. </summary>
		const glm::mat4& Projection();
		
		/// <summary> Get the Camera's view matrix. </summary>
		[[nodiscard]] glm::mat4 View() const;
		
		/// <summary> Set the Camera dirty, so that it computes a new projection matrix. </summary>
		void SetDirty();
	};
}

#endif //FINALYEARPROJECT_CAMERA_H