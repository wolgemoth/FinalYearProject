#pragma once

#ifndef _FYP_TIME_H
#define _FYP_TIME_H

class Application;

class Time {

friend class Application;

private:

	inline static float s_Elapsed        = 0.0f;
	inline static float s_DeltaTime      = 0.0f;
	inline static float s_FixedDeltaTime = 0.0f;

	Time() = delete;
	Time(const Time& _other) = delete;
	~Time() = delete;

	Time& operator = (const Time& _other) = delete;
	Time& operator = (Time&& _other) = delete;

public:

	static float Elapsed();
	static float DeltaTime();

	static void  FixedDeltaTime(const float& _value);
	static float FixedDeltaTime();

};

#endif // !_FYP_TIME_H
