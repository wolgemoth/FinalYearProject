#ifndef FINALYEARPROJECT_MODEL_H
#define FINALYEARPROJECT_MODEL_H

#include "../core/Debug.h"

#include <earcut.hpp>

#include <GL/glew.h>
#include <glm/ext/vector_float4.hpp>

#include <array>
#include <cstddef>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

namespace LouiEriksson::Engine {
	
	class File;
	
} // LouiEriksson::Engine

namespace LouiEriksson::Engine::Graphics {
	
	class Mesh {
	
		friend LouiEriksson::Engine::File;
		
	private:
		
		/// <summary> Currently bound VAO. </summary>
		inline static GLuint s_CurrentVAO { GL_NONE };
		
		/// <summary> Currently bound VBOs. </summary>
		inline static Hashmap<GLenum, GLuint> s_CurrentVBOs;
		
		GLenum m_Format, m_IndexFormat;
		
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
		
		~Mesh();
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLubyte>& _indices);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLushort>& _indices);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLuint>& _indices);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLubyte>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents = true, const GLenum& _format = GL_TRIANGLES);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLushort>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents = true, const GLenum& _format = GL_TRIANGLES);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLuint>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents = true, const GLenum& _format = GL_TRIANGLES);
		
		struct Earcut final {
		
			template <typename N>
			static std::vector<N> Triangulate(const std::vector<glm::vec3>& _polyline) {
				
				static_assert(std::is_integral_v<N>, "Type must be an integer type!");
				
				Debug::Assert(_polyline.size() <= std::numeric_limits<N>::max(), "Vertex count exceeds the type's limit!", LogType::Error);
				
				// TODO: Make this better.
				
				using Point = std::array<float, 2>;
				std::vector<std::vector<Point>> polygon;
				
				std::vector<Point> polyline;
				for (auto& item : _polyline) {
					polyline.push_back({item.x, item.y});
				}
				polygon.push_back(polyline);
				
				return mapbox::earcut<N>(polygon);
			}
			
			template <typename N>
			static std::vector<N> Triangulate(const std::vector<std::vector<glm::vec3>>& _polygon) {
				
				static_assert(std::is_integral_v<N>, "Type must be an integer type!");
				
				Debug::Assert(_polygon.size() <= std::numeric_limits<N>::max(), "Vertex count exceeds the type's limit!", LogType::Error);
				
				// TODO: Make this better.
				
				using Point = std::array<float, 2>;
				std::vector<std::vector<Point>> polygon;
				
				for (auto& item1 : _polygon) {
					
					std::vector<Point> polyline;
					for (auto& item2 : item1) {
						polyline.push_back({item2.x, item2.y});
					}
					
					polygon.push_back(polyline);
				}
				
				return mapbox::earcut<N>(polygon);
			}
		};
		
		/// <summary> Container for various primitive mesh types. </summary>
		struct Primitives final {
			
			struct Sphere final {
			
				static std::weak_ptr<Mesh> Instance();
				
			};
			
			struct Cube final {
			
				static std::weak_ptr<Mesh> Instance();
				
			};
			
			/// <summary> Information for a quad primitive. </summary>
			struct Quad final {
	
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
			
			struct Grid final {
			
				static std::shared_ptr<Mesh> Create(const glm::ivec2& _resolution);
			};
			
			struct PointCloud final {
				
				static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices);
		
			};
		};
		
		/// <summary> Bind the provided mesh. </summary>
		static void Bind(const Mesh& _mesh);
		
		/// <summary> Unbind the currently bound mesh. </summary>
		static void Unbind();
		
		static void BindVBO(const GLenum& _type, const GLuint& _vbo);
		
		static void UnbindVBO(const GLenum& _type);
		
		[[nodiscard]] const GLenum&      Format() const noexcept;
		[[nodiscard]] const GLenum& IndexFormat() const noexcept;
		
		[[nodiscard]] const GLuint&          VAO_ID() const noexcept;
		[[nodiscard]] const GLuint&  PositionVBO_ID() const noexcept;
		[[nodiscard]] const GLuint&  TexCoordVBO_ID() const noexcept;
		[[nodiscard]] const GLuint&     IndexVBO_ID() const noexcept;
		[[nodiscard]] const GLuint&    NormalVBO_ID() const noexcept;
		[[nodiscard]] const GLuint&   TangentVBO_ID() const noexcept;
		[[nodiscard]] const GLuint& BitangentVBO_ID() const noexcept;
		
		[[nodiscard]] const size_t& VertexCount() const noexcept;
		[[nodiscard]] const size_t&  IndexCount() const noexcept;
		
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_MODEL_H