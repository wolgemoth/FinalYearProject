#ifndef FINALYEARPROJECT_OSM_HPP
#define FINALYEARPROJECT_OSM_HPP

#include "../../core/utils/Utils.hpp"
#include "../../core/utils/ThreadUtils.hpp"
#include "../../networking/Requests.hpp"
#include "../maths/Conversions.hpp"

#include <array>
#include <functional>
#include <future>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::Spatial {

	class OSM final {
	
	private:
		
		inline static const std::array<std::string, 2U> s_OverpassAPIInterpreters {
			"https://overpass.kumi.systems/api/interpreter",
            "https://overpass-api.de/api/interpreter"
		};
		
	public:
		
		struct Utils final {
			
			/**
			* @brief Calculates the minimum and maximum elevation from a vector of coordinate points.
			*
			* This function takes a vector of coordinate points as input and returns a vec2 object with the minimum and maximum elevation values. The elevation value is taken from the z-coordinate of each point in the vector.
			*
			* @param[in] _coords The vector of coordinate points.
			* @return A vec2 object representing the minimum and maximum elevation values.
			*/
			template<typename T, glm::precision Q = glm::defaultp>
			static vec2 GetMinMaxElevation(const std::vector<glm::vec<3, T, Q>>& _coords) {
				
				auto result = vec2 {
					 std::numeric_limits<T>::infinity(),
					-std::numeric_limits<T>::infinity()
				};
				
				for (const auto& c : _coords) {
					result.x = std::min(result.x, c.z);
					result.y = std::max(result.y, c.z);
				}
			
				return result;
			}
			
			/**
			 * @brief Tries to convert a string to a value in metres.
			 *
			 * This function takes a string as input and tries to convert it to a value in metres. It supports various formats, such as feet and inches, decimal or integer values, and units of measurement. If the conversion fails, it returns an empty optional.
			 *
			 * @param[in] _str The string to convert.
			 * @return An optional float value representing the converted value in metres.
			 *
			 * @note This function uses regular expressions to match different formats.
			 */
			template<typename T>
			static std::optional<float> TryConvertToMetres(const std::string& _str) {
				
				using Distance = Maths::Conversions::Distance;
				
		        std::optional<T> result;
		        
				try {
					result = Distance::Convert(
						Engine::Utils::Parse<T>(_str),
						Distance::TryGuessUnit(_str).value_or(Distance::Unit::Metre),
						Distance::Unit::Metre
					);
				}
				catch (const std::exception& e) {
					Debug::Log(e);
					
		            result = std::nullopt;
				}
		        
		        return result;
			}
		};
		
		/**
		* @brief Asynchronously sends a query request to the Overpass API using the given bounding box and timeout.
		*
		* @param[in] _bounds  The bounding box defining the area of interest.
		* @param[in] _timeout  The maximum time to wait for the response in seconds.
		* @param     _callback The callback function that will be called with the response object.
		* @return A std::future<void> representing the status of the asynchronous operation.
		*/
		template<typename T = float, glm::precision Q = glm::defaultp>
		static std::future<void> QueryOverpassBoundingBoxAsync(const glm::vec<4, T, Q>& _bounds, const std::chrono::system_clock::duration& _timeout, std::function<void(const Networking::Requests::Response&)> _callback, Threading::Utils::CancellationToken& _cancellationToken) { {
		
				std::ostringstream message;
				message <<
					"[out:json][timeout:" << std::chrono::duration_cast<std::chrono::seconds>(_timeout).count() << "][maxsize:67108864];\n"
			        "(\n"
					"    node(" << _bounds.x << ", " << _bounds.y << ", " << _bounds.z << ", " << _bounds.w << ");\n"
			        "    <;\n"
			        ");\n"
			        "out;";
				
		        // Query the Overpass API:
		        return OSM::OverpassQueryAsync(message.str(), _timeout, std::move(_callback), _cancellationToken);
		    }
		}
		
		/**
		* @brief Asynchronously sends an Overpass query request using the given request string, timeout, and callback function.
		*
		* @param[in] _request  The query request string.
		* @param[in] _timeout  The maximum time to wait for the response in seconds.
		* @param     _callback The callback function that will be called with the response object.
		* @return A std::future<void> representing the status of the asynchronous operation.
		*/
	    static std::future<void> OverpassQueryAsync(const std::string& _request, const std::chrono::system_clock::duration& _timeout, std::function<void(const Networking::Requests::Response&)> _callback, Threading::Utils::CancellationToken& _cancellationToken) {
			
			return std::async([_request, _timeout, _callback, &_cancellationToken]() {
				
		         try {
					 
			        auto message = Networking::Requests::Client(s_OverpassAPIInterpreters[1]);
					message.Set(CURLOPT_TIMEOUT, std::chrono::duration_cast<std::chrono::seconds>(_timeout).count());
					
					const auto data = Engine::Utils::Minimise(_request);
					message.Set(CURLOPT_POSTFIELDS, data.c_str());
		
			        auto task = message.SendAsync();
			        
					const auto status = Threading::Utils::Wait(task, _timeout, _cancellationToken);
					
					Networking::Requests::Response response;
					if (status == std::future_status::ready) {
						response = task.get();
					}
					else if (status == std::future_status::timeout) {
						Debug::Log("OpenStreetMap Query timeout!", LogType::Error);
					}
					else {
						Debug::Log("OpenStreetMap Query failure!", LogType::Error);
					}
					
					if (_callback != nullptr) {
		                _callback(response);
					}
		        }
		        catch (const std::exception& e) {
					
					Debug::Log(e);
					
					if (_callback != nullptr) {
		                _callback({});
					}
		        }
			});
	    }
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_OSM_HPP