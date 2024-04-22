#ifndef FINALYEARPROJECT_SERIALISATION_HPP
#define FINALYEARPROJECT_SERIALISATION_HPP

#include "utils/Utils.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cereal/archives/xml.hpp>

#include <string>
#include <string_view>

namespace LouiEriksson::Engine {
	
	/**
	 * @class Serialisation
	 * @brief The Serialisation class provides methods for serializing and deserializing data.
	 */
	class Serialisation final {
	
	public:
		
		/**
		 * @brief The type of serialisation format.
		 */
		enum Format : unsigned char {
			XML, /**< @brief XML format. */
		};
		
		 Serialisation()                            = delete;
		 Serialisation(const Serialisation& _other) = delete;
		~Serialisation()                            = delete;
		
		Serialisation& operator = (const Serialisation& _other) = delete;
		
		/**
		 * @fn std::string Serialisation::ParseNext(cereal::XMLInputArchive& _xml, int _log)
		 * @brief Parse the next element from the XML input archive.
		 *
		 * This function parses the next element from the given XML input archive and returns the parsed value as a string.
		 *
		 * @param[in] _xml The XML input archive to parse.
		 * @param[in] _log Amount of indentation to apply when logging.
		 * @return The parsed value as a string.
		 *
		 * @note This function assumes that the XML input archive is positioned on an element.
		 * @note The XML input archive's current node is moved to the next element after parsing.
		 */
		static std::string ParseNext(cereal::XMLInputArchive& _xml, const int& _log = 0) {
	
			std::string result;
			_xml(result);
		
			if (_log >= 0) {
		
				const auto* const name = _xml.getNodeName();
				
				if (name != nullptr) {
					
					for (auto i = 0; i < _log; ++i) {
						Debug::Log("\t", LogType::Info, true);
					}
				
					Debug::Log(std::string(name) + " " + "\"" + result + "\"", LogType::Info);
				}
			}
		
			return result;
		}
	
		static std::string Serialise(const glm::vec3& _value, const Format& _method = Format::XML) {
		
			std::stringstream result;
		
			if (_method == Format::XML) {
				result << _value.x << " " << _value.y << " " << _value.z;
			}
			else {
				throw std::runtime_error("Binary serialisation not implemented for this data type.");
			}
		
			return result.str();
		}
	
		static std::string Serialise(const glm::quat& _value, const Format& _method = Format::XML) {
		
			std::stringstream result;
		
			if (_method == Format::XML) {
				result << _value.x << " " << _value.y << " " << _value.z << " " << _value.w;
			}
			else {
				throw std::runtime_error("Binary serialisation not implemented for this data type.");
			}
		
			return result.str();
		}
		
		/**
		 * @brief Deserialise data from a string.
		 *
		 * This function deserialises data from a string and returns the deserialised object.
		 * By default, the data is expected to be in XML format.
		 *
		 * @tparam T The type of object to deserialise.
		 * @param[in] _str The string containing the data to deserialise.
		 * @param[in] _method The format of the input data (default is XML).
		 * @return The deserialised object of type T.
		 *
		 * @note The resulting object may not be fully populated if the input string is incomplete or invalid.
		 */
		template <typename T>
		static T Deserialise(const std::string_view& _str, const Format& _method = Format::XML) {
			
			// TODO: Add support for deserialisation of different formats!
			
			T result;
	
			const char delimiter = ' ';
	
			const auto split = Utils::Split(_str, delimiter);
			for (size_t i = 0U; i < split.size(); ++i) {
				result[i] = Utils::TryParse<float>(split[i]).value_or(0.0);
			}
	
			return result;
		}
	};
	
	template<>
	inline float Serialisation::Deserialise(const std::string_view& _str, const Serialisation::Format& _method) {
		return Utils::Parse<float>(_str.data());
	}

} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_SERIALISATION_HPP