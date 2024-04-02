#ifndef FINALYEARPROJECT_REQUESTS_H
#define FINALYEARPROJECT_REQUESTS_H

#include <curl/curl.h>

#include <cstddef>
#include <future>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace LouiEriksson::Engine::Networking {
	
	/*
	 * Note to self:
	 *      To debug, open terminal and type "nc -l 8080" to listen to port 8080 with netcat.
	 *      you can then send requests to "http://localhost:8080/" and see the output in realtime.
	 *      Interrupting operation with Ctrl+D enables user to type a custom response.
	 */
	
	struct Requests {
		
		class Client;
		
		struct Response {
			
			friend Requests;
			
		public:
			
			class Data {
			
				friend Client;
				
			private:
				
				std::vector<char> m_Raw;
				
			public:
				
				[[nodiscard]] const std::vector<char>&      Raw() const;
				[[nodiscard]]       std::istringstream ToStream() const;
				
				Data();
			};
			
			Response();
			
			[[nodiscard]] const CURLcode& Status() const noexcept;
			
			[[nodiscard]] const Data& Content() const noexcept;
			
			static bool Success(const Requests::Response& _response, const bool& _verbose = true);
			
		private:
			
			CURLcode m_Status;
			Data     m_Content;
			
		};
		
		class Client {
		
		private:
			
			using Handle_t = std::shared_ptr<void>;
			using Header_t = std::shared_ptr<curl_slist>;
			
			Handle_t m_Handle;
			
		public:
			
			[[nodiscard]]        const Handle_t& Handle();
			[[nodiscard]] static Header_t  Header();
			
			template <typename T>
			void Set(const CURLoption& _option, const T& _value) {
				curl_easy_setopt(Handle().get(), _option, _value);
			}
			
			template <typename T>
			[[nodiscard]] T Get(const CURLINFO _info) {
				
				T result;
				
				curl_easy_getinfo(Handle().get(), _info, &result);
				
				return result;
			}
			
			Response Send();
			
			std::future<Requests::Response> SendAsync();
			
			void Dispose();
			
			Client();
			Client(const std::string& _uri);
			
		};
		
		static size_t WriteFunction(void* _data, size_t _stride, size_t _count, std::vector<char>* _userData);
		
		static void Init();
		
		static void Dispose();
	};
	
} // LouiEriksson::Engine::Networking

#endif //FINALYEARPROJECT_REQUESTS_H