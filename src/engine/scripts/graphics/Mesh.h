#ifndef FINALYEARPROJECT_MODEL_H
#define FINALYEARPROJECT_MODEL_H

// @Assessor: This class was submitted for GACP. Please don't mark it for 3DGP.

#include <GL/glew.h>
#include <glm/ext/vector_float4.hpp>

#include <cstddef>
#include <memory>

namespace LouiEriksson {
	
	class Mesh {
	
		friend class File;
		
	private:
		
		/// <summary> Currently bound mesh. </summary>
		inline static GLuint m_CurrentVAO { GL_NONE };
		
		GLuint m_VAO_ID;
		
		GLuint  m_PositionVBO_ID;
		GLuint  m_TexCoordVBO_ID;
		GLuint    m_NormalVBO_ID;
		GLuint   m_TangentVBO_ID;
		GLuint m_BitangentVBO_ID;
		
		size_t m_VertexCount;
		
		Mesh();
		
	public:
		
		/// <summary> Container for various primitive mesh types. </summary>
		struct Primitives {
			
			/// <summary> Information for a quad primitive. </summary>
			struct Quad {
	
			private:
				
				// Every screen and texture coordinate for every vertex in the mesh.
				static constexpr glm::vec4 s_VertexData[] {
					glm::vec4(-1.0f, -1.0f, 0.0f, 0.0f),
					glm::vec4( 1.0f, -1.0f, 1.0f, 0.0f),
					glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
					glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
					glm::vec4( 1.0f, -1.0f, 1.0f, 0.0f),
					glm::vec4( 1.0f,  1.0f, 1.0f, 1.0f),
				};
		
				// Number of vertices in the mesh.
				static constexpr unsigned long s_VertexCount = sizeof(s_VertexData);
				
				// Static Quad instance.
				inline static std::shared_ptr<Mesh> s_Instance { nullptr };
				
			public:
				
				/// <summary> Returns a pointer to the static instance of the mesh. </summary>
				static std::weak_ptr<Mesh> Instance();
				
			};
			
		};
		
		~Mesh();
		
		/// <summary> Bind the provided mesh. </summary>
		static void   Bind(const Mesh& _mesh);
		
		/// <summary> Unbind the currently bound mesh. </summary>
		static void Unbind();
		
		[[nodiscard]] GLuint          VAO_ID() const;
		[[nodiscard]] GLuint  PositionVBO_ID() const;
		[[nodiscard]] GLuint  TexCoordVBO_ID() const;
		[[nodiscard]] GLuint    NormalVBO_ID() const;
		[[nodiscard]] GLuint   TangentVBO_ID() const;
		[[nodiscard]] GLuint BitangentVBO_ID() const;
		
		[[nodiscard]] unsigned long VertexCount() const;
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_MODEL_H