#ifndef _CAMERA_H
#define _CAMERA_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "Constants.h"

using namespace std;

class Camera
{
public:
	Camera();
	~Camera();

	SDL_Rect GetCameraRect();
	void ChangeCameraX(float x);
	void ChangeCameraY(float y);

private:
	SDL_Rect mRect;
};

#endif // !_CAMERA_H
