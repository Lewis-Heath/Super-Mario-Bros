#include "SoundEffect.h"

SoundEffect::SoundEffect()
{
}

SoundEffect::~SoundEffect()
{
	//Free up the memory
	Free();
}

bool SoundEffect::LoadFromFile(string path)
{
	Free();

	mSound = NULL;

	mSound = Mix_LoadWAV(path.c_str());

	if (mSound == NULL)
	{
		return false;
	}
	return true;
}

void SoundEffect::Free()
{
	if (mSound != NULL)
	{
		Mix_FreeChunk(mSound);
		mSound = NULL;
	}
}

void SoundEffect::Play()
{
	//Play the sound effect
	Mix_VolumeChunk(mSound, 5);
	Mix_PlayChannel(-1, mSound, 0);
}
