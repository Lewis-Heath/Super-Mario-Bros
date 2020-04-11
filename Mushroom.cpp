#include "Mushroom.h"

Mushroom::Mushroom(SDL_Renderer* renderer, Camera* camera, LevelMap* map, Brick* brick)
{
	string imagePath = "Images/Mushroom.png";
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(imagePath.c_str()))
	{
		cout << "Failed to load texture: " << imagePath << endl;
		return;
	}
	mBrick = brick;
	mLevelMap = map;
	mPosition = brick->getPosition();
	mPosition.y -= 36;
	mOriginalPosition = mPosition;
	mMovementSpeed = 80.0f;
	mDead = false;
	mSpawned = false;
	mBounceMax = 12.0f;
	mBounceSpeed = 30.0f;
	mBouncing = false;
	mTotalMovement = 0.0f;
	mOutOfBlock = false;
	mFacingDirection = FACING_RIGHT;
	mCollidingX = false;
	mCollidingY = true;
	mOnPlatform = true;
	mObjectFrame = 0;
	mJumpForce = 0.0f;
	mJumping = false;
	mSourceRect = new SDL_Rect{ mObjectFrame * 16, 0, 16, 16 };
}

Mushroom::~Mushroom()
{
	mRenderer = NULL;
	delete mTexture;
	mTexture = NULL;
	mLevelMap = NULL;
}

void Mushroom::Render()
{
	if(!mDead && mSpawned)
		mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, NULL, mSourceRect);
}

void Mushroom::Update(float deltaTime, SDL_Event e)
{
	SetSourceRect(mObjectFrame * 16, 0, 16, 16);
	if (mBouncing && !mOutOfBlock)
	{
		float increment = deltaTime * mBounceSpeed;

		if (mTotalMovement < mBounceMax)
		{
			mPosition.y -= increment;
			mTotalMovement += increment;
		}

		if (mTotalMovement > mBounceMax)
		{
			mPosition.y = mOriginalPosition.y - mBounceMax;
			mBouncing = false;
			mOutOfBlock = true;
		}
	}

	if (mObjectFrame == 0)
	{
		if (mOutOfBlock)
		{

			SDL_Rect MushyRect = GetCollisionBox();
			int bottom = MushyRect.y + MushyRect.h;
			int left = MushyRect.x;

			if (bottom > FLOOR_HEIGHT)
			{
				mPosition.y = FLOOR_HEIGHT - MushyRect.h;
			}

			if (bottom >= FLOOR_HEIGHT)
			{
				mOnPlatform = true;
			}

			if (mJumping)
			{
			//	//Adjust the position
				mPosition.y -= mJumpForce * deltaTime;
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
				else if (mFacingDirection == FACING_RIGHT)
				{
					MoveRight(deltaTime);
				}
			}
		}
	}
}

SDL_Rect Mushroom::GetCollisionBox()
{
	int x = mPosition.x;
	int y = mPosition.y;
	int w = 16 * SCALE;
	int h = 16 * SCALE;
	return SDL_Rect{ x, y, w, h };
}

void Mushroom::SetDead(bool state)
{
	mDead = state;
}

bool Mushroom::GetDead()
{
	return mDead;
}

bool Mushroom::GetSpawned()
{
	return mSpawned;
}

void Mushroom::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

Vector2D Mushroom::getPosition()
{
	return mPosition;
}

bool Mushroom::GetJumping()
{
	return mJumping;
}

void Mushroom::Bounce()
{
	mBouncing = true;
	mTotalMovement = 0;
}

Brick* Mushroom::GetBrick()
{
	return mBrick;
}

void Mushroom::SetSpawned(bool state)
{
	mSpawned = state;
}

bool Mushroom::GetOutOfBlock()
{
	return mOutOfBlock;
}

FACING Mushroom::GetFacingDirection()
{
	return mFacingDirection;
}

void Mushroom::SetJumping(bool state)
{
	mJumping = state;
}

void Mushroom::SetSourceRect(int x, int y, int w, int h)
{
	mSourceRect->x = x;
	mSourceRect->y = y;
	mSourceRect->w = w;
	mSourceRect->h = h;
}

bool Mushroom::CheckCollidingX(SDL_Rect BrickRect)
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

bool Mushroom::CheckCollidingY(SDL_Rect BrickRect)
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

void Mushroom::CheckOnPlatform(SDL_Rect BrickRect)
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

bool Mushroom::TopHit(SDL_Rect BrickRect)
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
		//cout << "Top Hit" << ", " << mPlayerNumber << endl;
		return true;
	}
	return false;
}

void Mushroom::AddGravity(float deltaTime)
{
	float increment;
	increment = deltaTime * 25;
	mPosition.y += increment;
}

void Mushroom::SetCollidingX(bool state)
{
	mCollidingX = state;
}

bool Mushroom::GetCollidingX()
{
	return mCollidingX;
}

void Mushroom::SetCollidingY(bool state)
{
	mCollidingY = state;
}

bool Mushroom::GetCollidingY()
{
	return mCollidingY;
}

int Mushroom::GetObjFrame()
{
	return mObjectFrame;
}

void Mushroom::SetObjFrame(int num)
{
	mObjectFrame = num;
}

void Mushroom::MoveLeft(float deltaTime)
{
	if (!mJumping)
		mPosition.x -= mMovementSpeed * deltaTime;
	else
		mPosition.x -= mMovementSpeed / 1.45 * deltaTime;
}

void Mushroom::MoveRight(float deltaTime)
{
	if (!mJumping)
		mPosition.x += mMovementSpeed * deltaTime;
	else
		mPosition.x += mMovementSpeed / 1.45 * deltaTime;
}

void Mushroom::SetJumpForce(float force)
{
	mJumpForce = force;
}

void Mushroom::Jump(float force)
{
	if (!mJumping)
	{
		mJumpForce = force;
		mJumping = true;
	}
}

void Mushroom::SetTopBrick(SDL_Rect brickrect)
{
	mPosition.y = brickrect.y - GetCollisionBox().h - 5;
}

void Mushroom::ChangeDirection()
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
