#include "Flag.h"

Flag::Flag(SDL_Renderer* renderer, Camera* camera, string imagePath, Vector2D startPosition)
{
	mRenderer = renderer;
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	mTexture->LoadFromFile(imagePath);
	mPosition = startPosition;
	mStartPosition = startPosition;
	mSourceRect = new SDL_Rect{ 0, 0, 20, 16 };
	mBeenHit = false;
	mTotalMovement = 0.0f;
	mReachedBottom = false;
}

Flag::~Flag()
{
	mRenderer = NULL;
}

void Flag::Render()
{
	mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, mSourceRect);
}

void Flag::Update(float deltaTime, SDL_Event e)
{
	if (mBeenHit && !mReachedBottom)
	{
		float increment = deltaTime * 250;
		mPosition.y += increment;
		mTotalMovement += increment;
		if (mTotalMovement > 48 * 8 + 4)
		{
			mBeenHit = false;
			mReachedBottom = true;
		}
	}
}

Vector2D Flag::GetPosition()
{
	return mPosition;
}

SDL_Rect Flag::GetCollisionBox()
{
	int x = mStartPosition.x + 16 * SCALE - 3;
	int y = mStartPosition.y;
	int w = 12;
	int h = SCREEN_HEIGHT;
	return SDL_Rect{ x, y, w, h };
}

void Flag::SetBeenHit(bool state)
{
	mBeenHit = state;
}

bool Flag::GetBeenHit()
{
	return mBeenHit;
}

bool Flag::GetReachedBottom()
{
	return mReachedBottom;
}


