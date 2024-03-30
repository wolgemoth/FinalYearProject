#include "Mesh.h"

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
			  
    
	std::shared_ptr<Mesh> Mesh::Create(const std::vector<glm::vec3>& _vertices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents, const GLenum& _format) {
		
		auto result = std::shared_ptr<Graphics::Mesh>(new Mesh(_format));
		
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
			if (_generateTangents && !_normals.empty() && !_UVs.empty()) {
			
				// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
				
				std::vector<glm::vec3>   tangents;
				std::vector<glm::vec3> bitangents;
				
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
				
				// TODO: Perform tangent averaging / smoothing.
				
			    glGenBuffers(1, &result->m_TangentVBO_ID);
			    glBindBuffer(GL_ARRAY_BUFFER, result->m_TangentVBO_ID);
			    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(tangents.size() * sizeof(tangents[0])), tangents.data(), GL_STATIC_DRAW);
			
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				glEnableVertexAttribArray(3);
				
			    glGenBuffers(1, &result->m_BitangentVBO_ID);
			    glBindBuffer(GL_ARRAY_BUFFER, result->m_BitangentVBO_ID);
			    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(bitangents.size() * sizeof(bitangents[0])), bitangents.data(), GL_STATIC_DRAW);
				
				glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				glEnableVertexAttribArray(4);
			}
			
			Mesh::Unbind();
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
	
	
	std::shared_ptr<Mesh> Mesh::Primitives::Points::Create(std::vector<glm::vec3> _points) {
		
		auto result = std::make_shared<Mesh>(Mesh(GL_POINTS));
		
		result->m_VertexCount = _points.size();
		
		// Buffers to store mesh data:
		glGenVertexArrays(1, &result->m_VAO_ID);
		
		glGenBuffers(1, &result->m_PositionVBO_ID);
		
		// Positions:
		glBindVertexArray(Mesh::s_CurrentVAO = result->m_VAO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, result->m_PositionVBO_ID);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(float) * result->m_VertexCount * 3), _points.data(), GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		
		return result;
	}
	
} // LouiEriksson::Engine::Graphics