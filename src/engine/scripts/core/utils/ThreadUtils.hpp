#ifndef FINALYEARPROJECT_THREADUTILS_HPP
#define FINALYEARPROJECT_THREADUTILS_HPP

#include <atomic>
#include <chrono>
#include <future>

namespace LouiEriksson::Engine::Threading {

	struct Utils final {
		
		/**
		 * @brief Dispatcher for scheduling and dispatching tasks in a synchronized manner.
		 */
		struct Dispatcher final {
		
		private:
			
			std::mutex m_Lock;
			
			std::queue<std::function<void()>> m_Tasks;
		
		public:
			
			/**
			 * @brief Schedule a task by adding it to the task queue.
			 *
			 * @param[in] _task The task to be scheduled.
			 */
			void Schedule(std::function<void()>&& _task) {
				
				std::lock_guard<std::mutex> lock(m_Lock);
				
				m_Tasks.emplace(_task);
			}
			
			/**
			 * @brief Dispatches tasks from the task queue.
			 *
			 * @param[in] _count The number of tasks to dispatch. Defaults to 1.
			 */
			void Dispatch(const size_t& _count = 1) {
				
				std::lock_guard<std::mutex> lock(m_Lock);
				
				for (int i = 0; i < _count; ++i) {
					
					if (!m_Tasks.empty()) {
						m_Tasks.front()();
						m_Tasks.pop();
					}
				}
			}
		};
		
		/**
		 * @class CancellationToken
		 * @brief Represents a token that can be used to cancel an operation.
		 */
		struct CancellationToken {
		
		private:
			
			std::atomic<bool> m_Token;
			
		public:
			
			CancellationToken() : m_Token(false) {}
			
			/**
			 * @brief Cancels the current operation.
			 * @note After cancelling the operation, the IsCancellationRequested() function can be used to check
			 * if the cancellation has been requested.
			 */
			void Cancel() {
				m_Token = true;
			}
			
			/**
			    * @brief Check if cancellation is requested.
			    * @return True if cancellation is requested, false otherwise.
			    */
			bool IsCancellationRequested() {
				return m_Token;
			}
		};
		
		/**
		 * @brief Waits for a future to complete or until a timeout occurs, with support for cancellation.
		 *
		 * @tparam T The type of the value contained in the std::future.
		 * @param[in,out] _future The std::future object to wait for.
		 * @param[in] _timeout The maximum duration to wait for the future to complete.
		 * @param[in,out] _cancellationToken The CancellationToken object used to check for cancellation.
		 * @return The value of the completed future, or a default-constructed value of type T if cancellation occurred or the timeout expired.
		 */
		template <typename T>
		static std::future_status Wait(std::future<T>& _future, const std::chrono::system_clock::duration& _timeout, CancellationToken& _cancellationToken) {
			
			std::future_status result;
			
			const auto end = std::chrono::system_clock::now() + _timeout;
			
			do {
				result = Status(_future);
				
				if (result == std::future_status::ready) {
					break;
				}
			}
			while (!_cancellationToken.IsCancellationRequested() && std::chrono::system_clock::now() < end);
			
			return result;
		}
		
		/**
		 * @brief Returns the status of a std::future object.
		 *
		 * This function checks the status of the given std::future object `_future` without blocking.
		 *
		 * @tparam T The type of the value contained in the std::future.
		 * @param[in] _future The std::future object to check the status of.
		 * @return The status of the std::future object:
		 *   - std::future_status::ready if the result is available.
		 *   - std::future_status::timeout if the wait timed out.
		 *   - std::future_status::deferred if the result is a deferred task.
		 */
		template <typename T>
		static std::future_status Status(const std::future<T>& _future) {
			return _future.wait_for(std::chrono::seconds(0));
		}
	};
}

#endif //FINALYEARPROJECT_THREADUTILS_HPP