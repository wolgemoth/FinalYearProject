#pragma once

#ifndef FINALYEARPROJECT_SERIALISATION_H
#define FINALYEARPROJECT_SERIALISATION_H

#include "stdafx.h"

#include "Transform.h"
#include "Rigidbody.h"

namespace LouiEriksson {
	
	class Serialisation {
	
	private:
	
		static std::vector<std::string> Split(const std::string& _string, char _delimiter);
	
	public:
		
		/// <summary> The type of serialisation format. </summary>
		enum Format {
			/// <summary> XML format </summary>
			XML,
		};
		
		 Serialisation()                            = delete;
		 Serialisation(const Serialisation& _other) = delete;
		~Serialisation()                            = delete;
		
		Serialisation& operator = (const Serialisation& _other) = delete;
		
		/// <summary>
		/// Placeholder for an actual custom exception.
		/// </summary>
		static void NotImplementedException(const std::string& _type);
	
		static std::string ParseNext(cereal::XMLInputArchive& _xml, int _log = 0);
	
		static std::string Serialise(const glm::vec3& _value, const Format& _method = Format::XML);
	
		static std::string Serialise(const glm::quat& _value, const Format& _method = Format::XML);
	
		template <typename T>
		static T Deserialise(const std::string& _str, const Format& _method = Format::XML) {
			
			T result;
	
			char delimiter = ' ';
	
			auto split = Split(_str, delimiter);
			for (auto i = 0; i < split.size(); i++) {
				result[i] = std::stof(split[i]);
			}
	
			return result;
		}
	
		template<>
		float Deserialise(const std::string& _str, const Format& _method) {
			return std::stof(_str);
		}
	};
}

#endif //FINALYEARPROJECT_SERIALISATION_H