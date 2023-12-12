#include "Settings.h"

namespace LouiEriksson {
	
	bool Settings::PostProcessing::AmbientOcclusion::IsActiveAndEnabled() {
		return s_Enabled && s_Samples > 0 && s_Intensity > 0.0f && s_Radius > 0.0f;
	}
	
	bool Settings::PostProcessing::Bloom::IsActiveAndEnabled() {
		return s_Enabled && s_Intensity > 0.0f && s_Threshold <= s_Clamp;
	}
	
	bool Settings::PostProcessing::ToneMapping::IsActiveAndEnabled() {
		return s_Enabled;
	}
	
	bool Settings::PostProcessing::ToneMapping::AutoExposure::IsActiveAndEnabled() {
		return Settings::PostProcessing::ToneMapping::s_Enabled && s_Enabled && s_MinEV < s_MaxEV && s_MaxEV > 0.0f;
	}
	
	bool Settings::PostProcessing::AntiAliasing::IsActiveAndEnabled() {
		return s_Enabled;
	}
	
	bool Settings::PostProcessing::Grain::IsActiveAndEnabled() {
		return s_Enabled && s_Intensity > 0.0f;
	}
	
	bool Settings::PostProcessing::Vignette::IsActiveAndEnabled() {
		return s_Enabled && s_Intensity > 0.0f;
	}
	
} // LouiEriksson