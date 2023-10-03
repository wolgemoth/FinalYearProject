#include "stdafx.h"

#include "Time.h"

namespace LouiEriksson {
	
	float Time::Elapsed() {
		return s_Elapsed;
	}
	float Time::DeltaTime() {
		return s_DeltaTime;
	}
	
	void Time::FixedDeltaTime(const float& _value) {
		s_FixedDeltaTime = _value;
	}
	float Time::FixedDeltaTime() {
		return s_FixedDeltaTime;
	}
}