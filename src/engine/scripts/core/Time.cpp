#include "Time.h"

namespace LouiEriksson::Engine {
	
	void Time::Scale(const float& _value) noexcept {
	    s_Scale = _value;
	}
	const float& Time::Scale() noexcept {
		return s_Scale;
	}
	
	const float& Time::Elapsed() noexcept {
		return s_Elapsed;
	}
	
	float Time::DeltaTime() noexcept {
		return s_UnscaledDeltaTime * s_Scale;
	}
	
	const float& Time::UnscaledDeltaTime() noexcept {
		return s_UnscaledDeltaTime;
	}
	
	void Time::FixedDeltaTime(const float& _value) noexcept {
		s_FixedUnscaledDeltaTime = _value;
	}
	float Time::FixedDeltaTime() noexcept {
		return s_FixedUnscaledDeltaTime * s_Scale;
	}
	
	const float& Time::FixedUnscaledDeltaTime() noexcept {
		return s_FixedUnscaledDeltaTime;
	}
	
} // LouiEriksson::Engine