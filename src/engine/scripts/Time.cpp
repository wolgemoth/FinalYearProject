#include "stdafx.h"

#include "Time.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

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