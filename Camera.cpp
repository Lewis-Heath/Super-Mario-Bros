#include "Camera.h"

Camera::Camera()
{
	mRect = SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

Camera::~Camera()
{
}

SDL_Rect Camera::GetCameraRect()
{
	return mRect;
}

void Camera::ChangeCameraX(float x)
{
	mRect.x = x;
}

void Camera::ChangeCameraY(float y)
{
	mRect.y = y;
}
