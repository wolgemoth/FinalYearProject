#ifndef FINALYEARPROJECT_SOUND_H
#define FINALYEARPROJECT_SOUND_H

#include "stdafx.h"

namespace LouiEriksson {

	class Sound {
	
		/*
		 * Please refer to OpenAL-Soft spec:
		 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
		 */
		
	private:
		
		inline static ALCdevice*  s_Device  = nullptr;
		inline static ALCcontext* s_Context = nullptr;
		
		inline static unsigned int s_SDL_Device = 0u;
		
		inline static ALuint s_GlobalSource = AL_NONE;
		
	public:
	
		struct Clip {
			
			struct Format {
			
				SDL_AudioSpec m_Specification;
				
				[[nodiscard]] ALenum OpenALFormat() const;
				
				explicit Format(const SDL_AudioSpec& _audioSpec);
			};
			
			Format m_Format;
			
			ALuint m_ALBuffer;
			
			Uint8* m_Data;
			Uint32 m_Size;
			
			explicit Clip(const std::filesystem::path& _path);
			
			~Clip();
			
			void Free();
			
			void Dispose();
		};
		
		static void PlayGlobal(const Sound::Clip& _clip);
		
		static void Init();
	
		static void Dispose();
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_SOUND_H