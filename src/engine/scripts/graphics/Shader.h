#ifndef FINALYEARPROJECT_SHADER_H
#define FINALYEARPROJECT_SHADER_H

#include "../core/utils/Hashmap.h"

#include "Light.h"

#include <GL/glew.h>
#include <glm/fwd.hpp>

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace LouiEriksson::Engine {
	
	class Application;
	class File;
	class Resources;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {

	class Texture;
	class RenderTexture;
	class Cubemap;
	
	/**
	 * @struct SubShader
	 * @brief Represents a subshader object.
	 *
	 * This struct is used to store information about a subshader, including its file path and type.
	 */
	struct SubShader final {
		
		const char* const m_Path;
		
		GLint m_Type;
		
		SubShader(const char* _path, const GLenum& _type) noexcept;
		
	};
	
	/**
	 * @class Shader
	 * @brief Represents a shader program.
	 *
	 * The Shader class is responsible for compiling, linking and managing shader programs.
	 */
	class Shader final {
		
		friend LouiEriksson::Engine::Application;
		friend LouiEriksson::Engine::File;
		friend LouiEriksson::Engine::Resources;
		
	private:
		
		inline static GLint s_CurrentProgram { GL_NONE };
		
		Hashmap<std::string, GLint> m_ParameterIDs;
		
		GLint m_ProgramID;
		
		std::string m_Name;
		
		std::vector<GLint> m_SubShaders;
		
		void AttachShaders() const;
		
		void DetachShaders();
		
		void LinkShaders() const;
		
		void Compile(const std::string_view& _src, const GLenum& _type);
		
		static Hashmap<GLenum, std::string> ExtractSubshaders(const std::string& _src);
		
		explicit Shader(const std::vector<SubShader>& _subShaders);
		
		explicit Shader(const std::filesystem::path& _path);
		
		~Shader();
	
	public:
		
		/**
		* @brief Bind the provided Shader.
		*
		* @param[in] _id The ID of the Shader to bind.
		*/
		static void Bind(const GLint& _id);
		
		/**
		 * @brief Unbinds the currently bound Shader.
		 *
		 * This function unbinds the currently bound Shader.
		 * If there is no currently bound Shader, this function does nothing.
		 */
		static void Unbind();
		
		/**
		 * @brief Get the name of the shader.
		 *
		 * This function returns a std::string_view containing the name of the shader.
		 *
		 * @return std::string_view The name of the shader.
		 */
		[[nodiscard]] std::string_view Name() const noexcept;
		
		/**
		 * @brief Bind the attribute location for a shader program.
		 *
		 * This function binds the attribute location for a specified shader program.
		 *
		 * @param[in] _pos The index of the attribute location.
		 * @param[in] _name The name of the attribute.
		 */
		void BindAttribute(const GLint& _pos, const char* _name) const;
		
		/**
		 * @brief Get the ID of an attribute by name.
		 * @param[in] _name The name of the attribute.
		 * @param[in] _verbose Whether to display a message if the attribute is not found. Default is false.
		 * @return The ID of the attribute if found, -1 otherwise.
		 */
		GLint AttributeID(const std::string& _name, const bool& _verbose = false);
		
		/**
		 * @brief Assigns a boolean value to the shader program.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The boolean value to assign.
		 */
		static void Assign(const GLint& _id, const bool& _value);
		
		/**
		 * @brief Assigns an integer value to the Shader.
		 *
		 * This function assigns an integer value to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The integer value to assign.
		 */
		static void Assign(const GLint& _id, const GLint& _value);
		
		/**
		 * @brief Assigns an unsigned integer value to the Shader.
		 *
		 * This function assigns an unsigned integer value to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The unsigned integer value to assign.
		 */
		static void Assign(const GLint& _id, const GLuint& _value);
		
		/**
		 * @brief Assigns a floating-point value to the Shader program.
		 *
		 * This function assigns a floating-point value to the specified Shader program.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The floating-point value to assign.
		 */
		static void Assign(const GLint& _id, const GLfloat& _value);
		
		/**
		 * @brief Assigns a 2-dimensional vector to the Shader.
		 *
		 * This function assigns a 2-dimensional vector to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _x     The x-component of the vector.
		 * @param[in] _y     The y-component of the vector.
		 */
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y);
		
		/**
		 * @brief Assigns a 3-dimensional vector to the Shader.
		 *
		 * This function assigns a 3-dimensional vector to the specified Shader.
		 *
		 * @param[in] _id The ID of the shader program.
		 * @param[in] _x The x-component of the vector.
		 * @param[in] _y The y-component of the vector.
		 * @param[in] _z The z-component of the vector.
		 */
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z);
		
		/**
		 * @brief Assigns a 4-dimensional vector to the Shader.
		 *
		 * This function assigns a 4-dimensional vector to the specified Shader.
		 *
		 * @param[in] _id The ID of the shader program.
		 * @param[in] _x The x-component of the vector.
		 * @param[in] _y The y-component of the vector.
		 * @param[in] _z The z-component of the vector.
		 * @param[in] _w The w-component of the vector.
		 */
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z, const GLfloat& _w);
		
		/**
		 * @brief Assigns a 2-dimensional vector to the Shader.
		 *
		 * This function assigns a 2-dimensional vector to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The vector to assign.
		 */
		static void Assign(const GLint& _id, const glm::vec2& _value);
		
		/**
		 * @brief Assigns a 3-dimensional vector to the Shader program.
		 *
		 * This function assigns a 3-dimensional vector to the specified Shader program.
		 *
		 * @param[in] _id      The ID of the shader program.
		 * @param[in] _value   The vector to assign.
		 */
		static void Assign(const GLint& _id, const glm::vec3& _value);
		
		/**
		 * @brief Assigns a 4-dimensional vector to the Shader.
		 *
		 * This function assigns a 4-dimensional vector to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The vector to assign.
		 */
		static void Assign(const GLint& _id, const glm::vec4& _value);
		
		/**
		 * @brief Assigns a 2x2 matrix to the Shader.
		 *
		 * This function assigns a 2x2 matrix to the specified Shader.
		 *
		 * @param[in] _id The ID of the shader program.
		 * @param[in] _mat The matrix to assign.
		 */
		static void Assign(const GLint& _id, const glm::mat2& _mat);
		
		/**
		* @brief Assigns a 3x3 matrix to the Shader.
		*
		* This function assigns a 3x3 matrix to the specified Shader.
		* The matrix is assigned using the provided ID.
		*
		* @param[in] _id The ID of the shader program.
		* @param[in] _mat The matrix to assign.
		*/
		static void Assign(const GLint& _id, const glm::mat3& _mat);
		
		/**
		 * @brief Assigns a 4x4 matrix to the Shader program.
		 *
		 * This function assigns a 4x4 matrix to the specified Shader program.
		 *
		 * @param[in] _id The ID of the shader program.
		 * @param[in] _mat The matrix to assign.
		 */
		static void Assign(const GLint& _id, const glm::mat4& _mat);
		
		/**
		 * @brief Assigns a Texture to the shader.
		 *
		 * This function assigns a Texture to the specified shader program and image unit.
		 *
		 * @param[in] _id The ID of the shader program.
		 * @param[in] _texture The Texture to assign.
		 * @param[in] _imageUnit The image unit to bind the Texture to.
		 */
		static void Assign(const GLint& _id, const Texture& _texture, const GLint& _imageUnit);
		
		/**
		 * @brief Assigns a Cubemap texture to the shader.
		 *
		 * This function assigns a Cubemap to the specified shader program and image unit.
		 *
		 * @param[in] _id The ID of the shader uniform.
		 * @param[in] _texture The Cubemap texture to be assigned.
		 * @param[in] _imageUnit The texture unit to be activated and bound.
		 */
		static void Assign(const GLint& _id, const Cubemap& _texture, const GLint& _imageUnit);
		
		/**
		 * @brief Assigns a ShadowMap to the shader.
		 *
		 * This function assigns a ShadowMap to the specified shader program and image unit.
		 *
		 * @param[in] _id The uniform ID in the shader program.
		 * @param[in] _texture The shadow map object to assign.
		 * @param[in] _imageUnit The image unit to activate.
		 */
		static void Assign(const GLint& _id, const Light::Parameters::ShadowMap& _texture, const GLint& _imageUnit);
		
		/**
		 * @brief Assigns a RenderTexture's depth attachment to the shader.
		 *
		 * This function is used to assign the depth attachment of a RenderTexture to a shader. It is intended for use with frame buffer objects (FBOs) and may be deprecated in the future.
		 *
		 * @param[in] _id The ID of the shader uniform to bind the depth attachment to.
		 * @param[in] _texture The RenderTexture from which to retrieve the depth attachment.
		 * @param[in] _imageUnit The image unit index to which the depth attachment will be bound.
		 */
		static void AssignDepth(const GLint& _id, const RenderTexture& _texture, const GLint& _imageUnit);
		
		/**
		* @brief Get the ID of the Shader.
		* @returns The ID of the Shader.
		*/
		[[nodiscard]] GLint ID() const noexcept;
		
		explicit operator GLint() const noexcept { return ID(); }
	};

} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_SHADER_H