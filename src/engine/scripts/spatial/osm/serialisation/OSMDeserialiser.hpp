#ifndef FINALYEARPROJECT_OSMDESERIALISER_HPP
#define FINALYEARPROJECT_OSMDESERIALISER_HPP

#include "../../../core/utils/Hashmap.hpp"

#include <json.hpp>

#include <cstddef>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;

namespace LouiEriksson::Engine::Spatial::Serialisation {
	
	/**
	 * @brief The OSMDeserialiser class for deserializing OSM JSON data.
	 *
	 * This class provides a utility for deserializing OSM JSON data into structured objects.
	 */
	struct OSMDeserialiser final {
	
	public:
		
		struct OSMJSON final {
			
	        struct Root final {
	
	            struct Element final {
		            
		            struct Member final {
						
						const std::string type;
	                    const      size_t ref;
	                    const std::string role;
						
						explicit Member(json&& _osm_json) :
							type(Utils::As<std::string>(_osm_json["type"])),
							 ref(_osm_json["ref"]),
							role(Utils::As<std::string>(_osm_json["role"])) {}
	                };
					
					const std::string type;

	                const size_t id;
					
	                std::vector<size_t> nodes;
	                std::vector<Member> members;
	                
	                scalar_t lat, lon;
	
	                Hashmap<std::string, std::string> tags;
		            
		            [[nodiscard]] bool Closed() const {
						return !nodes.empty() && (nodes.front() == nodes.back());
					}
					
		            Element(json&& _osm_json) :
						   type(Utils::As<std::string>(_osm_json["type"])),
						     id(_osm_json["id"]),
						  nodes(),
						members(),
						   tags()
					{
						if (type == "node") {
							lat = _osm_json["lat"];
							lon = _osm_json["lon"];
						}
						else {
							lat = {};
							lon = {};
						}
						
						{
							auto jNodes = _osm_json["nodes"];
							if (!jNodes.empty()) {
								nodes = jNodes.get<std::vector<size_t>>();
							}
						}
						
						{
							auto jMembers = _osm_json["members"];
							members.reserve(jMembers.size());
							for (auto& item : jMembers) {
								members.emplace_back(std::move(item));
							}
						}
				
						if (_osm_json.contains("tags")) {
							
							std::map<std::string, std::string> jTags;
							_osm_json["tags"].get_to(jTags);
							
							tags.Reserve(jTags.size());
							for (auto& item : jTags) {
								tags.Assign((std::basic_string<char>&&)item.first, std::move(item.second));
							}
						}
					}
					
					bool operator == (const Element& _other) const { return id == _other.id; }
				    bool operator != (const Element& _other) const { return id != _other.id; }
	            };
		        
		        /**
		         * @brief Metadata about the OSM JSON file.
		         */
	            struct Osm3s final {
					
		            const std::string timestamp_osm_base, copyright;
					
		            explicit Osm3s(json&& _osm_json) :
						timestamp_osm_base(Utils::As<std::string>(_osm_json["timestamp_osm_base"])),
						         copyright(Utils::As<std::string>(_osm_json["copyright"         ])) {}
	            };
				
				const std::string version, generator;
				
				const Osm3s osm3s;
				
	            std::vector<Element> elements;
		        
		        Root(json&& _osm_json) :
					  version(Utils::As<std::string>(_osm_json["version"  ])),
					generator(Utils::As<std::string>(_osm_json["generator"])),
					    osm3s(std::move(_osm_json["osm3s"])),
					 elements()
				{
					auto jElements = _osm_json["elements"];
			
					elements.reserve(jElements.size());
					for (auto& item : jElements) {
						elements.emplace_back(std::move(item));
					}
				}
	        };
		
	        Root m_Root;
			
			OSMJSON(json&& _osm_json) : m_Root(std::move(_osm_json)) {}
		};
		
		template <typename T>
	    static T Deserialise(std::istringstream&& _data) {
			return { json::parse(std::move(_data)) };
		}
	};
	
} // LouiEriksson::Engine::Spatial::Serialisation

#endif //FINALYEARPROJECT_OSMDESERIALISER_HPP