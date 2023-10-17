#pragma once

#ifndef FINALYEARPROJECT_CAMERA_H
#define FINALYEARPROJECT_CAMERA_H

#include "stdafx.h"

#include "RenderTexture.h"
#include "Renderer.h"
#include "Window.h"

namespace LouiEriksson {
	
	class Window;
	
	class Camera : public Component {
	
		friend class Window;
	
	private:
	
		/// <summary> Window of the Camera. </summary>
		std::shared_ptr<Window>    m_Window;
	
		/// <summary> Transform of the Camera. </summary>
		std::shared_ptr<Transform> m_Transform;
		
		float m_FOV;
		float m_NearClip;
		float m_FarClip;
		
		bool m_IsDirty;
		
		unsigned int VAO0, VBO;
		
		RenderTexture m_RT;
		
		glm::mat4 m_Projection;
		
		void PostProcess(std::queue<std::shared_ptr<Shader>> _effects) const;
		
		static void Copy(const RenderTexture& _src, const RenderTexture& _dest) ;
		
		static void Copy(const RenderTexture& _src, const RenderTexture& _dest, const Shader& _shader) ;
		
		void Bloom() const;
		
	public:
	
		 explicit Camera(const std::shared_ptr<GameObject>& _parent);
		~Camera() override;
	
		/// <summary> Render clear the m_Camera. </summary>
		static void Clear();
		
		/// <summary> Called before rendering. </summary>
		void PreRender();
		
		/// <summary> Renders each Renderer using the Camera. </summary>
		void Render(const std::vector<std::shared_ptr<Renderer>>& _renderers);
		
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