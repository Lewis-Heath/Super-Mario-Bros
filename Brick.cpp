#include "Brick.h"
#include "Texture2D.h"
#include "Constants.h"
using namespace std;

Brick::Brick(SDL_Renderer* renderer, Camera* camera, std::string imagePath, Vector2D startPosition, string type, int tubeFrame)
{
	mRenderer = renderer;
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	mTexture->LoadFromFile(imagePath);
	mPosition = startPosition;
	mOriginalPosition = startPosition;
	mCurrentFrameTime = 0;
	mFrameTime = 400;
	mFrame = 0;
	mType = type;
	if (mType == "Tube")
	{
		mPosition.y += 10;
		mTubeFrame = tubeFrame;
	}
	mAnimating = true;
	mBounceMax = 10.0f;
	mBounceSpeed = 60.0f;
	mBouncing = false;
	mTotalMovement = 0.0f;
	mDead = false;
	if (mType != "Tube")
		mSourceRect = new SDL_Rect{ 16 * mFrame, 0, 16, 16 };
	else
		mSourceRect = new SDL_Rect{ 0, 32 * mTubeFrame, 24, 32 };
}

Brick::~Brick()
{
	mRenderer = NULL;
}

void Brick::Render()
{
	if (!mDead)
	{
		mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, mSourceRect);
	}
}

void Brick::Update(float deltaTime, SDL_Event e)
{
	if (mType != "Tube")
		SetSourceRect(16 * mFrame, 0, 16, 16);
	else
		SetSourceRect(0, 32 * mTubeFrame, 24, 32);

	if (mType == "Question" && mAnimating)
	{
		mCurrentFrameTime += deltaTime * 1000;

		if (mCurrentFrameTime > mFrameTime) // If current frame time is greater than frame time
		{
			mFrame++; // Increases frame
			if (mFrame >= 3) // If frame is greater than or equal to 2
				mFrame = 0; // Resets the frame to 0

			mCurrentFrameTime = 0; // Resets the current frame time
		}
	}

	if (mBouncing)
	{
		float increment = deltaTime * mBounceSpeed;
		
		if (mTotalMovement < mBounceMax)
		{
			mPosition.y -= increment;
			mTotalMovement += increment;
		}
		if (mTotalMovement >= mBounceMax)
		{
			mPosition.y += increment;
			if (mPosition.y >= mOriginalPosition.y)
			{
				mPosition.y = mOriginalPosition.y;
				mBouncing = false;
			}
		}
	}
}

void Brick::Debug()
{
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 1);
	SDL_RenderDrawRect(mRenderer, new SDL_Rect{ (int)mPosition.x, (int)mPosition.y, (int)(16 * SCALE), (int)(16 * SCALE) });
}

string Brick::GetType()
{
	return mType;
}

void Brick::SetFrame(int frame)
{
	mFrame = frame;
}

void Brick::SetAnimating(bool state)
{
	mAnimating = state;
}

int Brick::GetFrame()
{
	return mFrame;
}

void Brick::Bounce()
{
	mBouncing = true;
	mTotalMovement = 0;
}

bool Brick::GetBouncing()
{
	return mBouncing;
}

bool Brick::GetDead()
{
	return mDead;
}

void Brick::SetDead(bool state)
{
	mDead = state;
}

void Brick::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

Vector2D Brick::getPosition()
{
	return mPosition;
}

void Brick::SetSourceRect(int x, int y, int w, int h)
{
	mSourceRect->x = x;
	mSourceRect->y = y;
	mSourceRect->w = w;
	mSourceRect->h = h;
}

SDL_Rect Brick::GetCollisionBox()
{
	if (mType != "Tube")
	{
		int x = mPosition.x;
		int y = mPosition.y;
		int w = 16 * SCALE;
		int h = 16 * SCALE;
		return SDL_Rect{ x, y, w, h };
	}
	else
	{
		int x = mPosition.x;
		int y = mPosition.y;
		int w = 24 * SCALE;
		int h = 32 * SCALE;
		return SDL_Rect{ x, y, w, h };
	}
}
