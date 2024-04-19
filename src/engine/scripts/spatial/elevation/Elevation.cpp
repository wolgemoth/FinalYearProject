#include "Elevation.h"

#include "serialisation/ElevationDeserialiser.h"

#include "../../core/utils/Utils.h"
#include "../../networking/Requests.h"
#include "../maths/Coords.h"
#include "../../core/Debug.h"

#include <curl/curl.h>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>

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
	
	std::future<void> Elevation::LoadElevationAsync(const glm::vec4& _bounds, const ElevationProvider& _provider, const glm::ivec2& _dimensions, const std::function<void(const std::vector<float>&)>& _callback) {

		return std::async([_bounds, _provider, _dimensions, _callback]() {
			
	        auto elevation_points = std::vector<glm::vec2>();
	
	        for (auto y = _dimensions.y - 1; y >= 0; --y) {
	        for (auto x = _dimensions.x - 1; x >= 0; --x) {
	            elevation_points.emplace_back(Maths::Coords::GPS::PixelToGPS({ x, y }, _dimensions, _bounds));
	        }}
	
	        if (_provider == ElevationProvider::OpenElevation) {
		        std::reverse(elevation_points.begin(), elevation_points.end());
	        }
	        
	        Elevation::LoadElevation(elevation_points, _bounds, _provider, _callback);
		});
    }
	
    void Elevation::LoadElevation(const std::vector<glm::vec2>& _points, const glm::vec4& _bounds, const ElevationProvider& _provider, const std::function<void(const std::vector<float>&)>& _callback) {
        
        switch (_provider) {
            
			case ElevationProvider::OpenElevation: {
				
				const auto task = Elevation::PostRequestOpenElevationAsync(_points,
					
					[_callback](const Serialisation::ElevationDeserialiser::OEJSON::Root& _elevation_result) {
							
						std::vector<float> result;
						
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
							
						std::vector<float> result;
						
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

    std::future<void> Elevation::PostRequestOpenElevationAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OEJSON::Root&)>& _callback) {

		return std::async([_request, _callback]() {
			
			const auto timeout = 60L;
			
	        auto message = Networking::Requests::Client("https://api.open-elevation.com/api/v1/lookup");
			message.Set(CURLOPT_TIMEOUT, timeout);
			
	        std::stringstream ss;
	        
	        ss << "{\"locations\":[";
	        
	        for (auto i = 0; i < _request.size(); ++i) {
	            
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
				Debug::Log("OpenElevation Query failure!", LogType::Error);
			}
			else {
				Debug::Log("OpenElevation Query timeout!", LogType::Error);
			}
		});
    }

    std::future<void> Elevation::PostRequestOpenTopoDataAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OTDJSON::Root&)>& _callback) {
    
		return std::async([_request, _callback]() {
			
			std::vector<Serialisation::ElevationDeserialiser::OTDJSON> results;
			
			const auto timeout = 10L;
			
	        auto message = Networking::Requests::Client("https://api.opentopodata.org/v1/mapzen");
			message.Set(CURLOPT_TIMEOUT, timeout);
			
			auto header = Networking::Requests::Client::Header();
			message.Set(CURLOPT_HTTPHEADER, curl_slist_append(header.get(), "Content-Type: application/json"));
	
	        const auto requestIntervalMs = std::chrono::milliseconds(500);
			
	        const size_t maxRequestSize = 100;
	        for (auto i = 0; i < _request.size(); i += maxRequestSize) {
	
	            std::stringstream ss;
	
	            ss << R"({"locations": ")";
	
	            const auto count = std::min(_request.size() - i, maxRequestSize);
	            
	            for (auto j = 0; j < count; j++) {
	
	                auto location = _request[i + j];
	
					ss << location.x << ',' << location.y;
					
	                if (j != _request.size() - 1) {
						ss << '|';
	                }
	            }
	
				ss << R"(", "interpolation": "cubic"})";
				
	            if (i != 0 && i + maxRequestSize < (_request.size() - 1)) {
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
					Debug::Log("OpenTopoData Query failure!", LogType::Error);
				}
				else {
					Debug::Log("OpenTopoData Query timeout!", LogType::Error);
				}
	        }
			
			if (_callback != nullptr) {
				
				if (!results.empty()) {
					
					auto result = *results.begin();
					
					for (auto i = 1; i < results.size(); ++i) {
						Utils::MoveInto(results[i].m_Root.results, result.m_Root.results);
					}
					
	                _callback(result.m_Root);
				}
			}
		});
    }
	
} // LouiEriksson::Engine::Spatial