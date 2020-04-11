#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "GameScreenLevel1.h"
#include "LevelMap.h"
#include "SoundEffect.h"

using namespace std;

class Texture2D;
class Character
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
	int mPlayerNumber;
	float mCollisionRadius;
	bool mCollidingX;
	bool mCollidingY;
	bool mCharColliding;
	bool mOnPlatform;

private:
	FACING mFacingDirection;
	float mMovementSpeed;
	bool mMovingLeft;
	bool mMovingRight;
	bool mJumpKeyPressed;
	bool mShootKeyPressed;
	int mFrameTime;
	int mCurrentFrameTime;
	int mFrame;
	LevelMap* mCurrentLevelMap;
	string mSize;
	bool mDeathAnimation;
	bool mDead;
	string mChangeType;
	bool mChangingUp;
	int mChangeCounterUp;
	bool mChangeReposMediumUp;
	bool mChangeReposLargeUp;
	bool mChangingDown;
	int mChangeCounterDown;
	bool mChangeReposMediumDown;
	bool mChangeReposLargeDown;
	FACING mCollisionXDirection;
	SoundEffect* mJumpSound;
	SDL_Rect* mSourceRect;
	bool mReachedFlag;
	bool mWalkToCastle;
	bool mFlippedFlag;
	float mStatFlagTime;
	int mCastleDoorPositionX;
	bool mReachedCastle;
	int mAirKills;
	bool mEndScreen;
	bool mFlagScoreAdded;

public:
	Character(SDL_Renderer* renderer, Camera* camera, std::string imagePath, Vector2D startPosition, int playerNumber, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPosition(Vector2D newPosition);
	Vector2D getPosition();
	FACING GetFacingDirection();
	int GetFrame();
	bool GetMoving();
	float GetCollisionRadius();
	void SetSpeed(float speed);
	bool GetJumping();
	void SetJumping(bool state);
	void SetFrame(int frame);
	void Jump(float force);
	void SetCollidingX(bool state);
	bool GetCollidingX();
	void SetCollidingY(bool state);
	bool GetCollidingY();
	void SetCharColliding(bool state);
	bool GetCharColliding();
	void SetOnPlatform(bool state);
	SDL_Rect GetCollisionBox();
	void SetJumpForce(float force);
	void Debug();
	void CheckCharColliding(SDL_Rect OtherCharRect);
	bool CheckCollidingX(SDL_Rect BrickRect);
	bool CheckCollidingY(SDL_Rect BrickRect);
	void CheckOnPlatform(SDL_Rect BrickRect);
	bool BottomHit(SDL_Rect BrickRect);
	bool TopHit(SDL_Rect BrickRect);
	string GetSize();
	void SetSize(string size);
	void SetTopBrick(SDL_Rect brickrect);
	int GetPlayerNum();
	void ChangeSizeUp();
	bool GetChangingUp();
	void ChangeSizeDown();
	bool GetChangingDown();
	void SetChangeType(string type);
	bool GetDying();
	void SetDying(bool state);
	bool GetDead();
	bool GetShootKeyPressed();
	void SetShootKeyPressed(bool state);
	void SetCollidingXDirection(FACING direction);
	void SetSourceRect(int x, int y, int w, int h);
	bool GetReachedFlag();
	void SetReachedFlag(bool state);
	void SetWalkToCastle(bool state);
	bool GetWalkToCastle();
	bool GetReachedCastle();
	bool GetOnPlatform();
	void IncreaseAirKills();
	int GetAirKills();
	void SetEndScreen(bool state);
	bool GetFlagScoreAdded();
	void SetFlagScoreAdded(bool state);
	Vector2D mOldPos;
	Vector2D mNewPos;
};

#endif // !_CHARACTER_H
