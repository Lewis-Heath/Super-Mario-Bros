#ifndef _FIREBALL_H
#define _FIREBALL_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"
#include "LevelMap.h"
#include "Camera.h"

using namespace std;

class Fireball
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	Camera* mCamera;
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	void AddGravity(float deltaTime);
	bool mJumping;
	float mJumpForce;

private:
	FACING mFacingDirection;
	float mMovementSpeed;
	bool mMovingLeft;
	bool mMovingRight;
	int mFrameTime;
	int mCurrentFrameTime;
	int mFrame;
	LevelMap* mCurrentLevelMap;
	bool mCollidingX;
	bool mCollidingY;
	bool mOnPlatform;
	bool mDead;
	bool mBeenHit;
	float mDeathTime;
	float mDeathCounter;
	SDL_Rect* mSourceRect;

public:
	Fireball(SDL_Renderer* renderer, Camera* camera, LevelMap* map, string imagePath, Vector2D startPosition, FACING direction);
	~Fireball();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	bool CheckCollidingX(SDL_Rect BrickRect);
	bool CheckCollidingY(SDL_Rect BrickRect);
	void CheckOnPlatform(SDL_Rect BrickRect);
	void SetCollidingX(bool state);
	bool GetCollidingX();
	void SetCollidingY(bool state);
	bool GetCollidingY();
	void SetDead(bool state);
	bool GetDead();
	SDL_Rect GetCollisionBox();
	bool GetJumping();
	void SetPosition(Vector2D newPosition);
	Vector2D GetPosition();
	void SetJumpForce(float force);
	void Jump(float force);
	void SetFrame(int frame);
	void SetBeenHit(bool state);
	bool GetBeenHit();
	FACING GetFacingDirection();
	void SetSourceRect(int x, int y, int w, int h);
	Vector2D mOldPos;
	Vector2D mNewPos;

};

#endif // !_FIREBALL_H

