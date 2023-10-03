#include "stdafx.h"

#include "Mesh.h"

namespace LouiEriksson {
		
	Mesh::Mesh() {
	
		// Initialise the mesh's memory.
		m_Mesh = { 0 };
	}
	
	Mesh::~Mesh() {
		WfModelDestroy(&m_Mesh);
	}
	
	std::shared_ptr<Mesh> Mesh::Load(const char* _path) {
	
		Mesh* result = new Mesh();
		int err_code = WfModelLoad(_path, &(result->m_Mesh));
	
		/* ERROR HANDLING */
		if (err_code != 0) {
	
			std::stringstream err;
			err << "ERROR (Mesh.cpp [Load(const char*, Mesh&)]): Failed to load model \"" <<
				_path << "\". Error code: " << err.str();
	
			throw std::runtime_error(err.str());
		}
	
		return std::shared_ptr<Mesh>(result);
	}
	
	[[maybe_unused]] GLuint Mesh::PositionVBO_ID() const { return this->m_Mesh.positionVboId; }
	[[maybe_unused]] GLuint Mesh::TexCoordVBO_ID() const { return this->m_Mesh.texCoordVboId; }
	[[maybe_unused]] GLuint Mesh::NormalVBO_ID()   const { return this->m_Mesh.normalVboId;   }
	[[maybe_unused]] GLuint Mesh::VAO_ID()         const { return this->m_Mesh.vaoId;         }
	[[maybe_unused]] GLuint Mesh::Texture_ID()     const { return this->m_Mesh.textureId;     }
	[[maybe_unused]]    int Mesh::VertexCount()    const { return this->m_Mesh.vertexCount;   }
}