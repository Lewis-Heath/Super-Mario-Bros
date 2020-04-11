#pragma once
struct Vector2D
{
	float x;
	float y;
	
	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES,
};

enum FACING
{
	FACING_LEFT = 0,
	FACING_RIGHT,
};

enum COLDIRECTION
{
	TOP = 0,
	BOTTOM,
	LEFT,
	RIGHT,
};
