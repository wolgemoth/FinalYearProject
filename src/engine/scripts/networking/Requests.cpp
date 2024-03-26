#include "Requests.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace LouiEriksson::Engine::Networking {
	
	Requests::Response::Response() :
		m_Status(),
		m_Message() {}
	
	Requests::Response::Response(const CURLcode& _status, std::string& _message) :
		m_Status(_status),
		m_Message(_message) {}
	
	bool Requests::Response::Success(const Requests::Response& _response, const bool& _verbose) {
		
		const bool result = _response.Status() == CURLE_OK;
		
		if (!result && _verbose) {
			std::cerr << "Request failed. Status code: " << _response.Status() << ". Meaning: \"" << curl_easy_strerror(_response.Status()) << "\"";
		}
		
		return result;
	}
	
	const CURLcode& Requests::Response::Status() const noexcept {
		return m_Status;
	}
	
	const std::string& Requests::Response::Message() const noexcept {
		return m_Message;
	}
	
	Requests::Handle_t Requests::Handle() {
		
		const Handle_t result(curl_easy_init(), [](void* _ptr) {
			if (_ptr != nullptr) {
				curl_easy_cleanup(_ptr);
			}
		});
		
		return result;
	}
	
	std::size_t Requests::WriteFunction(void* contents, std::size_t _size, std::size_t _nmemb, std::string* response) {
		
		const auto result = _size * _nmemb;
	 
		response->append(reinterpret_cast<char*>(contents), result);
	 
		return result;
	}
	
	void Requests::Get(const std::string _url, const std::string _payload, void (*_callback)(const Response&)) {
		
		try {
			
			if (_callback != nullptr) {
				
				auto handle = Handle();
				
				if (handle) {
					
					Response response;
					
					// Set parameters:
					curl_easy_setopt(handle.get(), CURLOPT_URL,                  _url.c_str());
					curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION,       WriteFunction);
					curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA,     &response.m_Message);
					
					// Perform request:
					response.m_Status = curl_easy_perform(handle.get());
					
					_callback(response);
				}
				else {
					throw std::runtime_error("Failed creating a handle for the request!");
				}
			}
			else {
				throw std::runtime_error("Callback cannot be null!");
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
	void Requests::Post(const std::string _url, const std::string _payload, void (*_callback)(const Response&)) {
		
		try {
			
			if (_callback != nullptr) {
				
				auto handle = Handle();
				
				if (handle) {
					
					Response response;
					
					// Set parameters:
					curl_easy_setopt(handle.get(), CURLOPT_URL,                  _url.c_str());
					curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION,       WriteFunction);
					curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA,     &response.m_Message);
			        curl_easy_setopt(handle.get(), CURLOPT_POSTFIELDS,       _payload.c_str());
					
					// Perform request:
					response.m_Status = curl_easy_perform(handle.get());
					
					_callback(response);
				}
				else {
					throw std::runtime_error("Failed creating a handle for the request!");
				}
			}
			else {
				throw std::runtime_error("Callback cannot be null!");
			}
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
} // LouiEriksson::Engine::Networking