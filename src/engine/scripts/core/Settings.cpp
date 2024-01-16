#include "Settings.h"

#include "Resources.h"

#include <iostream>
#include <stdexcept>

// @Assessor: This class was submitted for GACP. Please don't mark it for GEP.

namespace LouiEriksson::Engine {
	
	void Settings::Init() {
		Settings::Graphics::Skybox::UpdateSkybox(Settings::Graphics::Skybox::s_CurrentSkyboxSelection);
		Settings::Graphics::Material::UpdateShader(Settings::Graphics::Material::s_CurrentShaderSelection);
	}
	
	bool Settings::PostProcessing::AmbientOcclusion::IsActiveAndEnabled() noexcept {
		return s_Enabled && s_Samples > 0 && s_Intensity > 0.0f && s_Radius > 0.0f;
	}
	
	bool Settings::PostProcessing::Bloom::IsActiveAndEnabled() noexcept {
		return s_Enabled && s_Intensity > 0.0f && s_Threshold <= s_Clamp;
	}
	
	bool Settings::PostProcessing::ToneMapping::IsActiveAndEnabled() noexcept {
		return s_Enabled;
	}
	
	bool Settings::PostProcessing::ToneMapping::AutoExposure::IsActiveAndEnabled() noexcept {
		return Settings::PostProcessing::ToneMapping::s_Enabled && s_Enabled && s_MinEV < s_MaxEV && s_MaxEV > 0.0f;
	}
	
	bool Settings::PostProcessing::AntiAliasing::IsActiveAndEnabled() noexcept {
		return s_Enabled;
	}
	
	bool Settings::PostProcessing::Grain::IsActiveAndEnabled() noexcept {
		return s_Enabled && s_Intensity > 0.0f;
	}
	
	bool Settings::PostProcessing::Vignette::IsActiveAndEnabled() noexcept {
		return s_Enabled && s_Intensity > 0.0f;
	}
	
	void Settings::Graphics::Skybox::UpdateSkybox(const int& _index) {
		
		std::shared_ptr<LouiEriksson::Engine::Graphics::Texture> item;
		if (Resources::TryGetTexture(s_AvailableSkyboxes.at(_index), item)) {
			
			s_Skybox = item;
			
			s_CurrentSkyboxSelection = _index;
		}
	}
	
	void Settings::Graphics::Material::UpdateShader(const int& _index) {
		
		std::shared_ptr<LouiEriksson::Engine::Graphics::Shader> item;
		if (Resources::TryGetShader(s_AvailableShaders.at(_index), item)) {
			
			s_Shader = item;
			
			s_CurrentShaderSelection = _index;
		}
	}
	
} // LouiEriksson::Engine