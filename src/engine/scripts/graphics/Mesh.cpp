#include "Mesh.h"

#include "../core/utils/Utils.h"
#include "../core/Debug.h"
#include "../core/Resources.h"

#include "TextureCPU.h"

#include <GL/glew.h>
#include <glm/geometric.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int2.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <memory>
#include <vector>

namespace LouiEriksson::Engine::Graphics {
	
	Mesh::Mesh(const GLenum& _format) noexcept :
		m_Format         (_format),
		m_IndexFormat    (GL_NONE),
		m_VAO_ID         (GL_NONE),
		m_IBO_ID         (GL_NONE),
		m_PositionVBO_ID (GL_NONE),
		m_TexCoordVBO_ID (GL_NONE),
		m_NormalVBO_ID   (GL_NONE),
		m_TangentVBO_ID  (GL_NONE),
		m_BitangentVBO_ID(GL_NONE),
		m_VertexCount(0),
		m_IndexCount (0){}
			  
	Mesh::~Mesh() {
		
		if (         m_VAO_ID != GL_NONE) { glDeleteVertexArrays(1,          &m_VAO_ID); }
		if ( m_PositionVBO_ID != GL_NONE) { glDeleteBuffers     (1,  &m_PositionVBO_ID); }
		if ( m_TexCoordVBO_ID != GL_NONE) { glDeleteBuffers     (1,  &m_TexCoordVBO_ID); }
		if (    m_IBO_ID != GL_NONE) { glDeleteBuffers     (1,     &m_IBO_ID); }
		if (   m_NormalVBO_ID != GL_NONE) { glDeleteBuffers     (1,    &m_NormalVBO_ID); }
		if (  m_TangentVBO_ID != GL_NONE) { glDeleteBuffers     (1,   &m_TangentVBO_ID); }
		if (m_BitangentVBO_ID != GL_NONE) { glDeleteBuffers     (1, &m_BitangentVBO_ID); }
	}
	
	std::weak_ptr<Mesh> Mesh::Primitives::Sphere::Instance() {
		return Resources::Get<Mesh>("sphere");
	}
	
	std::weak_ptr<Mesh> Mesh::Primitives::Cube::Instance() {
		return Resources::Get<Mesh>("cube");
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
	
	void Mesh::BindVBO(const GLenum& _type, const GLuint& _vbo) {
		glBindBuffer(_type, _vbo);
	}
	
	const GLenum& Mesh::     Format() const noexcept { return m_Format;      }
	const GLenum& Mesh::IndexFormat() const noexcept { return m_IndexFormat; }
	
	const GLuint& Mesh::VAO_ID()           const noexcept { return          m_VAO_ID; }
	const GLuint& Mesh::PositionVBO_ID()   const noexcept { return  m_PositionVBO_ID; }
	const GLuint& Mesh::TexCoordVBO_ID()   const noexcept { return  m_TexCoordVBO_ID; }
	const GLuint& Mesh::IndexVBO_ID()      const noexcept { return     m_IBO_ID; }
	const GLuint& Mesh::NormalVBO_ID()     const noexcept { return    m_NormalVBO_ID; }
	const GLuint& Mesh::TangentVBO_ID()    const noexcept { return   m_TangentVBO_ID; }
	const GLuint& Mesh::BitangentVBO_ID()  const noexcept { return m_BitangentVBO_ID; }
	
	const size_t& Mesh::VertexCount() const noexcept { return m_VertexCount; }
	const size_t& Mesh:: IndexCount() const noexcept { return  m_IndexCount; }
	
} // LouiEriksson::Engine::Graphics