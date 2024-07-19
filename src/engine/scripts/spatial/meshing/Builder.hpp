#ifndef FINALYEARPROJECT_BUILDER_HPP
#define FINALYEARPROJECT_BUILDER_HPP

#include "../../core/Resources.hpp"
#include "../../core/Types.hpp"
#include "../../core/utils/Hashmap.hpp"
#include "../../core/utils/Random.hpp"
#include "../../core/utils/Utils.hpp"
#include "../../ecs/Component.hpp"
#include "../../graphics/Renderer.hpp"
#include "../../graphics/TextureCPU.hpp"
#include "../maths/Coords.hpp"
#include "../osm/OSM.hpp"
#include "../osm/serialisation/OSMDeserialiser.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::Graphics {
	
	class Texture;
	class Mesh;
}

namespace LouiEriksson::Engine::Spatial::Meshing {

	struct Builder final {
	
	public:
		
		struct Point final {

			friend Builder;
			
		private:
			
			std::shared_ptr<Serialisation::OSMDeserialiser::OSMJSON::Root::Element> m_Node;
			
	        vec3 m_Coord;
			
		public:
			
			/**
			 * @brief Returns the node associated with the Point.
			 *
			 * @return A shared_ptr to the node associated with this Point.
			 */
			std::shared_ptr<Serialisation::OSMDeserialiser::OSMJSON::Root::Element> Node() const noexcept {
				return m_Node;
			}
			
			/**
			 * @brief Returns the coordinate of the Point.
			 *
			 * @return A constant reference to the coordinate of the Point.
			 */
			constexpr const vec3& Coord() const noexcept {
				return m_Coord;
			}
			
			Point(std::shared_ptr<Serialisation::OSMDeserialiser::OSMJSON::Root::Element> _node, const vec3& _coord) noexcept :
				m_Node(std::move(_node)),
				m_Coord(_coord) {}
		};
		
	    static void AddElements(Graphics::TextureCPU<scalar_t, 1>& _heightmap, const vec4& _bounds, const std::vector<std::shared_ptr<Serialisation::OSMDeserialiser::OSMJSON::Root::Element>>& _elements) {
			
			s_Points.Reserve(_elements.size());
			
	        for (const auto& element : _elements) {
	        
				auto id = element->id;
		        
		        s_Points.Assign(
			        std::move(id),
			        {
				        element,
				        {
					        element->lat,
					        element->lon,
					        GetElevation({ element->lat, element->lon }, _bounds, _heightmap)
				        }
			        }
		        );
	        }
		}
	 
	    static Hashmap<size_t, Builder::Point>::optional_ref TryGetElement(const size_t& _id) {
			return s_Points.Get(_id);
		}
		
	    static bool Validate(const std::vector<size_t>& _ids) {
			
	        bool result = !_ids.empty();
	
			if (result) {
				
		        for (const auto& id : _ids) {
		            
		            if (!s_Points.ContainsKey(id)) {
		                result = false;
		                
		                break;
		            }
		        }
			}
	        
	        return result;
		}
		
		static std::optional<std::shared_ptr<ECS::GameObject>> TryCreateTerrain(const ivec2& _resolution, const vec2& _size, const Graphics::TextureCPU<scalar_t, 1>& _heightmap, const std::weak_ptr<ECS::GameObject>& _parent) {
			
			std::optional<std::shared_ptr<ECS::GameObject>> result;
			
			try {
				auto go = _parent.lock()->GetScene()->Create("Grid");
				auto transform = go->AddComponent<Transform>();
				
				std::shared_ptr<Graphics::Mesh> mesh;
				
				// Determine if the mesh should use 8, 16, or 32-bit indices:
				{
					using vertex_t = GLfloat;
					
					const size_t vertex_count = (_resolution.x + 1) * (_resolution.y + 1);
				
					if (vertex_count > std::numeric_limits<GLushort>::max()) {          // (32-bit)
						mesh = Graphics::Mesh::Primitives::Grid::Create<vertex_t, GLuint>(_resolution, _size, _heightmap);
					}
					else if (vertex_count > std::numeric_limits<GLubyte>::max()) {      // (16-bit)
						mesh = Graphics::Mesh::Primitives::Grid::Create<vertex_t, GLushort>(_resolution, _size, _heightmap);
					}
					else {                                                              // (8-bit)
						mesh = Graphics::Mesh::Primitives::Grid::Create<vertex_t, GLubyte>(_resolution, _size, _heightmap);
					}
				}
				
				auto renderer = go->AddComponent<Graphics::Renderer>();
				renderer->SetMesh(mesh);
				renderer->SetTransform(transform);
				renderer->SetMaterial(Resources::Get<Graphics::Material>("terrain"));
				
				result = go;
			}
			catch (const std::exception& e) {
				Debug::Log(e);
				
				result = std::nullopt;
			}
			
			return result;
		}
		
	    static std::optional<std::shared_ptr<ECS::GameObject>> TryCreateWay(const Serialisation::OSMDeserialiser::OSMJSON::Root::Element& _element, const std::weak_ptr<ECS::GameObject>& _parent) {
	        
	        std::optional<std::shared_ptr<ECS::GameObject>> result = std::nullopt;
	        
//			try {
//
//				if (Validate(_element.nodes)) {
//
//					using vertex_t = GLfloat;
//
//					const auto coords = GetConsecutiveCoords<vertex_t>(_element.nodes);
//
//					if (coords.size() >= 2) {
//
//						auto go = _parent.lock()->GetScene()->Create("Way: " + std::to_string(_element.id)).lock();
//
//						std::vector<glm::vec<3, vertex_t>> vertices;
//
//						vertices.reserve(coords.size());
//				        for (const auto& c : coords) {
//							vertices.emplace_back(ToWorldSpace<vertex_t>(c, Settings::Spatial::s_Coord));
//						}
//
//						Debug::Log("Please implement a line renderer!", Info);
//
////		                auto lr = _output.GetComponent<LineRenderer>();
////		                lr->positionCount = points.size();
////		                lr->SetPositions(points);
//
//						result = go;
//					}
//					else {
//						throw std::runtime_error("Way has insufficient unique consecutive vertices to form any lines.");
//					}
//		        }
//		        else {
//					throw std::runtime_error("Provided IDs did not pass validation.");
//		        }
//			}
//			catch (const std::exception& e) {
//				Debug::Log(e);
//
//				result = std::nullopt;
//			}
	        
	        return result;
	    }
		
	    static std::optional<std::shared_ptr<ECS::GameObject>> TryCreateArea(const Serialisation::OSMDeserialiser::OSMJSON::Root::Element& _element, const std::weak_ptr<ECS::GameObject>& _parent) {
	
	        std::optional<std::shared_ptr<ECS::GameObject>> result = std::nullopt;

			try {

		        if (Validate(_element.nodes)) {

					using vertex_t = GLfloat;

					/* VERTEX DATA */

					// Generate vertices from coords:
					const auto coords = GetConsecutiveCoords<vertex_t>(_element.nodes);

					if (coords.size() >= 3U) {

						auto go = _parent.lock()->GetScene()->Create("Area: " + std::to_string(_element.id));
			            auto transform = go->AddComponent<Transform>();

						const auto renderer = go->AddComponent<Graphics::Renderer>();
						renderer->SetTransform(transform);
						renderer->SetMaterial(Resources::Get<Graphics::Material>("area"));

						std::vector<glm::vec<3, vertex_t>> vertices;
						
						vertices.reserve(coords.size());
				        for (const auto& c : coords) {
							
							// Offset from ground, to avoid clipping into terrain.
							const static constexpr glm::vec<3, vertex_t> ground_offset(0.0, 1.0, 0.0);
							
							vertices.emplace_back(ToWorldSpace<vertex_t>(c, Settings::Spatial::s_Coord) + ground_offset + ZFightingMitigation<vertex_t>());
						}

						// Generate UVs from vertices:
						const auto uvs = GenerateWorldUVs_XZ(vertices);

						/* INDEX DATA */

						// Determine if the mesh should use 8, 16, or 32-bit indices:
						if (vertices.size() > std::numeric_limits<GLushort>::max()) {

							// 32-bit:
							const auto indices = Graphics::Mesh::Earcut::TriangulateXZ<vertex_t, GLuint>(vertices);

							if (indices.size() >= 3U) {
								renderer->SetMesh(
									Graphics::Mesh::Create(vertices,
										indices,
										Graphics::Mesh::GenerateNormals<vertex_t>(vertices, indices),
										uvs,
										Graphics::Mesh::GenerateTangents<vertex_t>(vertices, uvs, indices)
									)
								);
							}
						}
						else if (vertices.size() > std::numeric_limits<GLubyte>::max()) {

							// 16-bit:
							const auto indices = Graphics::Mesh::Earcut::TriangulateXZ<vertex_t, GLushort>(vertices);

							if (indices.size() >= 3U) {
								renderer->SetMesh(
									Graphics::Mesh::Create(vertices,
										indices,
										Graphics::Mesh::GenerateNormals<vertex_t>(vertices, indices),
										uvs,
										Graphics::Mesh::GenerateTangents<vertex_t>(vertices, uvs, indices)
									)
								);
							}
						}
						else {

							// 8-bit:
							const auto indices = Graphics::Mesh::Earcut::TriangulateXZ<vertex_t, GLubyte>(vertices);

							if (indices.size() >= 3U) {
								renderer->SetMesh(
									Graphics::Mesh::Create(vertices,
										indices,
										Graphics::Mesh::GenerateNormals<vertex_t>(vertices, indices),
										uvs,
										Graphics::Mesh::GenerateTangents<vertex_t>(vertices, uvs, indices)
									)
								);
							}
						}

						result = go;
					}
					else {
						throw std::runtime_error("Area has insufficient unique consecutive vertices to form any polygons.");
					}
		        }
		        else {
					throw std::runtime_error("Provided IDs did not pass validation.");
		        }
			}
			catch(std::exception& e) {
				Debug::Log(e);

				result = std::nullopt;
			}
			
	        return result;
	    }
	    
	    static std::optional<std::shared_ptr<ECS::GameObject>> TryCreateBuilding(const Serialisation::OSMDeserialiser::OSMJSON::Root::Element& _element, const std::weak_ptr<ECS::GameObject>& _parent) {
	        
	        std::optional<std::shared_ptr<ECS::GameObject>> result = std::nullopt;
	
			try {
				
				if (Validate(_element.nodes)) {
		
					using vertex_t = GLfloat;
					
		            // Retrieve the building coordinates and transform them into world space.
					// This process generates two vertex rings that represent the "roof" and "floor".
					const auto coords = GetConsecutiveCoords<vertex_t>(_element.nodes);
					
					if (coords.size() >= 3U) {
						
						/* Instantiate GameObject */
						auto go = _parent.lock()->GetScene()->Create("Building: " + std::to_string(_element.id));
			            auto transform = go->AddComponent<Transform>();
						
						const auto renderer1 = go->AddComponent<Graphics::Renderer>();
						const auto renderer2 = go->AddComponent<Graphics::Renderer>();
						const auto renderer3 = go->AddComponent<Graphics::Renderer>();
						
						renderer1->SetTransform(transform);
						renderer1->SetMaterial(Resources::Get<Graphics::Material>("side"));
						
						renderer2->SetTransform(transform);
						renderer2->SetMaterial(Resources::Get<Graphics::Material>("roof"));
						
						renderer3->SetTransform(transform);
						renderer3->SetMaterial(Resources::Get<Graphics::Material>("floor"));
						
			            const auto height = GetBuildingHeight<vertex_t>(_element);
						
		                std::vector<glm::vec<3, vertex_t>> upper, lower;
						{
							upper.reserve(coords.size());
							lower.reserve(coords.size());
							
			                const auto ground = Engine::Spatial::OSM::Utils::GetMinMaxElevation<vertex_t>(coords);
			
			                for (const auto& c : coords) {
			                    lower.emplace_back(ToWorldSpace(glm::vec<3, vertex_t>(c.x, c.y, height.x + ground.x), Settings::Spatial::s_Coord) + ZFightingMitigation<vertex_t>());
			                    upper.emplace_back(ToWorldSpace(glm::vec<3, vertex_t>(c.x, c.y, height.y + ground.y), Settings::Spatial::s_Coord) + ZFightingMitigation<vertex_t>());
			                }
						}
						
						/* WALLS */
			            {
							const auto wall_verts = GenerateSideVertices(lower, upper);
							const auto wall_uvs   = GenerateWorldSideUVs(wall_verts);
							
							// Determine if the mesh should use 8, 16, or 32-bit indices:
							const auto triCount = (wall_verts.size() - 2U) * 3U;
							if (triCount > std::numeric_limits<GLushort>::max()) {
							
								// 32-bit:
								const auto wall_tris  = GenerateSideTriangles<GLuint>(lower, upper);
								const auto wall_norms = Graphics::Mesh::GenerateNormals <vertex_t>(wall_verts, wall_tris);
								const auto wall_tb    = Graphics::Mesh::GenerateTangents<vertex_t>(wall_verts, wall_uvs, wall_tris);
								
								renderer1->SetMesh(Graphics::Mesh::Create(wall_verts, wall_tris, wall_norms, wall_uvs, wall_tb));
							}
							else if (triCount > std::numeric_limits<GLubyte>::max()) {

								// 16-bit:
								const auto wall_tris  = GenerateSideTriangles<GLushort>(lower, upper);
								const auto wall_norms = Graphics::Mesh::GenerateNormals <vertex_t>(wall_verts, wall_tris);
								const auto wall_tb    = Graphics::Mesh::GenerateTangents<vertex_t>(wall_verts, wall_uvs, wall_tris);

								renderer1->SetMesh(Graphics::Mesh::Create(wall_verts, wall_tris, wall_norms, wall_uvs, wall_tb));
							}
							else {

								// 8-bit:
								const auto wall_tris  = GenerateSideTriangles<GLubyte>(lower, upper);
								const auto wall_norms = Graphics::Mesh::GenerateNormals <vertex_t>(wall_verts, wall_tris);
								const auto wall_tb    = Graphics::Mesh::GenerateTangents<vertex_t>(wall_verts, wall_uvs, wall_tris);

								renderer1->SetMesh(Graphics::Mesh::Create(wall_verts, wall_tris, wall_norms, wall_uvs, wall_tb));
							}
			            }
			            
			            /* FLOOR & ROOF */
			            {
							// Determine if the mesh should use 8, 16, or 32-bit indices:
							if (upper.size() > std::numeric_limits<GLushort>::max()) {
							
								// 32-bit:
								GenerateRoofAndFloor<vertex_t, GLuint>(renderer2, renderer3, upper, lower, height.x != 0.0);
							}
							else if (upper.size() > std::numeric_limits<GLubyte>::max()) {

								// 16-bit:
								GenerateRoofAndFloor<vertex_t, GLuint>(renderer2, renderer3, upper, lower, height.x != 0.0);
							}
							else {

								// 8-bit:
								GenerateRoofAndFloor<vertex_t, GLuint>(renderer2, renderer3, upper, lower, height.x != 0.0);
							}
			            }
						
						result = go;
					}
					else {
						throw std::runtime_error("Building has insufficient unique consecutive vertices to form any polygons.");
					}
		        }
		        else {
					throw std::runtime_error("Provided IDs did not pass validation.");
		        }
			}
	        catch(const std::exception& e) {
				Debug::Log(e);
				
				result = std::nullopt;
			}
	        
	        return result;
	    }
	    
		template<typename T>
	    static glm::vec<3, T> ToWorldSpace(const glm::vec<3, T>& _coord, const glm::vec<3, T>& _origin) {
         
	        const auto delta = _coord - _origin;
			
	        const auto pos = Maths::Coords::GPS::GPSToCartesian({
	            delta.x * Maths::Coords::WGS84::EquatorialStretchFactor(_coord.x),
	            static_cast<T>(90.0) + delta.y,
	            delta.z
			});
	  
			const auto offset = glm::vec<3, T>(
				0,
				Maths::Coords::WGS84::EarthRadius(_coord.x) + Maths::Coords::WGS84::AltitudeCompensation(_coord.x),
				0
			);
			
	        return pos - offset;
	    }
		
	private:
		
        // https://en.wikipedia.org/wiki/Storey
        static constexpr scalar_t s_StoreyHeight { 4.3 };
		
		inline static Hashmap<size_t, Point> s_Points;
		
		template<typename T, glm::precision Q = glm::defaultp>
		static glm::vec<3, T, Q> ZFightingMitigation(const T& _epsilon = 0.1) {
			return Random::Range(glm::vec<3, T, Q>(-abs(_epsilon)), glm::vec<3, T, Q>(abs(_epsilon)));
		}
		
		template<typename T, glm::precision Q = glm::defaultp>
	    static float GetElevation(const glm::vec<2, T, Q>& _coord, const glm::vec<4, T, Q>& _bounds, const Graphics::TextureCPU<scalar_t, 1>& _heightmap)  {
	        return _heightmap.GetPixelBilinear(Maths::Coords::GPS::GPSToUV(_coord, _bounds)).r;
	    }
		
		template<typename T, glm::precision Q = glm::defaultp>
	    static constexpr std::vector<glm::vec<3, T, Q>> ToWorldSpaceMany(const std::vector<size_t>& _elements, const glm::vec<3, T, Q>& _origin) {
	        
	        std::vector<glm::vec<3, T, Q>> result;
	
			result.reserve(_elements.size());
			for (const auto& item : _elements) {
				
	            result.emplace_back(
					ToWorldSpace(
						s_Points[item].m_Coord,
						_origin
					)
				);
			}
	        
	        return result;
		}
		
		template<typename T, glm::precision Q = glm::defaultp>
		static glm::vec<2, T, Q> GetBuildingHeight(const Serialisation::OSMDeserialiser::OSMJSON::Root::Element& _building) {
			
			const T defaultHeight = static_cast<T>(s_StoreyHeight * 2.0);
			
	        glm::vec<2, T, Q> result { static_cast<T>(0), defaultHeight };
			
			try {
				
				if (const auto str = _building.tags.Get("min_height")) {
					if (const auto parsed = OSM::Utils::TryConvertToMetres<T>(*str)) {
						result.x = *parsed;
					}
				}
				if (result.x == 0.0) {
					if (const auto str = _building.tags.Get("building:min_levels")) {
						if (const auto parsed = Utils::TryParse<T>(*str)) {
							result.x = *parsed * s_StoreyHeight;
						}
					}
				}
				
				if (const auto str = _building.tags.Get("height")) {
					if (const auto parsed = OSM::Utils::TryConvertToMetres<T>(*str)) {
						result.y = *parsed;
					}
				}
				if (result.y == defaultHeight) {
					if (const auto str = _building.tags.Get("building:levels")) {
						if (const auto parsed = Utils::TryParse<T>(*str)) {
							result.y = *parsed * s_StoreyHeight;
						}
					}
				}
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
			
			return result;
		}
		
		template<typename T, glm::precision Q = glm::defaultp>
		static bool IsClockwise_XZ(const std::vector<glm::vec<3, T, Q>>& _points) {
	
	        T result(0.0);
	
	        for (size_t i = 0U; i < _points.size() - 1U; ++i) {
	            
	            result += (_points[i + 1U].x - _points[  i  ].x) *
	                      (_points[i + 1U].z + _points[  i  ].z);
	        }
	        
	        return result >= 0.0;
	    }
	    
		template<typename T, glm::precision Q = glm::defaultp>
		static std::vector<glm::vec<3, T, Q>> GetCoords(const std::vector<size_t>& _ids) {
	
	        std::vector<glm::vec<3, T, Q>> results;
	
			results.reserve(_ids.size());
			for (const auto& item : _ids) {
                results.emplace_back(s_Points[item].m_Coord);
			}
	        
	        return results;
	    }
		
		template<typename T, glm::precision Q = glm::defaultp>
		static std::vector<glm::vec<3, T, Q>> GetDistinctCoords(const std::vector<size_t>& _ids) {
	
	        std::vector<glm::vec<3, T, Q>> results;
	
			// Hashset of already processed coords.
			std::unordered_set<glm::vec<3, T, Q>> processed;
			
			results.reserve(_ids.size());
			for (const auto& item : _ids) {
				
				auto coord = s_Points[item].m_Coord;
				
				// Add the coordinate to the result, only if it is unique to the other items in the set.
				if (processed.count(coord) == 0U) {
					processed.emplace(coord);
	                results.emplace_back(coord);
				}
			}
	        
	        return results;
	    }
		
		template<typename T, glm::precision Q = glm::defaultp>
		static std::vector<glm::vec<3, T, Q>> GetConsecutiveCoords(const std::vector<size_t>& _ids) {
	
	        std::vector<glm::vec<3, T, Q>> results;
	
			results.reserve(_ids.size());
			for (size_t i = 0U; i < _ids.size(); ++i) {
				
				auto coord = s_Points[_ids[i]].m_Coord;
				
				if (i == 0U || results.back() != coord) {
					results.emplace_back(coord);
				}
			}
	        
	        return results;
	    }
	
		template<typename T, glm::precision Q = glm::defaultp>
		static constexpr std::vector<glm::vec<2, T, Q>> GenerateWorldUVs_XZ(const std::vector<glm::vec<3, T, Q>>& _vertices) {
			
			std::vector<glm::vec<2, T, Q>> result;
	  
			result.reserve(_vertices.size());
			for (auto item : _vertices) {
		        result.emplace_back(item.x, item.z);
			}
			
			return result;
		}
		
		template<typename T, glm::qualifier Q = glm::defaultp>
		static constexpr std::vector<glm::vec<3, T, Q>> GenerateSideVertices(const std::vector<glm::vec<3, T, Q>>& _lower, const std::vector<glm::vec<3, T, Q>>& _upper) {
		
			std::vector<glm::vec<3, T, Q>> result;
			
			result.reserve(_lower.size() + _upper.size() + 4U);
			
			for (size_t i = 0U; i < _lower.size(); ++i) {
				result.emplace_back(_upper[i]);
				result.emplace_back(_lower[i]);
			}
			
			return result;
		}
		
		template<typename T, glm::qualifier Q = glm::defaultp>
		static constexpr std::vector<glm::vec<2, T, Q>> GenerateSideUVs(const std::vector<glm::vec<3, T, Q>>& _verts) {
		
            std::vector<glm::vec<2, T, Q>> wall_uvs;
			
			constexpr std::array<glm::vec<2, T, Q>, 4U> quad_uvs = {{
				{ 0U, 1U },
				{ 0U, 0U },
				{ 1U, 1U },
				{ 1U, 0U }
			}};
			
			wall_uvs.reserve(_verts.size());
            for (size_t i = 0U; i < _verts.size(); ++i) {
				wall_uvs.emplace_back(quad_uvs[i % 4U]);
            }
			
			return wall_uvs;
		}
		
		template<typename T, glm::qualifier Q = glm::defaultp>
		static constexpr std::vector<glm::vec<2, T, Q>> GenerateWorldSideUVs(const std::vector<glm::vec<3, T, Q>>& _verts) {
			
            std::vector<glm::vec<2, T, Q>> wall_uvs;
				
			if (_verts.size() > 2U) {
				
				constexpr std::array<glm::vec<2, T, Q>, 4U> quad_uvs = {{
					{ 0U, 1U },
					{ 0U, 0U },
					{ 1U, 1U },
					{ 1U, 0U }
				}};
				
				// Get the total height of the building from the first two vertices (upper, lower).
				const T height = abs(_verts[0U].y - _verts[1U].y);
				
				const T u_multiplier(0.2);
				
				wall_uvs.reserve(_verts.size());
	            for (size_t i = 0U; i < _verts.size(); ++i) {
					
					// Compute the length of the wall using the position of the next wall-defining vertex (offset of 2 away).
					auto next_wall_vertex = _verts[std::max((i + 2U) % (_verts.size() - 2U), static_cast<size_t>(0U))];
					auto wall_length = glm::length(next_wall_vertex - _verts[i]);
					
					/*
					 * Repeat the UV on the x-axis using the width of the wall, and repeat on the y axis using the building's
					 * height wrapped by the average height of a storey.
					 */
					auto local_uv = quad_uvs[i % 4U];
					
					const glm::vec<2, T, Q> world_uv(
						 local_uv.x * wall_length * u_multiplier,
						(local_uv.y * height) / s_StoreyHeight
					);
					
					wall_uvs.emplace_back(world_uv);
	            }
			}
			else {
				Debug::Log("Invalid size of vertex buffer for world-scale UV generation!", Error);
			}
			
			return wall_uvs;
		}
		
		template<typename T, typename U, glm::qualifier Q = glm::defaultp>
		static constexpr std::vector<T> GenerateSideTriangles(const std::vector<glm::vec<3, U, Q>>& _lower, const std::vector<glm::vec<3, U, Q>>& _upper) {
			
			if (_lower.size() != _upper.size()) {
				throw std::runtime_error("Size of upper and lower vertex rings do not match!");
			}
			
			constexpr std::array<std::array<T, 6U>, 2U> quad_tris = {{
				{{ 0U, 1U, 2U, 2U, 1U, 3U }}, // Forward
				{{ 3U, 1U, 2U, 2U, 1U, 0U }}  // Reverse
			}};
			
			const auto vertex_count = _lower.size() + _upper.size();
			std::vector<T> wall_tris((vertex_count - 2U) * 3U);
			
            // Compute in forward or reverse depending on the winding order of the top/bottom vertices.
			if (IsClockwise_XZ(_lower)) {
				
				for (size_t i = 0U, j = 0U; i < vertex_count - 3U; i += 2U, j += 6U) {
			
					for (size_t k = 0U; k < 6U; k++) {
						wall_tris[j + k] = i + quad_tris[0U][k];
					}
				}
			}
			else {
				
				for (long i = static_cast<long>(vertex_count - 4U), j = 0U; i >= 0; i -= 2, j += 6U) {
					
					for (size_t k = 0U; k < 6U; k++) {
						wall_tris[j + k] = static_cast<size_t>(i) + quad_tris[1U][k];
					}
				}
			}
			
			return wall_tris;
		}
		
		template<typename vertex_t, typename index_t, glm::qualifier Q = glm::defaultp>
		static void GenerateRoofAndFloor(std::shared_ptr<Graphics::Renderer> _roof, std::shared_ptr<Graphics::Renderer> _floor, const std::vector<glm::vec<3, vertex_t>>& _upper, const std::vector<glm::vec<3, vertex_t>>& _lower, bool _buildFloor) {
			
			// Roof:
			const auto upper_tris = Graphics::Mesh::Earcut::TriangulateXZ<vertex_t, index_t>(_upper);
			
			// Only continue if the mesh possesses a valid number of indices.
			if (upper_tris.size() >= 3U) {
				
				const auto upper_norms = std::vector<glm::vec<3, vertex_t>>(upper_tris.size(), { 0.0, 1.0, 0.0 });
				const auto upper_uvs   = GenerateWorldUVs_XZ(_upper);
				const auto upper_tb    = std::array<std::vector<glm::vec<3, vertex_t>>, 2U> {
					std::vector<glm::vec<3, vertex_t>>(_upper.size(), { 1.0, 0.0, 0.0 }),
					std::vector<glm::vec<3, vertex_t>>(_upper.size(), { 0.0, 0.0, 1.0 }),
				};
				
				_roof->SetMesh(Graphics::Mesh::Create(_upper, upper_tris, upper_norms, upper_uvs, upper_tb));
			    
			    // Add a "floor" to the building if the min height is non-zero.
			    if (_buildFloor) {
			
					// Reuse data from the roof to avoid running earcut twice.
			        std::vector<index_t> lower_tris;
			        lower_tris.reserve(upper_tris.size());
			        for (auto rit = upper_tris.rbegin(); rit != upper_tris.rend(); ++rit) {
			            lower_tris.emplace_back(*rit);
			        }
			
					const std::vector<glm::vec<3, vertex_t>> lower_norms(lower_tris.size(), { 0.0, -1.0, 0.0 });
			
					const auto& lower_uvs = upper_uvs;
					const auto lower_tb = std::array<std::vector<glm::vec<3, vertex_t>>, 2U> {
						std::vector<glm::vec<3, vertex_t>>(_lower.size(), { -1.0, 0.0,  0.0 }),
						std::vector<glm::vec<3, vertex_t>>(_lower.size(), {  0.0, 0.0, -1.0 }),
					};
			
					_floor->SetMesh(Graphics::Mesh::Create(_lower, lower_tris, lower_norms, lower_uvs, lower_tb));
			    }
			}
			
		}
	};
	
} // LouiEriksson::Engine::Spatial::Meshing

#endif //FINALYEARPROJECT_BUILDER_HPP