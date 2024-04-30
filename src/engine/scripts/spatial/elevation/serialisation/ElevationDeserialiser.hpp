#ifndef FINALYEARPROJECT_ELEVATIONDESERIALISER_HPP
#define FINALYEARPROJECT_ELEVATIONDESERIALISER_HPP

#include "../../../core/utils/Utils.hpp"

#include <json.hpp>

#include <sstream>
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;

namespace LouiEriksson::Engine::Spatial::Serialisation {

	class ElevationDeserialiser final {
	
	public:
	
		struct ElevationJSON {
			
			struct ElevationJSONRoot {
				
				struct ElevationJSONResult {
					
					[[nodiscard]] virtual const scalar_t&       Lat() const noexcept = 0;
					[[nodiscard]] virtual const scalar_t&      Long() const noexcept = 0;
					[[nodiscard]] virtual const scalar_t& Elevation() const noexcept = 0;
				};
			};
		};
		
		struct OEJSON final : ElevationJSON {
			
	        struct Root final : ElevationJSONRoot {
	
	            struct Result final : ElevationJSONResult {
					
	                const scalar_t latitude, longitude, elevation;
		            
		            [[nodiscard]] const scalar_t&       Lat() const noexcept final { return  latitude; }
					[[nodiscard]] const scalar_t&      Long() const noexcept final { return longitude; }
					[[nodiscard]] const scalar_t& Elevation() const noexcept final { return elevation; }
					
		            explicit Result(json&& _oe_json) :
						 latitude(_oe_json["latitude" ]),
						longitude(_oe_json["longitude"]),
						elevation(_oe_json["elevation"]) {}
	            };
				
	            std::vector<Result> results;
	
				explicit Root(json&& _oe_json) : results()
				{
					auto jResults = _oe_json["results"];
					
					results.reserve(jResults.size());
					for (auto& item : jResults) {
						results.emplace_back(std::move(item));
					}
				}
	        };
		
	        const Root m_Root;
	        
	        OEJSON(json&& _oe_json) : m_Root(std::move(_oe_json)) {}
		};
		
		struct OTDJSON final : ElevationJSON {
   
			struct Root final : ElevationJSONRoot {
	            
	            struct Result final : ElevationJSONResult {
		            
		            struct Location final {
		                
		                scalar_t lat, lng;
						
		                explicit Location(json&& _otd_json) :
							lat(_otd_json["lat"]),
							lng(_otd_json["lng"]) {}
	                };
					
	                std::string dataset;
					
	                scalar_t elevation;
	                
	                Location location;
		            
		            [[nodiscard]] const scalar_t&       Lat() const noexcept final { return location.lat; }
					[[nodiscard]] const scalar_t&      Long() const noexcept final { return location.lng; }
					[[nodiscard]] const scalar_t& Elevation() const noexcept final { return elevation;    }
					
		            explicit Result(json&& _otd_json) :
						  dataset(Utils::As<std::string>(_otd_json["dataset"])),
						elevation(_otd_json["elevation"]),
						 location(std::move(_otd_json["location"])) {}
	            };
	            
	            std::vector<Result> results;
	
				explicit Root(json&& _otd_json) : results()
				{
					auto jResults = _otd_json["results"];
					
					results.reserve(jResults.size());
					for (auto& item : jResults) {
						results.emplace_back(std::move(item));
					}
				}
				
				explicit Root(std::vector<Result>&& _results) : results(_results) {}
	        };
			
	        Root m_Root;
	        
	        OTDJSON(json&& _otd_json) : m_Root(std::move(_otd_json)) {}
		};
		
		template <typename T>
	    static constexpr T Deserialise(std::istringstream&& _data) {
			return { json::parse(_data) };
		}
	};
	
} // LouiEriksson::Engine::Spatial::Serialisation

#endif //FINALYEARPROJECT_ELEVATIONDESERIALISER_HPP