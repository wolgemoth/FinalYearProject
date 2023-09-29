#pragma once

#ifndef _SHADER_H
#define _SHADER_H

#include "File.h"

class Shader {

public:
	struct SubShader;

private:

	GLuint m_ProgramID;

	std::vector<GLuint> m_SubShaders;

	void AttachShaders();

	void DetachShaders();

	void LinkShaders();

	void Compile(const char* _src, GLuint _type);

	Shader(const std::vector<Shader::SubShader>& _subShaders);
	~Shader();

public:

	struct SubShader {

		const char* m_Path;
		GLuint      m_Type;

		SubShader(const char* _path, GLuint _type);
	};

	static std::shared_ptr<Shader> Create(const std::vector<Shader::SubShader>& _subShaders);

	void BindAttribute(GLuint _pos, const char* _name);

	GLint GetUniform(const char* _name);

	GLuint ID();
	operator GLuint() { return this->ID(); }
};

#endif // !_SHADER_H
