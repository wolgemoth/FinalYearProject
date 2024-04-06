#include "Shader.h"

#include "../core/Debug.h"
#include "../core/File.h"
#include "../core/utils/Hashmap.h"
#include "../core/utils/Utils.h"

#include "Light.h"
#include "Texture.h"
#include "textures/RenderTexture.h"

#include <GL/glew.h>
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <exception>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace LouiEriksson::Engine::Graphics {
	
	SubShader::SubShader(const char* _path, const GLenum& _type) noexcept :
			m_Path(_path),
			m_Type(static_cast<GLint>(_type)) {}
	
	Shader::Shader(const std::filesystem::path& _path) {
		
		Hashmap<GLenum, std::string> subShaders;
		
		subShaders = ExtractSubshaders(File::ReadAllText(_path));
		
		if (!subShaders.empty()) {
			
			m_Name = _path.stem().string();
			
			for (const auto& kvp : subShaders.GetAll()) {
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
	
	Shader::Shader(const std::vector<SubShader>& _subShaders) {
		
		if (!_subShaders.empty()) {
			
			m_Name = std::filesystem::path(_subShaders[0].m_Path).stem().string();
			
			for (const auto& shader: _subShaders) {
				
				Debug::Log("Loading Shader Asset \"" + std::string(shader.m_Path) + "\"... ", LogType::Info, true);
					const auto src = File::ReadAllText(shader.m_Path);
				Debug::Log("Done.", LogType::Info);
				
				Compile(src, shader.m_Type);
			}
			
			m_ProgramID = glCreateProgram();
			
			AttachShaders();
			  LinkShaders();
			DetachShaders();
		}
		else {
			Debug::Log("Attempted to create a shader object with no subshaders.", LogType::Error);
		}
	}
	
	Shader::~Shader() {
		Shader::Unbind();
		glDeleteProgram(m_ProgramID);
	}
	
	void Shader::AttachShaders() const {
		
		for (const auto& id: m_SubShaders) {
			glAttachShader(m_ProgramID, id);
		}
	}
	
	void Shader::DetachShaders() {
		for (const auto& id : m_SubShaders) {
			glDetachShader(m_ProgramID, id);
			glDeleteShader(id);
		}
	
		m_SubShaders.clear();
	}
	
	void Shader::LinkShaders() const {
		
		GLint success = 0;
		
		glLinkProgram(m_ProgramID);
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		
		if (success == 0) {
			
			std::stringstream err;
			err << "Shader linking error: \"" <<
				m_Name << "\", " << glGetError()<< '\n';
			
			{
				char buff[4096] = { 0 };
				int buff_length = 0;
				
				glGetProgramInfoLog(m_ProgramID, sizeof(buff) / sizeof(buff[0]), &buff_length, buff);
			
				err << buff;
			}
			
			Debug::Log(err.str(), LogType::Error);
		}
	}
	
	void Shader::Compile(const std::string_view& _src, const GLenum& _type) {
		
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
			
			std::stringstream ss;
			ss << "Shader compilation error:\n\t";
			
			for (const auto& error : errorLog) {
				ss << error;
			}
			
			Debug::Log("Failed.", LogType::Error);
			Debug::Log(ss.str(), LogType::Error);
		}
	}
	
	Hashmap<GLenum, std::string> Shader::ExtractSubshaders(const std::string& _src) {
		
		Hashmap<GLenum, std::string> result;
		
		// Get the individual lines of the shader.
		const auto lines = Utils::Split(_src, '\n');
		
		std::stringstream ss;
		
		// Parse over each line of the shader. Check for preprocessor definitions of
		// shader types, and use that to change the context of which shader type
		// is being read. Write the data to a stream, and copy that data on
		// context change.
		auto curr = GL_NONE;
		for (const auto& line : lines) {
			
			const auto l = Utils::Trim(line); // Trim leading and trailing whitespace from text.
			
			auto type = curr;
			
			     if (l == "#pragma vertex"  ) { type =   GL_VERTEX_SHADER; }
			else if (l == "#pragma geometry") { type = GL_GEOMETRY_SHADER; }
			else if (l == "#pragma fragment") { type = GL_FRAGMENT_SHADER; }
			
			if (type == curr) {
				ss << l << '\n';
			}
			else {
				
				if (curr != GL_NONE) {
					
					// Get existing string (uses default if none).
					std::string source;
					result.Get(curr, source);
					
					// Concatenate exiting with stream contents.
					source += ss.str();
					
					// Assign back to the hashmap,
					result.Assign(curr, source);
					
					// Clear the stream.
					ss.str(std::string());
				}
				
				// Set current to type.
				curr = type;
			}
		}
		
		if (curr != GL_NONE) {
			
			// Get existing string (uses default if none).
			std::string source;
			result.Get(curr, source);
			
			// Concatenate exiting with stream contents.
			source += ss.str();
			
			// Assign back to the hashmap,
			result.Assign(curr, source);
		}
		
		return result;
	}
	
	void Shader::Bind(const GLint& _id) {
		
		if (s_CurrentProgram != _id) {
			glUseProgram(s_CurrentProgram = _id);
		}
	}
	
	void Shader::Unbind() {
		
		if (s_CurrentProgram != GL_NONE) {
			glUseProgram(s_CurrentProgram = GL_NONE);
		}
	}
	
	std::string_view Shader::Name() const noexcept {
		return m_Name;
	}
	
	void Shader::BindAttribute(const GLint& _pos, const char* _name) const {
		glBindAttribLocation(ID(), _pos, _name);
	}
	
	GLint Shader::AttributeID(const std::string& _name) {
		
		GLint result = -1;
		
		if (!m_ParameterIDs.Get(_name, result)) {
			result = glGetUniformLocation(ID(), _name.c_str());
			
			if (result != -1) {
				m_ParameterIDs.Assign(_name, result);
			}
		}
		
		if (result == -1) {
//
//			Debug::Log(
//				"No parameter with name \"" + std::string(_name) +
//				"\" exists in program with ID \"" + std::to_string(ID()) +
//				"\"\n If you are sure this parameter exists, check that it is used in the shader as it could have been optimised-out.",
//				LogType::Info
//			);
		}
		
		return result;
	}
	
	void Shader::Assign(const GLint& _id, const bool&    _value) { glUniform1i (_id, static_cast<GLint>(_value)); }
	void Shader::Assign(const GLint& _id, const GLint&   _value) { glUniform1i (_id, _value); }
	void Shader::Assign(const GLint& _id, const GLuint&  _value) { glUniform1ui(_id, _value); }
	void Shader::Assign(const GLint& _id, const GLfloat& _value) { glUniform1f (_id, _value); }
	
	void Shader::Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y)                                       { glUniform2f(_id, _x, _y);         }
	void Shader::Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z)                    { glUniform3f(_id, _x, _y, _z);     }
	void Shader::Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z, const GLfloat& _w) { glUniform4f(_id, _x, _y, _z, _w); }
	
	void Shader::Assign(const GLint& _id, const glm::vec2& _value) { glUniform2fv(_id, 1, glm::value_ptr(_value)); }
	void Shader::Assign(const GLint& _id, const glm::vec3& _value) { glUniform3fv(_id, 1, glm::value_ptr(_value)); }
	void Shader::Assign(const GLint& _id, const glm::vec4& _value) { glUniform4fv(_id, 1, glm::value_ptr(_value)); }
	
	void Shader::Assign(const GLint& _id, const glm::mat2& _mat) { glUniformMatrix2fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
	void Shader::Assign(const GLint& _id, const glm::mat3& _mat) { glUniformMatrix3fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
	void Shader::Assign(const GLint& _id, const glm::mat4& _mat) { glUniformMatrix4fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
	
	void Shader::Assign(const GLint& _id, const Texture& _texture, const GLint& _imageUnit) {
		
		glActiveTexture(GL_TEXTURE0 + _imageUnit);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(_id, _imageUnit);
		
		Texture::Bind(_texture, true);
	}
	
	void Shader::Assign(const GLint& _id, const Cubemap& _texture, const GLint& _imageUnit) {
		
		glActiveTexture(GL_TEXTURE0 + _imageUnit);
		glEnable(GL_TEXTURE_CUBE_MAP);
		glUniform1i(_id, _imageUnit);
		
		Cubemap::Bind(_texture, true);
	}
	
	void Shader::Assign(const GLint& _id, const Light::Parameters::ShadowMap& _texture, const GLint& _imageUnit) {
		
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
	
	void Shader::AssignDepth(const GLint& _id, const RenderTexture& _texture, const GLint& _imageUnit) {
		
		// TODO: Refactoring the way depth attachments are represented can improve this!
		
		glActiveTexture(GL_TEXTURE0 + _imageUnit);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(_id, _imageUnit);
		
		Texture::Bind(_texture.DepthID(), true);
	}
	
	GLint Shader::ID() const noexcept {
		return m_ProgramID;
	}
	
} // LouiEriksson::Engine::Graphics