#include "stdafx.h"

#include "Mesh.h"

namespace LouiEriksson {
	
	Mesh::Mesh() {
        m_Mesh = { 0 };
	}
	
	Mesh::~Mesh() {
		
		if (m_Mesh.positionVboId > 0) {      glDeleteBuffers(1, &m_Mesh.positionVboId); }
		if (m_Mesh.texCoordVboId > 0) {      glDeleteBuffers(1, &m_Mesh.texCoordVboId); }
		if (m_Mesh.normalVboId   > 0) {      glDeleteBuffers(1, &m_Mesh.normalVboId  ); }
		if (m_Mesh.vaoId         > 0) { glDeleteVertexArrays(1, &m_Mesh.vaoId        ); }
		if (m_Mesh.textureId     > 0) {     glDeleteTextures(1, &m_Mesh.textureId    ); }
	}
	
	std::shared_ptr<Mesh> Mesh::Load(const std::string& _path) {
		
		Mesh* result = new Mesh();
		int code = WfModelLoad(_path.c_str(), &(result->m_Mesh));
	
		/* ERROR HANDLING */
		if (code != 0) {
	
			std::stringstream err;
			err << "ERROR (Mesh.cpp [TryLoad(const char*, Mesh&)]): Failed to load model \"" <<
				_path << "\". Error code: " << code;
			
			throw std::runtime_error(err.str());
		}
	
		return std::shared_ptr<Mesh>(result);
	}
	
	GLuint Mesh::PositionVBO_ID() const { return m_Mesh.positionVboId; }
	GLuint Mesh::TexCoordVBO_ID() const { return m_Mesh.texCoordVboId; }
	GLuint Mesh::NormalVBO_ID()   const { return m_Mesh.normalVboId;   }
	GLuint Mesh::VAO_ID()         const { return m_Mesh.vaoId;         }
	GLuint Mesh::Texture_ID()     const { return m_Mesh.textureId;     }
	
	unsigned long Mesh::VertexCount() const { return m_Mesh.vertexCount; }
}