#include "PowBlock.h"

PowBlock::PowBlock(SDL_Renderer* renderer, Camera* camera, LevelMap* map, Vector2D startPosition)
{
	string imagePath = "Images/PowBlock.png";
	mCamera = camera;
	mTexture = new Texture2D(renderer);
	if (!mTexture->LoadFromFile(imagePath.c_str()))
	{
		cout << "Failed to load texture: " << imagePath << endl;
		return;
	}

	mLevelMap = map;
	mSingleSpriteWidth = mTexture->GetWidth() / 3;
	mSingleSpriteHeight = mTexture->GetHeight();
	mNumberOfHitsLeft = 3;
	mPosition = startPosition;
	mSourceRect = new SDL_Rect{ 16 * (3 - mNumberOfHitsLeft), 0, 16, 16 };
	mBottomHit = false;
}

PowBlock::~PowBlock()
{
	mRenderer = NULL;
	delete mTexture;
	mTexture = NULL;
	mLevelMap = NULL;
}

void PowBlock::Render()
{
	if (mNumberOfHitsLeft > 0)
	{
		mTexture->Render(mPosition, mCamera->GetCameraRect(), SDL_FLIP_NONE, NULL, mSourceRect);
	}
}

void PowBlock::Update()
{
	SetSourceRect(16 * (3 - mNumberOfHitsLeft), 0, 16, 16);
}

SDL_Rect PowBlock::GetCollisionBox()
{
	int x = mPosition.x;
	int y = mPosition.y;
	int w = mSingleSpriteWidth * SCALE;
	int h = mSingleSpriteHeight * SCALE;
	return SDL_Rect{ x, y, w, h };
}

void PowBlock::TakeAHit()
{
	mNumberOfHitsLeft--;
	if (mNumberOfHitsLeft <= 0)
	{
		mNumberOfHitsLeft = 0;
	}
}

bool PowBlock::IsAvailable()
{
	return mNumberOfHitsLeft > 0;
}

void PowBlock::SetBottomHit(bool state)
{
	mBottomHit = state;
}

bool PowBlock::GetBottomHit()
{
	return mBottomHit;
}

int PowBlock::LivesLeft()
{
	return mNumberOfHitsLeft;
}

void PowBlock::SetSourceRect(int x, int y, int w, int h)
{
	mSourceRect->x = x;
	mSourceRect->y = y;
	mSourceRect->w = w;
	mSourceRect->h = h;
}
