#ifndef _FLAG_H
#define _FLAG_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "Camera.h"
#include "Texture2D.h"

using namespace std;

class Texture2D;
class Flag
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	Camera* mCamera;

public:
	Flag(SDL_Renderer* renderer, Camera* camera, string imagePath, Vector2D startPosition);
	~Flag();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	Vector2D GetPosition();
	SDL_Rect GetCollisionBox();
	void SetBeenHit(bool state);
	bool GetBeenHit();
	bool GetReachedBottom();

private:
	Vector2D mStartPosition;
	float mTotalMovement;
	SDL_Rect* mSourceRect;
	bool mBeenHit;
	bool mReachedBottom;
};

#endif // !_FLAG_H

