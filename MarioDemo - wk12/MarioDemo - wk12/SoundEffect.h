#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

/* std lib */
#include <iostream>

/* sdl lib */
#include <SDL.h>
#include <SDL_mixer.h>

/* usr lib */
//#include "Constants.h"

class SoundEffect {
private:
	Mix_Chunk* mSEffect;

public:
	SoundEffect(std::string path = std::string());
	~SoundEffect();

	void Play();
	void Load(std::string path);
	void Free();

};

#endif // SOUNDEFFECT_H