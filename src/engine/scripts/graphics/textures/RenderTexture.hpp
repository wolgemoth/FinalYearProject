#ifndef FINALYEARPROJECT_RENDER_TEXTURE_HPP
#define FINALYEARPROJECT_RENDER_TEXTURE_HPP

#include "../../core/Debug.hpp"
#include "../Texture.hpp"

#include <GL/glew.h>

#include <string>
#include <utility>

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
		)  {
			
			// Generate the "Frame Buffer Object".
			glGenFramebuffers(1, &m_FBO_ID);
			
			if (m_FBO_ID != GL_NONE) {
				
				Bind(*this);
				
				// COLOR
				glGenTextures(1, &m_TextureID);
				glBindTexture(GL_TEXTURE_2D, Texture::s_CurrentTexture = static_cast<GLint>(m_TextureID));
				
				glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(_format.PixelFormat()), _width, _height, 0, _format.TextureFormat(), GL_HALF_FLOAT, nullptr);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(_filterMode.Min()));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(_filterMode.Mag()));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(_wrapMode.WrapS()));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(_wrapMode.WrapT()));
				
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);
				
				// DEPTH
				switch (_depthMode) {
					case RenderTexture::Parameters::NONE: {
						break;
					}
					case RenderTexture::Parameters::RENDER_BUFFER: {
						
						glGenRenderbuffers(1, &m_RBO_ID);
						glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
						glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);
						
						break;
					}
					case RenderTexture::Parameters::FRAME_BUFFER: {
						
						glGenTextures(1, &m_Depth_ID);
						glBindTexture(GL_TEXTURE_2D, Texture::s_CurrentTexture = static_cast<GLint>(m_Depth_ID));
						
						// TODO: Investigate compatibility of GL_HALF_FLOAT vs GL_FLOAT on non-Nvidia cards.
						glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_HALF_FLOAT, nullptr);
						
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(_filterMode.Min()));
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(_filterMode.Mag()));
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(_wrapMode.WrapS()));
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(_wrapMode.WrapT()));
						
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Depth_ID, 0);
						
						break;
					}
					default: {
						Debug::Log("Unknown Depth Mode \"" + std::to_string(m_DepthMode) + "\".", Error);
	
						break;
					}
				}
				
				m_Width     = _width;
				m_Height    = _height;
				m_DepthMode = _depthMode;
			}
			else {
				Debug::Log("Failed generating Frame Buffer Object for new RenderTexture!", Error);
			}
		}
		
	public:
		
		RenderTexture(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode) : Texture(_width, _height, 0, _format, _filterMode, _wrapMode),
			m_FBO_ID   (GL_NONE),
			m_RBO_ID   (GL_NONE),
			m_Depth_ID (GL_NONE),
			m_DepthMode(RenderTexture::Parameters::DepthMode::NONE)
		{
			Create(_width, _height, _format, _filterMode, _wrapMode, _depthMode);
		}
		
		~RenderTexture() {
			Discard();
		}
		
		RenderTexture(             const RenderTexture& _other) = delete;
		RenderTexture& operator = (const RenderTexture& _other) = delete;
		
		RenderTexture (RenderTexture&& _other) noexcept : Texture(std::move(_other)),
		    m_FBO_ID   (_other.m_FBO_ID   ),
			m_RBO_ID   (_other.m_RBO_ID   ),
			m_Depth_ID (_other.m_Depth_ID ),
			m_DepthMode(_other.m_DepthMode)
		{
			if (&_other != this) {
			
				_other.m_FBO_ID    = GL_NONE;
				_other.m_RBO_ID    = GL_NONE;
				_other.m_Depth_ID  = GL_NONE;
				_other.m_DepthMode = Parameters::DepthMode::NONE;
			}
		}
		
		RenderTexture& operator = (RenderTexture&& _other) noexcept {
			
			if (this != &_other) {
				
				m_FBO_ID    = _other.m_FBO_ID;
				m_RBO_ID    = _other.m_RBO_ID;
				m_Depth_ID  = _other.m_Depth_ID;
				m_DepthMode = _other.m_DepthMode;
				
				Texture::operator = (std::move(_other));
			}
			
			return *this;
		}
		
		/**
		 * @brief Reinitialises the texture dimension with given width and height while maintaining the original format, filter mode, wrap mode, and depth attachment.
		 * @param[in] _width Specifies the new width for the texture.
		 * @param[in] _height Specifies the new height for the texture.
		 * @note This function discards the current texture and re-creates it with the new dimensions. The texture's other properties (format, filter mode, wrap mode, depth mode) remain unchanged.
		 */
		void Reinitialise(const int& _width, const int& _height) {
			
			Discard();
			Create(_width, _height, m_Format, m_FilterMode, m_WrapMode, m_DepthMode);
		}
		
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
		void Reinitialise(const int& _width, const int& _height, const Texture::Parameters::Format& _format, const Texture::Parameters::FilterMode& _filterMode, const Texture::Parameters::WrapMode& _wrapMode, const RenderTexture::Parameters::DepthMode& _depthMode)  {
		
			Discard();
			Create(_width, _height, _format, _filterMode, _wrapMode, _depthMode);
		}
		
		/**
		 * @brief Binds the provided RenderTexture.
		 *
		 * This function binds the provided RenderTexture. If _force is true, the binding is forced even if the current FBO is already set to the RenderTexture's FBO ID.
		 *
		 * @param[in] _rt The RenderTexture to bind.
		 * @param[in] _force Whether to call the underlying bind function of the graphics API regardless of the currently bound FBO.
		 */
		static void Bind(const RenderTexture& _rt, const bool& _force = false) {
			
			if (_force || s_CurrentFBO != _rt.m_FBO_ID) {
				glBindFramebuffer(GL_FRAMEBUFFER, s_CurrentFBO = _rt.m_FBO_ID);
			}
		}
		
		/**
		 * @brief Binds the provided RenderTexture.
		 *
		 * This function binds the provided RenderTexture. If _force is true, the binding is forced even if the current FBO is already set to the RenderTexture's FBO ID.
		 *
		 * @param[in] _fbo The RenderTexture to bind.
		 * @param[in] _force Whether to call the underlying bind function of the graphics API regardless of the currently bound FBO.
		 */
		static void Bind(const GLuint& _fbo, const bool& _force = false) {
			
			if (_force || s_CurrentFBO != _fbo) {
				glBindFramebuffer(GL_FRAMEBUFFER, s_CurrentFBO = _fbo);
			}
		}
		
		/**
		 * @brief Unbinds the currently bound RenderTexture.
		 *
		 * This function unbinds the currently bound RenderTexture. If _force is true or there is a currently bound FBO, the function will call glBindFramebuffer to bind GL_NONE, effectively unbinding the RenderTexture.
		 *
		 * @param[in] _force Whether to call the underlying bind function of the graphics API even if there is no currently bound FBO.
		 */
		static void Unbind(const bool& _force = false) {
			
			if (_force || s_CurrentFBO != GL_NONE) {
				glBindFramebuffer(GL_FRAMEBUFFER, s_CurrentFBO = GL_NONE);
			}
		}
		
		/** @inheritdoc */
		void Discard() const override  {
			
			if (m_FBO_ID != GL_NONE) {
				RenderTexture::Unbind();
				glDeleteFramebuffers (1, &m_FBO_ID);
			}
			
			if (m_RBO_ID != GL_NONE) {
				glBindRenderbuffer(GL_RENDERBUFFER, GL_NONE);
				glDeleteRenderbuffers(1, &m_RBO_ID);
			}
			
			if (m_Depth_ID != GL_NONE) {
				Texture::Unbind();
				glDeleteTextures(1, &m_Depth_ID);
			}
			
			Texture::Discard();
		}
		
		/**
		 * @brief Get the ID of the depth attachment.
		 * @return The ID of the depth attachment.
		 */
		[[nodiscard]] constexpr const GLuint& DepthID() const noexcept {
			return m_Depth_ID;
		}
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_RENDER_TEXTURE_HPP