#include "stdafx.h"

#include "Mesh.h"

#include <utility>
#include "Wavefront.h"

namespace LouiEriksson {
	
	Mesh::Mesh() {
		
		m_PositionVBO_ID = -1;
		m_TexCoordVBO_ID = -1;
		  m_NormalVBO_ID = -1;
		        m_VAO_ID = -1;
		    m_Texture_ID = -1;
			
		   m_VertexCount = -1;
	}
	
	Mesh::~Mesh() {
		
		if (m_PositionVBO_ID > 0) { glDeleteBuffers (1, &m_PositionVBO_ID); }
		if (m_TexCoordVBO_ID > 0) { glDeleteBuffers (1, &m_TexCoordVBO_ID); }
		if (  m_NormalVBO_ID > 0) { glDeleteBuffers (1, &m_NormalVBO_ID  ); }
		if (    m_Texture_ID > 0) { glDeleteTextures(1, &m_Texture_ID    ); }
	}
	
	[[maybe_unused]]
	std::shared_ptr<Mesh> Mesh::Load(const std::string& _path) {
		return Wavefront::LoadMesh(_path);
	}
	
	[[maybe_unused]] GLuint Mesh::PositionVBO_ID() const { return this->m_PositionVBO_ID; }
	[[maybe_unused]] GLuint Mesh::TexCoordVBO_ID() const { return this->m_TexCoordVBO_ID; }
	[[maybe_unused]] GLuint Mesh::NormalVBO_ID()   const { return this->m_NormalVBO_ID;   }
	[[maybe_unused]] GLuint Mesh::VAO_ID()         const { return this->m_VAO_ID;         }
	[[maybe_unused]] GLuint Mesh::Texture_ID()     const { return this->m_Texture_ID;     }
	
	[[maybe_unused]] unsigned long Mesh::VertexCount() const { return this->m_VertexCount; }
}