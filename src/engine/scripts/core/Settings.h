#ifndef FINALYEARPROJECT_SETTINGS_H
#define FINALYEARPROJECT_SETTINGS_H

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#include <memory>
#include <vector>

namespace LouiEriksson::Engine::Graphics {
	
	class Texture;
	class Shader;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine::UI {
	
	class GUI;
	
} // LouiEriksson::Engine::UI

namespace LouiEriksson::Engine {
	
	class Settings {
	
		friend class Application;
		
		friend UI::GUI;
		
	public:
		
		/// <summary> Initialise settings on application start. </summary>
		static void Init();
		
		/// <summary> Container for the application's graphics settings. </summary>
		struct Graphics {
			
			inline static bool s_GammaCorrection { true };
			
			/// <summary> Container for the application's v-sync settings. </summary>
			struct VSync {
				
				/* SKYBOX TEXTURE */
				inline static std::vector<const char*> s_AvailableOptions {
					"Adaptive",
					"Off",
					"1:1",
					"1:2",
					"1:3",
					"1:4",
				};
				
				inline static int s_CurrentSelection { 0 };
				
			};
			
			/// <summary> Container for the settings of the OrbitCam script. </summary>
			struct Perspective {
				
				inline static float      s_FOV { 60.0f };
				inline static float s_NearClip { 0.01f };
				inline static float  s_FarClip { 60.0f };
				
				struct Orbit {
					
					inline static bool s_Enabled { true };
					
					inline static glm::vec3 s_Speed  { -0.25f, 0.33f,   0.5f };
					inline static glm::vec3 s_Amount {   0.0f,  3.0f,   5.0f };
					inline static glm::vec3 s_Offset {   0.0f,  6.0f, -12.0f };
				};
				
			};
			
			/// <summary> Container for the settings of the application's skybox. </summary>
			struct Skybox {
				
				/* SKYBOX TEXTURE */
				inline static std::vector<const char*> s_AvailableSkyboxes {
					"little_paris_eiffel_tower_1k",
					"abandoned_workshop_02_1k",
					"blue_photo_studio_1k",
				};
				
				inline static int s_CurrentSkyboxSelection { 2 };
				
				inline static std::weak_ptr<LouiEriksson::Engine::Graphics::Texture> s_Skybox;
				
				static void UpdateSkybox(const int& _index);
				
				/* PARAMETERS */
				inline static float s_Blur     { 0.0f };
				inline static float s_Exposure { 1.0f };
				
			};
			
			/// <summary> Container for the settings of the application's "pbr" and "blinnphong" materials. </summary>
			struct Material {
				
				/* SHADER */
				inline static std::vector<const char*> s_AvailableShaders {
					"pbr",
					"blinnphong",
				};
				
				inline static int s_CurrentShaderSelection { 0 };
				
				inline static std::weak_ptr<LouiEriksson::Engine::Graphics::Shader> s_Shader;
				
				static void UpdateShader(const int& _index);
				
				/* PARAMETERS */
				inline static float s_RoughnessAmount    { 1.0f };
				inline static float s_DisplacementAmount { 0.3f };
				inline static float s_NormalAmount       { 1.0f };
				inline static float s_EmissionAmount     { 0.0f };
				inline static float s_AOAmount           { 2.0f };
				
				inline static glm::vec4 s_TextureScaleTranslate { 1.0f, 1.0f, 0.0f, 0.0f };
			
				/* SHADOWS */
				inline static std::vector<const char*> s_ShadowTechniques {
					"hard",
					"pcf",
					"poisson-disk",
					"pcss"
				};
				
				inline static int s_CurrentShadowTechnique { 3 };
				
				inline static std::vector<const char*> s_ShadowResolutions {
					"0",
					"32",
					"64",
					"128",
					"256",
					"512",
					"1024",
					"2048",
					"4096",
					"8196"
				};
				
				inline static int s_CurrentShadowResolutionSelection { 3 };
				
				inline static int s_ShadowSamples { 12 };
				
				inline static float s_ShadowBias       { 0.01f };
				inline static float s_ShadowNormalBias { 0.02f };
				
				inline static bool s_ParallaxShadows { true };
				
				/* LIGHT */
				inline static std::vector<const char*> s_AvailableLightTypes {
					"point",
					"directional",
					"spot"
				};
				
				inline static int s_CurrentLightType { 0 };
				
			    inline static glm::vec3 s_LightPosition   {    0.0f,   2.5f,  0.0f   }; // Position of light in world-space.
			    inline static glm::vec3 s_LightRotation   { -125.0f, 225.0f,  0.0f   }; // Position of light in world-space.
			    inline static glm::vec3 s_LightColor      {    1.0f,  0.91f,  0.874f }; // Color of light.
				inline static     float s_LightIntensity  {    3.0f };                  // Brightness of light.
			    inline static     float s_LightRange      {  100.0f };                  // Range of light.
			    inline static     float s_LightAngle      {  120.0f };                  // Cos of light's FOV (for spot lights).
				inline static     float s_LightSize       {    0.2f };                  // Size of the light (PCSS only).
			
			};
			
		};
		
		/// <summary> Container for the settings of the application's post-processing effects. </summary>
		struct PostProcessing {
			
			inline static bool s_Enabled { true };
			
			/// <summary> Container for the settings of the AmbientOcclusion post-processing effect. </summary>
			struct AmbientOcclusion {
			
				inline static bool s_Enabled { true };
			
				inline static int s_Samples   { 12 };
				inline static int s_Downscale {  1 };
				
				inline static float s_Intensity { 1.0f };
				inline static float s_Radius    { 0.2f };
				
				static bool IsActiveAndEnabled() noexcept;
				
			};
			
			/// <summary> Container for the settings of the Bloom post-processing effect. </summary>
			struct Bloom {
			
				inline static bool s_Enabled { true };
				
				inline static float s_Intensity   {  0.2f };
				inline static float s_Threshold   {  1.2f };
				inline static float s_Clamp       { 25.0f };
				inline static float s_LensDirt    {  0.6f };
				inline static float s_Anamorphism {  0.0f };
				inline static float s_Diffusion   {  3.0f };
			
				static bool IsActiveAndEnabled() noexcept;
				
			};
			
			/// <summary> Container for the settings of the "aces" post-processing effect. </summary>
			struct ToneMapping {
				
				inline static bool s_Enabled { true };
				
				inline static float s_Gain     { 0.0f };
				inline static float s_Exposure { 1.0f };
				
				static bool IsActiveAndEnabled() noexcept;
				
				/// <summary> Container for the settings of the AutoExposure post-processing effect. </summary>
				struct AutoExposure {
					
					inline static bool s_Enabled { true };
					
					inline static float s_MinEV        {  0.2f };
					inline static float s_MaxEV        {  4.0f };
					inline static float s_Compensation { -0.2f };
					inline static float s_SpeedDown    {  1.0f };
					inline static float s_SpeedUp      {  2.0f };
					
					static bool IsActiveAndEnabled() noexcept;
				};
				
			};
			
			/// <summary> Container for the settings of the "fxaa" post-processing effect. </summary>
			struct AntiAliasing {
			
				inline static bool s_Enabled { true };
			
				inline static float s_ContrastThreshold     { 0.0312f };
				inline static float s_RelativeThreshold     { 0.063f  };
				inline static float s_SubpixelBlending      { 0.75f   };
				inline static float s_EdgeBlending          { 1.0f    };
				inline static float s_LocalContrastModifier { 0.5f    };
				
				static bool IsActiveAndEnabled() noexcept;
				
			};
			
			/// <summary> Container for the settings of the "grain" post-processing effect. </summary>
			struct Grain {
			
				inline static bool s_Enabled { true };
			
				inline static float s_Intensity { 0.001f };
				
				static bool IsActiveAndEnabled() noexcept;
				
			};
			
			/// <summary> Container for the settings of the "vignette" post-processing effect. </summary>
			struct Vignette {
			
				inline static bool s_Enabled { true };
				
				inline static float s_Intensity  { 0.667f };
				inline static float s_Smoothness { 0.333f };
				
				static bool IsActiveAndEnabled() noexcept;
				
			};
			
		};
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_SETTINGS_H