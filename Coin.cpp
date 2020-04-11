#include "Coin.h"

Coin::Coin(SDL_Renderer* renderer, Camera* camera, LevelMap* map, Brick* brick, Vector2D startPosition, string type)
{
	string imagePath = "Images/Coin2.png";
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(imagePath.c_str()))
	{
		cout << "Failed to load texture: " << imagePath << endl;
		return;
	}
	mLevelMap = map;
	mPosition = startPosition;
	mType = type;
	mBrick = brick;
	mMovementSpeed = 300.0f;
	mCurrentFrameTime = 0;
	mFrameTime = 300.0f;
	mFrame = 0;
	mTotalMovement = 0;
	mMaxMovement = 72.0f;
	mStateFrame = 0;
	mDead = false;
	if (mType == "Still")
	{
		mSpawned = true;
	}
	else
	{
		mSpawned = false;
	}
	mSourceRect = new SDL_Rect{ mFrame * 10, mStateFrame * 10, 10, 10 };
}

Coin::~Coin()
{
	mRenderer = NULL;
	delete mTexture;
	mTexture = NULL;
	mLevelMap = NULL;
}

void Coin::Render()
{
	if (!mDead && mSpawned)
		mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, NULL, mSourceRect);
}

void Coin::Update(float deltaTime, SDL_Event e)
{
	SetSourceRect(mFrame * 10, mStateFrame * 10, 10, 10);
	if (!mDead && mSpawned)
	{
		mCurrentFrameTime += deltaTime * 1000;

		if (mCurrentFrameTime > mFrameTime) // If current frame time is greater than frame time
		{
			mFrame++; // Increases frame
			if (mFrame >= 3) // If frame is greater than or equal to 2
				mFrame = 0; // Resets the frame to 0

			mCurrentFrameTime = 0; // Resets the current frame time
		}

		if (mType == "Block")
		{
			if (mTotalMovement <= mMaxMovement)
				MoveUp(deltaTime);
			if (mTotalMovement > mMaxMovement)
			{
				mStateFrame = 1;
			}
		}

		if (mStateFrame == 1)
		{
			mEndCounter++;
			if (mEndCounter > 1000)
			{
				mDead = true;
			}
		}
	}
	
}

void Coin::MoveUp(float deltaTime)
{
	float increment = mMovementSpeed * deltaTime;
	mPosition.y -= increment;
	mTotalMovement += increment;
}

bool Coin::GetDead()
{
	return mDead;
}

void Coin::SetDead(bool state)
{
	mDead = state;
}

bool Coin::GetSpawned()
{
	return mSpawned;
}

void Coin::SetSpawned(bool state)
{
	mSpawned = state;
}

void Coin::SetFrame(int num)
{
	mFrame = num;
}

void Coin::SetStateFrame(int num)
{
	mStateFrame = num;
}

int Coin::GetStateFrame()
{
	return mStateFrame;
}

SDL_Rect Coin::GetCollisionBox()
{
	int x = mPosition.x;
	int y = mPosition.y;
	int w = 10 * SCALE;
	int h = 10 * SCALE;
	return SDL_Rect{ x, y, w, h };
}

void Coin::SetFrameTime(float num)
{
	mFrameTime = num;
}

Brick* Coin::GetBrick()
{
	return mBrick;
}

void Coin::SetSourceRect(int x, int y, int w, int h)
{
	mSourceRect->x = x;
	mSourceRect->y = y;
	mSourceRect->w = w;
	mSourceRect->h = h;
}



