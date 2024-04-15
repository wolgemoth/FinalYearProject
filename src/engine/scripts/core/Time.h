#ifndef FINALYEARPROJECT_TIME_H
#define FINALYEARPROJECT_TIME_H

namespace LouiEriksson::Engine {
	
	/**
	 * @class Time
	 * @brief Provides multiple utility functions for accessing and manipulating time in the application.
	 */
	class Time final {
	
		friend class Application;
	
	private:
		
		inline static float s_Scale                  { 1.0f }; /**< @brief Speed of time within the engine. */
		inline static float s_Elapsed                { 0.0f }; /**< @brief   Total time elapsed (unscaled). */
		inline static float s_UnscaledDeltaTime      { 0.0f }; /**< @brief             Unscaled delta time. */
		inline static float s_FixedUnscaledDeltaTime { 0.0f }; /**< @brief       Unscaled fixed delta time. */
	
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
		static void Scale(const float& _value) noexcept;
		
		/**
		 * @brief Get the speed of time within the engine.
		 * @return The speed of time within the engine.
		 */
		static const float& Scale() noexcept;
		
		/**
		 * @brief Get the total time elapsed since the engine was initialised.
		 * @return The total time elapsed.
		 */
		static const float& Elapsed() noexcept;
		
		/**
		 * @brief Get the scaled delta time (duration of the previous frame).
		 * @return The scaled delta time.
		 */
		static float DeltaTime() noexcept;
		
		/**
		 * @brief Get the unscaled delta time (duration of the previous frame).
		 * @return The unscaled delta time.
		 */
		static const float& UnscaledDeltaTime() noexcept;
		
		/**
		 * @brief Set the unscaled fixed delta time (time between physics updates).
		 * @param[in] _value The new fixed delta time.
		 */
		static void FixedDeltaTime(const float& _value) noexcept ;
		
		/**
		 * @brief Get the scaled fixed delta time (time between physics updates).
		 * @return The scaled fixed delta time.
		 */
		static float FixedDeltaTime() noexcept;
		
		/**
		 * @brief Get the unscaled fixed delta time (time between physics updates).
		 * @return The unscaled fixed delta time.
		 */
		static const float& FixedUnscaledDeltaTime() noexcept;
		
		Time& operator = (const Time&  _other) = delete;
		Time& operator =       (Time&& _other) = delete;
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_TIME_H