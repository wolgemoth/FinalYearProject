#include "Requests.h"

#include "../core/Debug.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include <algorithm>
#include <cstddef>
#include <exception>
#include <future>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace LouiEriksson::Engine::Networking {
	
	Requests::Client::Client() : m_Handle() {}
	
	Requests::Client::Client(const std::string& _uri) : m_Handle() {
		Set(CURLOPT_URL, _uri.c_str());
	}
	
	Requests::Response::Data::Data() : m_Raw() {}
	
	const Requests::Client::Handle_t& Requests::Client::Handle() {
		
		if (m_Handle == nullptr) {
			m_Handle.reset(curl_easy_init(), [](void* _ptr) { if (_ptr != nullptr) { curl_easy_cleanup(_ptr); }});
		}
		
		return m_Handle;
	}
	
	Requests::Client::Header_t Requests::Client::Header() {
		return Header_t({}, [](curl_slist* _ptr) { if (_ptr != nullptr) { curl_slist_free_all(_ptr); }});
	}
	
	Requests::Response Requests::Client::Send() {
	
		Response r;
		
		try {
			
			// Set parameters:
			Set(CURLOPT_WRITEFUNCTION, WriteFunction);
			Set(CURLOPT_WRITEDATA, &r.m_Content.m_Raw);
			
			// Perform request:
			r.m_Status = curl_easy_perform(Handle().get());
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		
		return r;
	}
	
	std::future<Requests::Response> Requests::Client::SendAsync() {
		
		return std::async([this]() {
			return Send();
		});
	}
	
	void Requests::Client::Dispose() {
	
		try {
			m_Handle.reset();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
	Requests::Response::Response() :
		m_Status(CURLcode(-1)),
		m_Content() {}
	
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
	
	const Requests::Response::Data& Requests::Response::Content() const noexcept {
		return m_Content;
	}
	
	const std::vector<char>& Requests::Response::Data::Raw() const {
		return m_Raw;
	}
	
	std::istringstream Requests::Response::Data::ToStream() const {
		
		std::stringstream ss;
		
		for (const auto& item : m_Raw) {
			ss << item;
		}
		
		return std::istringstream { ss.str() };
	}
	
	size_t Requests::WriteFunction(void* _data, size_t _stride, size_t _count, std::vector<char>* _userData) {
		
		const auto len = _stride * _count;
	 
		const auto* addr = reinterpret_cast<char*>(_data);
		_userData->insert(_userData->end(), addr, addr + len);
		
		return len;
	}
	
	void Requests::Init() {
		curl_global_init(CURL_GLOBAL_ALL);
	}
	
	void Requests::Dispose() {
		
		try {
            curl_global_cleanup();
		}
		catch (std::exception& e) {
			Debug::Log(e, Debug::LogType::Critical);
		}
	}
	
} // LouiEriksson::Engine::Networking