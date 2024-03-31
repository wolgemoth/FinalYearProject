#ifndef FINALYEARPROJECT_MODEL_H
#define FINALYEARPROJECT_MODEL_H

#include <GL/glew.h>
#include <glm/ext/vector_float4.hpp>

#include <cstddef>
#include <memory>

namespace LouiEriksson::Engine {
	
	class File;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Mesh {
	
		friend LouiEriksson::Engine::File;
		
	private:
		
		/// <summary> Currently bound mesh. </summary>
		inline static auto s_CurrentVAO { GL_NONE };
		
		GLenum m_Format;
		
		GLuint          m_VAO_ID,
		        m_PositionVBO_ID,
		        m_TexCoordVBO_ID,
		           m_IndexVBO_ID,
		          m_NormalVBO_ID,
		         m_TangentVBO_ID,
		       m_BitangentVBO_ID;
		
		size_t m_VertexCount,
		        m_IndexCount;
		
		static std::array<std::vector<glm::vec3>, 2> GenerateTangents(const std::vector<glm::vec3>& _vertices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs);

		explicit Mesh(const GLenum& _format) noexcept;
		
	public:
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLuint>& _indices);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLuint>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents = true, const GLenum& _format = GL_TRIANGLES);
		
		/// <summary> Container for various primitive mesh types. </summary>
		struct Primitives {
			
			/// <summary> Information for a quad primitive. </summary>
			struct Quad {
	
			private:
				
				/// <summary> Every screen and texture coordinate for every vertex in the mesh. </summary>
				static constexpr glm::vec4 s_VertexData[] {
					glm::vec4(-1.0f, -1.0f, 0.0f, 0.0f),
					glm::vec4( 1.0f, -1.0f, 1.0f, 0.0f),
					glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
					glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
					glm::vec4( 1.0f, -1.0f, 1.0f, 0.0f),
					glm::vec4( 1.0f,  1.0f, 1.0f, 1.0f),
				};
		
				/// <summary> Number of vertices in the mesh. </summary>
				static constexpr auto s_VertexCount = sizeof(s_VertexData);
				
				/// <summary> Static Quad instance. </summary>
				inline static std::shared_ptr<Mesh> s_Instance;
				
			public:
				
				/// <summary> Returns a pointer to the static instance of the mesh. </summary>
				static std::weak_ptr<Mesh> Instance();
				
			};
			
			struct Grid {
			
				static std::shared_ptr<Mesh> Create(const glm::ivec2& _resolution);
			};
			
		};
		
		~Mesh();
		
		/// <summary> Bind the provided mesh. </summary>
		static void Bind(const Mesh& _mesh);
		
		/// <summary> Unbind the currently bound mesh. </summary>
		static void Unbind();
		
		[[nodiscard]] const GLenum& Format() const noexcept;
		
		[[nodiscard]] const GLuint&          VAO_ID() const noexcept;
		[[nodiscard]] const GLuint&  PositionVBO_ID() const noexcept;
		[[nodiscard]] const GLuint&  TexCoordVBO_ID() const noexcept;
		[[nodiscard]] const GLuint&     IndexVBO_ID() const noexcept;
		[[nodiscard]] const GLuint&    NormalVBO_ID() const noexcept;
		[[nodiscard]] const GLuint&   TangentVBO_ID() const noexcept;
		[[nodiscard]] const GLuint& BitangentVBO_ID() const noexcept;
		
		[[nodiscard]] const std::size_t& VertexCount() const noexcept;
		[[nodiscard]] const std::size_t&  IndexCount() const noexcept;
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_MODEL_H