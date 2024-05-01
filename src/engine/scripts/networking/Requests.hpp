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
	
	/**
	 * @mainpage Version 1.0.0
	 * @class Requests
	 * @brief A class that wrapping libcurl, providing the functionality to send HTTP requests and receive responses.
	 */
	struct Requests final {
		
		class Client;
		
		struct Response final {
			
			friend Requests;
			
		public:
			
			/**
			 * @class Data
			 * @brief Represents data returned from a request.
			 */
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
			
			/**
			 * @brief Checks if a request was successful.
			 *
			 * This function checks if a request was successful by inspecting the status code of the response object.
			 *
			 * @param[in] _response The response object to check.
			 * @param[in] _verbose Optional parameter indicating whether to print an error message if the request fails.
			 *                 Default value is true.
			 *
			 * @return True if the request was successful, False otherwise.
			 *
			 * @note This function assumes that the response object has already been initialized with a status code
			 *       and content data. It uses the Status() method of the Response class to get the status code.
			 *       If the status code is not CURLE_OK, it considers the request as failed.
			 *       If _verbose is true and the request fails, it prints an error message.
			 */
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
		
		/**
		 * @class Client
		 * @brief Represents a client used to send HTTP requests using libcurl.
		 */
		class Client final {
		
		private:
			
			using Handle_t = std::shared_ptr<void>;
			using Header_t = std::shared_ptr<curl_slist>;
			
			Handle_t m_Handle;
			
			static size_t WriteFunction(void* _data, size_t _stride, size_t _count, std::vector<char>* _userData) {
		
			const auto len = _stride * _count;
		 
			const auto* const addr = reinterpret_cast<char*>(_data);
			_userData->insert(_userData->end(), addr, addr + len);
			
			return len;
		}
		
		public:
			
			/**
			 * @brief Returns the handle associated with the Client object.
			 *
			 * @note If the handle is null, a new handle is created.
			 *
			 * @return A const reference to the handle associated with the Client object.
			 */
			[[nodiscard]] const Handle_t& Handle() {
				
				if (m_Handle == nullptr) {
					m_Handle.reset(curl_easy_init(), [](void* _ptr) { if (_ptr != nullptr) { curl_easy_cleanup(_ptr); }});
				}
				
				return m_Handle;
			}
			
			[[nodiscard]] static Header_t Header() {
				return Header_t({}, [](curl_slist* _ptr) { if (_ptr != nullptr) { curl_slist_free_all(_ptr); }});
			}
			
			/**
			 * @brief Set the value of a specified option for the libcurl handle.
			 *
			 * This function sets the value of a specified option for the libcurl handle used in the Client class.
			 * The option to be set is specified by the _option parameter, and the value to be set is specified
			 * by the _value parameter. The curl_easy_setopt function is called to set the option and value.
			 *
			 * @tparam T The type of the value to be set.
			 * @param[in] _option The CURLoption specifying the option to set.
			 * @param[in] _value The value to set for the specified option.
			 */
			template <typename T>
			constexpr void Set(const CURLoption& _option, const T& _value) {
				curl_easy_setopt(Handle().get(), _option, _value);
			}
			
			/**
			 * @brief Retrieves information from a libcurl handle.
			 *
			 * This function is used to retrieve information about a libcurl handle. It takes as input the CURLINFO option to specify what information should be retrieved.
			 * The specified CURLINFO option is passed to the curl_easy_getinfo function along with the libcurl handle and a pointer to the result buffer.
			 * The result buffer needs to be provided by the caller and should have the appropriate type to store the result.
			 *
			 * @tparam T The type of the result. It should match the type expected by the specified CURLINFO option.
			 * @param _info The CURLINFO option specifying the information to retrieve.
			 * @return The retrieved information.
			 */
			template <typename T>
			[[nodiscard]] constexpr T Get(const CURLINFO _info) {
				
				T result;
				
				curl_easy_getinfo(Handle().get(), _info, &result);
				
				return result;
			}
			
			/**
			 * @brief Sends an HTTP request and returns the response.
			 *
			 * @return A `Response` object representing the response of the HTTP request.
			 */
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
			
			/**
			 * @brief Asynchronously sends an HTTP request and returns a future representing the response.
			 *
			 * @return A std::future object representing the response of the HTTP request.
			 */
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
		
		/**
		 * @brief Initialize the Curl library.
		 *
		 * This static method is used to initialize the Curl library by calling `curl_global_init(CURL_GLOBAL_ALL)`.
		 *
		 * @par Related Functions
		 * - curl_global_init()
		 *
		 * @see curl_global_init()
		 */
		static void Init() {
			curl_global_init(CURL_GLOBAL_ALL);
		}
		
		/**
		 * @brief Dispose the Curl library.
		 *
		 * This static method is used to dispose the Curl library by calling `curl_global_cleanup()`.
		 * If an exception occurs during the cleanup, it will be caught and logged.
		 *
		 * @note This method is declared `noexcept`, indicating that it does not throw any exceptions.
		 *
		 * @par Dependency
		 * - Requires the Curl library to be initialized.
		 *
		 * @par Related Functions
		 * - Debug::Log(const std::exception&, const LogType&)
		 *
		 * @see curl_global_cleanup()
		 */
		static void Dispose() noexcept {
			
			try {
	            curl_global_cleanup();
			}
			catch (const std::exception& e) {
				Debug::Log(e);
			}
		}
	};
	
} // LouiEriksson::Engine::Networking

#endif //FINALYEARPROJECT_REQUESTS_HPP