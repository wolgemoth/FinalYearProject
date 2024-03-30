#include "OSM_Utils.h"

#include "../../maths/Conversions.h"

#include <glm/ext/vector_float3.hpp>

#include <algorithm>
#include <exception>
#include <iostream>
#include <limits>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

namespace LouiEriksson::Engine::Spatial::OSM {
	
	auto Utils::GetMinMaxElevation(const std::vector<glm::vec3>& _coords) {
		
		auto result = glm::vec2 {
			 std::numeric_limits<float>::infinity(),
			-std::numeric_limits<float>::infinity()
		};
	
		for (const auto& _coord : _coords) {
			result.x = std::max(result.x, _coord.z);
			result.y = std::min(result.y, _coord.z);
		}
	
		return result;
	}
	
	bool Utils::TryConvertToMetres(const std::string& _str, double& _output) {
		
		using Distance = Maths::Conversions::Distance;
		
        bool result = true;
        
		try {
		
			std::smatch match;
			
	        // Check first if the number is in the special feet (') and inches (") format.
			if (std::regex_match(_str, match, std::regex(R"(^(\d*\.\d+|\d+)('|"")(?:(\d*\.\d+|\d+)(?!2)"")?)"))) {
	
	            double inches;
	
	            // Extract the information and convert to inches only.
			    if (match[2].str() == "'") {
	
				    inches = std::stof(match[1].str()) * 12;
	
				    if (match.size() != 1) {
					    inches += std::stof(match[3].str());
				    }
			    }
			    else {
				    inches = std::stof(match[1].str());
			    }
	
	            // Convert the inches into metres.
				_output = Distance::Convert(inches, Distance::Unit::Inch, Distance::Unit::Metre);
		    }
			else {
	            
	            // Check if the number is in the standard decimal/integer format.
	            if (std::regex_match(_str, match, std::regex(R"(^(\d*\.\d+|\d+)"))) {
	                
	                _output = std::stod(match.str());
	                
	                // Check if the number is in a specific unit.
	                if (std::regex_match(_str, match, std::regex(R"( \w+$)"))) {
	                
	                    // Attempt to convert the units to metres.
						Distance::Unit unit;
						
						if (Distance::TryGuessUnit(match.str(), unit)) {
							_output = Distance::Convert(_output, unit, Distance::Unit::Metre);
						}
	                }
	            }
	            else {
	            
	                // The number is not in any recognised format.
					throw std::runtime_error("Format exception: " + _str);
	            }
	        }
		}
		catch (const std::exception& e) {
			
			std::cerr << e.what() << std::endl;
	           
            result = false;
			
            _output = {};
		}
        
        return result;
	}
	
} // LouiEriksson::Engine::Spatial::OSM