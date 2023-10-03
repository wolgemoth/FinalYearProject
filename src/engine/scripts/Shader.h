#pragma once

#ifndef FINALYEARPROJECT_SHADER_H
#define FINALYEARPROJECT_SHADER_H

#include "File.h"

namespace LouiEriksson {

	class Shader {

	public:
		
		struct SubShader;
	
	private:
	
		GLuint m_ProgramID;
	
		std::vector<GLuint> m_SubShaders;
	
		void AttachShaders();
	
		void DetachShaders();
	
		void LinkShaders() const;
	
		void Compile(const char* _src, GLuint _type);
	
		explicit Shader(const std::vector<Shader::SubShader>& _subShaders);
		~Shader();
	
	public:
	
		struct SubShader {
	
			const char* m_Path;
			GLuint      m_Type;
	
			SubShader(const char* _path, GLuint _type);
		};
	
		static std::shared_ptr<Shader> Create(const std::vector<Shader::SubShader>& _subShaders);
	
		void BindAttribute(GLuint _pos, const char* _name) const;
	
		GLint GetUniform(const char* _name) const;
	
		[[nodiscard]] GLuint ID() const;
		explicit operator GLuint() const { return this->ID(); }
	};
}

#endif //FINALYEARPROJECT_SHADER_H