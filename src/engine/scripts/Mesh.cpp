#include "stdafx.h"

#include "Mesh.h"

// @Assessor: This class was submitted for GACP. Please don't mark it for 3DGP.

namespace LouiEriksson {
	
	Mesh::Mesh() {
		
		         m_VAO_ID = 0u;
		 m_PositionVBO_ID = 0u;
		 m_TexCoordVBO_ID = 0u;
		   m_NormalVBO_ID = 0u;
		  m_TangentVBO_ID = 0u;
		m_BitangentVBO_ID = 0u;
		
		m_VertexCount = 0u;
	}
	
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
	
	GLuint Mesh::VAO_ID()           const { return          m_VAO_ID; }
	GLuint Mesh::PositionVBO_ID()   const { return  m_PositionVBO_ID; }
	GLuint Mesh::TexCoordVBO_ID()   const { return  m_TexCoordVBO_ID; }
	GLuint Mesh::NormalVBO_ID()     const { return    m_NormalVBO_ID; }
	GLuint Mesh::TangentVBO_ID()    const { return   m_TangentVBO_ID; }
	GLuint Mesh::BitangentVBO_ID()  const { return m_BitangentVBO_ID; }
	
	unsigned long Mesh::VertexCount() const { return m_VertexCount; }
	
	std::weak_ptr<Mesh> Mesh::Primitives::Quad::Instance() {
		
		if (s_Instance == nullptr) {
			s_Instance = std::make_shared<Mesh>(Mesh());
			
			s_Instance->m_VertexCount = Mesh::Primitives::Quad::s_VertexCount;
			
			// Buffers to store mesh data.
			glGenVertexArrays(1, &s_Instance->m_VAO_ID);
			glGenBuffers(1, &s_Instance->m_PositionVBO_ID);
			glBindVertexArray(Mesh::m_CurrentVAO = s_Instance->m_VAO_ID);
			glBindBuffer(GL_ARRAY_BUFFER, s_Instance->m_PositionVBO_ID);
			glBufferData(GL_ARRAY_BUFFER, s_Instance->m_VertexCount, &Mesh::Primitives::Quad::s_VertexData, GL_STATIC_DRAW);
			
			// Positions, triangles (encoded within winding order).
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
			
			// Texture coordinates.
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
			
		}
		
		return s_Instance;
	}
}