#include "Mesh.h"

#include "../core/utils/Utils.h"

#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>

#include <memory>
#include <vector>

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
			  
	std::shared_ptr<Mesh> Mesh::Create(const std::vector<glm::vec3>& _vertices) {
		
		auto result = std::make_shared<Mesh>(Mesh(GL_POINTS));
		
		result->m_VertexCount = _vertices.size();
		
		// Buffers to store mesh data:
		glGenVertexArrays(1, &result->m_VAO_ID);
		
		glGenBuffers(1, &result->m_PositionVBO_ID);
		
		// Positions:
		Mesh::Bind(*result);
		
		glBindBuffer(GL_ARRAY_BUFFER, result->m_PositionVBO_ID);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * result->m_VertexCount * 3), _vertices.data(), GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		
		return result;
	}
    
	std::shared_ptr<Mesh> Mesh::Create(const std::vector<glm::vec3>& _vertices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents, const GLenum& _format) {
		
		auto result = std::make_shared<Mesh>(Mesh(_format));
		
		result->m_VertexCount = _vertices.size();
		
		if (result->m_VertexCount > 0) {
			
			glGenVertexArrays(1, &result->m_VAO_ID);
			
			Mesh::Bind(*result);
			
			// Create a generic 'buffer'
			glGenBuffers(1, &result->m_PositionVBO_ID);
			// Tell OpenGL that we want to activate the buffer and that it's a VBO
			glBindBuffer(GL_ARRAY_BUFFER, result->m_PositionVBO_ID);
			// With this buffer active, we can now send our data to OpenGL
			// We need to tell it how much data to send
			// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
			glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * result->m_VertexCount * 3), _vertices.data(), GL_STATIC_DRAW);
			
			// This tells OpenGL how we link the vertex data to the shader
			// (We will look at this properly in the lectures)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			glEnableVertexAttribArray(0);
			
			if (!_normals.empty()) {
				
				// Create a generic 'buffer'
				glGenBuffers(1, &result->m_NormalVBO_ID);
				// Tell OpenGL that we want to activate the buffer and that it's a VBO
				glBindBuffer(GL_ARRAY_BUFFER, result->m_NormalVBO_ID);
				// With this buffer active, we can now send our data to OpenGL
				// We need to tell it how much data to send
				// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
				glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * result->m_VertexCount * 3), _normals.data(), GL_STATIC_DRAW);
				
				// This tells OpenGL how we link the vertex data to the shader
				// (We will look at this properly in the lectures)
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				glEnableVertexAttribArray(1);
			}
			
			if (!_UVs.empty()) {
				
				// Create a generic 'buffer'
				glGenBuffers(1, &result->m_TexCoordVBO_ID);
				// Tell OpenGL that we want to activate the buffer and that it's a VBO
				glBindBuffer(GL_ARRAY_BUFFER, result->m_TexCoordVBO_ID);
				// With this buffer active, we can now send our data to OpenGL
				// We need to tell it how much data to send
				// We can also tell OpenGL how we intend to use this buffer - here we say GL_STATIC_DRAW because we're only writing it once
				glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * result->m_VertexCount * 2), _UVs.data(), GL_STATIC_DRAW);
				
				// This tells OpenGL how we link the vertex data to the shader
				// (We will look at this properly in the lectures)
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
				glEnableVertexAttribArray(2);
			}
			
			// If the mesh has both texture coordinates and _normals, compute the tangents and bitangents.
			if (_generateTangents) {
				
				const auto tb = GenerateTangents(_vertices, _normals, _UVs);
				auto& t = tb[0];
				auto& b = tb[1];
				
				if (!t.empty() && !b.empty()) {
				
					// TODO: Perform tangent averaging / smoothing.
					
				    glGenBuffers(1, &result->m_TangentVBO_ID);
				    glBindBuffer(GL_ARRAY_BUFFER, result->m_TangentVBO_ID);
				    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(t.size() * sizeof(t[0])), t.data(), GL_STATIC_DRAW);
				
					glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
					glEnableVertexAttribArray(3);
					
				    glGenBuffers(1, &result->m_BitangentVBO_ID);
				    glBindBuffer(GL_ARRAY_BUFFER, result->m_BitangentVBO_ID);
				    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(b.size() * sizeof(b[0])), b.data(), GL_STATIC_DRAW);
					
					glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
					glEnableVertexAttribArray(4);
				}
			}
			
            glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		return result;
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
		
		if (s_CurrentVAO != _mesh.VAO_ID()) {
			glBindVertexArray(s_CurrentVAO = _mesh.VAO_ID());
		}
	}
	
	void Mesh::Unbind() {
		
		if (s_CurrentVAO != GL_NONE) {
			glBindVertexArray(s_CurrentVAO = GL_NONE);
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
	
	std::array<std::vector<glm::vec3>, 2> Mesh::GenerateTangents(const std::vector<glm::vec3>& _vertices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs) {
		
		auto result = std::array<std::vector<glm::vec3>, 2>();
		
		// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
		
		auto&   tangents = result[0];
		auto& bitangents = result[1];
		
		for (auto i = 0; i < _vertices.size(); i += 3) {
		
			// Shortcuts for _vertices
	        const auto& pos0 = _vertices[  i  ];
	        const auto& pos1 = _vertices[i + 1];
	        const auto& pos2 = _vertices[i + 2];
	
	        // Shortcuts for _UVs
	        const auto& uv0 = _UVs[  i  ];
	        const auto& uv1 = _UVs[i + 1];
	        const auto& uv2 = _UVs[i + 2];
			
			const auto delta_pos1 = pos1 - pos0;
		    const auto delta_pos2 = pos2 - pos0;
			
		    const auto delta_texcoord1 = uv1 - uv0;
		    const auto delta_texcoord2 = uv2 - uv0;
		
			const auto f = 1.0f / (delta_texcoord1.x * delta_texcoord2.y - delta_texcoord2.x * delta_texcoord1.y);
		    const auto   tangent = f * (delta_pos1 * delta_texcoord2.y - delta_pos2 * delta_texcoord1.y);
		    const auto bitangent = f * (delta_pos2 * delta_texcoord1.x - delta_pos1 * delta_texcoord2.x);
			
			// Set the same tangent for all three _vertices of the triangle.
	        // They will be merged later, in vboindexer.cpp
	        tangents.emplace_back(tangent);
	        tangents.emplace_back(tangent);
	        tangents.emplace_back(tangent);
	
	        // Same thing for bitangents
	        bitangents.emplace_back(bitangent);
	        bitangents.emplace_back(bitangent);
	        bitangents.emplace_back(bitangent);
		}
		
		return result;
	}
	
	std::weak_ptr<Mesh> Mesh::Primitives::Quad::Instance() {
		
		if (s_Instance == nullptr) {
			s_Instance = std::make_shared<Mesh>(Mesh(GL_TRIANGLES));
			
			s_Instance->m_VertexCount = Mesh::Primitives::Quad::s_VertexCount;
			
			// Buffers to store mesh data:
			glGenVertexArrays(1, &s_Instance->m_VAO_ID);
			glGenBuffers(1, &s_Instance->m_PositionVBO_ID);
			glBindVertexArray(Mesh::s_CurrentVAO = s_Instance->m_VAO_ID);
			glBindBuffer(GL_ARRAY_BUFFER, s_Instance->m_PositionVBO_ID);
			glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(s_Instance->m_VertexCount), &Mesh::Primitives::Quad::s_VertexData, GL_STATIC_DRAW);
			
			// Positions, triangles (encoded within winding order):
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
			
			// Texture coordinates:
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (int*)(2 * sizeof(GLfloat)));
		}
		
		return s_Instance;
	}
	
	std::shared_ptr<Mesh> Mesh::Primitives::Grid::Create(const glm::ivec2& _resolution) {
	
		const auto& nX = _resolution.x;
		const auto& nY = _resolution.y;
		
	    /* VERTICES */
	    std::vector<glm::vec3> vertices(static_cast<std::size_t>((nX + 1) * (nY + 1)));
		
	    for (auto i = 0; i <= nX; i++) {
        for (auto j = 0; j <= nY; j++) {
			
	        vertices[i * (nY + 1) + j] = {
				static_cast<float>(i) / static_cast<float>(nX),
				static_cast<float>(j) / static_cast<float>(nY),
				0
			};
        }}
		
	    /* INDICES */
	    std::vector<int> indices(static_cast<std::size_t>(6 * nX * nY));
		
	    for (auto i = 0; i < nX; i++) {
        for (auto j = 0; j < nY; j++) {
			
			const auto v1 = i * (nY + 1) + j;
            const auto v2 = v1 + 1;
            const auto v3 = (i + 1) * (nY + 1) + j;
            const auto v4 = v3 + 1;

			auto idx = Utils::To1D({i, j}, nY);
			
            const auto t1 =  (2 * idx)      * 3;
            const auto t2 = ((2 * idx) + 1) * 3;
			
			indices[t1    ] = v1;
            indices[t1 + 1] = v2;
            indices[t1 + 2] = v3;
            indices[t2    ] = v2;
            indices[t2 + 1] = v4;
            indices[t2 + 2] = v3;
        }}
		
		throw std::runtime_error("NOT IMPLEMENTED");
		//return Create(vertices, indices, {}, false, GL_TRIANGLES);
	}
} // LouiEriksson::Engine::Graphics