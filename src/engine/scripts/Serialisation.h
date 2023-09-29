#pragma once

#ifndef _SERIALISATION_H
#define _SERIALISATION_H

#include "stdafx.h"

#include "Transform.h"
#include "Rigidbody.h"

class Serialisation {

private:

	 Serialisation() = delete;
	 Serialisation(const Serialisation& _other) = delete;
	~Serialisation() = delete;

	Serialisation& operator = (const Serialisation& _other) = delete;

	static std::vector<std::string> Split(const std::string& _string, char _delimiter);

public:

	enum Format {
		Text,
	};

	/// <summary>
	/// Placeholder for an actual custom exception.
	/// </summary>
	static void NotImplementedException(std::string _type);

	static std::string ParseNext(cereal::XMLInputArchive& _xml, int _log = 0);

	static std::string Serialise(const glm::vec3& _vec3, const Format& _method = Format::Text);

	static std::string Serialise(const glm::quat& _quat, const Format& _method = Format::Text);

	template <typename T>
	static T Deserialise(const std::string& _str, const Format& _method = Format::Text) {
		
		T result;

		char delimiter = ' ';

		auto split = Split(_str, delimiter);
		for (size_t i = 0; i < split.size(); i++) {
			result[i] = std::stof(split[i]);
		}

		return result;
	}

	template<>
	static float Deserialise(const std::string& _str, const Format& _method) {
		return std::stof(_str);
	}
};


#endif // !_SERIALISATION_H
