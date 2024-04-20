#ifndef FINALYEARPROJECT_ELEVATIONDESERIALISER_H
#define FINALYEARPROJECT_ELEVATIONDESERIALISER_H


#include <json.hpp>

#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace LouiEriksson::Engine::Spatial::Serialisation {

	class ElevationDeserialiser final {
	
	private:
		
        inline static const json s_Serialiser;

	public:
	
		struct ElevationJSON {
			
			struct ElevationJSONRoot {
				
				struct ElevationJSONResult {
					
					[[nodiscard]] virtual const float&       Lat() const noexcept = 0;
					[[nodiscard]] virtual const float&      Long() const noexcept = 0;
					[[nodiscard]] virtual const float& Elevation() const noexcept = 0;
				};
			};
		};
		
		struct OEJSON final : ElevationJSON {
			
	        struct Root final : ElevationJSONRoot {
	
	            struct Result final : ElevationJSONResult {
					
	                float  latitude,
					      longitude,
						  elevation;
		            
		            [[nodiscard]] inline const float&       Lat() const noexcept final { return  latitude; }
					[[nodiscard]] inline const float&      Long() const noexcept final { return longitude; }
					[[nodiscard]] inline const float& Elevation() const noexcept final { return elevation; }
					
		            explicit Result(json&& _oe_json);
	            };
				
	            std::vector<Result> results;
	
				explicit Root(json&& _oe_json);
	        };
		
	        Root m_Root;
	        
	        OEJSON(json&& _oe_json);
		};
		
		struct OTDJSON final : ElevationJSON {
   
			struct Root final : ElevationJSONRoot {
	            
	            struct Result final : ElevationJSONResult {
		            
		            struct Location {
		                
		                float lat,
						      lng;
						
		                explicit Location(json&& _otd_json);
	                };
					
	                std::string dataset;
					
	                float elevation;
	                
	                Location location;
		            
		            [[nodiscard]] inline const float&       Lat() const noexcept final { return location.lat; }
					[[nodiscard]] inline const float&      Long() const noexcept final { return location.lng; }
					[[nodiscard]] inline const float& Elevation() const noexcept final { return elevation;    }
					
		            explicit Result(json&& _otd_json);
	            };
	            
	            std::vector<Result> results;
	
				explicit Root(json&& _otd_json);
				explicit Root(std::vector<Result>&& _results);
	        };
			
	        Root m_Root;
	        
	        OTDJSON(json&& _otd_json);
		};
		
		template <typename T>
	    static constexpr T Deserialise(std::istringstream&& _data) {
			return { json::parse(_data) };
		}
	};
	
} // LouiEriksson::Engine::Spatial::Serialisation

#endif //FINALYEARPROJECT_ELEVATIONDESERIALISER_H