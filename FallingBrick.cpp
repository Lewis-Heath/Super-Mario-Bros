#include "FallingBrick.h"

FallingBrick::FallingBrick(SDL_Renderer* renderer, Camera* camera, Vector2D startPosition, float angle, FACING direction)
{
	string imagePath = "Images/SmallBrick.png";
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(imagePath.c_str()))
	{
		cout << "Failed to load texture: " << imagePath << endl;
		return;
	}

	mPosition = startPosition;
	mAngle = angle;
	mFacingDirection = direction;
	mMovementSpeed = 100.0f;;
	mGravCounter = 50;
	mJumpForce = 500.0f;
	mDead = false;
	mSourceRect = new SDL_Rect{ 0, 0, 8, 8 };
}

FallingBrick::~FallingBrick()
{
	mRenderer = NULL;
	delete mTexture;
	mTexture = NULL;
}

void FallingBrick::Render()
{
	mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, mAngle, mSourceRect);
}

void FallingBrick::Update(float deltaTime, SDL_Event e)
{
	if (mPosition.y > SCREEN_HEIGHT + 50)
	{
		mDead = true;
	}
	else
	{
		//Adjust the position
		mPosition.y -= mJumpForce * deltaTime;
		//Reduce the jump force
		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;

		AddGravity(deltaTime);
		if (mFacingDirection == FACING_LEFT)
		{
			MoveLeft(deltaTime);
		}
		else
		{
			MoveRight(deltaTime);
		}
	}
}

bool FallingBrick::GetDead()
{
	return mDead;
}

void FallingBrick::MoveLeft(float deltaTime)
{
	mPosition.x -= mMovementSpeed * deltaTime;
}

void FallingBrick::MoveRight(float deltaTime)
{
	mPosition.x += mMovementSpeed * deltaTime;
}

void FallingBrick::AddGravity(float deltaTime)
{
	float increment;
	increment = deltaTime * mGravCounter;
	mGravCounter += 0.2;
	mPosition.y += increment;
}
