#ifndef FINALYEARPROJECT_AUDIOSOURCE_H
#define FINALYEARPROJECT_AUDIOSOURCE_H

#include "Transform.h"

#include "Sound.h"

namespace LouiEriksson {
	
	class AudioSource : public Component {
	
	/*
	 * Please refer to OpenAL-Soft spec:
	 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
	 */
	
	private:
		
		ALuint m_Source;
		
	public:
		
		 explicit AudioSource(const std::shared_ptr<GameObject>& _parent);
		~AudioSource() override;
		
		void Init();
	
		void Tick();
		
		void Sync();
		
		void Play(const Sound::Clip& _clip) const;
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_AUDIOSOURCE_H