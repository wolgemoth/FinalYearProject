#ifndef FINALYEARPROJECT_SERIALISATION_H
#define FINALYEARPROJECT_SERIALISATION_H

#include "../core/utils/Utils.h"

#include <cereal/archives/xml.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>

#include <string>

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	class Serialisation {
	
	public:
		
		/// <summary> The type of serialisation format. </summary>
		enum Format : char {
			
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
	
			const char delimiter = ' ';
	
			auto split = Utils::Split(_str, delimiter);
			for (auto i = 0; i < split.size(); i++) {
				result[i] = std::stof(split[i]);
			}
	
			return result;
		}
	};
	
	template<>
	inline float Serialisation::Deserialise(const std::string& _str, const Serialisation::Format& _method) {
		return std::stof(_str);
	}

} // LouiEriksson

#endif //FINALYEARPROJECT_SERIALISATION_H