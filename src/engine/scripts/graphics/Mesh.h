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

	class TextureCPU;
	
	class Mesh {
	
		friend LouiEriksson::Engine::File;
		
	private:
		
		/** @brief Currently bound VAO. */
		inline static GLuint s_CurrentVAO { GL_NONE };
		
		/** @brief Currently bound VBOs. */
		inline static Hashmap<GLenum, GLuint> s_CurrentVBOs;
		
		GLenum      m_Format,
		       m_IndexFormat;
		
		GLuint          m_VAO_ID,
		        m_PositionVBO_ID,
		        m_TexCoordVBO_ID,
		           m_IndexVBO_ID,
		          m_NormalVBO_ID,
		         m_TangentVBO_ID,
		       m_BitangentVBO_ID;
		
		size_t m_VertexCount,
		        m_IndexCount;
		
		explicit Mesh(const GLenum& _format) noexcept;
		
	public:
		
		~Mesh();
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLubyte>& _indices);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLushort>& _indices);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLuint>& _indices);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLubyte>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents = true, const GLenum& _format = GL_TRIANGLES);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLushort>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents = true, const GLenum& _format = GL_TRIANGLES);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLuint>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, const bool& _generateTangents = true, const GLenum& _format = GL_TRIANGLES);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLubyte>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, std::array<std::vector<glm::vec3>, 2> _tangents, const GLenum& _format = GL_TRIANGLES);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLushort>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, std::array<std::vector<glm::vec3>, 2> _tangents, const GLenum& _format = GL_TRIANGLES);
		
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices, const std::vector<GLuint>& _indices, const std::vector<glm::vec3>& _normals, const std::vector<glm::vec2>& _UVs, std::array<std::vector<glm::vec3>, 2> _tangents, const GLenum& _format = GL_TRIANGLES);
		
		static std::array<std::vector<glm::vec3>, 2> GenerateTangents(const std::vector<glm::vec3>& _vertices, const std::vector<glm::vec2>& _UVs);

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
		
		/**
		* @brief Container for various primitive mesh types.
		*/
		struct Primitives final {
			
			/**
			* @brief Represents a sphere primitive.
			*/
			struct Sphere final {
			
				static std::weak_ptr<Mesh> Instance();
				
			};
			
			/**
			* @brief Represents a cube primitive.
			*/
			struct Cube final {
			
				static std::weak_ptr<Mesh> Instance();
				
			};
			
			/**
			* @brief Represents a quad primitive.
			*/
			struct Quad final {
	
			private:
				
				/** @brief Every screen and texture coordinate for every vertex in the mesh. */
				static constexpr glm::vec4 s_VertexData[] {
					glm::vec4(-1.0f, -1.0f, 0.0f, 0.0f),
					glm::vec4( 1.0f, -1.0f, 1.0f, 0.0f),
					glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
					glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
					glm::vec4( 1.0f, -1.0f, 1.0f, 0.0f),
					glm::vec4( 1.0f,  1.0f, 1.0f, 1.0f),
				};
		
				/** @brief Number of vertices in the mesh. */
				static constexpr auto s_VertexCount = sizeof(s_VertexData);
				
				/** @brief Static instance. */
				inline static std::shared_ptr<Mesh> s_Instance;
				
			public:
				
				/** @brief Returns a pointer to the static instance of the mesh. */
				static std::weak_ptr<Mesh> Instance();
				
			};
			
			struct Grid final {
			
				static std::shared_ptr<Mesh> Create(const glm::ivec2& _resolution);
				
				static std::shared_ptr<Mesh> Create(const glm::ivec2& _resolution, const Graphics::TextureCPU& _heights);
			};
			
			struct PointCloud final {
				
				static std::shared_ptr<Mesh> Create(const std::vector<glm::vec3>& _vertices);
		
			};
		};
		
		/**
		 * @brief Binds the provided mesh.
		 *
		 * This function binds the vertex array object (VAO) of the given mesh, if it is not already bound.
		 *
		 * @param[in] _mesh The mesh to bind.
		 */
		static void Bind(const Mesh& _mesh);
		
		/**
		 * @brief Unbinds the currently bound mesh.
		 *
		 * This static function unbinds the currently bound mesh by setting the current VAO to GL_NONE.
		 *
		 * @sa Bind(const Mesh &_mesh)
		 */
		static void Unbind();
		
		/**
		 * @brief Binds the specified VBO.
		 *
		 * This static function binds the given VBO of the specified type, if it is not already bound.
		 *
		 * @param[in] _type The type of the VBO.
		 * @param[in] _vbo The ID of the VBO to bind.
		 */
		static void BindVBO(const GLenum& _type, const GLuint& _vbo);
		
		/**
		 * @brief Unbinds the specified VBO.
		 *
		 * This static function unbinds the specified VBO of the given type by setting the current VBO ID for that type to GL_NONE.
		 *
		 * @param[in] _type The type of the VBO to unbind.
		 */
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