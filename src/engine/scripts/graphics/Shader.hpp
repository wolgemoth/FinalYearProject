#ifndef FINALYEARPROJECT_SHADER_HPP
#define FINALYEARPROJECT_SHADER_HPP

#include "../core/Debug.hpp"
#include "../core/File.hpp"
#include "../core/utils/Hashmap.hpp"
#include "../core/utils/Utils.hpp"
#include "Light.hpp"
#include "Texture.hpp"
#include "textures/Cubemap.hpp"
#include "textures/RenderTexture.hpp"

#include <GL/glew.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <array>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine {
	
	class Application;
	class Resources;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	/**
	 * @struct SubShader
	 * @brief Represents a subshader object.
	 *
	 * This struct is used to store information about a subshader, including its file path and type.
	 */
	struct SubShader final {
		
		const char* const m_Path;
		
		const GLint m_Type;
		
		constexpr SubShader(const char* _path, const GLenum& _type) noexcept :
			m_Path(_path),
			m_Type(static_cast<const GLint>(_type)) {}
		
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
		
		void AttachShaders() const {
			
			for (const auto& id: m_SubShaders) {
				glAttachShader(m_ProgramID, id);
			}
		}
		
		void DetachShaders() {
			
			for (const auto& id : m_SubShaders) {
				glDetachShader(m_ProgramID, id);
				glDeleteShader(id);
			}
		
			m_SubShaders.clear();
		}
		
		void LinkShaders() const {
			
			GLint success = 0;
			
			glLinkProgram(m_ProgramID);
			glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
			
			if (success == 0) {
				
				std::ostringstream err;
				err << "Shader linking error: \"" <<
					m_Name << "\", " << glGetError()<< '\n';
				
				{
					std::array<char, 4096U> buff{};
					
					int ctr = 0;
					glGetProgramInfoLog(m_ProgramID, buff.size(), &ctr, buff.data());
				
					err << buff.data();
				}
				
				Debug::Log(err.str(), LogType::Error);
			}
		}
		
		void Compile(const std::string_view& _src, const GLenum& _type) {
			
			GLint success = 0;
			
			{
				std::string type_string;
				
				     if (_type ==   GL_VERTEX_SHADER) { type_string =   "GL_VERTEX_SHADER"; }
				else if (_type == GL_GEOMETRY_SHADER) { type_string = "GL_GEOMETRY_SHADER"; }
				else if (_type == GL_FRAGMENT_SHADER) { type_string = "GL_FRAGMENT_SHADER"; }
				else {
					type_string = "UNKNOWN TYPE";
				}
				
				Debug::Log("Compiling Shader \"" + m_Name + "\" (" + type_string + ")... ", LogType::Info, true);
			}
			
			const auto* const src = _src.data();
	
			m_SubShaders.emplace_back(glCreateShader(_type));
			glShaderSource(m_SubShaders.back(), 1, &src, nullptr);
			glCompileShader(m_SubShaders.back());
			glGetShaderiv(m_SubShaders.back(), GL_COMPILE_STATUS, &success);
			
			if (success != 0) {
				Debug::Log("Done.", LogType::Info);
			}
			else {
				
				GLint maxLength = 0;
				
				glGetShaderiv(m_SubShaders.back(), GL_INFO_LOG_LENGTH, &maxLength);
				
				std::vector<GLchar> errorLog(maxLength);
				glGetShaderInfoLog(m_SubShaders.back(), maxLength, &maxLength, errorLog.data());
				
				std::ostringstream ss;
				ss << "Shader compilation error:\n\t";
				
				for (const auto& error : errorLog) {
					ss << error;
				}
				
				Debug::Log("Failed.", LogType::Error);
				Debug::Log(ss.str(), LogType::Error);
			}
		}
		
		static Hashmap<GLenum, std::string> ExtractSubshaders(std::stringstream&& _src) {
			
			Hashmap<GLenum, std::string> result;
			
			std::ostringstream ss;
			
			// Parse over each line of the shader. Check for preprocessor definitions of
			// shader types, and use that to change the context of which shader type
			// is being read. Write the data to a stream, and copy that data on
			// context change.
			auto curr = GL_NONE;
			
			std::string line;
	        while (std::getline(_src, line)) {
				
				auto type = curr;
				
				     if (line == "#pragma vertex"  ) { type =   GL_VERTEX_SHADER; }
				else if (line == "#pragma geometry") { type = GL_GEOMETRY_SHADER; }
				else if (line == "#pragma fragment") { type = GL_FRAGMENT_SHADER; }
				
				if (type == curr) {
					ss << line << '\n';
				}
				else {
					
					if (curr != GL_NONE) {
						
						// Get existing string (uses default if none).
						const auto existing = result.Get(curr);
						auto source = existing.has_value() ? *existing : "";
						
						// Concatenate exiting with stream contents.
						source += ss.str();
						
						// Assign back to the hashmap,
						result.Assign(curr, std::move(source));
						
						// Clear the stream.
						ss.str({});
					}
					
					// Set current to type.
					curr = type;
				}
			}
			
			if (curr != GL_NONE) {
				
				// Get existing string (uses default if none).
				const auto existing = result.Get(curr);
				auto source = existing.has_value() ? *existing : "";
				
				ss << '\n';         // Add a blank line to end of shader (opengl reasons).
				source += ss.str(); // Concatenate exiting with stream contents.
				
				// Assign back to the hashmap,
				result.Assign(curr, std::move(source));
			}
			
			return result;
		}
		
		explicit Shader(const std::vector<SubShader>& _subShaders) {
			
			if (!_subShaders.empty()) {
				
				m_Name = std::filesystem::path(_subShaders[0].m_Path).stem().string();
				
				for (const auto& shader: _subShaders) {
					
					Debug::Log("Loading Shader Asset \"" + std::string(shader.m_Path) + "\"... ", LogType::Info, true);
					
					Compile((File::ReadAllText(shader.m_Path) << "\n").str(), shader.m_Type);
					
					Debug::Log("Done.", LogType::Info);
				}
				
				m_ProgramID = glCreateProgram();
				
				AttachShaders();
				  LinkShaders();
				DetachShaders();
			}
			else {
				throw std::runtime_error("Attempted to create a shader object with no subshaders.");
			}
		}
		
		explicit Shader(const std::filesystem::path& _path) {
			
			Hashmap<GLenum, std::string> subShaders;
			
			subShaders = ExtractSubshaders(File::ReadAllText(_path));
			
			if (!subShaders.empty()) {
				
				m_Name = _path.stem().string();
				
				for (const auto& kvp : subShaders) {
					Compile(kvp.second, kvp.first);
				}
				
				m_ProgramID = glCreateProgram();
				
				AttachShaders();
				  LinkShaders();
				DetachShaders();
			}
			else {
				throw std::runtime_error("Attempted to create a shader object with no subshaders.");
			}
		}
		
		~Shader() {
			Shader::Unbind();
			glDeleteProgram(m_ProgramID);
		}
	
		constexpr operator const GLint& () const noexcept { return ID(); }
		
	public:
		
		/**
		* @brief Bind the provided Shader.
		*
		* @param[in] _id The ID of the Shader to bind.
		*/
		static void Bind(const GLint& _id) {
			
			if (s_CurrentProgram != _id) {
				glUseProgram(s_CurrentProgram = _id);
			}
		}
		
		/**
		 * @brief Unbinds the currently bound Shader.
		 *
		 * This function unbinds the currently bound Shader.
		 * If there is no currently bound Shader, this function does nothing.
		 */
		static void Unbind() {
			
			if (s_CurrentProgram != GL_NONE) {
				glUseProgram(s_CurrentProgram = GL_NONE);
			}
		}
		
		/**
		 * @brief Get the name of the shader.
		 *
		 * This function returns a std::string_view containing the name of the shader.
		 *
		 * @return std::string_view The name of the shader.
		 */
		[[nodiscard]] std::string_view Name() const noexcept { return m_Name; }
		
		/**
		 * @brief Bind the attribute location for a shader program.
		 *
		 * This function binds the attribute location for a specified shader program.
		 *
		 * @param[in] _pos The index of the attribute location.
		 * @param[in] _name The name of the attribute.
		 */
		void BindAttribute(const GLint& _pos, const char* _name) const { glBindAttribLocation(ID(), _pos, _name); }
		
		/**
		 * @brief Get the ID of an attribute by name.
		 * @param[in] _name The name of the attribute.
		 * @param[in] _verbose Whether to display a message if the attribute is not found. Default is false.
		 * @return The ID of the attribute if found, -1 otherwise.
		 */
		GLint AttributeID(const std::string& _name, const bool& _verbose = false) {
		
			GLint result;
			
			if (const auto id = m_ParameterIDs.Get(_name)) {
				result = *id;
			}
			else {
				result = glGetUniformLocation(ID(), _name.c_str());
				
				if (result != -1) {
					m_ParameterIDs.Assign(_name, result);
				}
				if (_verbose && result == -1) {
		
					Debug::Log(
						"No parameter with name \"" + std::string(_name) +
						"\" exists in program with ID \"" + std::to_string(ID()) +
						"\"\n If you are sure this parameter exists, check that it is used in the shader as it could have been optimised-out.",
						LogType::Info
					);
				}
			}
			
			return result;
		}
		
		/**
		 * @brief Assigns a boolean value to the shader program.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The boolean value to assign.
		 */
		static void Assign(const GLint& _id, const bool& _value) { glUniform1i (_id, static_cast<GLint>(_value)); }
		
		/**
		 * @brief Assigns an integer value to the Shader.
		 *
		 * This function assigns an integer value to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The integer value to assign.
		 */
		static void Assign(const GLint& _id, const GLint& _value) { glUniform1i (_id, _value); }
		
		/**
		 * @brief Assigns an unsigned integer value to the Shader.
		 *
		 * This function assigns an unsigned integer value to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The unsigned integer value to assign.
		 */
		static void Assign(const GLint& _id, const GLuint& _value) { glUniform1ui(_id, _value); }
		
		/**
		 * @brief Assigns a floating-point value to the Shader program.
		 *
		 * This function assigns a floating-point value to the specified Shader program.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The floating-point value to assign.
		 */
		static void Assign(const GLint& _id, const GLfloat& _value)  { glUniform1f (_id, _value); }
		
		/**
		 * @brief Assigns a 2-dimensional vector to the Shader.
		 *
		 * This function assigns a 2-dimensional vector to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _x     The x-component of the vector.
		 * @param[in] _y     The y-component of the vector.
		 */
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y) { glUniform2f(_id, _x, _y);         }
		
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
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z) { glUniform3f(_id, _x, _y, _z);     }
		
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
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z, const GLfloat& _w) { glUniform4f(_id, _x, _y, _z, _w); }
		
		/**
		 * @brief Assigns a 2-dimensional vector to the Shader.
		 *
		 * This function assigns a 2-dimensional vector to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The vector to assign.
		 */
		static void Assign(const GLint& _id, const vec2& _value) { glUniform2fv(_id, 1, glm::value_ptr(_value)); }
		
		/**
		 * @brief Assigns a 3-dimensional vector to the Shader program.
		 *
		 * This function assigns a 3-dimensional vector to the specified Shader program.
		 *
		 * @param[in] _id      The ID of the shader program.
		 * @param[in] _value   The vector to assign.
		 */
		static void Assign(const GLint& _id, const vec3& _value) { glUniform3fv(_id, 1, glm::value_ptr(_value)); }
		
		/**
		 * @brief Assigns a 4-dimensional vector to the Shader.
		 *
		 * This function assigns a 4-dimensional vector to the specified Shader.
		 *
		 * @param[in] _id    The ID of the shader program.
		 * @param[in] _value The vector to assign.
		 */
		static void Assign(const GLint& _id, const vec4& _value) { glUniform4fv(_id, 1, glm::value_ptr(_value)); }
		
		/**
		 * @brief Assigns a 2x2 matrix to the Shader.
		 *
		 * This function assigns a 2x2 matrix to the specified Shader.
		 *
		 * @param[in] _id The ID of the shader program.
		 * @param[in] _mat The matrix to assign.
		 */
		static void Assign(const GLint& _id, const mat2& _mat) { glUniformMatrix2fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
		
		/**
		* @brief Assigns a 3x3 matrix to the Shader.
		*
		* This function assigns a 3x3 matrix to the specified Shader.
		* The matrix is assigned using the provided ID.
		*
		* @param[in] _id The ID of the shader program.
		* @param[in] _mat The matrix to assign.
		*/
		static void Assign(const GLint& _id, const mat3& _mat) { glUniformMatrix3fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
		
		/**
		 * @brief Assigns a 4x4 matrix to the Shader program.
		 *
		 * This function assigns a 4x4 matrix to the specified Shader program.
		 *
		 * @param[in] _id The ID of the shader program.
		 * @param[in] _mat The matrix to assign.
		 */
		static void Assign(const GLint& _id, const mat4& _mat) { glUniformMatrix4fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
	
		
		/**
		 * @brief Assigns a Texture to the shader.
		 *
		 * This function assigns a Texture to the specified shader program and image unit.
		 *
		 * @param[in] _id The ID of the shader program.
		 * @param[in] _texture The Texture to assign.
		 * @param[in] _imageUnit The image unit to bind the Texture to.
		 */
		static void Assign(const GLint& _id, const Texture& _texture, const GLint& _imageUnit)  {
			
			glActiveTexture(GL_TEXTURE0 + _imageUnit);
			glEnable(GL_TEXTURE_2D);
			glUniform1i(_id, _imageUnit);
			
			Texture::Bind(_texture, true);
		}
		
		/**
		 * @brief Assigns a Cubemap texture to the shader.
		 *
		 * This function assigns a Cubemap to the specified shader program and image unit.
		 *
		 * @param[in] _id The ID of the shader uniform.
		 * @param[in] _texture The Cubemap texture to be assigned.
		 * @param[in] _imageUnit The texture unit to be activated and bound.
		 */
		static void Assign(const GLint& _id, const Cubemap& _texture, const GLint& _imageUnit) {
		
			glActiveTexture(GL_TEXTURE0 + _imageUnit);
			glEnable(GL_TEXTURE_CUBE_MAP);
			glUniform1i(_id, _imageUnit);
			
			Cubemap::Bind(_texture, true);
		}
		
		/**
		 * @brief Assigns a ShadowMap to the shader.
		 *
		 * This function assigns a ShadowMap to the specified shader program and image unit.
		 *
		 * @param[in] _id The uniform ID in the shader program.
		 * @param[in] _texture The shadow map object to assign.
		 * @param[in] _imageUnit The image unit to activate.
		 */
		static void Assign(const GLint& _id, const Light::Parameters::ShadowMap& _texture, const GLint& _imageUnit) {
		
			glActiveTexture(GL_TEXTURE0 + _imageUnit);
			glEnable(_texture.m_Target);
			glUniform1i(_id, _imageUnit);
		
			// TODO: Refactoring ShadowMap into "RenderCube" can improve this!
			switch (_texture.m_Target) {
				case GL_TEXTURE_2D:       { Texture::Bind(_texture.m_ShadowMap_Texture, true); break; }
				case GL_TEXTURE_CUBE_MAP: { Cubemap::Bind(_texture.m_ShadowMap_Texture, true); break; }
				default: {
					Debug::Log("Unknown target \"" + std::to_string(_texture.m_Target) + "\"", LogType::Error);
				}
			}
		}
		
		/**
		 * @brief Assigns a RenderTexture's depth attachment to the shader.
		 *
		 * This function is used to assign the depth attachment of a RenderTexture to a shader. It is intended for use with frame buffer objects (FBOs) and may be deprecated in the future.
		 *
		 * @param[in] _id The ID of the shader uniform to bind the depth attachment to.
		 * @param[in] _texture The RenderTexture from which to retrieve the depth attachment.
		 * @param[in] _imageUnit The image unit index to which the depth attachment will be bound.
		 */
		static void AssignDepth(const GLint& _id, const RenderTexture& _texture, const GLint& _imageUnit) {
		
			// TODO: Refactoring the way depth attachments are represented can improve this!
			
			glActiveTexture(GL_TEXTURE0 + _imageUnit);
			glEnable(GL_TEXTURE_2D);
			glUniform1i(_id, _imageUnit);
			
			Texture::Bind(_texture.DepthID(), true);
		}
		
		/**
		* @brief Get the ID of the Shader.
		* @returns The ID of the Shader.
		*/
		[[nodiscard]] constexpr const GLint& ID() const noexcept {
			return m_ProgramID;
		}
	};

} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_SHADER_HPP