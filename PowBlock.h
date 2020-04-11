#ifndef _POWBLOCK_H
#define _POWBLOCK_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "LevelMap.h"
#include "Texture2D.h"
#include "Camera.h"
#include <iostream>

using namespace std;

class PowBlock
{
public:
	PowBlock(SDL_Renderer* renderer, Camera* camera, LevelMap* map, Vector2D startPosition);
	~PowBlock();

	virtual void Render();
	virtual void Update();
	SDL_Rect GetCollisionBox();
	void TakeAHit();
	bool IsAvailable();
	void SetBottomHit(bool state);
	bool GetBottomHit();
	int LivesLeft();
	void SetSourceRect(int x, int y, int w, int h);

private:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	LevelMap* mLevelMap;
	Camera* mCamera;

	float mSingleSpriteWidth;
	float mSingleSpriteHeight;
	int mNumberOfHitsLeft;
	bool mBottomHit;
	SDL_Rect* mSourceRect;
};

#endif // !_POWBLOCK_H
