#include "Texture2D.h"
#include <SDL_image.h>
#include <iostream>
using namespace std;

Texture2D::Texture2D(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}

Texture2D::~Texture2D()
{
	//Free up the memory
	Free();

	mRenderer = NULL;
}

bool Texture2D::LoadFromFile(std::string path)
{
	Free();

	mTexture = NULL;

	//Load the image
	SDL_Surface* mSurface = IMG_Load(path.c_str());
	if (mSurface != NULL)
	{
		//Colour key the image - The colour to be transparent
		SDL_SetColorKey(mSurface, SDL_TRUE, SDL_MapRGB(mSurface->format, 0, 0xFF, 0xFF));
		//Create the texture from the pixels on the surface
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
		//Remove the loaded surface bow that we have the texture
		SDL_FreeSurface(mSurface);
	}
	else
	{
		cout << "Unable to create texture from surface. ERROR: " << IMG_GetError() << endl;
	}
	//Return the whether the process worked
	return mTexture != NULL;
}

void Texture2D::Free()
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

void Texture2D::Render(Vector2D newPosition, SDL_Rect camera, SDL_RendererFlip flip, double angle, SDL_Rect* sRect)
{
	//Set where to render texture
	SDL_Rect renderLocation;
	//Render to screen

	sRect != NULL ? renderLocation = { (int)newPosition.x - camera.x, (int)newPosition.y, sRect->w * 3, sRect->h * 3 } : renderLocation = { (int)newPosition.x - camera.x, (int)newPosition.y, mWidth, mHeight };
	SDL_RenderCopyEx(mRenderer, mTexture, sRect, &renderLocation, angle, NULL, flip);
}