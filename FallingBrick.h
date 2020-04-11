#ifndef _FALLINGBRICK_H
#define _FALLINGBRICK_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"
#include "Constants.h"
#include "Camera.h"

using namespace std;

class Texture2D;
class FallingBrick
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	Camera* mCamera;
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	void AddGravity(float deltaTime);

public:
	FallingBrick(SDL_Renderer* renderer, Camera* camera, Vector2D startPosition, float angle, FACING direction);
	~FallingBrick();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	bool GetDead();

private:
	FACING mFacingDirection;
	float mMovementSpeed;
	float mGravCounter;
	float mAngle;
	float mJumpForce;
	bool mDead;
	SDL_Rect* mSourceRect;
};

#endif // !_FALLINGBRICK_H