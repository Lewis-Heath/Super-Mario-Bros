#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include "SDL.h"
#include <iostream>
#include "Commons.h"
#include "LevelMap.h"
using namespace std;

class Fireball;
class Enemy;
class Mushroom;
class Character;
class Collisions
{
public:
	~Collisions();

	static Collisions* Instance();
	bool Circle(Character* character1, Character* character2);
	void CircleRePos(Character* character1, Vector2D OldPosition1, Vector2D NewPosition1, Character* character2, Vector2D OldPosition2, Vector2D NewPosition2);
	bool Box(SDL_Rect rect1, SDL_Rect rect2);
	void BoxRePosChar(Vector2D OldPosition, Vector2D NewPosition, Character* character1, SDL_Rect BrickRect);
	void BoxRePosMush(Vector2D OldPosition, Vector2D NewPosition, Mushroom* mushy, SDL_Rect BrickRect);
	void BoxRePosEnemy(Vector2D OldPosition, Vector2D NewPosition, Enemy* enemy, SDL_Rect BrickRect);
	void BoxRePosFireball(Vector2D OldPosition, Vector2D NewPosition, Fireball* fireball, SDL_Rect BrickRect);

private:
	Collisions();
	static Collisions* mInstance;
};


#endif // !_COLLISIONS_H

