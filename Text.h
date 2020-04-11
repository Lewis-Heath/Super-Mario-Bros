#ifndef _TEXT_H
#define _TEXT_H

#include <SDL_ttf.h>
#include "SDL.h"
#include "Commons.h"
#include "Constants.h"
#include <iostream>

using namespace std;

class Text
{
public:
	Text(SDL_Renderer* renderer, string textDisplayed, int fontSize, string type, Vector2D startPosition);
	~Text();

	bool LoadFromFile();
	void Render(SDL_Rect camera);
	void Free();
	void Update(float deltaTime, SDL_Event e);
	bool GetDead();
	void SetDead(bool state);

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; };

private:
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	string mTextDisplayed;
	TTF_Font* mFont;
	SDL_Color mColour;
	Vector2D mPosition;

	int mFontSize;
	int mWidth;
	int mHeight;
	float mLifetime;
	string mType;
	float mMovementSpeed;
	bool mDead;
};

#endif // !_TEXT_H


