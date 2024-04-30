#ifndef FINALYEARPROJECT_TIME_HPP
#define FINALYEARPROJECT_TIME_HPP

#include "Types.hpp"

namespace LouiEriksson::Engine {
	
	/**
	 * @class Time
	 * @brief Provides multiple utility functions for accessing and manipulating time in the application.
	 */
	class Time final {
	
		friend class Application;
	
	private:
		
		inline static tick_t s_Scale                  { 1.0 }; /**< @brief Speed of time within the engine. */
		inline static tick_t s_Elapsed                { 0.0 }; /**< @brief   Total time elapsed (unscaled). */
		inline static tick_t s_UnscaledDeltaTime      { 0.0 }; /**< @brief             Unscaled delta time. */
		inline static tick_t s_FixedUnscaledDeltaTime { 0.0 }; /**< @brief       Unscaled fixed delta time. */
	
	public:
		
		 Time()                   = delete;
		 Time(const Time& _other) = delete;
		~Time()                   = delete;
		
		/**
		 * @brief Set the speed of time within the engine.
		 *
		 * This function sets the speed of time within the engine.
		 * The input value should be between 0.0 and 1.0, where 0.0 represents
		 * a paused state and 1.0 represents the normal speed.
		 *
		 * @param[in] _value The new speed value to be set (between 0.0 and 1.0)
		 */
		template <typename T>
		static void Scale(const T& _value) noexcept {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
	        s_Scale = _value;
		}
		
		/**
		 * @brief Get the speed of time within the engine.
		 * @return The speed of time within the engine.
		 */
		template <typename T = tick_t>
		static constexpr T Scale() noexcept {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
			return static_cast<T>(s_Scale);
		}
		
		/**
		 * @brief Get the total time elapsed since the engine was initialised.
		 * @return The total time elapsed.
		 */
		template <typename T = tick_t>
		static constexpr T Elapsed() noexcept {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
			return static_cast<T>(s_Elapsed);
		}
		
		/**
		 * @brief Get the scaled delta time (duration of the previous frame).
		 * @return The scaled delta time.
		 */
		template <typename T = tick_t>
		static constexpr T DeltaTime() noexcept {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
			return static_cast<T>(s_UnscaledDeltaTime * s_Scale);
		}
		
		/**
		 * @brief Get the unscaled delta time (duration of the previous frame).
		 * @return The unscaled delta time.
		 */
		template <typename T = tick_t>
		static constexpr T UnscaledDeltaTime() noexcept {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
			return static_cast<T>(s_UnscaledDeltaTime);
		}
		
		/**
		 * @brief Set the unscaled fixed delta time (time between physics updates).
		 * @param[in] _value The new fixed delta time.
		 */
		template <typename T>
		static void FixedDeltaTime(const T& _value) noexcept {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
			s_FixedUnscaledDeltaTime = _value;
		}
		
		/**
		 * @brief Get the scaled fixed delta time (time between physics updates).
		 * @return The scaled fixed delta time.
		 */
		template <typename T = tick_t>
		static constexpr T FixedDeltaTime() noexcept {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
			return static_cast<T>(s_FixedUnscaledDeltaTime * s_Scale);
		}
		
		/**
		 * @brief Get the unscaled fixed delta time (time between physics updates).
		 * @return The unscaled fixed delta time.
		 */
		template <typename T = tick_t>
		static constexpr T FixedUnscaledDeltaTime() noexcept {
			
			static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
			
			return static_cast<T>(s_FixedUnscaledDeltaTime);
		}
		
		Time& operator = (const Time&  _other) = delete;
		Time& operator =       (Time&& _other) = delete;
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_TIME_HPP