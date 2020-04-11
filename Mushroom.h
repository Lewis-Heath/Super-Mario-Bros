#ifndef _MUSHROOM_H
#define _MUSHROOM_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "LevelMap.h"
#include "Texture2D.h"
#include "Brick.h"

using namespace std;

class Mushroom
{

protected:
	bool mJumping;
	float mJumpForce;
	void AddGravity(float deltaTime);
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);


public:
	Mushroom(SDL_Renderer* renderer, Camera* camera, LevelMap* map, Brick* brick);
	~Mushroom();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	void SetPosition(Vector2D newPosition);
	Vector2D getPosition();
	bool GetJumping();
	void SetJumping(bool state);
	SDL_Rect GetCollisionBox();
	void SetDead(bool state);
	bool GetDead();
	bool GetSpawned();
	void Bounce();
	Brick* GetBrick();
	void SetSpawned(bool state);
	bool GetOutOfBlock();
	FACING GetFacingDirection();
	bool CheckCollidingX(SDL_Rect BrickRect);
	bool CheckCollidingY(SDL_Rect BrickRect);
	void CheckOnPlatform(SDL_Rect BrickRect);
	void SetCollidingX(bool state);
	bool GetCollidingX();
	void SetCollidingY(bool state);
	bool GetCollidingY();
	int GetObjFrame();
	void SetObjFrame(int num);
	void SetJumpForce(float force);
	void Jump(float force);
	void SetTopBrick(SDL_Rect brickrect);
	void ChangeDirection();
	bool TopHit(SDL_Rect BrickRect);
	void SetSourceRect(int x, int y, int w, int h);
	Vector2D mOldPos;
	Vector2D mNewPos;

private:
	FACING mFacingDirection;
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Vector2D mOriginalPosition;
	Texture2D* mTexture;
	LevelMap* mLevelMap;
	Brick* mBrick;
	Camera* mCamera;
	bool mGravity;
	float mMovementSpeed;
	bool mDead;
	bool mSpawned;
	bool mBouncing;
	bool mOutOfBlock;
	float mBounceMax;
	float mBounceSpeed;
	float mTotalMovement;
	bool mCollidingX;
	bool mCollidingY;
	bool mOnPlatform;
	int mObjectFrame;
	SDL_Rect* mSourceRect;
	
};

#endif // !_MUSHROOM_H

