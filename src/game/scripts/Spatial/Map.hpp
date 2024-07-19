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
		
		Hashmap<std::string, std::pair<std::weak_ptr<ECS::GameObject>, std::pair<vec3, vec3>>> m_Aircraft;
		
		std::future<void> m_BuildTask;
		std::future<void> m_OpenSkyTask;
		
		Threading::Utils::Dispatcher        m_Dispatcher;
		Threading::Utils::CancellationToken m_CancellationToken;
		
		std::chrono::system_clock::time_point m_NextOpenSkyRequest;
		std::chrono::system_clock::time_point m_LastOpenSkyResponse;
		std::chrono::system_clock::duration   m_OpenSkyRequestInterval = std::chrono::seconds(20);
		
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
			m_NextOpenSkyRequest  (          ),
			m_GridSizeKm          (  3.0     ),
		    m_RequestDelay        (  5.0     ),
		    m_Subdivisions        (  0       ),
		    m_TimeSliceInterval   (100       ),
		    m_Scale               (  0.006278),
			m_ElevationProvider   (Elevation::ElevationProvider::OpenElevation),
			m_ElevationResolution (  0.5    ) {}
			
		~Map() override {
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
			
			m_BuildTask = std::async([this]() {
				BuildManyAsync(Settings::Spatial::s_Coord, m_GridSizeKm, m_ElevationProvider);
			});
	    }
		
		/** @inheritdoc */
		void Tick() override {
			
			BuildDynamicAsync(Settings::Spatial::s_Coord, m_GridSizeKm);
			
			m_Dispatcher.Dispatch(m_TimeSliceInterval);
			
			// Scale all map features:
			for (const auto& kvp : m_Features.GetAll()) {
			
				if (auto go = kvp.second.lock()) {
					
					auto transform = go->GetComponent<Transform>();
					
					if (const auto& t = transform) {
						t->Scale({m_Scale, m_Scale, m_Scale});
					}
				}
			}
			
			// Move and scale all aircraft in realtime:
			for (const auto& kvp : m_Aircraft.GetAll()) {
			
				auto entry = kvp.second;
				
				if (auto go = entry.first.lock()) {
					
					auto transform = go->GetComponent<Transform>();
					
					if (const auto& t = transform) {
						
						// Get aircraft values:
						auto raw_position = entry.second.first;
						auto raw_velocity = entry.second.second;
						
						// Get time since last update:
						auto time_since_response = std::chrono::duration<scalar_t>(std::chrono::system_clock::now() - m_LastOpenSkyResponse).count();
						
						// Update the aircraft's position by extrapolating its last known velocity across time.
						t->Position((raw_position + (raw_velocity * time_since_response)) * m_Scale);
						
						// Scale the aircraft with the size of the map.
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
		
		void BuildDynamicAsync(const vec3& _coord, const float& _sizeKm) {
		
			if (std::chrono::system_clock::now() > m_NextOpenSkyRequest) {
				m_NextOpenSkyRequest = std::chrono::system_clock::now() + m_OpenSkyRequestInterval;
				
				auto bounds = Maths::Coords::GPS::GPSToBounds(_coord, _sizeKm);
				
				m_OpenSkyTask = Engine::Spatial::OpenSky::QueryBoundingBoxAsync(
					bounds,
					m_OpenSkyRequestInterval,
					[this](const Engine::Spatial::Serialisation::OpenSkyDeserialiser::OpenSkyJSON::Root& _root) {
						
						m_Dispatcher.Schedule([this, _root]() {
							
							// Print the response from OpenSky:
							std::ostringstream response;
							response << "OpenSky Reponse:\n";
							response << "\tTimestamp: " << _root.time << '\n';
							response << "\tTransponders: ";
							
							if (_root.states.empty()) {
								response << "No aircraft in region.";
							}
							else {
								
								for (const auto& item : _root.states) {
									
									response << '\n';
									response << "\t"   << ((item.icao24        ).has_value() ?                *(item.icao24        )  : "null") << '\n';
									response << "\t\t" << ((item.callsign      ).has_value() ?                *(item.callsign      )  : "null") << '\n';
									response << "\t\t" << ((item.origin_country).has_value() ?                *(item.origin_country)  : "null") << '\n';
									response << "\t\t" << ((item.time_position ).has_value() ? std::to_string(*(item.time_position )) : "null") << '\n';
									response << "\t\t" << ((item.last_contact  ).has_value() ? std::to_string(*(item.last_contact  )) : "null") << '\n';
									response << "\t\t" << ((item.longitude     ).has_value() ? std::to_string(*(item.longitude     )) : "null") << '\n';
									response << "\t\t" << ((item.latitude      ).has_value() ? std::to_string(*(item.latitude      )) : "null") << '\n';
									response << "\t\t" << ((item.baro_altitude ).has_value() ? std::to_string(*(item.baro_altitude )) : "null") << '\n';
									
									response << "\t\t" << ((item.on_ground).has_value() ? (*(item.on_ground) ? "true" : "false") : "null") << '\n';
						
									response << "\t\t" << ((item.velocity      ).has_value() ? std::to_string(*(item.velocity      )) : "null") << '\n';
									response << "\t\t" << ((item.true_track    ).has_value() ? std::to_string(*(item.true_track    )) : "null") << '\n';
									response << "\t\t" << ((item.vertical_rate ).has_value() ? std::to_string(*(item.vertical_rate )) : "null") << '\n';
									
									if (item.sensors.has_value()) {
										
										for (const auto& sensor : *(item.sensors)) {
											response << "\t\t\t" << sensor << '\n';
										}
									}
									else {
										response << "\t\t" << "null" << '\n';
									}
									
									response << "\t\t" << ((item.geo_altitude).has_value() ? std::to_string(*(item.geo_altitude)) : "null") << '\n';
									response << "\t\t" << ((item.squawk      ).has_value() ?                *(item.squawk      )  : "null") << '\n';
									
									response << "\t\t" << ((item.spi).has_value() ? (*(item.spi) ? "true" : "false") : "null") << '\n';
									
									response << "\t\t" << ((item.position_source).has_value() ? std::to_string(*(item.position_source)) : "null") << '\n';
									response << "\t\t" << ((item.category       ).has_value() ? std::to_string(*(item.category       )) : "null");
									
								}
							}
							
							Debug::Log(response.str(), Info);
							
							/*
							 * Spawn, or update aircraft models.
							 * Keep note of which aircraft were updated, so we can remove the ones for which tracking was lost.
							 */
							std::vector<std::string_view> updatedAircraft;
							for (const auto& item : _root.states) {
								
								try {
									
									if (     !item.latitude.has_value()) { throw std::runtime_error("Transponder not broadcasting latitude." ); }
									if (    !item.longitude.has_value()) { throw std::runtime_error("Transponder not broadcasting longitude."); }
									if (   !item.true_track.has_value()) { throw std::runtime_error("Aircraft is not broadcasting bearing."  ); }
									
									/* Compute the altitude of the aircraft based on its state vector. */
									
									scalar_t altitude;
									
									if (item.on_ground.has_value()) {
										altitude = 0.0;
									}
									else if (item.baro_altitude.has_value()) {
										altitude = *(item.baro_altitude);
									}
									else {
										Debug::Log("Aircraft is not broadcasting a barometric altitude. A default value of 10000 metres will be used instead.", Warning);
										
										static constexpr auto default_altitude = 10000.0;
									
										altitude = default_altitude;
									}
									
									altitude += 30.0; // Offset for the aircraft's y-offset position in the model and lack of landing gear.
									
									std::shared_ptr<ECS::GameObject> gameobject;
									vec3 raw_velocity;
									vec3 raw_position;
									
									if (auto existing = m_Aircraft.Get(*item.icao24)) {
										gameobject = existing->first.lock();
										raw_position = existing->second.first;
										raw_velocity = existing->second.second;
									}
									else {
										
										raw_position = {};
										raw_velocity = {};
										
										if (const auto p = Parent()) {
											if (const auto s = p->GetScene()) {
												
												gameobject = s->Create(*item.icao24);
												auto transform = gameobject->AddComponent<Transform>();
												
												auto renderer = gameobject->AddComponent<Graphics::Renderer>();
												renderer->SetMesh(Resources::Get<Graphics::Mesh>("aircraft"));
												renderer->SetMaterial(Resources::Get<Graphics::Material>("aircraft"));
												renderer->SetTransform(transform);
											}
											else {
												throw std::runtime_error("No scene!");
											}
										}
										else {
											throw std::runtime_error("Invalid parent!");
										}
										
									}
									
									if (gameobject != nullptr) {
										
										auto t = gameobject->GetComponent<Transform>();
										if (t == nullptr) {
											t = gameobject->AddComponent<Transform>();
										}
										
										// Set the position of the aircraft in world-space using the transponder coordinates:
										auto position = Meshing::Builder::ToWorldSpace(
											{ item.latitude.value(), item.longitude.value(), altitude },
											Settings::Spatial::s_Coord
										);
										
										glm::quat rotation = glm::quat(0.0, 0.0, 0.0, 1.0);
										
										// If the aircraft is broadcasting velocity data, this can be extrapolated into pitch.
										if (item.velocity.has_value()) {
										if (item.vertical_rate.has_value()) {
											
											raw_velocity = rotation * vec3(0.0, item.vertical_rate.value(), item.velocity.value());
											
											rotation *= glm::quatLookAtRH(glm::normalize(raw_velocity), vec3(0.0, 1.0, 0.0));
										}}
										
										// Deduce the yaw of the aircraft using its bearing.
										rotation *= glm::angleAxis(glm::radians(item.true_track.value()), vec3(0.0, 1.0, 0.0));
										
										raw_position = position;
										
										t->Position(position);
										t->Rotation(rotation);
									}
									else {
										throw std::runtime_error("Null reference exception!");
									}
									
									updatedAircraft.emplace_back(item.icao24.value());
									
									m_Aircraft.Assign(item.icao24.value(), { gameobject, { raw_position, raw_velocity } });
								}
								catch(const std::exception& e) {
									Debug::Log(e);
								}
							}
							
							// Delete aircraft which we have lost track of:
							auto existingAircraft = m_Aircraft.Keys();
							for (const auto& item : existingAircraft) {

								if (std::find(updatedAircraft.begin(), updatedAircraft.end(), item) == updatedAircraft.end()) {

									if (const auto p = Parent()) {
										if (const auto s = p->GetScene()) {
											s->Remove(std::string(item));
										}
										else {
											throw std::runtime_error("No scene!");
										}
									}
									else {
										throw std::runtime_error("Invalid parent!");
									}

									m_Aircraft.Remove(item);
								}
							}
						});
					},
					m_CancellationToken
				);
			}
		}
		
		void Build(const vec4& _bounds, const Elevation::ElevationProvider& _provider)  {
   
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
					Warning
				);
				
	            r = 128;
	        }
	        
			// Query response data:
	        std::vector<std::shared_ptr<OSMDeserialiser::OSMJSON::Root::Element>> elements;
			std::vector<glm::vec<1, scalar_t>> elevations;
			
			Debug::Log("Starting Map Queries...", Info);
			
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
									
			                        if (!part->Node()->tags.empty() && !part->Node()->nodes.empty() && part->Node()->Closed()) {
			                            parts.emplace_back(part->Node());
			                            
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
		
		void BuildManyAsync(const vec3& _coord, const float& _sizeKm, const Elevation::ElevationProvider& _provider) {

			const auto     bounds = Maths::Coords::GPS::GPSToBounds(_coord, _sizeKm);
			const auto iterations = m_Subdivisions + 1;
			
	        for (auto i = 0; i < iterations; ++i) {
	        for (auto j = 0; j < iterations; ++j) {

	            const auto  width = std::abs(glm::mix(bounds.z, bounds.x, 1.0 / iterations) - bounds.z),
	                       height = std::abs(glm::mix(bounds.w, bounds.y, 1.0 / iterations) - bounds.w);

				const vec4 tile_bounds {
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