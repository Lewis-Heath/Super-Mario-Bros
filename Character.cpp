#include "Character.h"
#include "Texture2D.h"
#include "Constants.h"
using namespace std;

Character::Character(SDL_Renderer* renderer, Camera* camera, std::string imagePath, Vector2D startPosition, int playerNumber, LevelMap* map)
{
	mRenderer = renderer;
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	mTexture->LoadFromFile(imagePath);
	mPosition = startPosition;
	mPlayerNumber = playerNumber;
	mCurrentLevelMap = map;
	mFacingDirection = FACING_RIGHT;
	mMovingLeft = false;
	mMovingRight = false;
	mMovementSpeed = 150.0f;
	mJumpForce = 0.0f;
	mJumping = true;
	mCurrentFrameTime = 0;
	mFrameTime = 150;
	mFrame = 0;
	mCollisionRadius = 16.0f;
	mCharColliding = false;
	mCollidingX = false;
	mCollidingY = false;
	mOnPlatform = false;
	mOldPos = mPosition;
	mNewPos = mPosition;
	mSize = "Small";
	mDeathAnimation = false;
	mDead = false;
	mChangeType = "S->L";
	mChangingUp = false;
	mChangeCounterUp = 0;
	mChangeReposMediumUp = false;
	mChangeReposLargeUp = false;
	mChangingDown = false;
	mChangeCounterDown = 0;
	mChangeReposMediumDown = false;
	mChangeReposLargeDown = false;
	mCollisionXDirection = FACING_LEFT;
	mSourceRect = new SDL_Rect{ 16*mFrame, 0, 16, 16 };
	mJumpSound = new SoundEffect();
	mJumpSound->LoadFromFile("Sounds/Jump.WAV");
	mReachedFlag = false;
	mWalkToCastle = false;
	mFlippedFlag = false;
	mStatFlagTime = 0.0f;
	mReachedCastle = false;
	mCastleDoorPositionX = 170 * 48 + 5;
	mAirKills = 0;
	mEndScreen = false;
	mFlagScoreAdded = false;
}

Character::~Character()
{
	mRenderer = NULL;
}

void Character::Render()
{
	if (!mEndScreen)
	{
		if (!mDead && !mReachedCastle)
		{
			if (mDeathAnimation)
			{
				mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, new SDL_Rect{ 16 * 5, 0, 16, 16 });
			}
			else
			{
				if (!mFlippedFlag && mWalkToCastle)
				{
					mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_HORIZONTAL, 0.0f, mSourceRect);
				}
				else if (mFacingDirection == FACING_RIGHT)
				{
					mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, mSourceRect);
				}
				else if (mFacingDirection == FACING_LEFT)
				{
					mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_HORIZONTAL, 0.0f, mSourceRect);
				}
			}
		}
	}
	else
	{
		mTexture->Render(mPosition, SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, SDL_FLIP_NONE, 0.0f, mSourceRect);
	}
}

void Character::Update(float deltaTime, SDL_Event e)
{
	if (!mEndScreen)
	{
		if (!mDead)
		{
			if (mSize == "Small")
				SetSourceRect(16 * mFrame, 0, 16, 16);
			if (mSize == "Medium")
				SetSourceRect(16 * mFrame, 16, 16, 20);
			if (mSize == "Large")
				SetSourceRect(16 * mFrame, 36, 16, 24);
			if (mSize == "Fire")
				SetSourceRect(16 * mFrame, 60, 16, 24);

			if (!mDeathAnimation)
			{
				mCurrentFrameTime += deltaTime * 1000;

				if (mCurrentFrameTime > mFrameTime) // If current frame time is greater than frame time
				{
					mFrame++; // Increases frame
					if (mFrame >= 3) // If frame is greater than or equal to 2
						mFrame = 0; // Resets the frame to 0

					mCurrentFrameTime = 0; // Resets the current frame time
				}

				if (mChangingUp)
				{
					if (mChangeType == "S->L")
					{
						mChangeCounterUp += deltaTime * 3000;
						if (mChangeCounterUp > 100 && mChangeCounterUp <= 1500)
						{
							if (!mChangeReposMediumUp)
							{
								mPosition.y -= 12;
								mChangeReposMediumUp = true;
							}
							mSize = "Medium";
						}
						if (mChangeCounterUp > 1500)
						{
							if (!mChangeReposLargeUp)
							{
								mPosition.y -= 12;
								mChangeReposLargeUp = true;
							}
							mSize = "Large";
							mChangingUp = false;
						}
					}

					if (mChangeType == "L->F")
					{
						mChangeCounterUp += deltaTime * 3000;
						if (mChangeCounterUp > 100 && mChangeCounterUp <= 400)
						{
							mSize = "Fire";
						}
						if (mChangeCounterUp > 400 && mChangeCounterUp <= 800)
						{
							mSize = "Large";
						}
						if (mChangeCounterUp > 800 && mChangeCounterUp <= 1000)
						{
							mSize = "Fire";
						}
						if (mChangeCounterUp > 1000 && mChangeCounterUp <= 1100)
						{
							mSize = "Large";
						}
						if (mChangeCounterUp > 1100 && mChangeCounterUp <= 1200)
						{
							mSize = "Fire";
						}
						if (mChangeCounterUp > 1200 && mChangeCounterUp <= 1300)
						{
							mSize = "Large";
						}
						if (mChangeCounterUp > 1300 && mChangeCounterUp <= 1400)
						{
							mSize = "Fire";
						}
						if (mChangeCounterUp > 1400 && mChangeCounterUp <= 1500)
						{
							mSize = "Large";
						}
						if (mChangeCounterUp > 1500)
						{
							mSize = "Fire";
							mChangingUp = false;
						}
					}

					if (mChangeType == "S->F")
					{
						mChangeCounterUp += deltaTime * 3000;
						if (mChangeCounterUp > 100 && mChangeCounterUp <= 1000)
						{
							if (!mChangeReposMediumUp)
							{
								mPosition.y -= 12;
								mChangeReposMediumUp = true;
							}
							mSize = "Medium";
						}
						if (mChangeCounterUp > 1000 && mChangeCounterUp <= 1500)
						{
							if (!mChangeReposLargeUp)
							{
								mPosition.y -= 12;
								mChangeReposLargeUp = true;
							}
							mSize = "Large";
						}
						if (mChangeCounterUp > 1500)
						{
							mSize = "Fire";
							mChangingUp = false;
						}
					}
				}

				if (mChangingDown)
				{
					if (mChangeType == "L->S")
					{
						mChangeCounterDown += deltaTime * 3000;
						if (mChangeCounterDown > 100 && mChangeCounterDown <= 2500)
						{
							mSize = "Medium";
						}
						if (mChangeCounterDown > 2500)
						{
							mSize = "Small";
							mChangingDown = false;
						}
					}

					if (mChangeType == "F->L")
					{
						mChangeCounterDown += deltaTime * 3000;
						if (mChangeCounterDown > 100 && mChangeCounterDown <= 400)
						{
							mSize = "Large";
						}
						if (mChangeCounterDown > 400 && mChangeCounterDown <= 800)
						{
							mSize = "Fire";
						}
						if (mChangeCounterDown > 800 && mChangeCounterDown <= 1200)
						{
							mSize = "Large";
						}
						if (mChangeCounterDown > 1200 && mChangeCounterDown <= 1600)
						{
							mSize = "Fire";
						}
						if (mChangeCounterDown > 1600 && mChangeCounterDown <= 1800)
						{
							mSize = "Large";
						}
						if (mChangeCounterDown > 1800 && mChangeCounterDown <= 1900)
						{
							mSize = "Fire";
						}
						if (mChangeCounterDown > 1900 && mChangeCounterDown <= 2000)
						{
							mSize = "Large";
						}
						if (mChangeCounterDown > 2100 && mChangeCounterDown <= 2200)
						{
							mSize = "Fire";
						}
						if (mChangeCounterDown > 2300 && mChangeCounterDown <= 2400)
						{
							mSize = "Large";
						}
						if (mChangeCounterDown > 2400 && mChangeCounterDown <= 2500)
						{
							mSize = "Fire";
						}
						if (mChangeCounterDown > 2500)
						{
							mSize = "Large";
							mChangingDown = false;
						}
					}
				}

				SDL_Rect characterRect = GetCollisionBox();
				int bottom = characterRect.y + characterRect.h;
				int left = characterRect.x;

				if (bottom > FLOOR_HEIGHT)
				{
					SetPositionY(FLOOR_HEIGHT - characterRect.h);
				}

				if (left <= 0)
				{
					mPosition.x = 0;
				}

				if (bottom >= FLOOR_HEIGHT)
				{
					SetOnPlatform(true);
					mAirKills = 0;
				}

				if (mJumping && bottom < FLOOR_HEIGHT - 25)
				{
					mFrame = 3;
				}

				//Deal with jumping first
				if (mJumping)
				{
					//Adjust the position
					mPosition.y -= mJumpForce * deltaTime;
					//Reduce the jump force
					mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;
				}

				if (bottom < FLOOR_HEIGHT && !mCollidingY)
				{
					mJumping = true;
					SetOnPlatform(false);
				}

				if (!mOnPlatform)
				{
					AddGravity(deltaTime);
				}
				else
				{
					mJumping = false;
				}

				if (!mReachedFlag)
				{
					if (mJumpKeyPressed || mJumping && !mCharColliding)
					{
						if (!mJumping)
						{
							mJumpSound->Play();
						}
						Jump(INTIAL_JUMP_FORCE);
					}

					if (!mCollidingX && (mMovingLeft || mMovingRight))
					{
						if (mMovingLeft)
						{
							if (mCamera->GetCameraRect().x == 0)
							{
								if (left - mMovementSpeed * deltaTime > 1)
									MoveLeft(deltaTime);
							}
							else
							{
								if (left - mMovementSpeed * deltaTime > 1 && (mPosition.x - (GetCollisionBox().w * 2) - mMovementSpeed * deltaTime > mCamera->GetCameraRect().x))
									MoveLeft(deltaTime);
							}
						}
						else if (mMovingRight)
						{
							if ((mPosition.x + (GetCollisionBox().w * 2)) + mMovementSpeed * deltaTime < mCamera->GetCameraRect().w + mCamera->GetCameraRect().x)
								MoveRight(deltaTime);
						}
					}
					else if (mCollidingX && (mMovingLeft || mMovingRight))
					{
						if (mMovingLeft && mCollisionXDirection == FACING_RIGHT)
						{
							if (left - mMovementSpeed * deltaTime > 1 && (mPosition.x - (GetCollisionBox().w * 2) - mMovementSpeed * deltaTime > mCamera->GetCameraRect().x))
								MoveLeft(deltaTime);
						}
						else if (mMovingRight && mCollisionXDirection == FACING_LEFT)
						{
							if ((mPosition.x + (GetCollisionBox().w * 2)) + mMovementSpeed * deltaTime < mCamera->GetCameraRect().w + mCamera->GetCameraRect().x)
								MoveRight(deltaTime);
						}
					}

					else if (!mMovingLeft && !mMovingRight)
					{
						if (mOnPlatform)
						{
							mFrame = 0;
						}
					}

					if (mPlayerNumber == 1)
					{
						switch (e.type)
						{
						case SDL_KEYDOWN:
						{
							switch (e.key.keysym.sym)
							{
							case SDLK_a:
							{
								mMovingLeft = true;
								break;
							}

							case SDLK_d:
							{
								mMovingRight = true;
								break;
							}

							case SDLK_SPACE:
							{
								mJumpKeyPressed = true;
								break;
							}
							case SDLK_LSHIFT:
							{
								mShootKeyPressed = true;
								break;
							}
							case SDLK_9:
							{
								cout << mPosition.y << endl;
							}
							}
							break;
						}

						case SDL_KEYUP:
						{
							switch (e.key.keysym.sym)
							{
							case SDLK_a:
							{
								mMovingLeft = false;
								break;
							}

							case SDLK_d:
							{
								mMovingRight = false;
								break;
							}
							case SDLK_SPACE:
							{
								mJumpKeyPressed = false;
								break;
							}
							case SDLK_LSHIFT:
							{
								mShootKeyPressed = false;
								break;
							}
							}
							break;
						}
						}
					}

					if (mPlayerNumber == 2)
					{
						switch (e.type)
						{
						case SDL_KEYDOWN:
						{
							switch (e.key.keysym.sym)
							{
							case SDLK_LEFT:
							{
								mMovingLeft = true;
								break;
							}

							case SDLK_RIGHT:
							{
								mMovingRight = true;
								break;
							}

							case SDLK_RCTRL:
							{
								mJumpKeyPressed = true;
								break;
							}
							case SDLK_RSHIFT:
							{
								mShootKeyPressed = true;
								break;
							}
							}
							break;
						}

						case SDL_KEYUP:
						{
							switch (e.key.keysym.sym)
							{
							case SDLK_LEFT:
							{
								mMovingLeft = false;
								break;
							}

							case SDLK_RIGHT:
							{
								mMovingRight = false;
								break;
							}
							case SDLK_RCTRL:
							{
								mJumpKeyPressed = false;
								break;
							}
							case SDLK_RSHIFT:
							{
								mShootKeyPressed = false;
								break;
							}
							}
							break;
						}
						}
					}
				}
				else
				{
					if (!mReachedCastle)
					{
						if (!mFlippedFlag)
						{
							mJumpForce = 0;
							mFrame = 4;
						}
						if (!mWalkToCastle)
						{
							mPosition.y += deltaTime * 200;
						}
						else
						{
							if (mStatFlagTime < 0.5)
							{
								mStatFlagTime += deltaTime;
							}
							else
							{
								if (mPosition.x < mCastleDoorPositionX)
								{
									mFlippedFlag = true;
									mPosition.x += deltaTime * 150;
									mCurrentFrameTime += deltaTime * 500;

									if (mCurrentFrameTime > mFrameTime) // If current frame time is greater than frame time
									{
										mFrame++; // Increases frame
										if (mFrame >= 3) // If frame is greater than or equal to 2
											mFrame = 0; // Resets the frame to 0

										mCurrentFrameTime = 0; // Resets the current frame time
									}
								}
								else
								{
									mReachedCastle = true;
								}
							}
						}
					}
				}
			}
			else
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
	else
	{
		mCurrentFrameTime += deltaTime * 1000;

		if (mCurrentFrameTime > mFrameTime) // If current frame time is greater than frame time
		{
			mFrame++; // Increases frame
			if (mFrame >= 3) // If frame is greater than or equal to 2
				mFrame = 0; // Resets the frame to 0

			mCurrentFrameTime = 0; // Resets the current frame time
		}
		if (mSize == "Small")
			SetSourceRect(16 * mFrame, 0, 16, 16);
		if (mSize == "Medium")
			SetSourceRect(16 * mFrame, 16, 16, 20);
		if (mSize == "Large")
			SetSourceRect(16 * mFrame, 36, 16, 24);
		if (mSize == "Fire")
			SetSourceRect(16 * mFrame, 60, 16, 24);

		MoveRight(deltaTime);

		if (mPosition.x + GetCollisionBox().w > SCREEN_WIDTH)
		{
			mPosition.x = 0 - GetCollisionBox().w;
		}
	}
}

void Character::SetPositionX(float x)
{
	mPosition.x = x;
}

void Character::SetPositionY(float y)
{
	mPosition.y = y;
}

void Character::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

void Character::MoveLeft(float deltaTime)
{
	mFacingDirection = FACING_LEFT;
	mPosition.x -= mMovementSpeed * deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	mFacingDirection = FACING_RIGHT;
	mPosition.x += mMovementSpeed * deltaTime;
}

void Character::AddGravity(float deltaTime)
{
	float increment;
	increment = deltaTime * 100;
	mPosition.y += increment;
}

void Character::Jump(float force)
{
	if (!mJumping)
	{
		mJumpForce = force;
		mJumping = true;
	}
}

void Character::SetCollidingX(bool state)
{
	mCollidingX = state;
}

bool Character::GetCollidingX()
{
	return mCollidingX;
}

void Character::SetCollidingY(bool state)
{
	mCollidingY = state;
}

bool Character::GetCollidingY()
{
	return mCollidingY;
}

void Character::SetCharColliding(bool state)
{
	mCharColliding = state;
}

bool Character::GetCharColliding()
{
	return mCharColliding;
}

void Character::SetOnPlatform(bool state)
{
	mOnPlatform = state;
}

SDL_Rect Character::GetCollisionBox()
{
	if (mSize == "Small")
	{
		int x = mPosition.x + 6;
		int y = mPosition.y;
		int w = 16 * SCALE - 12;
		int h = 16 * SCALE;
		return SDL_Rect{ x, y, w , h };
	}

	if (mSize == "Medium")
	{
		int x = mPosition.x + 2;
		int y = mPosition.y;
		int w = 16 * SCALE - 4;
		int h = 20 * SCALE;
		return SDL_Rect{ x, y, w , h };
	}

	if (mSize == "Large" || mSize == "Fire")
	{
		int x = mPosition.x + 2;
		int y = mPosition.y;
		int w = 16 * SCALE - 4;
		int h = 24 * SCALE;
		return SDL_Rect{ x, y, w , h };
	}
}

void Character::SetJumpForce(float force)
{
	mJumpForce = force;
}

void Character::Debug()
{
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 1);
	if (mSize == "Small")
	{
		SDL_RenderDrawRect(mRenderer, new SDL_Rect{ (int)mPosition.x + 6, (int)mPosition.y, (int)(16 * SCALE) - 12 , (int)(16 * SCALE) });
	}

	if (mSize == "Large")
	{
		SDL_RenderDrawRect(mRenderer, new SDL_Rect{ (int)mPosition.x + 4, (int)mPosition.y, (int)(16 * SCALE) - 8 , (int)(24 * SCALE) });
	}
}

void Character::CheckCharColliding(SDL_Rect OtherCharRect)
{
	int Char1Top = GetCollisionBox().y;
	int Char1Bottom = GetCollisionBox().y + GetCollisionBox().h;
	int Char1Left = GetCollisionBox().x;
	int Char1Right = GetCollisionBox().x + GetCollisionBox().w;
	int Char2Top = OtherCharRect.y;
	int Char2Bottom = OtherCharRect.y + OtherCharRect.h;
	int Char2Left = OtherCharRect.x;
	int Char2Right = OtherCharRect.x + OtherCharRect.w;

	if (Char1Left > Char2Right || Char1Right < Char2Left || Char1Bottom < Char2Top || Char1Top > Char2Bottom)
	{
		SetCharColliding(false);
	}
}

bool Character::CheckCollidingX(SDL_Rect BrickRect)
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

bool Character::CheckCollidingY(SDL_Rect BrickRect)
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

void Character::CheckOnPlatform(SDL_Rect BrickRect)
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
		mAirKills = 0;
		SetOnPlatform(true);
	}
}

bool Character::BottomHit(SDL_Rect BrickRect)
{
	int CharLeft = GetCollisionBox().x;
	int BrickRight = BrickRect.x + BrickRect.w;
	int CharRight = GetCollisionBox().x + GetCollisionBox().w;
	int BrickLeft = BrickRect.x;
	int CharBottom = GetCollisionBox().y + GetCollisionBox().h;
	int BrickTop = BrickRect.y;
	int CharTop = GetCollisionBox().y;
	int BrickBottom = BrickRect.y + BrickRect.h;
	
	if (CharTop == BrickBottom && CharLeft <= BrickRight && CharRight >= BrickLeft)
	{
		return true;
	}
	return false;
}

bool Character::TopHit(SDL_Rect BrickRect)
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

void Character::ChangeSizeUp()
{
	mChangingUp = true;
	mChangeCounterUp = 0;
	mChangeReposMediumUp = false;
	mChangeReposLargeUp = false;
}

void Character::SetChangeType(string type)
{
	mChangeType = type;
}

bool Character::GetChangingUp()
{
	return mChangingUp;
}

void Character::ChangeSizeDown()
{
	mChangingDown = true;
	mChangeCounterDown = 0;
	mChangeReposMediumDown = false;
	mChangeReposLargeDown = false;
}

bool Character::GetChangingDown()
{
	return mChangingDown;
}

bool Character::GetDying()
{
	return mDeathAnimation;
}

void Character::SetDying(bool state)
{
	mDeathAnimation = state;
}

bool Character::GetDead()
{
	return mDead;
}

bool Character::GetShootKeyPressed()
{
	return mShootKeyPressed;
}

void Character::SetShootKeyPressed(bool state)
{
	mShootKeyPressed = state;
}

void Character::SetCollidingXDirection(FACING direction)
{
	mCollisionXDirection = direction;
}

void Character::SetSourceRect(int x, int y, int w, int h)
{
	mSourceRect->x = x;
	mSourceRect->y = y;
	mSourceRect->w = w;
	mSourceRect->h = h;
}

bool Character::GetReachedFlag()
{
	return mReachedFlag;
}

void Character::SetReachedFlag(bool state)
{
	mReachedFlag = state;
}

void Character::SetWalkToCastle(bool state)
{
	mWalkToCastle = state;
}

bool Character::GetWalkToCastle()
{
	return mWalkToCastle;
}

bool Character::GetReachedCastle()
{
	return mReachedCastle;
}

bool Character::GetOnPlatform()
{
	return mOnPlatform;
}

void Character::IncreaseAirKills()
{
	mAirKills++;
}

int Character::GetAirKills()
{
	return mAirKills;
}

void Character::SetEndScreen(bool state)
{
	mEndScreen = state;
}

bool Character::GetFlagScoreAdded()
{
	return mFlagScoreAdded;
}

void Character::SetFlagScoreAdded(bool state)
{
	mFlagScoreAdded = state;
}

string Character::GetSize()
{
	return mSize;
}

void Character::SetSize(string size)
{
	mSize = size;
}

void Character::SetTopBrick(SDL_Rect brickrect)
{
	mPosition.y = brickrect.y - GetCollisionBox().h - 5;
}

int Character::GetPlayerNum()
{
	return mPlayerNumber;
}

Vector2D Character::getPosition()
{
	return mPosition;
}

FACING Character::GetFacingDirection()
{
	return mFacingDirection;
}

float Character::GetCollisionRadius()
{
	return mCollisionRadius;
}

void Character::SetSpeed(float speed)
{
	mMovementSpeed = speed;
}

bool Character::GetJumping()
{
	return mJumping;
}

void Character::SetJumping(bool state)
{
	mJumping = state;
}

int Character::GetFrame()
{
	return mFrame;
}

void Character::SetFrame(int frame)
{
	mFrame = frame;
}

bool Character::GetMoving()
{
	if (mMovingLeft || mMovingRight || mJumping)
		return true;
	else
		return false;
}
