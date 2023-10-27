#include "Shader.h"

namespace LouiEriksson {
	
	Shader::SubShader::SubShader(const char* _path, GLint _type) : m_Path(_path), m_Type(_type) {}
	
	Shader::Shader(const std::vector<Shader::SubShader>& _subShaders) {
		
		if (!_subShaders.empty()) {
			
			for (auto& shader: _subShaders) {
				std::string src = File::ReadAllText(shader.m_Path);
				Compile(src.data(), shader.m_Type);
			}
			
			m_Name = std::filesystem::path(_subShaders[0].m_Path).stem().string();
			
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
	
	std::shared_ptr<Shader> Shader::Create(const std::vector<Shader::SubShader>& _subShaders) {
		return { new Shader(_subShaders), [](const Shader* _ptr) { delete _ptr; } };
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
				m_Name << "\", " << glGetError() << "\n";
			
			{
				char buff[4096] = { 0 };
				int buff_length = 0;
				
				glGetProgramInfoLog(m_ProgramID, sizeof(buff) / sizeof(buff[0]), &buff_length, buff);
			
				err << buff;
			}
			
			std::cout << err.str() << "\n";
			
			throw std::runtime_error(err.str());
		}
	}
	
	void Shader::Compile(const char* _src, const GLint& _type) {
		
		GLint success = 0;
		
		m_SubShaders.push_back(glCreateShader(_type));
		glShaderSource(m_SubShaders.back(), 1, &_src, nullptr);
		glCompileShader(m_SubShaders.back());
		glGetShaderiv(m_SubShaders.back(), GL_COMPILE_STATUS, &success);
		
		if (success == 0) {
			
			GLint maxLength = 0;
			
			glGetShaderiv(m_SubShaders.back(), GL_INFO_LOG_LENGTH, &maxLength);
			
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(m_SubShaders.back(), maxLength, &maxLength, &errorLog[0]);
			
			std::stringstream err;
			err << "ERROR (Shader.cpp [Compile(const char*, GLint)]): Failed to compile shader:\n\t";
			
			for (const auto& error: errorLog) {
				err << error;
			}
			
			throw std::runtime_error(err.str());
		}
	}
	
	void Shader::Compile(const std::vector<std::vector<SubShader>>& _shaders) {
		
		for (const auto& entry: _shaders) {
			
			auto shader = Create(entry);
			
			m_Cache.Add(shader->Name(), shader);
		}
	}
	
	void Shader::PreloadShaders() {
		
		// TODO: Load from a file in a standard serialisation format instead of using an initialiser list.
		Compile({
	        /* Lit Surface Shader */ {
                { "shaders/surface/surface.vert",             GL_VERTEX_SHADER },
                { "shaders/surface/surface.frag",           GL_FRAGMENT_SHADER }
            },
			 
	        /* Lit Surface Shader (PBR) */ {
				{ "shaders/pbr/pbr.vert",                     GL_VERTEX_SHADER },
                { "shaders/pbr/pbr.frag",                   GL_FRAGMENT_SHADER }
            },
			 
			 /* Shadow Depth Shader */ {
				{ "shaders/shadowDepth/shadowDepth.vert",     GL_VERTEX_SHADER },
                { "shaders/shadowDepth/shadowDepth.frag",   GL_FRAGMENT_SHADER }
            },
			 
			 /* Skybox Shader */ {
				{ "shaders/skybox/skybox.vert",                GL_VERTEX_SHADER },
				{ "shaders/skybox/skybox.frag",              GL_FRAGMENT_SHADER }
			 },
			 
	        /* Passthrough */ {
				{ "shaders/passthrough/passthrough.vert",     GL_VERTEX_SHADER },
                { "shaders/passthrough/passthrough.frag",   GL_FRAGMENT_SHADER }
            },
			 
	        /* ACES Tonemapping */ {
				{ "shaders/aces/aces.vert",                   GL_VERTEX_SHADER },
                { "shaders/aces/aces.frag",                 GL_FRAGMENT_SHADER }
            },
			 
	        /* Vignette */ {
				{ "shaders/vignette/vignette.vert",           GL_VERTEX_SHADER },
                { "shaders/vignette/vignette.frag",         GL_FRAGMENT_SHADER }
            },
			 
	        /* Grain */ {
				{ "shaders/grain/grain.vert",                 GL_VERTEX_SHADER },
                { "shaders/grain/grain.frag",               GL_FRAGMENT_SHADER }
            },
			 
			 /* Bloom - Threshold */ {
				{ "shaders/bloom/threshold/threshold.vert",   GL_VERTEX_SHADER },
                { "shaders/bloom/threshold/threshold.frag", GL_FRAGMENT_SHADER }
            },
			 
	        /* Bloom - Downscale */ {
				{ "shaders/bloom/downscale/downscale.vert",   GL_VERTEX_SHADER },
                { "shaders/bloom/downscale/downscale.frag", GL_FRAGMENT_SHADER }
            },
			 
	        /* Bloom - Upscale */ {
				{ "shaders/bloom/upscale/upscale.vert",       GL_VERTEX_SHADER },
                { "shaders/bloom/upscale/upscale.frag",     GL_FRAGMENT_SHADER }
            },
			 
	        /* Bloom - Combine */ {
				{ "shaders/bloom/combine/combine.vert",       GL_VERTEX_SHADER },
                { "shaders/bloom/combine/combine.frag",     GL_FRAGMENT_SHADER }
            },
		});
	}
	
	void Shader::Bind(const GLint& _id) {
		glUseProgram(_id);
	}
	
	void Shader::Unbind() {
		Shader::Bind(SHADER_PROGRAM_NONE);
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
			              "GL Error Code: " << errorCode << "\n";
			
			throw std::runtime_error(err.str());
		}
	}
	
	GLint Shader::AttributeID(const char* _name) const {
		return glGetUniformLocation(this->ID(), _name);
	}
	
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