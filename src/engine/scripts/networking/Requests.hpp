#ifndef FINALYEARPROJECT_REQUESTS_HPP
#define FINALYEARPROJECT_REQUESTS_HPP

#include "../core/Debug.hpp"

#include <curl/curl.h>
#include <curl/easy.h>

#include <cstddef>
#include <exception>
#include <future>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace LouiEriksson::Engine::Networking {
	
	/*
	 * Note to self:
	 *      To debug, open terminal and type "nc -l 8080" to listen to port 8080 with netcat.
	 *      you can then send requests to "http://localhost:8080/" and see the output in realtime.
	 *      Interrupting operation with Ctrl+D enables user to type a custom response.
	 */
	
	struct Requests final {
		
		class Client;
		
		struct Response final {
			
			friend Requests;
			
		public:
			
			class Data final {
			
				friend Client;
				
			private:
				
				std::vector<char> m_Raw;
				
			public:
				
				[[nodiscard]] constexpr const std::vector<char>& Raw() const { return m_Raw; }
				
				[[nodiscard]] std::istringstream ToStream() const {
					
					std::ostringstream ss;
					
					for (const auto& item : m_Raw) {
						ss << item;
					}
					
					return std::istringstream { ss.str() };
				}
				
				Data() noexcept : m_Raw() {}
			};
			
			Response() noexcept :
				m_Status(CURL_LAST),
				m_Content() {}
				
			[[nodiscard]] constexpr const CURLcode&  Status() const noexcept { return  m_Status; }
			[[nodiscard]] constexpr const     Data& Content() const noexcept { return m_Content; }
			
			static bool Success(const Requests::Response& _response, const bool& _verbose = true) {
				
				const bool result = _response.Status() == CURLE_OK;
				
				Debug::Assert(
					result || !_verbose,
					"Request failed. Status code: " + std::to_string(_response.Status()) + ". Meaning: \"" + curl_easy_strerror(_response.Status()) + "\"",
					LogType::Error
				);
				
				return result;
			}
			
		private:
			
			CURLcode m_Status;
			Data     m_Content;
			
		};
		
		class Client final {
		
		private:
			
			using Handle_t = std::shared_ptr<void>;
			using Header_t = std::shared_ptr<curl_slist>;
			
			Handle_t m_Handle;
			
		public:
			
			[[nodiscard]] const Handle_t& Handle() {
				
				if (m_Handle == nullptr) {
					m_Handle.reset(curl_easy_init(), [](void* _ptr) { if (_ptr != nullptr) { curl_easy_cleanup(_ptr); }});
				}
				
				return m_Handle;
			}
			
			[[nodiscard]] static Header_t Header() {
				return Header_t({}, [](curl_slist* _ptr) { if (_ptr != nullptr) { curl_slist_free_all(_ptr); }});
			}
			
			template <typename T>
			constexpr void Set(const CURLoption& _option, const T& _value) {
				curl_easy_setopt(Handle().get(), _option, _value);
			}
			
			template <typename T>
			[[nodiscard]] constexpr T Get(const CURLINFO _info) {
				
				T result;
				
				curl_easy_getinfo(Handle().get(), _info, &result);
				
				return result;
			}
			
			Response Send() {
			
				Response r;
				
				try {
					
					// Set parameters:
					Set(CURLOPT_WRITEFUNCTION, WriteFunction);
					Set(CURLOPT_WRITEDATA, &r.m_Content.m_Raw);
					
					// Perform request:
					r.m_Status = curl_easy_perform(Handle().get());
				}
				catch (const std::exception& e) {
					Debug::Log(e);
				}
				
				return r;
			}
			
			std::future<Requests::Response> SendAsync() {
				
				return std::async([this]() {
					return Send();
				});
			}
			
			void Dispose() noexcept {
				m_Handle.reset();
			}
			
			constexpr Client() : m_Handle() {}
			constexpr Client(const std::string_view& _uri) : m_Handle() {
				Set(CURLOPT_URL, _uri.data());
			}
			
		};
		
		static size_t WriteFunction(void* _data, size_t _stride, size_t _count, std::vector<char>* _userData) {
		
			const auto len = _stride * _count;
		 
			const auto* const addr = reinterpret_cast<char*>(_data);
			_userData->insert(_userData->end(), addr, addr + len);
			
			return len;
		}
		
		static void Init() {
			curl_global_init(CURL_GLOBAL_ALL);
		}
		
		static void Dispose() noexcept {
			
			try {
	            curl_global_cleanup();
			}
			catch (const std::exception& e) {
				Debug::Log(e, LogType::Critical);
			}
		}
	};
	
} // LouiEriksson::Engine::Networking

#endif //FINALYEARPROJECT_REQUESTS_HPP