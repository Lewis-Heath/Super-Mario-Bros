#ifndef _COIN_H
#define _COIN_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "LevelMap.h"
#include "Texture2D.h"
#include "Brick.h"

using namespace std;

class Coin 
{

protected:
	virtual void MoveUp(float deltaTime);

public:
	Coin(SDL_Renderer* renderer, Camera* camera, LevelMap* map, Brick* brick, Vector2D startPosition, string type);
	~Coin();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	bool GetDead();
	void SetDead(bool state);
	bool GetSpawned();
	void SetSpawned(bool state);
	void SetFrame(int num);
	void SetStateFrame(int num);
	int GetStateFrame();
	SDL_Rect GetCollisionBox();
	void SetFrameTime(float num);
	Brick* GetBrick();
	void SetSourceRect(int x, int y, int w, int h);

private:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	LevelMap* mLevelMap;
	Brick* mBrick;
	Camera* mCamera;
	int mFrameTime;
	int mCurrentFrameTime;
	int mFrame;
	float mMovementSpeed;
	bool mDead;
	string mType;
	float mTotalMovement;
	bool mSpawned;
	float mMaxMovement;
	int mStateFrame;
	int mEndCounter;
	SDL_Rect* mSourceRect;

};

#endif // !_COIN_H