#ifndef FINALYEARPROJECT_SOUND_H
#define FINALYEARPROJECT_SOUND_H

class Sound {

private:

public:

	struct Clip {
		
		SDL_AudioSpec m_Specification;
	
		std::vector<char> m_Buffer;
		
		Clip(const std::string& _path);
	};
	
	static void Play(const Clip& _clip);
	
	static void Init();

};

#endif //FINALYEARPROJECT_SOUND_H