#ifndef FINALYEARPROJECT_SETTINGS_H
#define FINALYEARPROJECT_SETTINGS_H

#include "Texture.h"
#include "Shader.h"

class Application;
class Camera;
class GUI;
class OrbitCam;

namespace LouiEriksson {
	
	class Settings {
	
		friend Application;
		friend GUI;
		
	public:
		
		static void Init();
		
		struct Graphics {
			
			friend Camera;
			
			struct Perspective {
				
				friend OrbitCam;
			
				struct Orbit {
					
					inline static bool s_Enabled = true;
					
					inline static glm::vec3 s_Speed  = glm::vec3(-0.25f, 0.33f,   0.5f);
					inline static glm::vec3 s_Amount = glm::vec3(  0.0f,  4.0f,   5.0f);
					inline static glm::vec3 s_Offset = glm::vec3(  0.0f,  2.5f, -12.0f);
				};
			};
			
			struct Skybox {
				
				/* SKYBOX TEXTURE */
				inline static std::vector<const char*> s_AvailableSkyboxes = {
					"little_paris_eiffel_tower_4k",
					"abandoned_workshop_02_8k",
					"blue_photo_studio_4k",
				};
				
				inline static int s_CurrentSkyboxSelection = 0;
				
				inline static std::weak_ptr<Texture> s_Skybox;
				
				static void UpdateSkybox(const int& _index);
				
				/* PARAMETERS */
				inline static float s_Blur     = 0.0f;
				inline static float s_Exposure = 1.0f;
				
			};
			
			struct Material {
				
				/* SHADER */
				inline static std::vector<const char*> s_AvailableShaders = {
					"pbr",
					"blinnphong",
				};
				
				inline static int s_CurrentShaderSelection = 0;
				
				inline static std::weak_ptr<Shader> s_Shader;
				
				static void UpdateShader(const int& _index);
				
				/* PARAMETERS */
				inline static float s_Displacement = 0.01;
				inline static glm::vec4 s_TextureScaleTranslate = glm::vec4(3.0f, 3.0f, 0.0f, 0.0f);
			};
		};
		
		struct PostProcessing {
			
			friend Camera;
			
			inline static bool s_Enabled = true;
			
			struct AmbientOcclusion {
			
				inline static bool s_Enabled = true;
			
				inline static int s_Samples   = 12;
				inline static int s_Downscale =  1;
				
				inline static float s_Intensity = 1.0f;
				inline static float s_Radius    = 0.2f;
				
				static bool IsActiveAndEnabled();
			};
			
			struct Bloom {
			
				inline static bool s_Enabled = true;
				
				inline static float s_Intensity   =  0.2f;
				inline static float s_Threshold   =  1.2f;
				inline static float s_Clamp       = 25.0f;
				inline static float s_LensDirt    =  0.6f;
				inline static float s_Anamorphism =  0.0f;
				inline static float s_Diffusion   =  3.0f;
			
				static bool IsActiveAndEnabled();
			};
			
			struct ToneMapping {
				
				inline static bool s_Enabled = true;
				
				inline static float s_Gain     = 0.0f;
				inline static float s_Exposure = 1.0f;
				
				static bool IsActiveAndEnabled();
				
				struct AutoExposure {
					
					inline static bool s_Enabled = true;
					
					inline static float s_MinEV        =  0.2f;
					inline static float s_MaxEV        =  4.0f;
					inline static float s_Compensation = -0.2f;
					inline static float s_SpeedDown    =  1.0f;
					inline static float s_SpeedUp      =  2.0f;
					
					static bool IsActiveAndEnabled();
				};
			};
			
			struct AntiAliasing {
			
				inline static bool s_Enabled = true;
			
				inline static float s_ContrastThreshold     = 0.0312f;
				inline static float s_RelativeThreshold     = 0.063f;
				inline static float s_SubpixelBlending     = 0.75f;
				inline static float s_EdgeBlending          = 1.0f;
				inline static float s_LocalContrastModifier = 0.5f;
				
				static bool IsActiveAndEnabled();
			};
			
			struct Grain {
			
				inline static bool s_Enabled = true;
			
				inline static float s_Intensity = 0.005f;
				
				static bool IsActiveAndEnabled();
			};
			
			struct Vignette {
			
				inline static bool s_Enabled = true;
				
				inline static float s_Intensity = 0.33f;
				
				static bool IsActiveAndEnabled();
			};
		};
	};
}

#endif //FINALYEARPROJECT_SETTINGS_H