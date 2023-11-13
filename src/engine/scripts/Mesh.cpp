#include "stdafx.h"

#include "Mesh.h"

namespace LouiEriksson {
	
	Mesh::Mesh() {
		
		m_VAO_ID         = 0u;
		m_PositionVBO_ID = 0u;
		m_TexCoordVBO_ID = 0u;
		m_NormalVBO_ID   = 0u;
		m_TangentVBO_ID  = 0u;
		m_Texture_ID     = 0u;
	}
	
	Mesh::~Mesh() {
		
		if (        m_VAO_ID != 0u) { glDeleteVertexArrays(1,         &m_VAO_ID); }
		if (m_PositionVBO_ID != 0u) { glDeleteBuffers     (1, &m_PositionVBO_ID); }
		if (m_TexCoordVBO_ID != 0u) { glDeleteBuffers     (1, &m_TexCoordVBO_ID); }
		if (  m_NormalVBO_ID != 0u) { glDeleteBuffers     (1,   &m_NormalVBO_ID); }
		if ( m_TangentVBO_ID != 0u) { glDeleteBuffers     (1,  &m_TangentVBO_ID); }
		if (    m_Texture_ID != 0u) { glDeleteTextures    (1,     &m_Texture_ID); }
	}
	
	GLuint Mesh::VAO_ID()         const { return         m_VAO_ID; }
	GLuint Mesh::PositionVBO_ID() const { return m_PositionVBO_ID; }
	GLuint Mesh::TexCoordVBO_ID() const { return m_TexCoordVBO_ID; }
	GLuint Mesh::NormalVBO_ID()   const { return   m_NormalVBO_ID; }
	GLuint Mesh::TangentVBO_ID()  const { return  m_TangentVBO_ID; }
	GLuint Mesh::Texture_ID()     const { return     m_Texture_ID; }
	
	unsigned long Mesh::VertexCount() const { return m_VertexCount; }
}