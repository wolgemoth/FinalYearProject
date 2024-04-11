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
	
	Elevation::Heightmap::Heightmap(const std::vector<float>& _pixels, const size_t& _width, const size_t& _height) :
		m_Pixels (_pixels),
		m_Width  (std::max(_width,  static_cast<size_t>(1))),
		m_Height (std::max(_height, static_cast<size_t>(1)))
	{
		if (m_Pixels.empty()) {
			
			Debug::Log("Texture initialised with no pixels!", LogType::Warning);
			
			m_Pixels.emplace_back(1.0);
			m_Width  = 1;
			m_Height = 1;
		}
		else if (m_Width * m_Height != m_Pixels.size()) {
			
			Debug::Log("The width and height of the texture does not match the length of the provided buffer!", LogType::Warning);
			
			m_Pixels.resize(m_Width * m_Height);
		}
	}
	
	size_t Elevation::Heightmap::Width()  const noexcept { return m_Width;  }
	size_t Elevation::Heightmap::Height() const noexcept { return m_Height; }
	
	size_t Elevation::Heightmap::GetIndex(const glm::ivec2& _pixel) const {
		
		const auto index = Utils::To1D(
			{
				static_cast<size_t>(_pixel.x),
				static_cast<size_t>(_pixel.y)
			},
			Width()
		);
		
		return std::clamp(index, static_cast<size_t>(0), m_Pixels.size() - 1);
	}
	
	float Elevation::Heightmap::GetPixel(const glm::ivec2& _pixel) const {
		return m_Pixels[GetIndex(_pixel)];
	}
	
	float Elevation::Heightmap::GetPixelBilinear(const glm::vec2& _uv) const {
		
		const glm::ivec2 p(
			static_cast<int>(_uv.x *  Width()),
			static_cast<int>(_uv.y * Height())
		);
		
		const glm::vec2 d = {
			_uv.x - p.x,
			_uv.y - p.y
		};
		
		const auto A = GetPixel({ p.x - 1, p.y - 1 });
		const auto B = GetPixel({ p.x + 1, p.y - 1 });
		const auto C = GetPixel({ p.x - 1, p.y + 1 });
		const auto D = GetPixel({ p.x + 1, p.y + 1 });
		
		return A * (1.0 - d.x) * (1.0 - d.y) +
		       B * (1.0 - d.y) * d.x +
			   C * (1.0 - d.x) * d.y +
			   D * d.x * d.y;
	}
	
	const std::vector<float>& Elevation::Heightmap::Pixels() const noexcept {
		return m_Pixels;
	}
	
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
				
				auto task = Elevation::PostRequestOpenElevationAsync(_points,
					
					[_callback](const Serialisation::ElevationDeserialiser::OEJSON::Root& _elevation_result) {
							
						auto result = std::vector<float>(_elevation_result.results.size());
						
						for (auto i = 0; i < result.size(); ++i) {
							result[i] = _elevation_result.results[i].elevation;
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
				
				auto task = Elevation::PostRequestOpenTopoDataAsync(_points,
					
					[_callback](const Serialisation::ElevationDeserialiser::OTDJSON::Root& _elevation_result) {
							
						auto result = std::vector<float>(_elevation_result.results.size());
						
						for (auto i = 0; i < result.size(); ++i) {
							result[i] = _elevation_result.results[i].elevation;
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
		 
			Serialisation::ElevationDeserialiser::OEJSON result{{{}}};
			
			if (status == std::future_status::ready) {
				result = Serialisation::ElevationDeserialiser::Deserialise<Serialisation::ElevationDeserialiser::OEJSON>(task.get().Content().ToStream());
			}
			else if (status == std::future_status::timeout) {
				Debug::Log("Operation timeout!", LogType::Debug);
			}
			else {
				Debug::Log("Operation failure!", LogType::Error);
			}
			
			if (_callback != nullptr) {
		        _callback(result.m_Root);
			}
		});
    }

    std::future<void> Elevation::PostRequestOpenTopoDataAsync(const std::vector<glm::vec2>& _request, const std::function<void(const Serialisation::ElevationDeserialiser::OTDJSON::Root&)>& _callback) {
    
		return std::async([_request, _callback]() {
			
			Serialisation::ElevationDeserialiser::OTDJSON result("");
			
			const auto timeout = 60L;
			
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
					result = Serialisation::ElevationDeserialiser::Deserialise<Serialisation::ElevationDeserialiser::OTDJSON>(task.get().Content().ToStream());
				}
				else if (status == std::future_status::timeout) {
					Debug::Log("Operation timeout!", LogType::Debug);
				}
				else {
					Debug::Log("Operation failure!", LogType::Error);
				}
	        }
	        
			if (_callback != nullptr) {
	            _callback(result.m_Root);
			}
		});
    }
} // LouiEriksson::Engine::Spatial