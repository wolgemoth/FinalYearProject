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
	
	class Camera : public Component {
	
		friend class Window;
	
	private:
	
		std::shared_ptr<Mesh> m_Cube;
		
		/// <summary> Cubemap Skybox. </summary>
		std::weak_ptr<Texture> m_Skybox;
		
		/// <summary> HDRI Skybox. </summary>
		std::weak_ptr<Texture> m_HDRI;
		
		// <summary> Lens Dirt Texture </summary>
		std::weak_ptr<Texture> m_LensDirt;
		
		/// <summary> Window of the Camera. </summary>
		std::shared_ptr<Window> m_Window;
	
		/// <summary> Transform of the Camera. </summary>
		std::shared_ptr<Transform> m_Transform;
		
		float m_FOV;
		float m_NearClip;
		float m_FarClip;
		
		float m_CurrentExposure;
		float m_TargetExposure;
		
		bool m_IsDirty;
		
		RenderTexture m_RT;
		
		RenderTexture m_Position_Buffer;
		RenderTexture   m_Normal_Buffer;
		
		glm::mat4 m_Projection;
		
		void GeometryPass(const std::vector<std::shared_ptr<Renderer>>& _renderers);
		
		void ShadowPass(const std::vector<std::shared_ptr<Renderer>>& _renderers, const std::vector<std::shared_ptr<Light>>& _lights);
		
		void PostProcess(std::queue<std::weak_ptr<Shader>> _effects) const;
		
		static void Copy(const RenderTexture& _src, const RenderTexture& _dest) ;
		
		static void Blit(const RenderTexture& _src, const RenderTexture& _dest, std::weak_ptr<Shader> _shader) ;

		void Blur(const RenderTexture& _rt, const float& _intensity, const int& _passes, const bool& _highQuality, const bool& _consistentDPI) const;

		void AutoExposure();
		
		void AmbientOcclusion() const;
		
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
		void SetWindow(std::shared_ptr<Window> _window);
		
		/// <summary> Get the Camera's Window. </summary>
		[[nodiscard]] std::shared_ptr<Window> GetWindow() const;
		
		/// <summary> Set the Camera's Transform. </summary>
		void SetTransform(std::shared_ptr<Transform> _transform);
		
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
		void ClearColor(glm::vec4 _color);
		
		/// <summary> Get the Camera's clear color. </summary>
		[[nodiscard]] glm::vec4 ClearColor() const;
		
		/// <summary> Get the Camera's projection matrix. </summary>
		const glm::mat4& Projection();
		
		/// <summary> Get the Camera's view matrix. </summary>
		[[nodiscard]] glm::mat4 View() const;
		
		/// <summary> Set the Camera dirty, so that it computes a new projection matrix. </summary>
		void SetDirty();
	};
}

#endif //FINALYEARPROJECT_CAMERA_H