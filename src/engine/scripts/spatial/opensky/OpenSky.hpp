#ifndef FINALYEARPROJECT_OPENSKY_HPP
#define FINALYEARPROJECT_OPENSKY_HPP

#include "../../core/utils/ThreadUtils.hpp"
#include "../../networking/Requests.hpp"

#include "serialisation/OpenSkyDeserialiser.hpp"

namespace LouiEriksson::Engine::Spatial {
	
	struct OpenSky final {
	
	private:
		
		inline static const std::string s_URL      = "https://opensky-network.org/api/";
		inline static const std::string s_Endpoint = "states/all";
	
	public:
		
		template <typename T, glm::qualifier Q = glm::defaultp>
		static std::future<void> QueryBoundingBoxAsync(const glm::vec<4, T, Q>& _bounds, const std::chrono::system_clock::duration& _timeout, const std::function<void(const Serialisation::OpenSkyDeserialiser::OpenSkyJSON::Root&)>& _callback, Threading::Utils::CancellationToken& _cancellationToken) {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
			return std::async([_bounds, _callback, _timeout, &_cancellationToken]() {
				
				try {
					
					std::ostringstream query;
					query << s_URL
					      << s_Endpoint
					      << "?"
					      <<  "lamin=" << std::to_string(_bounds.x)
					      << "&lomin=" << std::to_string(_bounds.y)
					      << "&lamax=" << std::to_string(_bounds.z)
					      << "&lomax=" << std::to_string(_bounds.w);
					
					auto message = Networking::Requests::Client(query.str());
					message.Set(CURLOPT_TIMEOUT, std::chrono::duration_cast<std::chrono::seconds>(_timeout).count());
					
					auto task = message.SendAsync();
					
					const auto status = Threading::Utils::Wait(task, _timeout, _cancellationToken);
					
					if (status == std::future_status::ready) {
					
						auto result = Serialisation::OpenSkyDeserialiser::Deserialise<Serialisation::OpenSkyDeserialiser::OpenSkyJSON>(task.get().Content().ToStream());

						if (_callback != nullptr) {
							_callback(result.m_Root);
						}
					}
					else if (status == std::future_status::timeout) {
						Debug::Log("OpenSky Query timeout!", Error);
					}
					else {
						Debug::Log("OpenSky Query failure!", Error);
					}
				}
				catch (const std::exception& e) {
					Debug::Log(e);
				}
			});
		}
		
	};

} // LouiEriksson::Engine::Spatial

#endif //FINALYEARPROJECT_OPENSKY_HPP