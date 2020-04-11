#include "Fireball.h"

Fireball::Fireball(SDL_Renderer* renderer, Camera* camera, LevelMap* map, string imagePath, Vector2D startPosition, FACING direction)
{
	mRenderer = renderer;
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	mTexture->LoadFromFile(imagePath);
	mPosition = startPosition;
	mCurrentLevelMap = map;
	mFacingDirection = direction;
	mMovingLeft = false;
	mMovingRight = false;
	mMovementSpeed = 500.0f;
	mJumpForce = 0.0f;
	mJumping = true;
	mCurrentFrameTime = 0;
	mFrameTime = 200;
	mFrame = 0;
	mDead = false;
	mCollidingX = false;
	mCollidingY = false;
	mOnPlatform = false;
	mBeenHit = false;
	mDeathTime = 500.0f;
	mDeathCounter = 0.0f;
	mSourceRect = new SDL_Rect{ 8 * mFrame, 0, 8, 8 };
}

Fireball::~Fireball()
{
	mRenderer = NULL;
}

void Fireball::Render()
{
	if (!mBeenHit)
	{
		if (mFacingDirection == FACING_RIGHT)
			mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, mSourceRect);
		else
			mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_HORIZONTAL, 0.0f, mSourceRect);
	}
	else
	{
		mTexture->Render(Vector2D(mPosition.x - 2, mPosition.y - 2), mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, mSourceRect);
	}
}

void Fireball::Update(float deltaTime, SDL_Event e)
{
	if (!mBeenHit)
	{
		SetSourceRect(8 * mFrame, 0, 8, 8);
	}
	else
	{
		if (mFrame == 3)
			SetSourceRect( 32, 0, 12, 12 );
		if (mFrame == 4)
			SetSourceRect(44, 0, 12, 12);
		if (mFrame == 5)
			SetSourceRect(56, 0, 12, 12);
	}

	if (!mBeenHit)
	{
		mCurrentFrameTime += deltaTime * 1000;

		if (mCurrentFrameTime > mFrameTime) // If current frame time is greater than frame time
		{
			mFrame++; // Increases frame
			if (mFrame >= 3) // If frame is greater than or equal to 2
				mFrame = 0; // Resets the frame to 0

			mCurrentFrameTime = 0; // Resets the current frame time
		}

		SDL_Rect FireballRect = GetCollisionBox();
		int bottom = FireballRect.y + FireballRect.h;
		int left = FireballRect.x;

		if (bottom > FLOOR_HEIGHT)
		{
			mPosition.y = FLOOR_HEIGHT - FireballRect.h;
		}

		if (bottom >= FLOOR_HEIGHT)
		{
			mOnPlatform = true;
		}

		if (mJumping)
		{
			//	//Adjust the position
			mPosition.y -= mJumpForce * deltaTime ;
			//	//Reduce the jump force
			mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;
		}

		if (bottom < FLOOR_HEIGHT && !mCollidingY)
		{
			mJumping = true;
			mOnPlatform = false;
		}

		if (!mOnPlatform)
		{
			AddGravity(deltaTime);
		}
		else
		{
			mJumping = false;
			Jump(250.0f);
		}

		if (!mCollidingX || mOnPlatform)
		{
			if (mFacingDirection == FACING_LEFT)
			{
				if (left - mMovementSpeed * deltaTime > 1)
				{
					MoveLeft(deltaTime);
				}
				else
				{
					mFacingDirection = FACING_RIGHT;
				}
			}
			else
			{
				MoveRight(deltaTime);
			}
		}
	}
	else
	{
		mCurrentFrameTime += deltaTime * 1000;

		if (mCurrentFrameTime > mFrameTime) // If current frame time is greater than frame time
		{
			mFrame++; // Increases frame
			if (mFrame >= 6) // If frame is greater than or equal to 2
				mFrame = 3; // Resets the frame to 0

			mCurrentFrameTime = 0; // Resets the current frame time
		}

		mDeathCounter += deltaTime * 1000;
		if (mDeathCounter > mDeathTime)
		{
			mDead = true;
		}
	}
}

SDL_Rect Fireball::GetCollisionBox()
{
	int x = mPosition.x;
	int y = mPosition.y;
	int w = 8 * SCALE;
	int h = 8 * SCALE;
	return SDL_Rect{ x, y, w, h };
}

void Fireball::SetDead(bool state)
{
	mDead = state;
}

bool Fireball::GetDead()
{
	return mDead;
}

Vector2D Fireball::GetPosition()
{
	return mPosition;
}

bool Fireball::GetJumping()
{
	return mJumping;
}

void Fireball::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

void Fireball::SetSourceRect(int x, int y, int w, int h)
{
	mSourceRect->x = x;
	mSourceRect->y = y;
	mSourceRect->w = w;
	mSourceRect->h = h;
}

FACING Fireball::GetFacingDirection()
{
	return mFacingDirection;
}

bool Fireball::CheckCollidingX(SDL_Rect BrickRect)
{
	int CharLeft = GetCollisionBox().x;
	int BrickRight = BrickRect.x + BrickRect.w;
	int CharRight = GetCollisionBox().x + GetCollisionBox().w;
	int BrickLeft = BrickRect.x;
	int CharTop = GetCollisionBox().y;
	int BrickBottom = BrickRect.y + BrickRect.h;
	int CharBottom = GetCollisionBox().y + GetCollisionBox().h;
	int BrickTop = BrickRect.y;
	if (CharTop > BrickBottom || CharBottom < BrickTop || CharLeft > BrickRight || CharRight < BrickLeft)
	{
		return false;
	}
	return true;
}

bool Fireball::CheckCollidingY(SDL_Rect BrickRect)
{
	int CharLeft = GetCollisionBox().x;
	int BrickRight = BrickRect.x + BrickRect.w;
	int CharRight = GetCollisionBox().x + GetCollisionBox().w;
	int BrickLeft = BrickRect.x;
	int CharBottom = GetCollisionBox().y + GetCollisionBox().h;
	int BrickTop = BrickRect.y;
	int CharTop = GetCollisionBox().y;
	int BrickBottom = BrickRect.y + BrickRect.h;
	if (CharLeft > BrickRight || CharRight < BrickLeft || CharBottom < BrickTop || CharTop > BrickBottom)
	{
		return false;
	}
	return true;
}

void Fireball::CheckOnPlatform(SDL_Rect BrickRect)
{
	int CharLeft = GetCollisionBox().x;
	int BrickRight = BrickRect.x + BrickRect.w;
	int CharRight = GetCollisionBox().x + GetCollisionBox().w;
	int BrickLeft = BrickRect.x;
	int CharBottom = GetCollisionBox().y + GetCollisionBox().h;
	int BrickTop = BrickRect.y;
	int CharTop = GetCollisionBox().y;
	int BrickBottom = BrickRect.y + BrickRect.h;

	if (CharBottom >= BrickTop && CharBottom < BrickBottom && mCollidingY && CharRight >= BrickLeft && CharLeft <= BrickRight)
	{
		mOnPlatform = true;
	}
}

void Fireball::SetFrame(int frame)
{
	mFrame = frame;
}

void Fireball::SetBeenHit(bool state)
{
	mBeenHit = state;
}

bool Fireball::GetBeenHit()
{
	return mBeenHit;
}

void Fireball::AddGravity(float deltaTime)
{
	float increment;
	increment = deltaTime * 25;
	mPosition.y += increment;
}

void Fireball::SetCollidingX(bool state)
{
	mCollidingX = state;
}

bool Fireball::GetCollidingX()
{
	return mCollidingX;
}

void Fireball::SetCollidingY(bool state)
{
	mCollidingY = state;
}

bool Fireball::GetCollidingY()
{
	return mCollidingY;
}

void Fireball::MoveLeft(float deltaTime)
{
	if (!mJumping)
		mPosition.x -= mMovementSpeed * deltaTime;
	else
		mPosition.x -= mMovementSpeed / 2* deltaTime;
}

void Fireball::MoveRight(float deltaTime)
{
	if (!mJumping)
		mPosition.x += mMovementSpeed * deltaTime;
	else
		mPosition.x += mMovementSpeed / 2* deltaTime;
}

void Fireball::SetJumpForce(float force)
{
	mJumpForce = force;
}

void Fireball::Jump(float force)
{
	if (!mJumping)
	{
		mJumpForce = force;
		mJumping = true;
	}
}