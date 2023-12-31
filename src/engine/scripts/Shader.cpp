#include "stdafx.h"

#include "Shader.h"
#include "File.h"
#include "Utils.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	Shader::SubShader::SubShader(const char* _path, GLenum _type) : m_Path(_path), m_Type(static_cast<GLint>(_type)) {}
	
	Shader::Shader(const std::filesystem::path& _path) {
		
		Hashmap<GLenum, std::string> subShaders;
		std::cout << "Loading Shader Asset \"" << _path.c_str() << "\"... ";
		
		try {
			subShaders = ExtractSubshaders(File::ReadAllText(_path));
			
			std::cout << "Done.\n";
		}
		catch (const std::exception& e){
			std::cout << "Failed.\n";
			
			std::cout << e.what()<< '\n';
		}
		
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
			throw std::runtime_error(
				"ERROR (Shader.cpp [Shader(const std::vector<SubShader>&)]): Attempted to create a shader object with no subshaders.");
		}
	}
	
	Shader::Shader(const std::vector<Shader::SubShader>& _subShaders) {
		
		if (!_subShaders.empty()) {
			
			m_Name = std::filesystem::path(_subShaders[0].m_Path).stem().string();
			
			for (const auto& shader: _subShaders) {
				
				std::cout << "Loading Shader Asset \"" << shader.m_Path << "\"... ";
					std::string src = File::ReadAllText(shader.m_Path);
				std::cout << "Done.\n";
				
				Compile(src, shader.m_Type);
			}
			
			m_ProgramID = glCreateProgram();
			
			AttachShaders();
			  LinkShaders();
			DetachShaders();
		}
		else {
			throw std::runtime_error(
				"ERROR (Shader.cpp [Shader(const std::vector<SubShader>&)]): Attempted to create a shader object with no subshaders.");
		}
	}
	
	Shader::~Shader() {
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
			err << "ERROR (Shader.cpp [LinkShaders()]): \"" <<
				m_Name << "\", " << glGetError()<< '\n';
			
			{
				char buff[4096] = { 0 };
				int buff_length = 0;
				
				glGetProgramInfoLog(m_ProgramID, sizeof(buff) / sizeof(buff[0]), &buff_length, buff);
			
				err << buff;
			}
			
			std::cout << err.str()<< '\n';
			
			throw std::runtime_error(err.str());
		}
	}
	
	void Shader::Compile(const std::string& _src, const GLenum& _type) {
		
		GLint success = 0;
		
		{
			std::string type_string;
			
			     if (_type ==   GL_VERTEX_SHADER) { type_string =   "GL_VERTEX_SHADER"; }
			else if (_type == GL_GEOMETRY_SHADER) { type_string = "GL_GEOMETRY_SHADER"; }
			else if (_type == GL_FRAGMENT_SHADER) { type_string = "GL_FRAGMENT_SHADER"; }
			else {
				type_string = "UNKNOWN TYPE";
			}
			
			std::cout << "Compiling Shader Program \"" << m_Name << "\" (" << type_string << ")... ";
		}
		
		const auto* const src = _src.c_str();

		m_SubShaders.push_back(glCreateShader(_type));
		glShaderSource(m_SubShaders.back(), 1, &src, nullptr);
		glCompileShader(m_SubShaders.back());
		glGetShaderiv(m_SubShaders.back(), GL_COMPILE_STATUS, &success);
		
		if (success == 0) {
			
			GLint maxLength = 0;
			
			glGetShaderiv(m_SubShaders.back(), GL_INFO_LOG_LENGTH, &maxLength);
			
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(m_SubShaders.back(), maxLength, &maxLength, errorLog.data());
			
			std::stringstream err;
			err << "ERROR (Shader.cpp [Compile(const char*, GLint)]): Failed to compile shader:\n\t";
			
			for (const auto& error : errorLog) {
				err << error;
			}
			
			std::cout << "Failed.\n";
		
			throw std::runtime_error(err.str());
		}
		else {
			std::cout << "Done.\n";
		}
	}
	
	Hashmap<GLenum, std::string> Shader::ExtractSubshaders(const std::string& _src) {
		
		Hashmap<GLenum, std::string> result;
		
		// Get the individual lines of the shader.
		auto lines = Utils::Split(_src, '\n');
		
		std::stringstream ss;
		
		// Parse over each line of the shader. Check for preprocessor definitions of
		// shader types, and use that to change the context of which shader type
		// is being read. Write the data to a stream, and copy that data on
		// context change.
		GLenum curr = GL_NONE;
		for (auto& line : lines) {
			
			line = Utils::Trim(line); // Trim leading and trailing whitespace from text.
			
			GLenum type = curr;
			
			     if (line == "#pragma vertex"  ) { type =   GL_VERTEX_SHADER; }
			else if (line == "#pragma geometry") { type = GL_GEOMETRY_SHADER; }
			else if (line == "#pragma fragment") { type = GL_FRAGMENT_SHADER; }
			
			if (type == curr) {
				ss << line << '\n';
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
			s_CurrentProgram = _id;
			
			glUseProgram(_id);
		}
	}
	
	void Shader::Unbind() {
		Shader::Bind(GL_NONE);
	}
	
	const std::string& Shader::Name() const {
		return m_Name;
	}
	
	void Shader::BindAttribute(const GLint& _pos, const char* _name) const {
		
		glBindAttribLocation(this->ID(), _pos, _name);
		
		const GLenum errorCode(glGetError());
		if (errorCode != 0u) {
			
			std::stringstream err;
			
			err << "ERROR (Shader.cpp [TryBindAttribute(const GLint&, const char*)]): Shader \"" <<
			    Name() << "\": Attempt at binding attribute \"" <<
			    _name  << "\", to location (" << _pos << ") failed. " <<
			              "GL Error Code: " << errorCode<< '\n';
			
			throw std::runtime_error(err.str());
		}
	}
	
	GLint Shader::AttributeID(const char* _name) {
		
		GLint result = -1;
		
		if (!m_ParameterIDs.Get(_name, result)) {
			result = glGetUniformLocation(this->ID(), _name);
			
			if (result != -1) {
				m_ParameterIDs.Assign(_name, result);
			}
		}
		
		if (result == -1) {
//
//			std::stringstream ss;
//				ss << "No parameter with name \"" << _name
//				<< "\" exists in program with ID \"" << ID() <<
//				"\"\n If you are sure this parameter exists, check that it is used in the shader. It could have been optimised-out.\n";
//
//			std::cout << ss.str();
		}
		
		return result;
	}
	
	void Shader::Assign(const GLint& _id, const bool&    _value) { glUniform1i (_id, static_cast<GLint>(_value)); }
	void Shader::Assign(const GLint& _id, const GLint&   _value) { glUniform1i (_id, _value); }
	void Shader::Assign(const GLint& _id, const GLuint&  _value) { glUniform1ui(_id, _value); }
	void Shader::Assign(const GLint& _id, const GLfloat& _value) { glUniform1f (_id, _value); }
	
	void Shader::Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y) {
		glUniform2f(_id, _x, _y);
	}
	
	void Shader::Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z) {
		glUniform3f(_id, _x, _y, _z);
	}
	
	void Shader::Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z, const GLfloat& _w) {
		glUniform4f(_id, _x, _y, _z, _w);
	}
	
	void Shader::Assign(const GLint& _id, const glm::vec2& _value) { glUniform2fv(_id, 1, glm::value_ptr(_value)); }
	void Shader::Assign(const GLint& _id, const glm::vec3& _value) { glUniform3fv(_id, 1, glm::value_ptr(_value)); }
	void Shader::Assign(const GLint& _id, const glm::vec4& _value) { glUniform4fv(_id, 1, glm::value_ptr(_value)); }
	
	void Shader::Assign(const GLint& _id, const glm::mat2& _mat) { glUniformMatrix2fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
	void Shader::Assign(const GLint& _id, const glm::mat3& _mat) { glUniformMatrix3fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
	void Shader::Assign(const GLint& _id, const glm::mat4& _mat) { glUniformMatrix4fv(_id, 1, GL_FALSE, glm::value_ptr(_mat)); }
	
	void Shader::Assign(const GLint& _id, const GLuint& _textureID, const GLint& _imageUnit, const GLenum& _target) {
		
		glActiveTexture(GL_TEXTURE0 + _imageUnit);
		glEnable(_target);
		glUniform1i(_id, _imageUnit);
		glBindTexture(_target, _textureID);
	}
	
	GLint Shader::ID() const {
		return this->m_ProgramID;
	}
}