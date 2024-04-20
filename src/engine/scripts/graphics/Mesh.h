#ifndef FINALYEARPROJECT_MODEL_H
#define FINALYEARPROJECT_MODEL_H

#include "../core/Debug.h"
#include "../core/utils/Utils.h"

#include "TextureCPU.h"

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

	class Mesh final {
	
		friend LouiEriksson::Engine::File;
		
	private:
		
		/** @brief Currently bound VAO. */
		inline static GLuint s_CurrentVAO { GL_NONE };
		
		GLenum      m_Format,
		       m_IndexFormat;
		
		GLuint          m_VAO_ID,
		                m_IBO_ID,
		        m_PositionVBO_ID,
		        m_TexCoordVBO_ID,
		          m_NormalVBO_ID,
		         m_TangentVBO_ID,
		       m_BitangentVBO_ID,
			       m_VertexCount,
		            m_IndexCount;
		
		explicit constexpr Mesh(const GLenum& _format) noexcept  :
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
			m_IndexCount (0) {}
		
	public:
		
		~Mesh() {
			if (         m_VAO_ID != GL_NONE) { glDeleteVertexArrays(1,          &m_VAO_ID); }
			if ( m_PositionVBO_ID != GL_NONE) { glDeleteBuffers     (1,  &m_PositionVBO_ID); }
			if ( m_TexCoordVBO_ID != GL_NONE) { glDeleteBuffers     (1,  &m_TexCoordVBO_ID); }
			if (         m_IBO_ID != GL_NONE) { glDeleteBuffers     (1,          &m_IBO_ID); }
			if (   m_NormalVBO_ID != GL_NONE) { glDeleteBuffers     (1,    &m_NormalVBO_ID); }
			if (  m_TangentVBO_ID != GL_NONE) { glDeleteBuffers     (1,   &m_TangentVBO_ID); }
			if (m_BitangentVBO_ID != GL_NONE) { glDeleteBuffers     (1, &m_BitangentVBO_ID); }
		}
		
		template<typename T>
		static constexpr void validate_index_format() {
			static_assert(is_gl_integer<T>(), "Invalid index format T");
		}
		
		template<typename T>
		static constexpr bool is_gl_integer() noexcept {
		    return std::is_same_v<T, GLbyte   > ||
		           std::is_same_v<T, GLshort  > ||
		           std::is_same_v<T, GLint    > ||
		           std::is_same_v<T, GLfixed  > || // (ES)
		           std::is_same_v<T, GLclampx > || // (ES)
		           std::is_same_v<T, GLint64  > ||
		           std::is_same_v<T, GLuint64 > ||
		           std::is_same_v<T, GLubyte  > ||
		           std::is_same_v<T, GLushort > ||
		           std::is_same_v<T, GLuint   >;
		}
		
		template <typename T>
		static constexpr GLenum GetGLType() {
			
		    GLenum result = GL_INVALID_ENUM;
		
		    if      constexpr (std::is_same_v<T, GLfloat>)    { result = GL_FLOAT;            }
		    else if constexpr (std::is_same_v<T, GLdouble>)   { result = GL_DOUBLE;           }
		    else if constexpr (std::is_same_v<T, GLbyte>)     { result = GL_BYTE;             }
		    else if constexpr (std::is_same_v<T, GLubyte>)    { result = GL_UNSIGNED_BYTE;    }
		    else if constexpr (std::is_same_v<T, GLshort>)    { result = GL_SHORT;            }
		    else if constexpr (std::is_same_v<T, GLushort>)   { result = GL_UNSIGNED_SHORT;   }
		    else if constexpr (std::is_same_v<T, GLint>)      { result = GL_INT;              }
		    else if constexpr (std::is_same_v<T, GLuint>)     { result = GL_UNSIGNED_INT;     }
		    else if constexpr (std::is_same_v<T, GLboolean >) { result = GL_BOOL;             }
		    else if constexpr (std::is_same_v<T, GLhalf>)     { result = GL_HALF_FLOAT;       }
		    else if constexpr (std::is_same_v<T, GLint64>)    { result = GL_INT64_NV;         }
		    else if constexpr (std::is_same_v<T, GLuint64>)   { result = GL_UNSIGNED_INT64_NV;}
		    else {
				static_assert([]{ return false; }(), "Type not supported by GetGLType");
			}
		
		    return result;
		}
		
		template<typename T, typename U, glm::precision P = glm::defaultp>
		static std::shared_ptr<Mesh> Create(const std::vector<glm::vec<3, T, P>>& _vertices, const std::vector<U>& _indices, const std::vector<glm::vec<3, T, P>>& _normals, const std::vector<glm::vec<2, T, P>>& _UVs, std::array<std::vector<glm::vec<3, T, P>>, 2> _tangents, const GLenum& _format = GL_TRIANGLES) {
			
			validate_index_format<U>();
			
			auto result = std::make_shared<Mesh>(Mesh(_format));
			
			try {
				
				if (!_vertices.empty()) {
					
					if (!_indices.empty()) {
						
						result->m_IndexFormat = GetGLType<U>();
						
						result->m_VertexCount = _vertices.size();
						result-> m_IndexCount =  _indices.size();
						
						glGenVertexArrays(1, &result->m_VAO_ID);
						
						Bind(*result);
						
						glGenBuffers(1, &result->m_PositionVBO_ID);
						
						BindVBO(GL_ARRAY_BUFFER, result->m_PositionVBO_ID);
						glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(_vertices.size() * sizeof(_vertices[0])), _vertices.data(), GL_STATIC_DRAW);
						
						glGenBuffers(1, &result->m_IBO_ID);
						
						BindVBO(GL_ELEMENT_ARRAY_BUFFER, result->m_IBO_ID);
		                glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(_indices.size() * sizeof(_indices[0])), _indices.data(), GL_STATIC_DRAW);
						
						glVertexAttribPointer(0, 3, GetGLType<T>(), GL_FALSE, 0, nullptr);
						glEnableVertexAttribArray(0);
						
						if (!_normals.empty()) {
							
							glGenBuffers(1, &result->m_NormalVBO_ID);
			
							BindVBO(GL_ARRAY_BUFFER, result->m_NormalVBO_ID);
							glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(_normals.size() * sizeof(_normals[0])), _normals.data(), GL_STATIC_DRAW);
							
							glVertexAttribPointer(1, 3, GetGLType<T>(), GL_FALSE, 0, nullptr);
							glEnableVertexAttribArray(1);
						}
						
						if (!_UVs.empty()) {
							
							glGenBuffers(1, &result->m_TexCoordVBO_ID);
							
							BindVBO(GL_ARRAY_BUFFER, result->m_TexCoordVBO_ID);
							glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(_UVs.size() * sizeof(_UVs[0])), _UVs.data(), GL_STATIC_DRAW);
							
							glVertexAttribPointer(2, 2, GetGLType<T>(), GL_FALSE, 0, nullptr);
							glEnableVertexAttribArray(2);
							
							{
								const auto& t = _tangents[0];
								const auto& b = _tangents[1];
								
								if (!t.empty() && !b.empty()) {
								
								    glGenBuffers(1, &result->m_TangentVBO_ID);
								    BindVBO(GL_ARRAY_BUFFER, result->m_TangentVBO_ID);
								    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(t.size() * sizeof(t[0])), t.data(), GL_STATIC_DRAW);
								
									glVertexAttribPointer(3, 3, GetGLType<T>(), GL_FALSE, 0, nullptr);
									glEnableVertexAttribArray(3);
									
								    glGenBuffers(1, &result->m_BitangentVBO_ID);
								    BindVBO(GL_ARRAY_BUFFER, result->m_BitangentVBO_ID);
								    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(b.size() * sizeof(b[0])), b.data(), GL_STATIC_DRAW);
									
									glVertexAttribPointer(4, 3, GetGLType<T>(), GL_FALSE, 0, nullptr);
									glEnableVertexAttribArray(4);
								}
							}
						}
						
			            BindVBO(GL_ARRAY_BUFFER, 0);
					}
					else {
						throw std::runtime_error("Mesh has no indices!");
					}
				}
				else {
					throw std::runtime_error("Mesh has no vertices!");
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
				
				result.reset();
			}
			
			return result;
		}
		
		template<typename T, glm::precision P = glm::defaultp>
		static std::array<std::vector<glm::vec<3, T, P>>, 2> GenerateTangents(const std::vector<glm::vec<3, T, P>>& _vertices, const std::vector<glm::vec<2, T, P>>& _uvs) {
			
			auto result = std::array<std::vector<glm::vec<3, T, P>>, 2>();
			
			// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
			
			if (!_vertices.empty() && !_uvs.empty()) {
				
				if (_vertices.size() == _uvs.size()) {
					
					auto&   tangents = result[0];
					auto& bitangents = result[1];
					
					  tangents.reserve(_vertices.size());
					bitangents.reserve(_vertices.size());
					
					for (size_t i = 0; i < _vertices.size(); i += 3) {
					
						// Shortcuts for _vertices
				        const auto& pos0 = _vertices[std::min(  i  , _vertices.size() - 1)];
				        const auto& pos1 = _vertices[std::min(i + 1, _vertices.size() - 1)];
				        const auto& pos2 = _vertices[std::min(i + 2, _vertices.size() - 1)];
				
				        // Shortcuts for _UVs
				        const auto& uv0 = _uvs[std::min(  i  , _uvs.size() - 1)];
				        const auto& uv1 = _uvs[std::min(i + 1, _uvs.size() - 1)];
				        const auto& uv2 = _uvs[std::min(i + 2, _uvs.size() - 1)];
						
						const auto delta_pos1 = pos1 - pos0;
					    const auto delta_pos2 = pos2 - pos0;
						
					    const auto delta_texcoord1 = uv1 - uv0;
					    const auto delta_texcoord2 = uv2 - uv0;
					
						const auto f = static_cast<T>(1.0) / (delta_texcoord1.x * delta_texcoord2.y - delta_texcoord2.x * delta_texcoord1.y);
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
				}
				else {
					Debug::Log("Vertices / UVs collection length mismatch!", LogType::Error);
				}
			}
			else {
				Debug::Log("Vertices / UVs cannot be empty!", LogType::Error);
			}
			
			return result;
		}
		
		template<typename T, typename U, glm::precision P = glm::defaultp>
		static std::vector<glm::vec<3, T, P>> GenerateNormals(const std::vector<glm::vec<3, T, P>>& _vertices, const std::vector<U>& _indices) {
			
			validate_index_format<U>();
			
			std::vector<glm::vec<3, T, P>> result(_vertices.size());
			
			if (_vertices.size() >= 3 && _indices.size() >= 3) {
				
				for (size_t i = 0; i < _indices.size(); i += 3) {
					
					const auto iA =   i  ;
					const auto iB = i + 1;
					const auto iC = i + 2;
					
					const auto idxA = std::min(static_cast<size_t>(_indices[iA]), _vertices.size() - 1);
					const auto idxB = std::min(static_cast<size_t>(_indices[iB]), _vertices.size() - 1);
					const auto idxC = std::min(static_cast<size_t>(_indices[iC]), _vertices.size() - 1);
					
			        const auto& v1 = _vertices[idxA];
			        const auto& v2 = _vertices[idxB];
			        const auto& v3 = _vertices[idxC];
			
			        const auto n = glm::normalize(glm::cross((v2 - v1), (v3 - v1)));
					
			        result[idxA] += n;
			        result[idxB] += n;
			        result[idxC] += n;
			    }
				
				for (auto& item : result) {
					item = glm::normalize(item);
				}
			}
			else {
				Debug::Log("Invalid vertex / index buffer size!", LogType::Error);
			}
			
		    return result;
		}
		
		struct Earcut final {
			
			template<typename T, typename U, glm::precision P = glm::defaultp>
			static std::vector<U> TriangulateXY(const std::vector<glm::vec<2, T, P>>& _polyline) {
				
				validate_index_format<U>();

				static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
				
				Debug::Assert(_polyline.size() <= std::numeric_limits<U>::max(), "Vertex count exceeds the type's limit!", LogType::Error);
				
				// TODO: Make this better.
				
				using Point = std::array<float, 2>;
				std::vector<std::vector<Point>> polygon;
				
				std::vector<Point> polyline;
				for (auto it = _polyline.rbegin(); it != _polyline.rend(); ++it) {
					polyline.push_back({ it->x, it->y });
				}
				polygon.push_back(polyline);
				
				return mapbox::earcut<U>(polygon);
			}
			
			template<typename T, typename U, glm::precision P = glm::defaultp>
			static std::vector<U> TriangulateXY(const std::vector<std::vector<glm::vec<2, T, P>>>& _polygon) {
				
				validate_index_format<U>();
			
				static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
				
				Debug::Assert(_polygon.size() <= std::numeric_limits<U>::max(), "Vertex count exceeds the type's limit!", LogType::Error);
				
				// TODO: Make this better.
				
				using Point = std::array<float, 2>;
				std::vector<std::vector<Point>> polygon;
				
				for (auto& item1 : _polygon) {
					
					std::vector<Point> polyline;
					for (auto it = item1.rbegin(); it != item1.rend(); ++it) {
						polyline.push_back({ it->x, it->y });
					}
					polygon.push_back(polyline);
				}
				
				return mapbox::earcut<U>(polygon);
			}
			
			template<typename T, typename U, glm::precision P = glm::defaultp>
			static std::vector<U> TriangulateXZ(const std::vector<glm::vec<3, T, P>>& _polyline) {
				
				validate_index_format<U>();
			
				static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
				
				Debug::Assert(_polyline.size() <= std::numeric_limits<U>::max(), "Vertex count exceeds the type's limit!", LogType::Error);
				
				// TODO: Make this better.
				
				using Point = std::array<float, 2>;
				std::vector<std::vector<Point>> polygon;
				
				std::vector<Point> polyline;
				for (auto it = _polyline.rbegin(); it != _polyline.rend(); ++it) {
					polyline.push_back({ it->x, it->z });
				}
				polygon.push_back(polyline);
				
				return mapbox::earcut<U>(polygon);
			}
			
			template<typename T, glm::precision P = glm::defaultp, typename N>
			static std::vector<N> TriangulateXZ(const std::vector<std::vector<glm::vec<3, T, P>>>& _polygon) {
				
				validate_index_format<N>();
			
				static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
				
				Debug::Assert(_polygon.size() <= std::numeric_limits<N>::max(), "Vertex count exceeds the type's limit!", LogType::Error);
				
				// TODO: Make this better.
				
				using Point = std::array<float, 2>;
				std::vector<std::vector<Point>> polygon;
				
				for (auto& item1 : _polygon) {
					
					std::vector<Point> polyline;
					for (auto it = item1.rbegin(); it != item1.rend(); ++it) {
						polyline.push_back({ it->x, it->z });
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
			template<typename T, glm::precision P = glm::defaultp>
			struct Quad final {
	
			private:
				
				/** @brief Every screen and texture coordinate for every vertex in the mesh. */
				inline static constexpr std::array<glm::vec<4, T, P>, 6> s_VertexData {
					glm::vec<4, T, P>( -1.0, -1.0,  0.0,  0.0 ),
					glm::vec<4, T, P>(  1.0, -1.0,  1.0,  0.0 ),
					glm::vec<4, T, P>( -1.0,  1.0,  0.0,  1.0 ),
					glm::vec<4, T, P>( -1.0,  1.0,  0.0,  1.0 ),
					glm::vec<4, T, P>(  1.0, -1.0,  1.0,  0.0 ),
					glm::vec<4, T, P>(  1.0,  1.0,  1.0,  1.0 ),
				};
		
				/** @brief Static instance. */
				inline static std::shared_ptr<Mesh> s_Instance;
				
			public:
				
				/** @brief Returns a pointer to the static instance of the mesh. */
				static std::weak_ptr<Mesh> Instance() {
					
					if (s_Instance == nullptr) {
						s_Instance = std::make_shared<Mesh>(Mesh(GL_TRIANGLES));
						
						s_Instance->m_VertexCount = s_VertexData.size();
						
						glGenVertexArrays(1, &s_Instance->m_VAO_ID);
						
						Bind(*s_Instance);
						
						glGenBuffers(1, &s_Instance->m_PositionVBO_ID);
						
						BindVBO(GL_ARRAY_BUFFER, s_Instance->m_PositionVBO_ID);
						glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(s_Instance->m_VertexCount * sizeof(s_VertexData[0])), s_VertexData.data(), GL_STATIC_DRAW);
						
						// Positions, triangles (encoded within winding order):
						glEnableVertexAttribArray(0);
						glVertexAttribPointer(0, 2, GetGLType<T>(), GL_FALSE, sizeof(s_VertexData[0]), nullptr);
						
						// Texture coordinates:
						glEnableVertexAttribArray(1);
						glVertexAttribPointer(1, 2, GetGLType<T>(), GL_FALSE, sizeof(s_VertexData[0]), (GLvoid*)(2 * sizeof(T)));
					}
					
					return s_Instance;

				}
				
			};
			
			struct Grid final {
			
				template<typename T, typename U, glm::precision P = glm::defaultp>
				static std::shared_ptr<Mesh> Create(const  glm::vec<2, U>& _resolution, const glm::vec<2, T, P>& _size) {
					
					validate_index_format<U>();
					
					Debug::Assert(((_resolution.x + 1) * (_resolution.y + 1)) < std::numeric_limits<U>::max(), "Grid resolution exceeds limits of index type!");
					
					std::shared_ptr<Mesh> result;
					
					try {
						
						const auto& nX = _resolution.x;
						const auto& nY = _resolution.y;
					
						if (nX > 0 && nY > 0) {
							
							const U vertex_count = (nX + 1) * (nY + 1);
							
						    /* VERTEX DATA */
						    std::vector<glm::vec<3, T, P>> vertices;
							std::vector<glm::vec<2, T, P>> uvs;
							
							vertices.reserve(vertex_count);
							     uvs.reserve(vertex_count);
							
							std::array<std::vector<glm::vec<3, T, P>>, 2> tangents {
								std::vector<glm::vec<3, T, P>>(vertex_count, glm::vec<3, T, P>(1, 0, 0)),
								std::vector<glm::vec<3, T, P>>(vertex_count, glm::vec<3, T, P>(0, 0, 1)),
							};
							
					        for (U j(0); j <= nY; j++) {
						    for (U i(0); i <= nX; i++) {
								
								const auto fi = static_cast<float>(i);
								const auto fj = static_cast<float>(j);
								
								const glm::vec<2, T, P> uv {
			                        static_cast<T>(fj / static_cast<float>(nY)),
									static_cast<T>(fi / static_cast<float>(nX)),
								};
								
						        vertices.emplace_back(
								    _size.x * (uv.x - static_cast<T>(0.5)),
							        0.0,
							        _size.y * (uv.y - static_cast<T>(0.5))
						        );
								
					            uvs.emplace_back(uv);
					        }}
							
						    /* INDEX DATA */
						    std::vector<U> indices;
							indices.reserve(static_cast<size_t>(6 * nX * nY));
							
					        for (U j(0); j < nY; j++) {
						    for (U i(0); i < nX; i++) {
								
								const auto v1 = Utils::To1D({i, j}, nX + 1);
					            const auto v2 = v1 + 1;
					            const auto v3 = Utils::To1D({i, j + 1}, nX + 1);
					            const auto v4 = v3 + 1;
								
								indices.emplace_back(v1);
					            indices.emplace_back(v2);
					            indices.emplace_back(v3);
					            indices.emplace_back(v3);
					            indices.emplace_back(v2);
					            indices.emplace_back(v4);
					        }}
						}
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
					
					return result;
				}
				
				template<typename T, typename U, glm::precision P = glm::defaultp>
				static std::shared_ptr<Mesh> Create(const glm::vec<2, U>& _resolution, const glm::vec<2, T, P>& _size, const Graphics::TextureCPU<T, 1>& _heights) {
					
					validate_index_format<U>();
					
					Debug::Assert(((_resolution.x + 1) * (_resolution.y + 1)) < std::numeric_limits<U>::max(), "Grid resolution exceeds limits of index type!");
					
					std::shared_ptr<Mesh> result;
					
					try {
						
						const auto& nX = _resolution.x;
						const auto& nY = _resolution.y;
					
						if (nX > 0 && nY > 0) {
						
							const U vertex_count = (nX + 1) * (nY + 1);
							
						    /* VERTEX DATA */
						    std::vector<glm::vec<3, T, P>> vertices;
							std::vector<glm::vec<2, T, P>> uvs;
							
							vertices.reserve(vertex_count);
							     uvs.reserve(vertex_count);
							
							std::array<std::vector<glm::vec<3, T, P>>, 2> tangents {
								std::vector<glm::vec<3, T, P>>(vertex_count, glm::vec<3, T, P>(1, 0, 0)),
								std::vector<glm::vec<3, T, P>>(vertex_count, glm::vec<3, T, P>(0, 0, 1)),
							};
							
					        for (U j(0); j <= nY; j++) {
						    for (U i(0); i <= nX; i++) {
								
								const auto fi = static_cast<float>(i);
								const auto fj = static_cast<float>(j);
								
								const glm::vec<2, T, P> uv {
			                        static_cast<T>(fj / static_cast<float>(nY)),
									static_cast<T>(fi / static_cast<float>(nX)),
								};
								
								const auto t_offset = static_cast<T>(1.0) / glm::vec<2, T, P>(_heights.Width(), _heights.Height());
									
						        vertices.emplace_back(
								    _size.x * (uv.x - static_cast<T>(0.5)),
							        _heights.GetPixelBilinear(uv - t_offset),
							        _size.y * (uv.y - static_cast<T>(0.5))
						        );
								
					            uvs.emplace_back(uv);
					        }}
							
						    /* INDEX DATA */
						    std::vector<U> indices;
							indices.reserve(static_cast<size_t>(6 * nX * nY));
							
					        for (U j(0); j < nY; j++) {
						    for (U i(0); i < nX; i++) {
								
								const auto v1 = Utils::To1D({i, j}, nX + 1);
					            const auto v2 = v1 + 1;
					            const auto v3 = Utils::To1D({i, j + 1}, nX + 1);
					            const auto v4 = v3 + 1;
								
								indices.emplace_back(v1);
					            indices.emplace_back(v2);
					            indices.emplace_back(v3);
					            indices.emplace_back(v3);
					            indices.emplace_back(v2);
					            indices.emplace_back(v4);
					        }}
							
							const auto normals = GenerateNormals(vertices, indices);
							result = Mesh::Create(vertices, indices, normals, uvs, tangents, GL_TRIANGLES);
						}
						else {
							throw std::runtime_error("Cannot construct a grid mesh with a resolution of less than 1 on the x or y axes!");
						}
					}
					catch (const std::exception& e) {
						Debug::Log(e);
					}
					
					return result;
				}
			};
			
			struct PointCloud final {
				
				template<typename T, typename U, glm::precision P = glm::defaultp>
				static std::shared_ptr<Mesh> Create(const std::vector<glm::vec<3, T, P>>& _vertices) {
					
					validate_index_format<U>();
					
					auto result = std::make_shared<Mesh>(Mesh(GL_POINTS));
					
					if (!_vertices.empty()) {
						
						const size_t limit32 = std::numeric_limits<U>::max();
						
						Debug::Assert(_vertices.size() <= limit32, "Point cloud has too many vertices! Exceeds the 32-bit limit and will be truncated.", LogType::Warning);
						
						result->m_VertexCount = std::min(_vertices.size(), limit32);
						
						// Buffers to store mesh data:
						glGenVertexArrays(1, &result->m_VAO_ID);
						
						glGenBuffers(1, &result->m_PositionVBO_ID);
						glGenBuffers(1, &result->m_IBO_ID);
						
						Bind(*result);
						
						BindVBO(GL_ARRAY_BUFFER, result->m_PositionVBO_ID);
						glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(_vertices.size() * sizeof(_vertices[0])), _vertices.data(), GL_STATIC_DRAW);
						
						glEnableVertexAttribArray(0);
						glVertexAttribPointer(0, 3, GetGLType<T>(), GL_FALSE, 0, nullptr);
					}
					
					return result;
				}
		
			};
		};
		
		/**
		 * @brief Binds the provided mesh.
		 *
		 * This function binds the vertex array object (VAO) of the given mesh, if it is not already bound.
		 *
		 * @param[in] _mesh The mesh to bind.
		 */
		inline static void Bind(const Mesh& _mesh)  {
			
			if (s_CurrentVAO != _mesh.VAO_ID()) {
				glBindVertexArray(s_CurrentVAO = _mesh.VAO_ID());
			}
		}
		
		/**
		 * @brief Unbinds the currently bound mesh.
		 *
		 * This static function unbinds the currently bound mesh by setting the current VAO to GL_NONE.
		 *
		 * @sa Bind(const Mesh &_mesh)
		 */
		inline static void Unbind() {
			
			if (s_CurrentVAO != GL_NONE) {
				glBindVertexArray(s_CurrentVAO = GL_NONE);
			}
		}
		
		/**
		 * @brief Binds the specified VBO.
		 *
		 * This static function binds the given VBO to the target.
		 *
		 * @param[in] _type The type of the VBO.
		 * @param[in] _vbo The ID of the VBO to bind.
		 */
		inline static void BindVBO(const GLenum& _type, const GLuint& _vbo)  {
			glBindBuffer(_type, _vbo);
		}
		
		[[nodiscard]] constexpr const GLenum&      Format() const noexcept { return      m_Format; }
		[[nodiscard]] constexpr const GLenum& IndexFormat() const noexcept { return m_IndexFormat; }
		
		[[nodiscard]] constexpr const GLuint&          VAO_ID() const noexcept { return          m_VAO_ID; }
		[[nodiscard]] constexpr const GLuint&  PositionVBO_ID() const noexcept { return  m_PositionVBO_ID; }
		[[nodiscard]] constexpr const GLuint&  TexCoordVBO_ID() const noexcept { return  m_TexCoordVBO_ID; }
		[[nodiscard]] constexpr const GLuint&     IndexVBO_ID() const noexcept { return          m_IBO_ID; }
		[[nodiscard]] constexpr const GLuint&    NormalVBO_ID() const noexcept { return    m_NormalVBO_ID; }
		[[nodiscard]] constexpr const GLuint&   TangentVBO_ID() const noexcept { return   m_TangentVBO_ID; }
		[[nodiscard]] constexpr const GLuint& BitangentVBO_ID() const noexcept { return m_BitangentVBO_ID; }
		
		[[nodiscard]] constexpr const GLuint& VertexCount() const noexcept { return m_VertexCount; }
		[[nodiscard]] constexpr const GLuint&  IndexCount() const noexcept { return  m_IndexCount; }
	};
	
} // LouiEriksson::Engine::Graphics

#endif //FINALYEARPROJECT_MODEL_H