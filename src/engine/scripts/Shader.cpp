#include "stdafx.h"

#include "Shader.h"

Shader::SubShader::SubShader(const char* _path, GLuint _type) : m_Path(_path), m_Type(_type) {}

Shader::Shader(const std::vector<Shader::SubShader>& _subShaders) {

	for (auto shader : _subShaders) {
		std::string src = File::ReadAllText(shader.m_Path);
		Compile(src.data(), shader.m_Type);
	}

	m_ProgramID = glCreateProgram();

	AttachShaders();
	LinkShaders();
	DetachShaders();
}

Shader::~Shader() {
	glDeleteProgram(m_ProgramID);
}

std::shared_ptr<Shader> Shader::Create(const std::vector<Shader::SubShader>& _subShaders) {
	return std::shared_ptr<Shader>(new Shader(_subShaders), [](Shader* _ptr) { delete _ptr; });
}

void Shader::AttachShaders() {
	for (auto id : m_SubShaders) {
		glAttachShader(m_ProgramID, id);
	}
}

void Shader::DetachShaders() {
	for (auto id : m_SubShaders) {
		glDetachShader(m_ProgramID, id);
		glDeleteShader(id);
	}

	m_SubShaders.clear();
}

void Shader::LinkShaders() {

	try {
		GLint success = 0;

		glLinkProgram(m_ProgramID);
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);

		if (!success) {
			throw std::runtime_error("ERROR (Shader.cpp [LinkShaders()]): Failed to link shader program.");
		}
	}
	catch (std::exception e) {
		throw e;
	}
}

void Shader::Compile(const char* _src, GLuint _type) {

	try {
		GLint success = 0;

		m_SubShaders.push_back(0);
		m_SubShaders.back() = glCreateShader(_type);
		glShaderSource(m_SubShaders.back(), 1, &_src, NULL);
		glCompileShader(m_SubShaders.back());
		glGetShaderiv(m_SubShaders.back(), GL_COMPILE_STATUS, &success);

		if (!success) {

			GLint maxLength = 0;

			glGetShaderiv(m_SubShaders.back(), GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(m_SubShaders.back(), maxLength, &maxLength, &errorLog[0]);

			std::stringstream err;
			err << "ERROR (Shader.cpp [Compile(const char*, GLuint)]): Failed to compile shader\"" <<
				&errorLog.at(0) << "\".";

			throw std::runtime_error(err.str());
		}
	}
	catch (std::exception e) {
		throw e;
	}
}

GLint Shader::GetUniform(const char* _name) {
	return glGetUniformLocation(this->ID(), _name);
}

void Shader::BindAttribute(GLuint _pos, const char* _name) {
	glBindAttribLocation(this->ID(), _pos, _name);
}

GLuint Shader::ID() {
	return this->m_ProgramID;
}
