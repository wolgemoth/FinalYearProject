#include "stdafx.h"

#include "Time.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	const float& Time::Elapsed() {
		return s_Elapsed;
	}
	const float& Time::DeltaTime() {
		return s_DeltaTime;
	}
	
	void Time::FixedDeltaTime(const float& _value) {
		s_FixedDeltaTime = _value;
	}
	const float& Time::FixedDeltaTime() {
		return s_FixedDeltaTime;
	}
}