#ifndef FINALYEARPROJECT_AUDIOSOURCE_H
#define FINALYEARPROJECT_AUDIOSOURCE_H

#include "Transform.h"

#include "Sound.h"

namespace LouiEriksson {
	
	class AudioSource : public Component {
	
	private:
		
		ALuint m_Source;
		
		 explicit AudioSource(const std::shared_ptr<GameObject>& _parent);
		~AudioSource() override;
		
	public:
		
		void Init();
	
		void Play(const Sound::Clip& _clip) const;
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_AUDIOSOURCE_H