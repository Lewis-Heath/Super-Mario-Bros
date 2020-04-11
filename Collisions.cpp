#include "Character.h"
#include "Collisions.h"
using namespace std;

//Initalisethe instance to null
Collisions* Collisions::mInstance = NULL;

Collisions::Collisions()
{
}

Collisions::~Collisions()
{
	mInstance = NULL;
}

Collisions* Collisions::Instance()
{
	if (!mInstance)
	{
		mInstance = new Collisions;
	}

	return mInstance;
}

bool Collisions::Circle(Character* character1, Character* character2)
{
	Vector2D vec = Vector2D((character1->getPosition().x - character2->getPosition().x),
							(character1->getPosition().y - character2->getPosition().y));

	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	double combinedDistance = (character1->GetCollisionRadius() + character2->GetCollisionRadius());
	return distance < combinedDistance;
}

void Collisions::CircleRePos(Character* character1, Vector2D OldPosition1, Vector2D NewPosition1, Character* character2, Vector2D OldPosition2, Vector2D NewPosition2)
{
	character1->SetCharColliding(true);
	character2->SetCharColliding(true);

	int MarioCenterX = character1->GetCollisionBox().x + character1->GetCollisionBox().w / 2;
	int LuigiCenterX = character2->GetCollisionBox().x + character2->GetCollisionBox().w / 2;
	 
	if (character1->GetJumping())
	{
		character1->SetJumpForce(0.0f);
	}

	if (character2->GetJumping())
	{
		character2->SetJumpForce(0.0f);
	}

	if (character1->GetMoving())
	{
		Vector2D penetration = Vector2D((NewPosition1.x - OldPosition1.x), (NewPosition1.y - OldPosition1.y));
		if (NewPosition1.x + character1->GetCollisionBox().w / 2 <= LuigiCenterX)
		{
			character1->SetPosition(Vector2D((NewPosition1.x - penetration.x) - 1, (NewPosition1.y - penetration.y)));
		}
		if (NewPosition1.x + character1->GetCollisionBox().w / 2 >= LuigiCenterX)
		{
			character1->SetPosition(Vector2D((NewPosition1.x - penetration.x) + 1, (NewPosition1.y - penetration.y)));
		}
	}

	if (character2->GetMoving())
	{
		Vector2D penetration = Vector2D((NewPosition2.x - OldPosition2.x), (NewPosition2.y - OldPosition2.y));
		if (NewPosition2.x + character2->GetCollisionBox().w / 2 <= MarioCenterX)
		{
			character2->SetPosition(Vector2D((NewPosition2.x - penetration.x) - 1, (NewPosition2.y - penetration.y)));
		}
		if (NewPosition2.x + character2->GetCollisionBox().w / 2 >= MarioCenterX)
		{
			character2->SetPosition(Vector2D((NewPosition2.x - penetration.x) + 1, (NewPosition2.y - penetration.y)));
		}
	}
}

bool Collisions::Box(SDL_Rect rect1, SDL_Rect rect2)
{
	if ((rect1.y + rect1.h > rect2.y) && (rect1.y < rect2.y + rect2.h) && (rect1.x + rect1.w > rect2.x) && (rect1.x < rect2.x + rect2.w))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Collisions::BoxRePosChar(Vector2D OldPosition, Vector2D NewPosition, Character* character1, SDL_Rect BrickRect)
{
	int BrickRight = BrickRect.x + BrickRect.w;
	int BrickLeft = BrickRect.x;
	int BrickTop = BrickRect.y;
	int BrickBottom = BrickRect.y + BrickRect.h;
	Vector2D penetration = Vector2D((NewPosition.x -OldPosition.x), (NewPosition.y - OldPosition.y));
	character1->SetPosition(Vector2D((NewPosition.x - penetration.x), (NewPosition.y - penetration.y)));
	int CharTop = character1->GetCollisionBox().y;
	int CharBottom = character1->GetCollisionBox().y + character1->GetCollisionBox().h;
	int CharLeft = character1->GetCollisionBox().x;
	int CharRight = character1->GetCollisionBox().x + character1->GetCollisionBox().w;
	if (CharBottom == BrickTop || CharTop == BrickBottom || CharBottom + 1 == BrickTop || CharBottom + 2 == BrickTop || CharBottom + 3 == BrickTop || CharBottom + 4 == BrickTop)
	{
		character1->SetJumpForce(0.0f);
		character1->SetCollidingY(true);
	}
	if (CharLeft == BrickRight || CharRight == BrickLeft)
	{
		character1->SetCollidingX(true);
		character1->SetCollidingXDirection(character1->GetFacingDirection());
	}
}

void Collisions::BoxRePosMush(Vector2D OldPosition, Vector2D NewPosition, Mushroom* mushy, SDL_Rect BrickRect)
{
	int BrickRight = BrickRect.x + BrickRect.w;
	int BrickLeft = BrickRect.x;
	int BrickTop = BrickRect.y;
	int BrickBottom = BrickRect.y + BrickRect.h;
	Vector2D penetration = Vector2D((NewPosition.x - OldPosition.x), (NewPosition.y - OldPosition.y));
	mushy->SetPosition(Vector2D((NewPosition.x - penetration.x), (NewPosition.y - penetration.y)));
	int MushTop = mushy->GetCollisionBox().y;
	int MushBottom = mushy->GetCollisionBox().y + mushy->GetCollisionBox().h;
	int MushLeft = mushy->GetCollisionBox().x;
	int MushRight = mushy->GetCollisionBox().x + mushy->GetCollisionBox().w;

	if (MushBottom == BrickTop || MushTop == BrickBottom || MushBottom + 1 == BrickTop || MushBottom + 2 == BrickTop || MushBottom + 3 == BrickTop || MushBottom + 4 == BrickTop)
	{
		mushy->SetCollidingY(true);
	}
	if (MushLeft == BrickRight || MushRight == BrickLeft)
	{
		mushy->ChangeDirection();
		mushy->SetCollidingX(true);
	}
}

void Collisions::BoxRePosEnemy(Vector2D OldPosition, Vector2D NewPosition, Enemy* enemy, SDL_Rect BrickRect)
{
	Vector2D penetration = Vector2D((NewPosition.x - OldPosition.x), (NewPosition.y - OldPosition.y));
	enemy->SetPosition(Vector2D((NewPosition.x - penetration.x), (NewPosition.y - penetration.y)));
	enemy->ChangeDirection();
}

void Collisions::BoxRePosFireball(Vector2D OldPosition, Vector2D NewPosition, Fireball* fireball, SDL_Rect BrickRect)
{
	int BrickRight = BrickRect.x + BrickRect.w;
	int BrickLeft = BrickRect.x;
	int BrickTop = BrickRect.y;
	int BrickBottom = BrickRect.y + BrickRect.h;
	Vector2D penetration = Vector2D((NewPosition.x - OldPosition.x), (NewPosition.y - OldPosition.y));
	fireball->SetPosition(Vector2D((NewPosition.x - penetration.x), (NewPosition.y - penetration.y)));
	int FireballTop = fireball->GetCollisionBox().y;
	int FireballBottom = fireball->GetCollisionBox().y + fireball->GetCollisionBox().h;
	int FireballLeft = fireball->GetCollisionBox().x;
	int FireballRight = fireball->GetCollisionBox().x + fireball->GetCollisionBox().w;

	if (FireballBottom == BrickTop || FireballTop == BrickBottom || FireballBottom + 1 == BrickTop || FireballBottom + 2 == BrickTop /*|| FireballBottom + 3== BrickTop || FireballBottom + 4 == BrickTop*/)
	{
		fireball->SetCollidingY(true);
	}
	if (FireballLeft == BrickRight || FireballRight == BrickLeft)
	{
		fireball->SetBeenHit(true);
		fireball->SetFrame(3);
		fireball->SetCollidingX(true);
	}
}


