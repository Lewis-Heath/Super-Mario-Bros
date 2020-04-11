#ifndef _BRICK_H
#define _BRICK_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "Camera.h"

using namespace std;

class Texture2D;
class Brick
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	Camera* mCamera;

public:
	Brick(SDL_Renderer* renderer, Camera* camera, std::string imagePath, Vector2D startPosition, string type, int tubeFrame);
	~Brick();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	void SetPosition(Vector2D newPosition);
	Vector2D getPosition();
	SDL_Rect GetCollisionBox();
	void Debug();
	string GetType();
	void SetFrame(int frame);
	void SetAnimating(bool state);
	int GetFrame();
	string mType;
	void Bounce();
	bool GetBouncing();
	bool GetDead();
	void SetDead(bool state);
	void SetSourceRect(int x, int y, int w, int h);

private:
	int mFrameTime;
	int mCurrentFrameTime;
	int mFrame;
	int mAnimating;
	bool mBouncing;
	float mBounceMax;
	float mBounceSpeed;
	Vector2D mOriginalPosition;
	float mTotalMovement;
	bool mDead;
	int mTubeFrame;
	SDL_Rect* mSourceRect;

};
#endif // !_BRICK_H
