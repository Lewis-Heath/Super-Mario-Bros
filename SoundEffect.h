#ifndef _SOUNDEFFECT_H
#define _SOUNDEFFECT_H

#include "SDL.h"
#include "Commons.h"
#include "Constants.h"
#include <SDL_mixer.h>
#include <iostream>

using namespace std;

class SoundEffect
{
public:
	SoundEffect();
	~SoundEffect();

	bool LoadFromFile(string path);
	void Free();
	void Play();

private:
	Mix_Chunk* mSound;
};

#endif // _SOUNDEFFECT_H
