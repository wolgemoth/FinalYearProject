#include "stdafx.h"

#include "Serialisation.h"

namespace LouiEriksson {
	
	std::vector<std::string> Serialisation::Split(const std::string& _string, char _delimiter) {
	
		std::vector<std::string> result;
	
		std::stringstream stream(_string);
	
		std::string segment;
		while (std::getline(stream, segment, _delimiter)) {
			result.push_back(segment);
		}
	
		return result;
	}
	
	void Serialisation::NotImplementedException(const std::string& _type) {
	
		std::stringstream err;
		err << "ERROR (Scene.cpp [TryLoad(std::filesystem::path)]): Deserialisation for type\"" <<
			_type << "\" Has not been implemented.";
	
		throw std::runtime_error(err.str());
	}
	
	std::string Serialisation::ParseNext(cereal::XMLInputArchive& _xml, int _log) {
	
		std::string name = _xml.getNodeName();
		std::string result;
		_xml(result);
	
		if (_log >= 0) {
	
			for (size_t i = 0; i < _log; i++) {
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
}