#ifndef _ENEMY_H
#define _ENEMY_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"
#include "LevelMap.h"
#include "Camera.h"

using namespace std;

class Texture2D;
class Enemy
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	Camera*	mCamera;
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
	string mType;
	bool mCollidingX;
	bool mCollidingY;
	bool mOnPlatform;
	bool mDead;
	bool mBeenHit;
	float mDeathTime;
	float mDeathCounter;
	string mDeathType;
	string mState;
	bool mChangingState;
	float mChangingCounter;
	bool mStayStationary;
	SDL_Rect* mSourceRect;
	int mKills;

public:
	Enemy(SDL_Renderer* renderer, Camera* camera, LevelMap* map, string imagePath, Vector2D startPosition, string type);
	~Enemy();

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
	void SetTopBrick(SDL_Rect brickrect);
	void ChangeDirection();
	bool TopHit(SDL_Rect BrickRect);
	void SetFrame(int frame);
	void SetBeenHit(bool state);
	bool GetBeenHit();
	FACING GetFacingDirection();
	void SetDeathType(string type);
	string GetEnemyType();
	void SetState(string state);
	void SetMovementSpeed(float speed);
	void SetFacingDirection(FACING facing);
	void SetChanging(bool state);
	bool GetChanging();
	string GetState();
	void SetStationary(bool state);
	void SetSourceRect(int x, int y, int w, int h);
	int GetKills();
	void IncreaseKills();
	Vector2D mOldPos;
	Vector2D mNewPos;
};

#endif // !_ENEMY_H
