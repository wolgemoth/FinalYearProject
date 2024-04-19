#ifndef FINALYEARPROJECT_RENDER_TEXTURE_H
#define FINALYEARPROJECT_RENDER_TEXTURE_H

#include "../Texture.h"

#include <GL/glew.h>

namespace LouiEriksson::Engine::Graphics {
	
	class RenderTexture final : public Texture {
		
		friend class Camera;
		friend class Light;
		
	public:
		
		struct Parameters final {
			
			/**
			 * @enum DepthMode
			 * @brief Specifies the depth attachment mode.
			 *
			 * The DepthMode enum specifies the mode for generating a depth attachment in a rendering context.
			 */
			enum DepthMode : unsigned char {
				         NONE, /**< @brief No depth attachment will be generated. */
				RENDER_BUFFER, /**< @brief A render buffer will be generated as a depth attachment. */
				 FRAME_BUFFER  /**< @brief A frame buffer will be generated as a depth attachment. */
			};
			
		};
		
		 RenderTexture(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode);
		~RenderTexture();
		
		RenderTexture(             const RenderTexture& _other) = delete;
		RenderTexture& operator = (const RenderTexture& _other) = delete;
		
		RenderTexture             (RenderTexture&& _other) noexcept;
		RenderTexture& operator = (RenderTexture&& _other) noexcept;
		
		/**
		 * @brief Reinitialises the texture dimension with given width and height while maintaining the original format, filter mode, wrap mode, and depth attachment.
		 * @param[in] _width Specifies the new width for the texture.
		 * @param[in] _height Specifies the new height for the texture.
		 * @note This function discards the current texture and re-creates it with the new dimensions. The texture's other properties (format, filter mode, wrap mode, depth mode) remain unchanged.
		 */
		void Reinitialise(const int& _width, const int& _height);
		
		/**
		 * @brief Reinitialise the texture with given width, height, format, filter mode, wrap mode, and depth attachment.
		 *
		 * This function reinitialises the texture with the given width, height, format, filter mode, wrap mode, and depth attachment. It discards the current texture and creates a new one with the provided parameters.
		 *
		 * @param[in] _width The width of the new texture.
		 * @param[in] _height The height of the new texture.
		 * @param[in] _format The format of the new texture.
		 * @param[in] _filterMode The filter mode of the new texture.
		 * @param[in] _wrapMode The wrap mode of the new texture.
		 * @param[in] _depthMode The depth attachment of the new texture.
		 */
		void Reinitialise(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode);
		
		/**
		 * @brief Binds the provided RenderTexture.
		 *
		 * This function binds the provided RenderTexture. If _force is true, the binding is forced even if the current FBO is already set to the RenderTexture's FBO ID.
		 *
		 * @param[in] _rt The RenderTexture to bind.
		 * @param[in] _force Whether to call the underlying bind function of the graphics API regardless of the currently bound FBO.
		 */
		static void Bind(const RenderTexture& _rt, const bool& _force = false);
		
		/**
		 * @brief Binds the provided RenderTexture.
		 *
		 * This function binds the provided RenderTexture. If _force is true, the binding is forced even if the current FBO is already set to the RenderTexture's FBO ID.
		 *
		 * @param[in] _fbo The RenderTexture to bind.
		 * @param[in] _force Whether to call the underlying bind function of the graphics API regardless of the currently bound FBO.
		 */
		static void Bind(const GLuint& _fbo, const bool& _force = false);
		
		/**
		 * @brief Unbinds the currently bound RenderTexture.
		 *
		 * This function unbinds the currently bound RenderTexture. If _force is true or there is a currently bound FBO, the function will call glBindFramebuffer to bind GL_NONE, effectively unbinding the RenderTexture.
		 *
		 * @param[in] _force Whether to call the underlying bind function of the graphics API even if there is no currently bound FBO.
		 */
		static void Unbind(const bool& _force = false);
		
		/** @inheritdoc */
		void Discard() const override;
		
		/**
		 * @brief Get the ID of the depth attachment.
		 * @return The ID of the depth attachment.
		 */
		[[nodiscard]] GLuint DepthID() const noexcept;
	
	private:
		
		/** @brief Currently bound FBO. */
		inline static GLuint s_CurrentFBO { GL_NONE };
		
		GLuint m_FBO_ID,
			   m_RBO_ID,
			   m_Depth_ID;
		
		/** @brief RenderTexture's depth attachment mode. */
		Parameters::DepthMode m_DepthMode;
		
		/**
		 * @brief Creates a new RenderTexture with the given parameters.
		 *
		 * This function creates a new RenderTexture with the specified width, height, format, filter mode, wrap mode, and depth attachment.
		 *
		 * @param[in] _width The width of the RenderTexture.
		 * @param[in] _height The height of the RenderTexture.
		 * @param[in] _format The format of the RenderTexture's color attachment.
		 * @param[in] _filterMode The filter mode of the RenderTexture's color attachment.
		 * @param[in] _wrapMode The wrap mode of the RenderTexture's color attachment.
		 * @param[in] _depthMode The depth attachment mode of the RenderTexture.
		 */
		void Create(
			const int& _width,
			const int& _height,
			const       Texture::Parameters::Format&         _format,
			const       Texture::Parameters::FilterMode& _filterMode,
			const       Texture::Parameters::WrapMode&     _wrapMode,
			const RenderTexture::Parameters::DepthMode&   _depthMode
		);
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_RENDER_TEXTURE_H