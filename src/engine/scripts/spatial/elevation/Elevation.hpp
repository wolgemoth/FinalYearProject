#ifndef FINALYEARPROJECT_ELEVATION_HPP
#define FINALYEARPROJECT_ELEVATION_HPP

#include "../../core/Debug.hpp"
#include "../../core/Types.hpp"
#include "../../core/utils/Utils.hpp"
#include "../../networking/Requests.hpp"
#include "../maths/Coords.hpp"

#include "serialisation/ElevationDeserialiser.hpp"

#include <curl/curl.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

namespace LouiEriksson::Engine::Spatial {

	class Elevation final {
	
	public:
		
        enum ElevationProvider : unsigned char {
            OpenElevation = 0U, /**< @brief <a href="https://open-elevation.com/">OpenElevation</a> */
            OpenTopoData  = 1U, /**< @brief <a href="https://www.opentopodata.org/">OpenTopoData</a> */
        };
		
		static std::future<void> LoadElevationAsync(const glm::vec4& _bounds, const ElevationProvider& _provider, const glm::ivec2& _dimensions, const std::function<void(const std::vector<glm::vec<1, scalar_t>>&)>& _callback) {
	
			return std::async([_bounds, _provider, _dimensions, _callback]() {
				
				try {
					
			        auto elevation_points = std::vector<glm::vec2>();
			
			        for (long y = _dimensions.y - 1; y >= 0; --y) {
			        for (long x = _dimensions.x - 1; x >= 0; --x) {
			            elevation_points.emplace_back(Maths::Coords::GPS::PixelToGPS({ x, y }, _dimensions, _bounds));
			        }}
			
			        if (_provider == ElevationProvider::OpenElevation) {
				        std::reverse(elevation_points.begin(), elevation_points.end());
			        }
			        
			        Elevation::LoadElevation(elevation_points, _provider, _callback);
				}
				catch (const std::exception& e) {
					Debug::Log(e);
				}
				
			});
	    }
	
	private:
	
        static void LoadElevation(const std::vector<glm::vec2>& _points, const ElevationProvider& _provider, const std::function<void(const std::vector<glm::vec<1, scalar_t>>&)>& _callback) {
	        
	        switch (_provider) {
	        
				case ElevationProvider::OpenElevation: {
					
					const auto task = Elevation::PostRequestOpenElevationAsync(_points,
						
						[_callback](const Serialisation::ElevationDeserialiser::OEJSON::Root& _elevation_result) {
							
							std::vector<glm::vec<1, scalar_t>> result;
							
							result.reserve(_elevation_result.results.size());
							for (const auto& item : _elevation_result.results) {
								result.emplace_back(item.elevation);
							}
							
							if (_callback != nullptr) {
								_callback(result);
							}
						}
					);
					
					task.wait();
	                
	                break;
	            }
				case ElevationProvider::OpenTopoData: {
					
					const auto task = Elevation::PostRequestOpenTopoDataAsync(_points,
						
						[_callback](const Serialisation::ElevationDeserialiser::OTDJSON::Root& _elevation_result) {
							
							std::vector<glm::vec<1, scalar_t>> result;
							
							result.reserve(_elevation_result.results.size());
							for (const auto& item : _elevation_result.results) {
								result.emplace_back(item.elevation);
							}
							
							if (_callback != nullptr) {
								_callback(result);
							}
						}
					);
	    
					task.wait();
					
	                break;
	            }
	            default: {
	               throw std::runtime_error("Not implemented!");
	            }
	        }
	    }
	
        static std::future<void> PostRequestOpenElevationAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OEJSON::Root&)>& _callback) {
	
			return std::async([_request, _callback]() {
				
				try {
					
					const auto timeout = 60L;
					
			        auto message = Networking::Requests::Client("https://api.open-elevation.com/api/v1/lookup");
					message.Set(CURLOPT_TIMEOUT, timeout);
					
			        std::ostringstream ss;
			        
			        ss << "{\"locations\":[";
			        
			        for (size_t i = 0U; i < _request.size(); ++i) {
			            
			            auto location = _request[i];
			            
						ss << "{\"latitude\":" << location.x << ","
						   <<  "\"longitude\":" << location.y << "}";
						
			            if (i != _request.size() - 1) {
			                ss << ',';
			            }
			        }
			        
			        ss << "]}";
					
					const auto data = Utils::Minimise(ss.str());
					message.Set(CURLOPT_POSTFIELDS, data.c_str());
		
			        auto task = message.SendAsync();
					const auto status = task.wait_for(std::chrono::seconds(timeout));
					
					if (status == std::future_status::ready) {
						
						auto result = Serialisation::ElevationDeserialiser::Deserialise<Serialisation::ElevationDeserialiser::OEJSON>(task.get().Content().ToStream());
						
						if (_callback != nullptr) {
					        _callback(result.m_Root);
						}
					}
					else if (status != std::future_status::timeout) {
						throw std::runtime_error("OpenElevation Query failure!");
					}
					else {
						throw std::runtime_error("OpenElevation Query timeout!");
					}
				}
				catch (const std::exception& e) {
					Debug::Log(e);
				}
			});
	    }

        static std::future<void> PostRequestOpenTopoDataAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OTDJSON::Root&)>& _callback) {
	    
			return std::async([_request, _callback]() {
				
				try {
					
					std::vector<Serialisation::ElevationDeserialiser::OTDJSON> results;
					
					const auto timeout = 10L;
					
			        auto message = Networking::Requests::Client("https://api.opentopodata.org/v1/mapzen");
					message.Set(CURLOPT_TIMEOUT, timeout);
					
					auto header = Networking::Requests::Client::Header();
					message.Set(CURLOPT_HTTPHEADER, curl_slist_append(header.get(), "Content-Type: application/json"));
			
			        const auto requestIntervalMs = std::chrono::milliseconds(500);
					
			        const size_t maxRequestSize = 100U;
			        for (size_t i = 0U; i < _request.size(); i += maxRequestSize) {
			
			            std::ostringstream ss;
			
			            ss << R"({"locations": ")";
			
			            const auto count = std::min(_request.size() - i, maxRequestSize);
			            
			            for (size_t j = 0U; j < count; j++) {
			
			                auto location = _request[i + j];
			
							ss << location.x << ',' << location.y;
							
			                if (j != _request.size() - 1U) {
								ss << '|';
			                }
			            }
			
						ss << R"(", "interpolation": "cubic"})";
						
			            if (i != 0U && i + maxRequestSize < (_request.size() - 1U)) {
							std::this_thread::sleep_for(requestIntervalMs);
			            }
						
						const auto data = Utils::Minimise(ss.str());
						message.Set(CURLOPT_POSTFIELDS, data.c_str());
						
			            auto task = message.SendAsync();
						const auto status = task.wait_for(std::chrono::seconds(timeout));
					 
						if (status == std::future_status::ready) {
							results.emplace_back(Serialisation::ElevationDeserialiser::Deserialise<Serialisation::ElevationDeserialiser::OTDJSON>(task.get().Content().ToStream()));
						}
						else if (status != std::future_status::timeout) {
							throw std::runtime_error("OpenTopoData Query failure!");
						}
						else {
							throw std::runtime_error("OpenTopoData Query timeout!");
						}
			        }
					
					if (_callback != nullptr) {
						
						if (!results.empty()) {
							
							auto result = *results.begin();
							
							for (size_t i = 1U; i < results.size(); ++i) {
								Utils::MoveInto(std::move(results[i].m_Root.results), result.m_Root.results);
							}
							
			                _callback(result.m_Root);
						}
					}
				}
				catch (const std::exception& e) {
					Debug::Log(e);
				}
			});
	    }
		
	};
	
} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_ELEVATION_HPP