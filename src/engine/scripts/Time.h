#ifndef FINALYEARPROJECT_TIME_H
#define FINALYEARPROJECT_TIME_H

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	class Application;
	
	class Time {
	
		friend class Application;
	
	private:
	
		inline static float s_Elapsed        = 0.0f;
		inline static float s_DeltaTime      = 0.0f;
		inline static float s_FixedDeltaTime = 0.0f;
	
	public:
		
		 Time()                   = delete;
		 Time(const Time& _other) = delete;
		~Time()                   = delete;
		
		static const float& Elapsed();
		static const float& DeltaTime();
	
		static void  FixedDeltaTime(const float& _value);
		static const float& FixedDeltaTime();
		
		Time& operator = (const Time&  _other) = delete;
		Time& operator =       (Time&& _other) = delete;
	};
}

#endif //FINALYEARPROJECT_TIME_H