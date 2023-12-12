#ifndef FINALYEARPROJECT_SETTINGS_H
#define FINALYEARPROJECT_SETTINGS_H

class Camera;
class GUI;

namespace LouiEriksson {
	
	class Settings {
	
		friend GUI;
		
	public:
		
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