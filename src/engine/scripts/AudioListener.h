#ifndef FINALYEARPROJECT_AUDIOLISTENER_H
#define FINALYEARPROJECT_AUDIOLISTENER_H

#include "Transform.h"

#include "Sound.h"

namespace LouiEriksson {
	
	class AudioListener: public Component {
	
	/*
	 * Please refer to OpenAL-Soft spec:
	 * https://github.com/kcat/openal-soft/wiki/Programmer%27s-Guide
	 */
	
	private:
		
		 explicit AudioListener(const std::shared_ptr<GameObject>& _parent);
		~AudioListener() override;
		
	public:
		
		void Init();
	
		void Tick();
		
		void Sync();
		
	};
	
} // LouiEriksson

#endif //FINALYEARPROJECT_AUDIOLISTENER_H