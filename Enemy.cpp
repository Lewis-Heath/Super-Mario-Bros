#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, Camera* camera, LevelMap* map, string imagePath, Vector2D startPosition, string type)
{
	mRenderer = renderer;
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	mTexture->LoadFromFile(imagePath);
	mPosition = startPosition;
	mCurrentLevelMap = map;
	mType = type;
	mFacingDirection = FACING_LEFT;
	mMovingLeft = false;
	mMovingRight = false;
	mMovementSpeed = 30.0f;
	mJumpForce = 0.0f;
	mJumping = true;
	mCurrentFrameTime = 0;
	mFrameTime = 400;
	mFrame = 0;
	mCollidingX = false;
	mCollidingY = false;
	mOnPlatform = false;
	mBeenHit = false;
	mDeathTime = 1000.0f;
	mDeathCounter = 0.0f;
	mChangingCounter = 0.0f;
	mChangingState = false;
	mStayStationary = false;
	mKills = 0;
	if (mType == "Koopa")
	{
		mState = "Standing";
		mSourceRect = new SDL_Rect{ 16 * mFrame, 0, 16, 20 };
	}
	else
	{
		mSourceRect = new SDL_Rect{ 16 * mFrame, 0, 16, 16 };
	}
}

Enemy::~Enemy()
{
	mRenderer = NULL;
}

void Enemy::Render()
{
	if (mType == "Goomba")
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
			if (mDeathType == "Fireball")
				mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_VERTICAL, 0.0f, mSourceRect);
			else
				mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, mSourceRect);
		}
	}

	if (mType == "Koopa")
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
			if (mDeathType == "Fireball")
				mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_VERTICAL, 0.0f, mSourceRect);
			else
				mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, mSourceRect);
		}
	}
}

void Enemy::Update(float deltaTime, SDL_Event e)
{
	if (mType == "Goomba")
	{
		SetSourceRect(16 * mFrame, 0, 16, 16);
	}

	if (mType == "Koopa")
	{
		SetSourceRect(16 * mFrame, 0, 16, 20);
	}

	if (!mBeenHit)
	{
		if (mType == "Goomba" || mState == "Standing")
		{
			mCurrentFrameTime += deltaTime * 1000;

			if (mCurrentFrameTime > mFrameTime) // If current frame time is greater than frame time
			{
				mFrame++; // Increases frame
				if (mFrame >= 2) // If frame is greater than or equal to 2
					mFrame = 0; // Resets the frame to 0

				mCurrentFrameTime = 0; // Resets the current frame time
			}
		}

		if (mChangingState)
		{
			mChangingCounter += deltaTime * 1000;
			if (mChangingCounter > 200)
			{
				mState = "Shell";
				mChangingState = false;
			}
		}
		
		SDL_Rect EnemyRect = GetCollisionBox();
		int bottom = EnemyRect.y + EnemyRect.h;
		int left = EnemyRect.x;

		if (bottom > FLOOR_HEIGHT)
		{
			mPosition.y = FLOOR_HEIGHT - EnemyRect.h;
		}

		if (bottom >= FLOOR_HEIGHT)
		{
			mOnPlatform = true;
		}

		if (mJumping)
		{
			//	//Adjust the position
			mPosition.y -= mJumpForce * deltaTime / 2;
			//	//Reduce the jump force
			mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime / 2;
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
		}

		if (!mStayStationary)
		{
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
	}
	else
	{
		if (mDeathType == "Squash")
		{
			mDeathCounter += deltaTime * 1000;
			if (mDeathCounter > mDeathTime)
			{
				mDead = true;
			}
		}

		if (mDeathType == "Fireball")
		{
			if (mPosition.y + GetCollisionBox().h > SCREEN_HEIGHT + 50)
			{
				mDead = true;
			}
			else
			{
				if (mJumping)
				{
					//Adjust the position
					mPosition.y -= mJumpForce * deltaTime;
					//Reduce the jump force
					mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;
				}
				AddGravity(deltaTime);
			}
		}
	}
}

SDL_Rect Enemy::GetCollisionBox()
{
	if (mType == "Goomba")
	{
		int x = mPosition.x;
		int y = mPosition.y;
		int w = 16 * SCALE;
		int h = 16 * SCALE;
		return SDL_Rect{ x, y, w, h };
	}

	if (mType == "Koopa")
	{
		if (mState == "Standing")
		{
			int x = mPosition.x;
			int y = mPosition.y;
			int w = 16 * SCALE;
			int h = 20 * SCALE;
			return SDL_Rect{ x, y, w, h };
		}

		if (mState == "Shell" || mState == "SafeShell")
		{
			int x = mPosition.x;
			int y = mPosition.y;
			int w = 16 * SCALE;
			int h = 16 * SCALE;
			return SDL_Rect{ x, y, w, h };
		}
	}
}

void Enemy::SetDead(bool state)
{
	mDead = state;
}

bool Enemy::GetDead()
{
	return mDead;
}

Vector2D Enemy::GetPosition()
{
	return mPosition;
}

bool Enemy::GetJumping()
{
	return mJumping;
}

void Enemy::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

FACING Enemy::GetFacingDirection()
{
	return mFacingDirection;
}

void Enemy::SetDeathType(string type)
{
	mDeathType = type;
}

string Enemy::GetEnemyType()
{
	return mType;
}

void Enemy::SetState(string state)
{
	mState = state;
}

void Enemy::SetMovementSpeed(float speed)
{
	mMovementSpeed = speed;
}

void Enemy::SetFacingDirection(FACING facing)
{
	mFacingDirection = facing;
}

void Enemy::SetChanging(bool state)
{
	mChangingState = state;
}

bool Enemy::GetChanging()
{
	return mChangingState;
}

string Enemy::GetState()
{
	return mState;
}

void Enemy::SetStationary(bool state)
{
	mStayStationary = state;
}

bool Enemy::CheckCollidingX(SDL_Rect BrickRect)
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

bool Enemy::CheckCollidingY(SDL_Rect BrickRect)
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

void Enemy::CheckOnPlatform(SDL_Rect BrickRect)
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

bool Enemy::TopHit(SDL_Rect BrickRect)
{
	int CharLeft = GetCollisionBox().x;
	int BrickRight = BrickRect.x + BrickRect.w;
	int CharRight = GetCollisionBox().x + GetCollisionBox().w;
	int BrickLeft = BrickRect.x;
	int CharBottom = GetCollisionBox().y + GetCollisionBox().h;
	int BrickTop = BrickRect.y;
	int CharTop = GetCollisionBox().y;
	int BrickBottom = BrickRect.y + BrickRect.h;

	if (BrickTop == CharBottom && CharLeft <= BrickRight && CharRight >= BrickLeft)
	{
		return true;
	}
	return false;
}

void Enemy::SetFrame(int frame)
{
	mFrame = frame;
}

void Enemy::SetBeenHit(bool state)
{
	mBeenHit = state;
}

bool Enemy::GetBeenHit()
{
	return mBeenHit;
}

void Enemy::AddGravity(float deltaTime)
{
	float increment;
	increment = deltaTime * 25;
	mPosition.y += increment;
}

void Enemy::SetCollidingX(bool state)
{
	mCollidingX = state;
}

bool Enemy::GetCollidingX()
{
	return mCollidingX;
}

void Enemy::SetCollidingY(bool state)
{
	mCollidingY = state;
}

bool Enemy::GetCollidingY()
{
	return mCollidingY;
}

void Enemy::MoveLeft(float deltaTime)
{
	if (!mJumping)
		mPosition.x -= mMovementSpeed * deltaTime;
	else
		mPosition.x -= mMovementSpeed / 1.45 * deltaTime;
}

void Enemy::MoveRight(float deltaTime)
{
	if (!mJumping)
		mPosition.x += mMovementSpeed * deltaTime;
	else
		mPosition.x += mMovementSpeed / 1.45 * deltaTime;
}

void Enemy::SetJumpForce(float force)
{
	mJumpForce = force;
}

void Enemy::Jump(float force)
{
	if (!mJumping)
	{
		mJumpForce = force;
		mJumping = true;
	}
}

void Enemy::SetTopBrick(SDL_Rect brickrect)
{
	mPosition.y = brickrect.y - GetCollisionBox().h - 5;
}

void Enemy::ChangeDirection()
{
	if (mFacingDirection == FACING_LEFT)
	{
		mFacingDirection = FACING_RIGHT;
	}
	else
	{
		mFacingDirection = FACING_LEFT;
	}
}

void Enemy::SetSourceRect(int x, int y, int w, int h)
{
	mSourceRect->x = x;
	mSourceRect->y = y;
	mSourceRect->w = w;
	mSourceRect->h = h;
}

int Enemy::GetKills()
{
	return mKills;
}

void Enemy::IncreaseKills()
{
	mKills++;
}
