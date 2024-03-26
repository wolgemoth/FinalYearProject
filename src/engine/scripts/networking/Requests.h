#ifndef FINALYEARPROJECT_REQUESTS_H
#define FINALYEARPROJECT_REQUESTS_H

#include <curl/curl.h>

#include <cstddef>
#include <memory>
#include <string>

namespace LouiEriksson::Engine::Networking {
	
	struct Requests {
		
		using Handle_t = std::shared_ptr<void>;
	
		struct Response {
			
			friend Requests;
			
		private:
			
			CURLcode    m_Status;
			std::string m_Message;
			
			Response();
			Response(const CURLcode& _status, std::string& _message);
			
		public:
			
			[[nodiscard]] const CURLcode& Status() const noexcept;
			
			[[nodiscard]] const std::string& Message() const noexcept;
			
			static bool Success(const Requests::Response& _response, const bool& _verbose = true);
			
		};
		
		static Handle_t Handle();
		
		static std::size_t WriteFunction(void* contents, std::size_t  _size, std::size_t _nmemb, std::string* response);

		static void Get(const std::string _url, const std::string _payload, void (*_callback)(const Response&));
	
		static void Post(const std::string _url, const std::string _payload, void (*_callback)(const Response&));
		
	};
	
} // LouiEriksson::Engine::Networking

#endif //FINALYEARPROJECT_REQUESTS_H