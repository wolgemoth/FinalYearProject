#ifndef FINALYEARPROJECT_MAP_HPP
#define FINALYEARPROJECT_MAP_HPP

#include "../../include/engine_core.hpp"
#include "../../include/engine_graphics.hpp"
#include "../../include/engine_spatial.hpp"

using namespace LouiEriksson::Engine;
using namespace LouiEriksson::Engine::Spatial;

namespace LouiEriksson::Game::Scripts::Spatial {
	
	class Map final : public Script {
	
		/* VARIABLES */
		
	private:
		
		inline static int s_ObjectCounter { 0 };
  
		std::weak_ptr<Transform> m_Transform;
		
		Hashmap<std::string_view, std::weak_ptr<ECS::GameObject>> m_Features;
		
		std::future<void> m_BuildTask;
		
		Threading::Utils::Dispatcher        m_Dispatcher;
		Threading::Utils::CancellationToken m_CancellationToken;
		
	public:
	
	    float m_GridSizeKm;
	    float m_RequestDelay;
	    int m_Subdivisions;
	    int m_TimeSliceInterval;
	    
	    float m_Scale;
	    
	    Elevation::ElevationProvider m_ElevationProvider;
	    float m_ElevationResolution;
		
		/* METHODS */
		
		explicit Map(const std::weak_ptr<ECS::GameObject>& _parent) : Script(_parent),
			m_CancellationToken   (          ),
			m_GridSizeKm          (  3.0     ),
		    m_RequestDelay        (  5.0     ),
		    m_Subdivisions        (  0       ),
		    m_TimeSliceInterval   (100       ),
		    m_Scale               (  0.006278),
			m_ElevationProvider   (Elevation::ElevationProvider::OpenElevation),
			m_ElevationResolution (  0.5    ) {}
			
		~Map() {
			m_CancellationToken.Cancel();
		}
		
		/** @inheritdoc */
		[[nodiscard]] std::type_index TypeID() const noexcept override { return typeid(Map); };

	protected:
	
		/** @inheritdoc */
		void Begin() override  {
			
			if (const auto p = Parent()) {
				m_Transform = p->AddComponent<Transform>();
			}
			
			Settings::Spatial::s_Coord = GenerateCoord<scalar_t>({ 22.28180, 114.17347 }); // HK
			//Settings::Spatial::s_Coord = GenerateCoord<scalar_t>({ 50.74794, -1.87812 }); // Bournemouth
			//Settings::Spatial::s_Coord = GenerateCoord<scalar_t>({ 0.0, 0.0 }); // Null Island
			//Settings::Spatial::s_Coord = GenerateCoord<scalar_t>({ 40.76956, -73.97380 }); // NY
			//Settings::Spatial::s_Coord = GenerateCoord<scalar_t>({ 51.5028, -0.1105,}); // London
			//Settings::Spatial::s_Coord = GenerateCoord<scalar_t>({35.69067, 139.81876}); // Tokyo
			//Settings::Spatial::s_Coord = GenerateCoord<scalar_t>({-4.0513, 39.6658}); // Mombasa
			//Settings::Spatial::s_Coord = GenerateCoord<scalar_t>({-33.9894, 18.5555}); // Cape Town
			
			m_BuildTask = std::async([this]() {
				BuildManyAsync(Settings::Spatial::s_Coord, m_GridSizeKm, m_ElevationProvider);
			});
	    }
		
		/** @inheritdoc */
		void Tick() override {
			
			m_Dispatcher.Dispatch(m_TimeSliceInterval);
			
			for (const auto& kvp : m_Features.GetAll()) {
			
				if (auto go = kvp.second.lock()) {
					
					auto transform = go->GetComponent<Transform>();
					
					if (const auto t = transform) {
						t->Scale({m_Scale, m_Scale, m_Scale});
					}
				}
			}
	    }
		
	private:
		
		template <typename T>
		glm::vec<3, T> GenerateCoord(glm::vec<2, T> _coord) {
			return {
				_coord.x,
				_coord.y,
				0.0
			};
		}
		
		void Build(const glm::vec4& _bounds, const Elevation::ElevationProvider& _provider)  {
   
			using OSMDeserialiser = Engine::Spatial::Serialisation::OSMDeserialiser;
			
	        static constexpr std::array<double, 2U> providerResolution_as {
	            3.0,
	            1.0
	        };
	        
	        auto r = static_cast<int>(std::ceil(
	            m_GridSizeKm * 1000.0 /
					Maths::Conversions::Distance::ArcSecondsToMetres(
						providerResolution_as.at(static_cast<int>(m_ElevationProvider)), Settings::Spatial::s_Coord.x
					) * m_ElevationResolution
	        ));
	        
	        if (r > 128) {
	        
				Debug::Log("Requested elevation resolution for this tile (" + std::to_string(r) + ") "
			        "has reached the 128p hard limit. Elevation resolution will be clamped to 128.",
					LogType::Warning
				);
				
	            r = 128;
	        }
	        
			// Query response data:
	        std::vector<std::shared_ptr<OSMDeserialiser::OSMJSON::Root::Element>> elements;
			std::vector<glm::vec<1, scalar_t>> elevations;
			
			Debug::Log("Starting Map Queries...", LogType::Info);
			
	        const auto osm_query = OSM::QueryOverpassBoundingBoxAsync(_bounds, std::chrono::seconds(60),
				[&elements](const Networking::Requests::Response& _response) {
					if (Networking::Requests::Response::Success(_response)) {
						
						auto result = OSMDeserialiser::Deserialise<OSMDeserialiser::OSMJSON::Root>(_response.Content().ToStream()).elements;
						
						elements.reserve(result.size());
						for (const auto& item : result) {
							elements.emplace_back(std::make_shared<OSMDeserialiser::OSMJSON::Root::Element>(item));
						}
						
						Debug::Log("OpenStreetMap Query Success!");
					}
		        },
				m_CancellationToken
			);
	        
			const auto elevation_query = Elevation::LoadElevationAsync(_bounds, _provider, { r + 1, r + 1 }, std::chrono::seconds(60),
				[&elevations](const std::vector<glm::vec<1, scalar_t>>& _result) {
					elevations = _result;
					
					Debug::Log("Elevation Query Success!");
		        },
				m_CancellationToken
			);
	        
			elevation_query.wait();
			if (m_CancellationToken.IsCancellationRequested()) { return; }
			
			Graphics::TextureCPU<scalar_t, 1> heightmap(elevations, r + 1, r + 1);
			
			/* TERRAIN */
			{
				// Get the midpoint of the bounds and deduce the amount by which to stretch the terrain to account for the latitude.
				const auto stretchFactor = Maths::Coords::WGS84::EquatorialStretchFactor(glm::mix(_bounds.x, _bounds.z, 0.5));
				
				m_Dispatcher.Schedule([this, r, heightmap, stretchFactor]() {
					if (auto go = Meshing::Builder::TryCreateTerrain( { r, r }, { m_GridSizeKm * 1000.0 * stretchFactor, m_GridSizeKm * 1000.0 * stretchFactor }, heightmap, Parent())) {
						m_Features.Assign(std::move(go.value()->Name()), std::move(*go));
					}
				});
			}
			
	        osm_query.wait();
			if (m_CancellationToken.IsCancellationRequested()) { return; }
			
			Debug::Log("...Finished All Queries.");
			
	        Meshing::Builder::AddElements(heightmap, _bounds, elements);
			
			/*
			 * Prewarm features hashmap based on size of elements.
			 * This may overallocate, but we can call Trim() after
			 * building if needed.
			 */
			m_Features.Reserve(elements.size());
			
			/* FEATURES */
			
			// Hashset indicating if an element has already been processed or not.
			std::unordered_set<ulong> processedElements;
			processedElements.reserve(elements.size());
		 
			// Build elements:
	        for (const auto& element : elements) {
	        
				if (m_CancellationToken.IsCancellationRequested()) { return; }
				
				if (const auto str = element->tags.Get("type")) {
					
		            if (*str == "building"          &&
						element->type == "relation" &&
					   !element->tags.empty()       &&
					   !element->members.empty()
		            ) {
		                
		                auto parts = std::vector<std::shared_ptr<OSMDeserialiser::OSMJSON::Root::Element>>();
		
		                for (const auto& member : element->members) {
							
		                    if (member.role == "part") {
		                    
								if (const auto part = Meshing::Builder::TryGetElement(member.ref)) {
									
			                        if (!part->m_Node->tags.empty() && !part->m_Node->nodes.empty() && part->m_Node->Closed()) {
			                            parts.emplace_back(part->m_Node);
			                            
			                            processedElements.insert(member.ref);
			                        }
								}
		                    }
		                    else if (member.role == "outline") {
		                        processedElements.insert(member.ref);
		                    }
		                }
		
		                for (const auto& part : parts) {
							
							m_Dispatcher.Schedule([this, part]() {
								if (auto go = Meshing::Builder::TryCreateBuilding(*part, Parent())) {
				                    m_Features.Assign(go.value()->Name(), *go);
								};
							});
		                }
		            }
		        }
	        }
			
	        for (const auto& element : elements) {
	        
				if (m_CancellationToken.IsCancellationRequested()) { return; }
				
				// Check if element is not in the hashset of processed elements...
	            if (processedElements.count(element->id) == 0U) {
	            
					// Check if the element is a 'way' (chain of nodes).
	                if (element->type == "way" && !element->tags.empty() && !element->nodes.empty() && Meshing::Builder::Validate(element->nodes)) {
	                
						// If element is road...
	                    if (element->tags.ContainsKey("highway")) {
							
							// Create road:
							m_Dispatcher.Schedule([this, element]() {
								if (auto go = Meshing::Builder::TryCreateWay(*element, Parent())) {
				                    m_Features.Assign(go.value()->Name(), *go);
								};
							});
	                    }
	                    else if (element->Closed()) { // Way forms a continuous loop...
	
							// Try to deduce if the way is representing a building or an area.
	                        auto isBuilding =
	                            !element->tags.ContainsKey("landuse") &&
	                            (element->tags.ContainsKey("building") || element->tags.ContainsKey("building:part"));
							
	                        if (isBuilding) {
								
								// Create building:
								m_Dispatcher.Schedule([this, element]() {
									if (auto go = Meshing::Builder::TryCreateBuilding(*element, Parent())) {
										m_Features.Assign(go.value()->Name(), *go);
									};
								});
	                        }
	                        else {
								
								// Create area:
								m_Dispatcher.Schedule([this, element]() {
		                            if (auto go = Meshing::Builder::TryCreateArea(*element, Parent())) {
			                            m_Features.Assign(go.value()->Name(), *go);
									}
								});
	                        }
	                    }
	                }
	            }
	        }
	    }
		
		void BuildManyAsync(const glm::vec3& _coord, const float& _sizeKm, const Elevation::ElevationProvider& _provider) {

			const auto     bounds = Maths::Coords::GPS::GPSToBounds(_coord, _sizeKm);
			const auto iterations = m_Subdivisions + 1;
			
	        for (auto i = 0; i < iterations; ++i) {
	        for (auto j = 0; j < iterations; ++j) {

	            const auto  width = std::abs(glm::mix(bounds.z, bounds.x, 1.0 / iterations) - bounds.z),
	                       height = std::abs(glm::mix(bounds.w, bounds.y, 1.0 / iterations) - bounds.w);

				const glm::vec4 tile_bounds {
		             bounds.x + (static_cast<float>(   i   ) * width ),
		             bounds.y + (static_cast<float>(   j   ) * height),
		             bounds.x + (static_cast<float>((i + 1)) * width ),
		             bounds.y + (static_cast<float>((j + 1)) * height)
		        };
				
	            Build(tile_bounds, _provider);
				
				if (i != iterations && j != iterations) {
					std::this_thread::sleep_for(std::chrono::duration<double>(m_RequestDelay));
				}
	        }}
	    }
	};
	
} // LouiEriksson::Game::Scripts::Spatial

#endif //FINALYEARPROJECT_MAP_HPP