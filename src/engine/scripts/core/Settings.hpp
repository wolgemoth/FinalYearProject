#ifndef FINALYEARPROJECT_SETTINGS_HPP
#define FINALYEARPROJECT_SETTINGS_HPP

#include "Resources.hpp"
#include "Types.hpp"

#include <glm/ext/vector_float4.hpp>

#include <memory>
#include <vector>

namespace LouiEriksson::Engine::Graphics {
	
	class Texture;
	class Cubemap;
	class Shader;
	
} // LouiEriksson::Engine::Graphics

namespace LouiEriksson::Engine::UI {
	
	class GUI;
	
} // LouiEriksson::Engine::UI

namespace LouiEriksson::Engine {
	
	/**
	 * @class Settings
	 * @brief Container for application settings.
	 *
	 * This class is responsible for storing and providing access to the various settings of the
	 * application
	 */
	class Settings final {
	
		friend class Application;
		
		friend UI::GUI;
		
	public:
		
		/**
		 * @brief Initialises the settings.
		 *
		 * This function is used to initialise the application settings. It updates the skybox and material shader
		 * based on the current selection values.
		 *
		 * @note This function should be called before using any other settings functions.
		 *
		 * @sa Settings::Graphics::Skybox::UpdateSkybox, Settings::Graphics::Material::UpdateShader
		 */
		static void Init() {
			Settings::Graphics::Skybox::UpdateSkybox(Settings::Graphics::Skybox::s_CurrentSkyboxSelection);
			Settings::Graphics::Material::UpdateShader(Settings::Graphics::Material::s_CurrentShaderSelection);
		}
		
		struct Spatial final {
			
			/** @brief Origin coordinate of the coordinate system (in latitude, longitude, and altitude). */
			inline static glm::vec3 s_Coord { 51.4706, -0.461941, 0.0 };//{ 22.28180, 114.17347, 0.0 };
			
		};
		
		/** @brief Container for the application's graphics settings. */
		struct Graphics final {
			
			inline static bool s_GammaCorrection { true };
			
			/** @brief Container for the application's v-sync settings. */
			struct VSync final {
				
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
			
			/** @brief Container for the settings of the camera. */
			struct Perspective final {
				
				inline static scalar_t      s_FOV { 60.0 };
				inline static scalar_t s_NearClip { 0.01 };
				inline static scalar_t  s_FarClip { 60.0 };
			};
			
			/** @brief Container for the settings of the application's skybox. */
			struct Skybox final {
				
				/* SKYBOX TEXTURE */
				inline static std::vector<const char*> s_AvailableSkyboxes {
					"assets/textures/linear/cubemaps/8k_stars_milky_way/",
					"assets/textures/linear/cubemaps/daylight/",
					"assets/textures/linear/cubemaps/abandoned_workshop_02_1k/",
					"assets/textures/linear/cubemaps/blue_photo_studio_1k/",
					"assets/textures/linear/cubemaps/little_paris_eiffel_tower_1k/",
				};
				
				inline static int s_CurrentSkyboxSelection { 3 };
				
				inline static std::weak_ptr<LouiEriksson::Engine::Graphics::Cubemap> s_Skybox;
				
				static void UpdateSkybox(const size_t& _index) {
					
					if (const auto item = Resources::Get<Engine::Graphics::Cubemap>(s_AvailableSkyboxes.at(_index))) {
						s_Skybox = item;
						
						s_CurrentSkyboxSelection = _index;
					}
				}
				
				/* PARAMETERS */
				inline static scalar_t s_Blur     { 0.0 };
				inline static scalar_t s_Exposure { 1.0 };
				
			};
			
			/** @brief Container for the settings of the application's materials. */
			struct Material final {
				
				/* SHADER */
				inline static std::vector<const char*> s_AvailableShaders {
					"pbr",
					"blinnphong",
				};
				
				inline static int s_CurrentShaderSelection { 0 };
				
				inline static std::weak_ptr<LouiEriksson::Engine::Graphics::Shader> s_Shader;
				
				static void UpdateShader(const size_t& _index) {
					
					if (const auto item = Resources::Get<Engine::Graphics::Shader>(s_AvailableShaders.at(_index))) {
						s_Shader = item;
						
						s_CurrentShaderSelection = _index;
					}
				}
				
				inline static glm::vec4 s_TextureScaleTranslate { 1.0, 1.0, 0.0, 0.0 };
			
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
				
				inline static int s_CurrentShadowResolutionSelection { 6 };
				
				inline static int s_ShadowSamples { 24 };
				
				inline static scalar_t s_ShadowBias       { 0.01 };
				inline static scalar_t s_ShadowNormalBias { 0.02 };
				
				inline static bool s_ParallaxShadows { true };
				
				/* LIGHT */
				inline static std::vector<const char*> s_AvailableLightTypes {
					"point",
					"directional",
					"spot"
				};
				
				inline static int s_CurrentLightType { 0 };
				
			    inline static glm::vec3 s_LightPosition   {   0.0,  2.5,  0.0   }; /**< @brief Position of light in world-space.     */
			    inline static glm::vec3 s_LightRotation   { -45.0, 45.0,  0.0   }; /**< @brief Position of light in world-space.     */
			    inline static glm::vec3 s_LightColor      {   1.0,  0.91, 0.874 }; /**< @brief Color of light.                       */
				inline static  scalar_t s_LightIntensity  {   3.0 };               /**< @brief Brightness of light.                  */
			    inline static  scalar_t s_LightRange      { 100.0 };               /**< @brief Range of light.                       */
			    inline static  scalar_t s_LightAngle      { 120.0 };               /**< @brief Cos of light's FOV (for spot lights). */
				inline static  scalar_t s_LightSize       {   1.0 };               /**< @brief Size of the light (PCSS only).        */
			
			};
		};
		
		/** @brief Container for the settings of the application's post-processing effects. */
		struct PostProcessing final {
			
			inline static bool s_Enabled { true };
			
			/** @brief Container for the settings of the AmbientOcclusion post-processing effect. */
			struct AmbientOcclusion final {
			
				inline static bool s_Enabled { true };
			
				inline static int s_Samples   { 12 };
				inline static int s_Downscale {  1 };
				
				inline static scalar_t s_Intensity { 1.0 };
				inline static scalar_t s_Radius    { 0.2 };
				
				static bool IsActiveAndEnabled() noexcept {
					return s_Enabled && s_Samples > 0 && s_Intensity > 0.0 && s_Radius > 0.0;
				}
				
			};
			
			/** @brief Container for the settings of the Bloom post-processing effect. */
			struct Bloom final {
			
				inline static bool s_Enabled { true };
				
				inline static scalar_t s_Intensity   {  0.5 };
				inline static scalar_t s_Threshold   {  1.2 };
				inline static scalar_t s_Clamp       { 25.0 };
				inline static scalar_t s_LensDirt    {  0.2 };
				inline static scalar_t s_Anamorphism {  0.0 };
				inline static scalar_t s_Diffusion   {  3.0 };
			
				static bool IsActiveAndEnabled() noexcept {
					return s_Enabled && s_Intensity > 0.0 && s_Threshold <= s_Clamp && s_Diffusion > 0.0;
				}
				
			};
			
			/** @brief Container for the settings of the "aces" post-processing effect. */
			struct ToneMapping final {
				
				inline static bool s_Enabled { true };
				
				inline static scalar_t s_Gain     { 0.0 };
				inline static scalar_t s_Exposure { 1.0 };
				
				static bool IsActiveAndEnabled() noexcept  {
					return s_Enabled;
				}
				
				/** @brief Container for the settings of the AutoExposure post-processing effect. */
				struct AutoExposure final {
					
					inline static bool s_Enabled { true };
					
					inline static scalar_t s_MinEV        { 0.2 };
					inline static scalar_t s_MaxEV        { 4.0 };
					inline static scalar_t s_Compensation { 0.0 };
					inline static scalar_t s_SpeedDown    { 1.0 };
					inline static scalar_t s_SpeedUp      { 1.0 };
					
					static bool IsActiveAndEnabled() noexcept {
						return Settings::PostProcessing::ToneMapping::s_Enabled && s_Enabled && s_MinEV < s_MaxEV && s_MaxEV > 0.0;
					}
					
				};
				
			};
			
			/** @brief Container for the settings of the "fxaa" post-processing effect. */
			struct AntiAliasing final {
			
				inline static bool s_Enabled { true };
			
				inline static scalar_t s_ContrastThreshold     { 0.0312 };
				inline static scalar_t s_RelativeThreshold     { 0.063  };
				inline static scalar_t s_SubpixelBlending      { 0.75   };
				inline static scalar_t s_EdgeBlending          { 1.0    };
				inline static scalar_t s_LocalContrastModifier { 0.5    };
				
				static bool IsActiveAndEnabled() noexcept {
					return s_Enabled;
				}
				
			};
			
			/** @brief Container for the settings of the "grain" post-processing effect. */
			struct Grain final {
			
				inline static bool s_Enabled { true };
			
				inline static scalar_t s_Intensity { 0.001 };
				
				static bool IsActiveAndEnabled() noexcept {
					return s_Enabled && s_Intensity > 0.0;
				}
				
			};

			/** @brief Container for the settings of the "vignette" post-processing effect. */
			struct Vignette final {
			
				inline static bool s_Enabled { true };
				
				inline static scalar_t s_Intensity  { 0.667 };
				inline static scalar_t s_Smoothness { 0.333 };
				
				static bool IsActiveAndEnabled() noexcept {
					return s_Enabled && s_Intensity > 0.0;
				}
				
			};
			
		};
		
	};
	
} // LouiEriksson::Engine

#endif //FINALYEARPROJECT_SETTINGS_HPP