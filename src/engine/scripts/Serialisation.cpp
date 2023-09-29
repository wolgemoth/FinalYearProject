#include "stdafx.h"

#include "Serialisation.h"

std::vector<std::string> Serialisation::Split(const std::string& _string, char _delimiter) {

	std::vector<std::string> result;

	std::stringstream stream(_string);

	std::string segment;
	while (std::getline(stream, segment, _delimiter)) {
		result.push_back(segment);
	}

	return result;
}

void Serialisation::NotImplementedException(std::string _type) {

	std::stringstream err;
	err << "ERROR (Scene.cpp [Load(std::filesystem::path)]): Deserialisation for type\"" <<
		_type << "\" Has not been implemented.";

	throw std::runtime_error(err.str());
}

std::string Serialisation::ParseNext(cereal::XMLInputArchive& _xml, int _log) {

	std::string name = _xml.getNodeName();
	std::string result;
	_xml(result);

	if (_log > 0) {

		for (size_t i = 0; i < _log; i++) {
			std::cout << "\t";
		}

		std::cout << name << " " << "\"" << result << "\"\n";
	}

	return result;
}

std::string Serialisation::Serialise(const glm::vec3& _vec3, const Format& _method) {

	std::stringstream result;

	if (_method == Format::Text) {
		result << _vec3.x << " " << _vec3.y << " " << _vec3.z;
	}
	else {
		throw std::exception("Binary serialisation not implemented for this data type.");
	}

	return result.str();
}

std::string Serialisation::Serialise(const glm::quat& _quat, const Format& _method) {

	std::stringstream result;

	if (_method == Format::Text) {
		result << _quat.x << " " << _quat.y << " " << _quat.z << " " << _quat.w;
	}
	else {
		throw std::exception("Binary serialisation not implemented for this data type.");
	}

	return result.str();
}
