#include "Serialisation.h"

#include <cereal/archives/xml.hpp>

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>

#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace LouiEriksson::Engine {
	
	std::string Serialisation::ParseNext(cereal::XMLInputArchive& _xml, int _log) {
	
		const auto *const name = _xml.getNodeName();
		
		std::string result;
		_xml(result);
	
		if (_log >= 0) {
	
			for (size_t i = 0; i < _log; ++i) {
				std::cout << "\t";
			}
	
			std::cout << name << " " << "\"" << result << "\"\n";
		}
	
		return result;
	}
	
	std::string Serialisation::Serialise(const glm::vec3& _value, const Format& _method) {
	
		std::stringstream result;
	
		if (_method == Format::XML) {
			result << _value.x << " " << _value.y << " " << _value.z;
		}
		else {
			throw std::runtime_error("Binary serialisation not implemented for this data type.");
		}
	
		return result.str();
	}
	
	std::string Serialisation::Serialise(const glm::quat& _value, const Format& _method) {
	
		std::stringstream result;
	
		if (_method == Format::XML) {
			result << _value.x << " " << _value.y << " " << _value.z << " " << _value.w;
		}
		else {
			throw std::runtime_error("Binary serialisation not implemented for this data type.");
		}
	
		return result.str();
	}

} // LouiEriksson::Engine