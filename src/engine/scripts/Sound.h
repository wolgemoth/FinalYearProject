#ifndef FINALYEARPROJECT_SOUND_H
#define FINALYEARPROJECT_SOUND_H

class Sound {

public:

	struct Clip {
		
		SDL_AudioSpec m_Specification;
		
		Uint8* data;
		Uint32 size;
		
		Clip(const std::filesystem::path& _path);
		
		~Clip();
	};
	
	static void Play(const Clip& _clip);
	
	static void Init();

};

#endif //FINALYEARPROJECT_SOUND_H