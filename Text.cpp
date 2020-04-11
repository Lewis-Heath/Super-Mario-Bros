#include "Text.h"

Text::Text(SDL_Renderer* renderer, string textDisplayed, int fontSize, string type, Vector2D startPosition)
{
	mRenderer = renderer;
	mTextDisplayed = textDisplayed;
	mFontSize = fontSize;
	mColour = { 255, 255, 255, 255 };
	mType = type;
	mPosition = startPosition;
	mLifetime = 0.0f;
	mMovementSpeed = 100.0f;
	mDead = false;
}

Text::~Text()
{
	//Free up the memory
	Free();

	mRenderer = NULL;
}

void Text::Render(SDL_Rect camera)
{
	SDL_Rect renderLocation;
	int cameraX = camera.x;
	if (mType == "Stat")
	{
		cameraX = 0;
	}
	renderLocation = { (int)mPosition.x - cameraX, (int)mPosition.y, mWidth, mHeight };
	SDL_RenderCopy(mRenderer, mTexture, nullptr, &renderLocation);
}

bool Text::LoadFromFile()
{
	Free();

	mTexture = NULL;

	string path = "Fonts/smalle.ttf";
	mFont = TTF_OpenFont(path.c_str(), mFontSize);
	if (mFont != NULL)
	{
		SDL_Surface* mSurface = TTF_RenderText_Solid(mFont, mTextDisplayed.c_str(), mColour);
		if (mSurface != NULL)
		{
			mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurface);
			if (mTexture == NULL)
			{
				cout << "Unable to create texture from surface. ERROR: " << SDL_GetError() << endl;
			}
			else
			{
				//Set the dimensions
				mWidth = mSurface->w;
				mHeight = mSurface->h;
			}
		}
		else
		{
			cout << "Unable to create texture from surface. ERROR: " << SDL_GetError() << endl;
		}
		SDL_FreeSurface(mSurface);
		SDL_QueryTexture(mTexture, nullptr, nullptr, &mWidth, &mHeight);
	}
	else
	{
		cout << "Failed to load font. ERROR: " << SDL_GetError() << endl;
	}

	//Return the whether the process worked
	return mTexture != NULL;
	
}

void Text::Free()
{
	//Check if the texture exists before removing it
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Text::Update(float deltaTime, SDL_Event e)
{
	if (!mDead && mType == "Temp")
	{
		mLifetime += deltaTime;
		mPosition.y -= deltaTime * mMovementSpeed;
		if (mLifetime > 0.75)
		{
			mDead = true;
		}
	}
}

bool Text::GetDead()
{
	return mDead;
}

void Text::SetDead(bool state)
{
	mDead = state;
}
