#include "stdafx.h"

#include "Settings.h"
#include "Resources.h"

namespace LouiEriksson {
	
	void Settings::Init() {
		Settings::Graphics::Skybox::UpdateSkybox(Settings::Graphics::Skybox::s_CurrentSkyboxSelection);
		Settings::Graphics::Material::UpdateShader(Settings::Graphics::Material::s_CurrentShaderSelection);
	}
	
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
	
	void Settings::Graphics::Skybox::UpdateSkybox(const int& _index) {
		
		try {
			s_Skybox = Resources::GetTexture(s_AvailableSkyboxes.at(_index));
			
			s_CurrentSkyboxSelection = _index;
		}
		catch (const std::runtime_error& e) {
			std::cout << e.what() << "\n";
		}
	}
	
	void Settings::Graphics::Material::UpdateShader(const int& _index) {
		
		try {
			s_Shader = Resources::GetShader(s_AvailableShaders.at(_index));
			
			s_CurrentShaderSelection = _index;
		}
		catch (const std::runtime_error& e) {
			std::cout << e.what() << "\n";
		}
	}
	
} // LouiEriksson