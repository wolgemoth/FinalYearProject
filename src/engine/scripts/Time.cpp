#include "stdafx.h"

#include "Time.h"

// @Assessor: This class was submitted for PFG. Please don't mark it for GACP or GEP.

namespace LouiEriksson {
	
	void Time::Scale(const float& _value) {
	    s_Scale = _value;
	}
	const float& Time::Scale() {
		return s_Scale;
	}
	
	const float& Time::Elapsed() {
		return s_Elapsed;
	}
	
	const float Time::DeltaTime() {
		return s_UnscaledDeltaTime * s_Scale;
	}
	
	const float& Time::UnscaledDeltaTime() {
		return s_UnscaledDeltaTime;
	}
	
	void Time::FixedDeltaTime(const float& _value) {
		s_FixedUnscaledDeltaTime = _value;
	}
	const float Time::FixedDeltaTime() {
		return s_FixedUnscaledDeltaTime * s_Scale;
	}
	
	const float& Time::FixedUnscaledDeltaTime() {
		return s_FixedUnscaledDeltaTime;
	}
}