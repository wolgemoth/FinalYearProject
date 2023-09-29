#include "stdafx.h"

#include "Mesh.h"

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

GLuint Mesh::PositionVBO_ID() { return this->m_Mesh.positionVboId; }
GLuint Mesh::TexCoordVBO_ID() { return this->m_Mesh.texCoordVboId; }
GLuint Mesh::NormalVBO_ID()   { return this->m_Mesh.normalVboId;   }
GLuint Mesh::VAO_ID()         { return this->m_Mesh.vaoId;         }
GLuint Mesh::Texture_ID()     { return this->m_Mesh.textureId;     }
   int Mesh::VertexCount()    { return this->m_Mesh.vertexCount;   }
