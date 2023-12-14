#ifndef FINALYEARPROJECT_SHADER_H
#define FINALYEARPROJECT_SHADER_H

namespace LouiEriksson {

	class Shader {
		
		friend class Application;
		friend class Resources;
		
	private:
		
		inline static GLint s_CurrentProgram = GL_NONE;
		
		struct SubShader {
			
			const char* m_Path;
			
			GLint m_Type;
			
			SubShader(const char* _path, GLenum _type);
		};
		
		Hashmap<std::string, GLint> m_ParameterIDs;
		
		GLint m_ProgramID;
		
		std::string m_Name;
		
		std::vector<GLint> m_SubShaders;
		
		void AttachShaders() const;
		
		void DetachShaders();
		
		void LinkShaders() const;
		
		void Compile(const std::string& _src, const GLenum& _type);
		
		static Hashmap<GLenum, std::string> ExtractSubshaders(const std::string& _src);
		
		explicit Shader(const std::vector<Shader::SubShader>& _subShaders);
		
		explicit Shader(const std::filesystem::path& _path);
		
		~Shader();
	
	public:
		
		
		
		static void Bind(const GLint& _id);
		
		static void Unbind();
		
		[[nodiscard]] const std::string& Name() const;
		
		void BindAttribute(const GLint& _pos, const char* _name) const;
		
		GLint AttributeID(const char* _name);
		
		/// <summary> Assign a boolean to the Shader. </summary>
		static void Assign(const GLint& _id, const bool& _value);
		
		/// <summary> Assign an integer to the Shader. </summary>
		static void Assign(const GLint& _id, const GLint& _value);
		
		/// <summary> Assign an unsigned integer to the Shader. </summary>
		static void Assign(const GLint& _id, const GLuint& _value);
		
		/// <summary> Assign a float to the Shader. </summary>
		static void Assign(const GLint& _id, const GLfloat& _value);
		
		/// <summary> Assign a 2-dimensional vector to the Shader. </summary>
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y);
		
		/// <summary> Assign a 3-dimensional vector to the Shader. </summary>
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z);
		
		/// <summary> Assign a 4-dimensional vector to the Shader. </summary>
		static void Assign(const GLint& _id, const GLfloat& _x, const GLfloat& _y, const GLfloat& _z, const GLfloat& _w);
		
		/// <summary> Assign a 2-dimensional vector to the Shader. </summary>
		static void Assign(const GLint& _id, const glm::vec2& _value);
		
		/// <summary> Assign a 3-dimensional vector to the Shader. </summary>
		static void Assign(const GLint& _id, const glm::vec3& _value);
		
		/// <summary> Assign a 4-dimensional vector to the Shader. </summary>
		static void Assign(const GLint& _id, const glm::vec4& _value);
		
		/// <summary> Assign a 2-dimensional matrix to the Shader. </summary>
		static void Assign(const GLint& _id, const glm::mat2& _mat);
		
		/// <summary> Assign a 3-dimensional matrix to the Shader. </summary>
		static void Assign(const GLint& _id, const glm::mat3& _mat);
		
		/// <summary> Assign a 4-dimensional matrix to the Shader. </summary>
		static void Assign(const GLint& _id, const glm::mat4& _mat);
		
		static void Assign(const GLint& _id, const GLuint& _textureID, const GLint& _imageUnit, const GLenum& _target);
		
		/// <summary> Get the ID of the Shader. </summary>
		[[nodiscard]] GLint ID() const;
		
		explicit operator GLint() const { return this->ID(); }
	};
}

#endif //FINALYEARPROJECT_SHADER_H