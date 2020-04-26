#include "SoundEffect.h"

SoundEffect::SoundEffect(std::string path) {
	mSEffect = nullptr;

	if(!path.empty())
		Load(path);
}

SoundEffect::~SoundEffect() {
	Free();
}

void SoundEffect::Play() {
	Mix_PlayChannel(-1, mSEffect, 0);
}

void SoundEffect::Load(std::string path) {
	Mix_Chunk* newSFX = Mix_LoadWAV(path.c_str());

	if (newSFX != nullptr) {
		mSEffect = newSFX;
	} else {
		std::cout << "Failed to load " << path << 
			". Error: " << Mix_GetError() << std::endl;
	}
}

void SoundEffect::Free() {
	Mix_FreeChunk(mSEffect);
	mSEffect = nullptr;
}
