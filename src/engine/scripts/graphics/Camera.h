#ifndef FINALYEARPROJECT_CAMERA_H
#define FINALYEARPROJECT_CAMERA_H

#include "../ecs/Component.h"

#include "textures/RenderTexture.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>

#include <memory>
#include <typeindex>
#include <vector>

namespace LouiEriksson::Engine {
	
	struct Transform;
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
	 * @brief Represents a Camera component that can be attached to a GameObject.
	 */
	class Camera final : public ECS::Component {
	
		friend LouiEriksson::Engine::Window;
	
	private:
		
		inline static std::weak_ptr<Shader> s_Passthrough{};
		
		/** @brief Window of the camera. */
		std::weak_ptr<Window> m_Window;
	
		/** @brief Camera Transform. */
		std::weak_ptr<Transform> m_Transform;
		
		/* PERSPECTIVE */
		
		/** @brief Projection matrix. */
		glm::mat4 m_Projection;
		
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
		void GeometryPass(const std::vector<std::weak_ptr<Renderer>>& _renderers);
		
		/**
		 * \brief Deferred-rendering shadow pass.
		 *
		 * \param[in] _renderers The list of renderers to perform the geometry pass for.
		 */
		void ShadowPass(const std::vector<std::weak_ptr<Renderer>>& _renderers, const std::vector<std::weak_ptr<Light>>& _lights) const;
		
		/**
		 * @brief Copies the contents of one RenderTexture to another.
		 *
		 * This function uses a passthrough shader to perform the copy operation.
		 *
		 * @param[in] _src The source RenderTexture to copy from.
		 * @param[in] _dest The destination RenderTexture to copy to.
		 */
		static void Copy(const RenderTexture& _src, const RenderTexture& _dest) ;
		
		/**
		 * @brief Blits the contents of one RenderTexture to another using a specific shader.
		 *
		 * @param[in] _src The source RenderTexture to blit from.
		 * @param[in] _dest The destination RenderTexture to blit to.
		 * @param[in] _shader The shader to use for the blit operation.
		 */
		static void Blit(const RenderTexture& _src, const RenderTexture& _dest, const std::weak_ptr<Shader>& _shader) ;
		
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
		static void Draw(const Graphics::Mesh& _mesh);
		
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
		static void Blur(const RenderTexture& _rt, const float& _intensity, const int& _passes, const bool& _highQuality, const bool& _consistentDPI) ;
		
		/**
		 * @brief Auto exposure effect for use in conjunction with tonemapping.
		 *
		 * This method calculates the average luminosity of the image and adjusts the exposure value accordingly to
		 * compensate for overexposure or underexposure. The calculation is done using a shader program and a mask texture.
		 */
		void AutoExposure();
		
		/** @brief Ambient occlusion post-processing effect using SSAO technique. */
		void AmbientOcclusion();
		
		/** @brief Physically-based bloom effect using 13-tap sampling method. */
		void Bloom();
		
		/** @brief Set the Camera as dirty, so that a new projection matrix is computed when requested. */
		void SetDirty() noexcept;
		
	public:
		
		/** @brief Represents different actions that can be taken during the rendering process. */
		enum RenderFlags : unsigned char {
			        NONE = 0u,       /**< @brief No special action to be taken. */
			REINITIALISE = 1u << 0u, /**< @brief Reinitialise the g-buffer. */
		};
		
		explicit Camera(const std::weak_ptr<ECS::GameObject>& _parent);
		
		~Camera() override;
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Camera); };
		
		/**
		 * @brief Pre-render stage of the rendering pipeline.
		 *
		 * This function is called before rendering the frame. It performs tasks such as
		 * reinitialising the g-buffer and clearing the mip chain.
		 *
		 * @param[in] _flags The render flags indicating any which actions should be taken.
		 */
		void PreRender(const RenderFlags& _flags);
		
		/**
		 * \brief Renders each Renderer using the Camera.
		 * \param[in] _renderers The list of Renderers to be rendered.
		 * \param[in] _lights The list of Lights to be used during rendering.
		 */
		void Render(const std::vector<std::weak_ptr<Renderer>>& _renderers, const std::vector<std::weak_ptr<Light>>& _lights);
		
		/**
		 * @brief Called after rendering.
		 *
		 * This function is called after the rendering process and performs any
		 * post-processing effects before the final image is presented.
		 *
		 * @see Settings::PostProcessing, Settings::Graphics, Draw(), Blit(), Shader::Bind(), RenderTexture::Unbind(), glEnable(), glDisable(), GL_FRAMEBUFFER_SRGB, Mesh::Primitives::Quad::Instance()
		 */
		void PostRender();
		
		/**
		 * @brief Set the window associated with the camera.
		 *
		 * @param[in] _window The weak pointer to the window.
		 */
		void SetWindow(const std::weak_ptr<Window>& _window);
		
		/**
		 * @brief Get the window associated with the camera.
		 * @return The weak pointer to the window.
		 */
		[[nodiscard]] constexpr const std::weak_ptr<Window>& GetWindow() const noexcept {
			return m_Window;
		}
		
		/**
		 * @brief Get the Camera's Transform.
		 * @param[in] _transform The weak pointer to the transform.
		 */
		void SetTransform(const std::weak_ptr<Transform>& _transform) noexcept;
		
		/**
		 * @brief Get the Camera's Transform.
		 * @return The weak pointer to the transform.
		 */
		[[nodiscard]] const std::weak_ptr<Transform>& GetTransform() const noexcept {
			return m_Transform;
		}
		
		/**
		 * @brief Get the Camera's aspect.
		 * @return The aspect of window the camera is bound to, or -1.0 if no valid window is bound.
		 */
		[[nodiscard]] float Aspect() const;
		
		/**
		 * @brief Set the Camera's field of view.
		 * @param[in] _fov The new field of view.
		 */
		void FOV(const float& _fov) noexcept;
		
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
		void NearClip(const float& _nearClip) noexcept;
		
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
		void FarClip(const float& _farClip) noexcept;
		
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
		static void ClearColor(glm::vec4 _color);
		
		/**
		 * @brief Get the Camera's clear color.
		 * @return The Camera's clear color.
		 */
		[[nodiscard]] static glm::vec4 ClearColor();
		
		/**
		 * @brief Get the Camera's projection matrix.
		 * @return The Camera's projection matrix.
		 */
		const glm::mat4& Projection();
		
		/**
		 * @brief Get the Camera's view matrix.
		 * @return The Camera's view matrix.
		 */
		[[nodiscard]] glm::mat4 View() const;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_CAMERA_H