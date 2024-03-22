#include "Mesh.h"

#include <GL/glew.h>

#include <memory>

namespace LouiEriksson::Engine::Graphics {
	
	Mesh::Mesh(const GLenum& _format) noexcept :
	          m_Format(_format),
	               m_VAO_ID(0u),
	       m_PositionVBO_ID(0u),
	       m_TexCoordVBO_ID(0u),
	         m_NormalVBO_ID(0u),
	        m_TangentVBO_ID(0u),
	      m_BitangentVBO_ID(0u),
	          m_VertexCount(0u) {}
	
	Mesh::~Mesh() {
		
		if (         m_VAO_ID != 0u) { glDeleteVertexArrays(1,          &m_VAO_ID); }
		if ( m_PositionVBO_ID != 0u) { glDeleteBuffers     (1,  &m_PositionVBO_ID); }
		if ( m_TexCoordVBO_ID != 0u) { glDeleteBuffers     (1,  &m_TexCoordVBO_ID); }
		if (   m_NormalVBO_ID != 0u) { glDeleteBuffers     (1,    &m_NormalVBO_ID); }
		if (  m_TangentVBO_ID != 0u) { glDeleteBuffers     (1,   &m_TangentVBO_ID); }
		if (m_BitangentVBO_ID != 0u) { glDeleteBuffers     (1, &m_BitangentVBO_ID); }
	}
	
	void Mesh::Bind(const Mesh& _mesh) {
		
		if (m_CurrentVAO != GL_NONE) {
			glBindVertexArray(m_CurrentVAO = _mesh.VAO_ID());
		}
	}
	
	void Mesh::Unbind() {
		
		if (m_CurrentVAO != GL_NONE) {
			glBindVertexArray(m_CurrentVAO = GL_NONE);
		}
	}
	
	const GLenum& Mesh::Format() const noexcept { return m_Format; }
	
	const GLuint& Mesh::VAO_ID()           const noexcept { return          m_VAO_ID; }
	const GLuint& Mesh::PositionVBO_ID()   const noexcept { return  m_PositionVBO_ID; }
	const GLuint& Mesh::TexCoordVBO_ID()   const noexcept { return  m_TexCoordVBO_ID; }
	const GLuint& Mesh::NormalVBO_ID()     const noexcept { return    m_NormalVBO_ID; }
	const GLuint& Mesh::TangentVBO_ID()    const noexcept { return   m_TangentVBO_ID; }
	const GLuint& Mesh::BitangentVBO_ID()  const noexcept { return m_BitangentVBO_ID; }
	
	const unsigned long& Mesh::VertexCount() const noexcept { return m_VertexCount; }
	
	std::weak_ptr<Mesh> Mesh::Primitives::Quad::Instance() {
		
		if (s_Instance == nullptr) {
			s_Instance = std::make_shared<Mesh>(Mesh(GL_TRIANGLES));
			
			s_Instance->m_VertexCount = Mesh::Primitives::Quad::s_VertexCount;
			
			// Buffers to store mesh data.
			glGenVertexArrays(1, &s_Instance->m_VAO_ID);
			glGenBuffers(1, &s_Instance->m_PositionVBO_ID);
			glBindVertexArray(Mesh::m_CurrentVAO = s_Instance->m_VAO_ID);
			glBindBuffer(GL_ARRAY_BUFFER, s_Instance->m_PositionVBO_ID);
			glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(s_Instance->m_VertexCount), &Mesh::Primitives::Quad::s_VertexData, GL_STATIC_DRAW);
			
			// Positions, triangles (encoded within winding order).
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
			
			// Texture coordinates.
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (int*)(2 * sizeof(GLfloat)));
			
		}
		
		return s_Instance;
	}
	
	
	std::shared_ptr<Mesh> Mesh::Primitives::Points::CreateInstance(std::vector<glm::vec3> _points) {
		
		auto result = std::make_shared<Mesh>(Mesh(GL_POINTS));
		
		result->m_VertexCount = _points.size();
		
		// Buffers to store mesh data.
		glGenVertexArrays(1, &result->m_VAO_ID);
		glGenBuffers(1, &result->m_PositionVBO_ID);
		glBindVertexArray(Mesh::m_CurrentVAO = result->m_VAO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, result->m_PositionVBO_ID);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * result->m_VertexCount * 3), _points.data(), GL_STATIC_DRAW);
		
		// Positions, triangles (encoded within winding order).
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		
		return result;
	}
	
} // LouiEriksson::Engine::Graphics