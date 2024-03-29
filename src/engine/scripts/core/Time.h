#ifndef FINALYEARPROJECT_TIME_H
#define FINALYEARPROJECT_TIME_H

namespace LouiEriksson::Engine {
	
	class Time {
	
		friend class Application;
	
	private:
		
		/// <summary> Speed of time within the engine. </summary>
		inline static float s_Scale { 1.0f };
		
		/// <summary> Total time elapsed (unscaled). </summary>
		inline static float s_Elapsed { 0.0f };
		
		/// <summary> Unscaled delta time. </summary>
		inline static float s_UnscaledDeltaTime { 0.0f };
		
		/// <summary> Unscaled fixed delta time. </summary>
		inline static float s_FixedUnscaledDeltaTime { 0.0f };
	
	public:
		
		 Time()                   = delete;
		 Time(const Time& _other) = delete;
		~Time()                   = delete;
		
		/// <summary>
		/// Set the speed of time within the engine.
		/// This should be between 0.0 and 1.0.
		/// </summary>
		static void Scale(const float& _value) noexcept;
		
		/// <summary> Get the speed of time within the engine. </summary>
		static const float& Scale() noexcept;
		
		/// <summary> Get the total time elapsed since the engine was initialised. </summary>
		static const float& Elapsed() noexcept;
		
		/// <summary> Get the scaled delta time. (Duration of the previous frame.) </summary>
		static float DeltaTime() noexcept;
		
		/// <summary> Get the unscaled delta time. (Duration of the previous frame.) </summary>
		static const float& UnscaledDeltaTime() noexcept;
		
		/// <summary> Set the unscaled fixed delta time. (Time between physics updates.) </summary>
		static void FixedDeltaTime(const float& _value) noexcept ;
		
		/// <summary> Get the scaled fixed delta time. (Time between physics updates.) </summary>
		static float FixedDeltaTime() noexcept;
		
		/// <summary> Get the unscaled fixed delta time. (Time between physics updates.) </summary>
		static const float& FixedUnscaledDeltaTime() noexcept;
		
		Time& operator = (const Time&  _other) = delete;
		Time& operator =       (Time&& _other) = delete;
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_TIME_H