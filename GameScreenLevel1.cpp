#include "GameScreenLevel1.h"
#include <iostream>
#include "Texture2D.h"
#include "Constants.h"
using namespace std;

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	string name;
	cout << "Please enter your name: ";
	cin >> name;
	mName = name;
	mCamera = new Camera();
	SetLevelMap("Level1.txt");
	SetUpLevel();
	mLevelMap = NULL;
}

GameScreenLevel1::~GameScreenLevel1()
{
	mBackgroundTexture = NULL;
	for (int i = 0; i < 2; i++)
	{
		delete Characters[i];
		Characters[i] = NULL;
	}
	for (int i = 0; i < BrickCounter; i++)
	{
		delete Bricks[i];
		Bricks[i] = NULL;
	}
	for (int i = 0; i < MushroomCounter; i++)
	{
		delete Mushrooms[i];
		Mushrooms[i] = NULL;
	}
	for (int i = 0; i < FallingBrickCounter; i++)
	{
		delete FallingBricks[i];
		FallingBricks[i] = NULL;
	}
	for (int i = 0; i < CoinCounter; i++)
	{
		delete Coins[i];
		Coins[i] = NULL;
	}
	for (int i = 0; i < EnemyCounter; i++)
	{
		delete Enemies[i];
		Enemies[i] = NULL;
	}
	for (int i = 0; i < FireballCounter; i++)
	{
		delete Fireballs[i];
		Fireballs[i] = NULL;
	}
	for (int i = 0; i < TempScoreCounter; i++)
	{
		delete TempScores[i];
		TempScores[i] = NULL;
	}
	delete mPowBlock;
	mPowBlock = NULL;
	delete mFlag;
	mFlag = NULL;
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
}

void GameScreenLevel1::LoadMusic(string path)
{
	gMusic = Mix_LoadMUS(path.c_str());
	if (gMusic == NULL)
	{
		cout << "Failed to load background music! Error: " << Mix_GetError() << endl;
	}
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	if (mStarted && !mLevelFinished)
	{
		if (!Characters[0]->GetDead() && !Characters[1]->GetDead())
		{
			if (Characters[0]->GetReachedCastle() && Characters[1]->GetReachedCastle())
			{
				mLevelFinished = true;
				mEndScreen = "You Win";
			}
		}
		if (Characters[0]->GetDead() && !Characters[1]->GetDead())
		{
			if (Characters[1]->GetReachedCastle())
			{
				mLevelFinished = true;
				mEndScreen = "You Win";
			}
		}
		if (!Characters[0]->GetDead() && Characters[1]->GetDead())
		{
			if (Characters[0]->GetReachedCastle())
			{
				mLevelFinished = true;
				mEndScreen = "You Win";
			}
		}
		if (Characters[0]->GetDead() && Characters[1]->GetDead())
		{
			mLevelFinished = true;
			Mix_PauseMusic();
			mGameOverSound->Play();
			mEndScreen = "You died";
		}

		if (Mix_PlayingMusic() == 0 && !Characters[0]->GetReachedFlag() && !Characters[1]->GetReachedFlag())
		{
			Mix_VolumeMusic(10);
			Mix_PlayMusic(gMusic, -1);
		}

		mPrevcount = int(mCountdown);
		mCountdown -= deltaTime;
		mICountdown = int(mCountdown);
		if (mPrevcount != mICountdown)
		{
			string time = to_string(mICountdown);
			mValueCountText->Free();
			mValueCountText = NULL;
			mValueCountText = new Text(mRenderer, time, 40, "Stat", Vector2D(725, 40));
			mValueCountText->LoadFromFile();
		}

		if (mICountdown <= 100 && !mOutOfTimePlayed)
		{
			mOutOfTimePlayed = true;
			mRunningOutOfTime->Play();
		}

		if (mScreenshake)
		{
			mScreenShakeTime -= deltaTime;
			mWobble++;
			mBackgroundYPos = sin(mWobble);
			mBackgroundYPos *= 3.0f;

			//End the shake after the duration
			if (mScreenShakeTime <= 0.0f)
			{
				mScreenshake = false;
				mBackgroundYPos = 0.0f;
			}
		}

		TimeSinceLastFireball += deltaTime;

		/*if (Collisions::Instance()->Circle(Characters[0], Characters[1]) && !Characters[0]->GetDying() && !Characters[1]->GetDying())
		{
			Collisions::Instance()->CircleRePos(Characters[0], Characters[0]->mOldPos, Characters[0]->mNewPos, Characters[1], Characters[1]->mOldPos, Characters[1]->mNewPos);
		}

		Characters[0]->CheckCharColliding(Characters[1]->GetCollisionBox());
		Characters[1]->CheckCharColliding(Characters[0]->GetCollisionBox());*/


		DeleteItemsFromList();

		mFlag->Update(deltaTime, e);

		for (int v = 0; v < FallingBrickCounter; v++)
		{
			FallingBricks[v]->Update(deltaTime, e);
		}

		for (int v = 0; v < TempScoreCounter; v++)
		{
			TempScores[v]->Update(deltaTime, e);
		}

		for (int i = 0; i < 2; i++)
		{
			Characters[i]->mOldPos = Characters[i]->getPosition();
			//Update the player
			Characters[i]->Update(deltaTime, e);
			Characters[i]->mNewPos = Characters[i]->getPosition();

			if (Collisions::Instance()->Box(Characters[i]->GetCollisionBox(), mFlag->GetCollisionBox()) && (!mFlag->GetBeenHit() || mFlag->GetReachedBottom()))
			{
				if (!Characters[i]->GetFlagScoreAdded())
				{
					Mix_PauseMusic();
					mReachedCastleSound->Play();
					int scoreIncrease = 0;
					if (Characters[i]->getPosition().y > 100 && Characters[i]->getPosition().y <= 250)
						scoreIncrease = 2500;
					else if (Characters[i]->getPosition().y > 250 && Characters[i]->getPosition().y <= 300)
						scoreIncrease = 2000;
					else if (Characters[i]->getPosition().y > 300 && Characters[i]->getPosition().y <= 400)
						scoreIncrease = 1000;
					else if (Characters[i]->getPosition().y > 400 && Characters[i]->getPosition().y <= 600)
						scoreIncrease = 500;
					scoreIncrease = scoreIncrease + mCountdown* 8;
					mScore = mScore + scoreIncrease;
					string score = to_string(mScore);
					string scoreAddition = to_string(scoreIncrease);
					mValueScoreText = NULL;
					mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
					mValueScoreText->LoadFromFile();
					TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(Characters[i]->getPosition().x, Characters[i]->getPosition().y - 36));
					TempScores[TempScoreCounter]->LoadFromFile();
					TempScoreCounter++;
					Characters[i]->SetFlagScoreAdded(true);
				}
				
				mFlag->SetBeenHit(true);
				Characters[i]->SetReachedFlag(true);
				if (!Characters[i]->GetOnPlatform())
					Characters[i]->SetPositionX(mFlag->GetPosition().x + 15);
			}

			if (mFlag->GetReachedBottom() && Characters[i]->GetReachedFlag() && !Characters[i]->GetWalkToCastle() && Characters[i]->GetOnPlatform())
			{
				Characters[i]->SetPosition(Vector2D(mFlag->GetPosition().x + 48 + 12, mFlag->GetPosition().y - Characters[i]->GetCollisionBox().h / 2));
				Characters[i]->SetWalkToCastle(true);
			}

			if (mPowBlock->LivesLeft() > 0)
			{
				mPowBlock->Update();
				UpdatePOWBlock(Characters[i]->mOldPos, Characters[i]->mNewPos, Characters[i]);
			}

			for (int v = 0; v < CoinCounter; v++)
			{
				Coins[v]->Update(deltaTime, e);
				UpdateCoin(Characters[i], Coins[v]);
			}

			//cout << deltaTime << endl;

			for (int v = 0; v < BrickCounter; v++)
			{
				int center = Bricks[v]->getPosition().x + Bricks[v]->GetCollisionBox().w / 2;
				if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
				{
					Bricks[v]->Update(deltaTime, e);
					if (Bricks[v]->GetType() == "Question")
					{
						UpdateQuestionBlock(Characters[i]->mOldPos, Characters[i]->mNewPos, Characters[i], Bricks[v]);
					}
					if (Collisions::Instance()->Box(Characters[i]->GetCollisionBox(), Bricks[v]->GetCollisionBox()) && !Bricks[v]->GetDead() && !Characters[i]->GetDying())
					{
						Collisions::Instance()->BoxRePosChar(Characters[i]->mOldPos, Characters[i]->mNewPos, Characters[i], Bricks[v]->GetCollisionBox());
						if (Characters[i]->BottomHit(Bricks[v]->GetCollisionBox()))
						{
							UpdateBounce(Bricks[v], i + 1);
							if (Characters[i]->GetSize() == "Large" || Characters[i]->GetSize() == "Fire")
							{
								Bricks[v]->SetDead(true);
								mBreakingBrickSound->Play();
								Vector2D BrickPosition = Vector2D(Bricks[v]->getPosition().x + Bricks[v]->GetCollisionBox().w / 2 - 4, Bricks[v]->getPosition().y + Bricks[v]->GetCollisionBox().h / 2 - 4);
								FallingBricks[FallingBrickCounter] = new FallingBrick(mRenderer, mCamera, Vector2D(BrickPosition.x, BrickPosition.y), 360 - 45, FACING_LEFT);
								FallingBrickCounter++;
								FallingBricks[FallingBrickCounter] = new FallingBrick(mRenderer, mCamera, Vector2D(BrickPosition.x, BrickPosition.y + 50), 360 - 45, FACING_LEFT);
								FallingBrickCounter++;
								FallingBricks[FallingBrickCounter] = new FallingBrick(mRenderer, mCamera, Vector2D(BrickPosition.x, BrickPosition.y), 45, FACING_RIGHT);
								FallingBrickCounter++;
								FallingBricks[FallingBrickCounter] = new FallingBrick(mRenderer, mCamera, Vector2D(BrickPosition.x, BrickPosition.y + 50), 45, FACING_RIGHT);
								FallingBrickCounter++;
							}
						}
					}
				}
			}
			CheckCollidingCharacter(Characters[i]);

			for (int c = 0; c < MushroomCounter; c++)
			{
				Mushrooms[c]->mOldPos = Mushrooms[c]->getPosition();
				Mushrooms[c]->Update(deltaTime, e);
				Mushrooms[c]->mNewPos = Mushrooms[c]->getPosition();
				if (Mushrooms[c]->GetOutOfBlock())
				{
					UpdateMushroom(Characters[i], Mushrooms[c], c);
					for (int v = 0; v < BrickCounter; v++)
					{
						if (Collisions::Instance()->Box(Mushrooms[c]->GetCollisionBox(), Bricks[v]->GetCollisionBox()) && Mushrooms[c]->GetOutOfBlock())
						{
							Collisions::Instance()->BoxRePosMush(Mushrooms[c]->mOldPos, Mushrooms[c]->mNewPos, Mushrooms[c], Bricks[v]->GetCollisionBox());
						}
					}
					CheckCollidingMushroom(Mushrooms[c]);
				}
			}

			for (int c = 0; c < FireballCounter; c++)
			{
				Fireballs[c]->mOldPos = Fireballs[c]->GetPosition();
				Fireballs[c]->Update(deltaTime, e);
				Fireballs[c]->mNewPos = Fireballs[c]->GetPosition();
				int center = Fireballs[c]->GetPosition().x + Fireballs[c]->GetCollisionBox().w / 2;
				if (center <= mCamera->GetCameraRect().x - 100 && center >= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
				{
					Fireballs[c]->SetDead(true);
				}
				else
				{
					for (int v = 0; v < BrickCounter; v++)
					{
						if (Collisions::Instance()->Box(Fireballs[c]->GetCollisionBox(), Bricks[v]->GetCollisionBox()))
						{
							Collisions::Instance()->BoxRePosFireball(Fireballs[c]->mOldPos, Fireballs[c]->mNewPos, Fireballs[c], Bricks[v]->GetCollisionBox());
							if (Fireballs[c]->GetBeenHit())
							{
								mStompGoomba->Play();
							}
						}
					}
					for (int v = 0; v < EnemyCounter; v++)
					{
						UpdateFireball(Fireballs[c], Enemies[v]);
					}
					CheckCollidingFireball(Fireballs[c]);
				}
			}

			for (int c = 0; c < EnemyCounter; c++)
			{
				int center = Enemies[c]->GetPosition().x + Enemies[c]->GetCollisionBox().w / 2;
				if (center >= mCamera->GetCameraRect().x - 200 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 200)
				{
					Enemies[c]->mOldPos = Enemies[c]->GetPosition();
					Enemies[c]->Update(deltaTime, e);
					Enemies[c]->mNewPos = Enemies[c]->GetPosition();

					if (Enemies[c]->GetState() == "SafeShell")
					{
						UpdateShell(Characters[i], Enemies[c]);
					}

					else
					{
						UpdateEnemy(Characters[i], Enemies[c]);
					}

					for (int v = 0; v < BrickCounter; v++)
					{
						if (Collisions::Instance()->Box(Enemies[c]->GetCollisionBox(), Bricks[v]->GetCollisionBox()) && !Enemies[c]->GetBeenHit())
						{
							if (Enemies[c]->GetState() == "Shell")
								mShellHitsWall->Play();
							Collisions::Instance()->BoxRePosEnemy(Enemies[c]->mOldPos, Enemies[c]->mNewPos, Enemies[c], Bricks[v]->GetCollisionBox());
						}
					}

					for (int v = 0; v < EnemyCounter; v++)
					{
						if (Enemies[v] != Enemies[c])
						{
							if (Collisions::Instance()->Box(Enemies[c]->GetCollisionBox(), Enemies[v]->GetCollisionBox()) && !Enemies[v]->GetBeenHit() && !Enemies[c]->GetBeenHit())
							{
								if (Enemies[c]->GetState() == "Shell" || Enemies[c]->GetState() == "SafeShell")
								{
									int scoreIncrease = 0;
									if(Enemies[c]->GetKills() == 0)
										scoreIncrease = 100;
									else if(Enemies[c]->GetKills() == 1)
										scoreIncrease = 200;
									else if (Enemies[c]->GetKills() == 2)
										scoreIncrease = 400;
									else if (Enemies[c]->GetKills() == 3)
										scoreIncrease = 500;
									else if (Enemies[c]->GetKills() == 4)
										scoreIncrease = 800;
									else if (Enemies[c]->GetKills() == 5)
										scoreIncrease = 1000;
									mScore = mScore + scoreIncrease;
									string score = to_string(mScore);
									string scoreAddition = to_string(scoreIncrease);
									mValueScoreText = NULL;
									mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
									mValueScoreText->LoadFromFile();
									TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(Enemies[v]->GetPosition().x, Enemies[v]->GetPosition().y - 36));
									TempScores[TempScoreCounter]->LoadFromFile();
									TempScoreCounter++;
									mStompGoomba->Play();
									Enemies[v]->SetDeathType("Fireball");
									Enemies[v]->Jump(300.0f);
									Enemies[v]->SetBeenHit(true);
									Enemies[c]->IncreaseKills();
								}
								else
								{
									Enemies[v]->ChangeDirection();
									Enemies[c]->ChangeDirection();
								}
							}
						}
					}
				}
			}

			if (Characters[i]->GetShootKeyPressed() && Characters[i]->GetSize() == "Fire")
			{
				if (TimeSinceLastFireball > 0.5f && !Characters[i]->GetCollidingX())
				{
					if (Characters[i]->GetFacingDirection() == FACING_RIGHT)
					{
						Fireballs[FireballCounter] = new Fireball(mRenderer, mCamera, mLevelMap, "Images/FireBall.png", Vector2D(Characters[i]->getPosition().x + Characters[i]->GetCollisionBox().w, Characters[i]->getPosition().y + Characters[i]->GetCollisionBox().h / 2), FACING_RIGHT);
					}
					else
					{
						Fireballs[FireballCounter] = new Fireball(mRenderer, mCamera, mLevelMap, "Images/FireBall.png", Vector2D(Characters[i]->getPosition().x - Characters[i]->GetCollisionBox().w, Characters[i]->getPosition().y + Characters[i]->GetCollisionBox().h / 2), FACING_LEFT);
					}
					mFireballSound->Play();
					FireballCounter++;
					Characters[i]->SetShootKeyPressed(false);
					TimeSinceLastFireball = 0.0f;
				}
			}
		}
	}

	else 
	{
		if (!mLevelFinished)
		{
			switch (e.type)
			{
				case SDL_KEYDOWN:
				{
					switch (e.key.keysym.sym)
					{
						case SDLK_RETURN:
						{
							mStarted = true;
							break;
						}
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < 2; i++)
			{
				Characters[i]->Update(deltaTime, e);
			}
		}
	}


	if (!mLevelFinished)
	{
		if (!Characters[0]->GetDead() && !Characters[1]->GetDead() && !Characters[0]->GetDying() && !Characters[1]->GetDying())
		{
			mCamera->ChangeCameraX((Characters[0]->getPosition().x + Characters[0]->GetCollisionBox().w / 2 + Characters[1]->getPosition().x + Characters[1]->GetCollisionBox().w / 2) / 2 - SCREEN_WIDTH / 2);
		}
		else if (Characters[0]->GetDead() || Characters[0]->GetDying())
		{
			mCamera->ChangeCameraX((Characters[1]->getPosition().x + Characters[1]->GetCollisionBox().w / 2) - SCREEN_WIDTH / 2);
		}
		else if (Characters[1]->GetDead() || Characters[1]->GetDying())
		{
			mCamera->ChangeCameraX((Characters[0]->getPosition().x + Characters[0]->GetCollisionBox().w / 2) - SCREEN_WIDTH / 2);
		}

		if (mCamera->GetCameraRect().x < 0)
		{
			mCamera->ChangeCameraX(0);
		}

		if (mCamera->GetCameraRect().x > MAP_WIDTH * 48 - SCREEN_WIDTH)
		{
			mCamera->ChangeCameraX(MAP_WIDTH * 48 - SCREEN_WIDTH);
		}
	}
}

void GameScreenLevel1::UpdatePOWBlock(Vector2D CharOldPos, Vector2D CharNewPos, Character* character1)
{
	if (Collisions::Instance()->Box(character1->GetCollisionBox(), mPowBlock->GetCollisionBox()) && !character1->GetDying())
	{
		Collisions::Instance()->BoxRePosChar(CharOldPos, CharNewPos, character1, mPowBlock->GetCollisionBox());
		if (character1->BottomHit(mPowBlock->GetCollisionBox()))
		{
			mPowBlock->TakeAHit();
			DoScreenshake();
		}   
	}
}

void GameScreenLevel1::UpdateQuestionBlock(Vector2D CharOldPos, Vector2D CharNewPos, Character* character1, Brick* brick)
{
	if (Collisions::Instance()->Box(character1->GetCollisionBox(), brick->GetCollisionBox()) && !character1->GetDying())
	{
		Collisions::Instance()->BoxRePosChar(CharOldPos, CharNewPos, character1, brick->GetCollisionBox());
		if (character1->BottomHit(brick->GetCollisionBox()))
		{
			for (int i = 0; i < MushroomCounter; i++)
			{
				if (Mushrooms[i]->GetBrick() == brick && !Mushrooms[i]->GetSpawned())
				{
					if (character1->GetSize() != "Small")
					{
						Mushrooms[i]->SetObjFrame(1);
					}
					mMushSpawnSound->Play();
					Mushrooms[i]->SetSpawned(true);
					Mushrooms[i]->Bounce();
				}
			}
			for (int i = 0; i < CoinCounter; i++)
			{
				if (Coins[i]->GetBrick() == brick && !Coins[i]->GetSpawned())
				{
					mCoins++;
					string coins = to_string(mCoins);
					mValueCoinText = NULL;
					mValueCoinText = new Text(mRenderer, coins, 40, "Stat", Vector2D(420, 40));
					mValueCoinText->LoadFromFile();
					mScore = mScore + 200;
					string score = to_string(mScore);
					mValueScoreText = NULL;
					mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
					mValueScoreText->LoadFromFile();
					mCoinSound->Play();
					Coins[i]->SetSpawned(true);
				}
			}
			if (brick->GetFrame() != 3)
				UpdateBounce(brick, character1->GetPlayerNum());
			brick->SetAnimating(false);
			brick->SetFrame(3);
		}
	}
}

void GameScreenLevel1::UpdateMushroom(Character* character1, Mushroom* mushy, int index)
{
	if (Collisions::Instance()->Box(character1->GetCollisionBox(), mushy->GetCollisionBox()) && mushy->GetOutOfBlock() && !mushy->GetDead() && !character1->GetDying())
	{
		string scoreAddition = "1000";
		if (mushy->GetObjFrame() == 0 && character1->GetSize() == "Small")
		{
			mScore = mScore + 1000;
			string score = to_string(mScore);
			mValueScoreText = NULL;
			mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
			mValueScoreText->LoadFromFile();
			TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(mushy->getPosition().x, character1->getPosition().y - 36));
			TempScores[TempScoreCounter]->LoadFromFile();
			TempScoreCounter++;
			character1->SetChangeType("S->L");
			character1->ChangeSizeUp();
			mushy->SetDead(true);
			mPowerUpSound->Play();
		}

		if (mushy->GetObjFrame() == 1 && character1->GetSize() == "Small")
		{
			mScore = mScore + 1000;
			string score = to_string(mScore);
			mValueScoreText = NULL;
			mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
			mValueScoreText->LoadFromFile();
			TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(mushy->getPosition().x, mushy->getPosition().y - 36));
			TempScores[TempScoreCounter]->LoadFromFile();
			TempScoreCounter++;
			character1->SetChangeType("S->F");
			character1->ChangeSizeUp();
			mushy->SetDead(true);
			mPowerUpSound->Play();
		}

		if (mushy->GetObjFrame() == 1 && character1->GetSize() == "Large")
		{
			mScore = mScore + 1000;
			string score = to_string(mScore);
			mValueScoreText = NULL;
			mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
			mValueScoreText->LoadFromFile();
			TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(mushy->getPosition().x, mushy->getPosition().y - 36));
			TempScores[TempScoreCounter]->LoadFromFile();
			TempScoreCounter++;
			character1->SetChangeType("L->F");
			character1->ChangeSizeUp();
			mushy->SetDead(true);
			mPowerUpSound->Play();
		}

		if (mushy->GetObjFrame() == 1 && character1->GetSize() == "Fire")
		{
			mScore = mScore + 1000;
			string score = to_string(mScore);
			mValueScoreText = NULL;
			mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
			mValueScoreText->LoadFromFile();
			TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(mushy->getPosition().x, mushy->getPosition().y - 36));
			TempScores[TempScoreCounter]->LoadFromFile();
			TempScoreCounter++;
			mushy->SetDead(true);
			mPowerUpSound->Play();
		}
	}
}

void GameScreenLevel1::UpdateCoin(Character* character1, Coin* coin)
{
	if (Collisions::Instance()->Box(character1->GetCollisionBox(), coin->GetCollisionBox()) && !coin->GetDead() && coin->GetStateFrame() != 1 && !character1->GetDying())
	{
		mCoinSound->Play();
		mCoins++;
		string coins = to_string(mCoins);
		mValueCoinText = NULL;
		mValueCoinText = new Text(mRenderer, coins, 40, "Stat", Vector2D(420, 40));
		mValueCoinText->LoadFromFile();
		mScore = mScore + 200;
		string score = to_string(mScore);
		mValueScoreText = NULL;
		mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
		mValueScoreText->LoadFromFile();
		coin->SetFrameTime(150.0f);
		coin->SetStateFrame(1);
		coin->SetFrame(0);
	}
}

void GameScreenLevel1::UpdateEnemy(Character* character1, Enemy* enemy)
{
	int CharTop = character1->GetCollisionBox().y;
	int CharBottom = character1->GetCollisionBox().y + character1->GetCollisionBox().h;
	int CharLeft = character1->GetCollisionBox().x;
	int CharRight = character1->GetCollisionBox().x + character1->GetCollisionBox().w;
	int EnemyTop = enemy->GetCollisionBox().y;
	int EnemyBottom = enemy->GetCollisionBox().y + enemy->GetCollisionBox().h;
	int EnemyLeft = enemy->GetCollisionBox().x;
	int EnemyRight = enemy->GetCollisionBox().x + enemy->GetCollisionBox().w;

	if (Collisions::Instance()->Box(character1->GetCollisionBox(), enemy->GetCollisionBox()) && !enemy->GetBeenHit() && !character1->GetDying() && !character1->GetChangingDown() && !character1->GetChangingUp())
	{
		if (character1->GetJumping() && CharBottom > EnemyTop && CharBottom < EnemyBottom - (enemy->GetCollisionBox().h / 5 * 4)  && ((CharRight > EnemyLeft && CharRight < EnemyRight) || (CharLeft < EnemyRight && CharLeft > EnemyLeft)))
		{
			if (enemy->GetEnemyType() == "Goomba")
			{
				int scoreIncrease = 0;
				if (character1->GetAirKills() == 0)
					scoreIncrease = 100;
				else if (character1->GetAirKills() == 1)
					scoreIncrease = 200;
				else if (character1->GetAirKills() == 2)
					scoreIncrease = 400;
				else if (character1->GetAirKills() == 3)
					scoreIncrease = 500;
				else if (character1->GetAirKills() == 4)
					scoreIncrease = 800;
				else if (character1->GetAirKills() == 5)
					scoreIncrease = 1000;
				mScore = mScore + scoreIncrease;
				string score = to_string(mScore);
				string scoreAddition = to_string(scoreIncrease);
				mValueScoreText = NULL;
				mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
				mValueScoreText->LoadFromFile();
				TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(enemy->GetPosition().x, enemy->GetPosition().y - 36));
				TempScores[TempScoreCounter]->LoadFromFile();
				TempScoreCounter++;
				mStompGoomba->Play();
				enemy->SetDeathType("Squash");
				enemy->SetBeenHit(true);
				enemy->SetFrame(2);
				character1->SetJumping(false);
				character1->SetOnPlatform(true);
				character1->SetCollidingY(true);
				character1->Jump(300.0f);
				character1->IncreaseAirKills();
			}

			if (enemy->GetEnemyType() == "Koopa")
			{
				if (enemy->GetState() != "Shell")
				{
					int scoreIncrease = 0;
					if (character1->GetAirKills() == 0)
						scoreIncrease = 100;
					else if (character1->GetAirKills() == 1)
						scoreIncrease = 200;
					else if (character1->GetAirKills() == 2)
						scoreIncrease = 400;
					else if (character1->GetAirKills() == 3)
						scoreIncrease = 500;
					else if (character1->GetAirKills() == 4)
						scoreIncrease = 800;
					else if (character1->GetAirKills() == 5)
						scoreIncrease = 1000;
					mScore = mScore + scoreIncrease;
					string score = to_string(mScore);
					string scoreAddition = to_string(scoreIncrease);
					mValueScoreText = NULL;
					mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
					mValueScoreText->LoadFromFile();
					TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(enemy->GetPosition().x, enemy->GetPosition().y - 36));
					TempScores[TempScoreCounter]->LoadFromFile();
					TempScoreCounter++;
					mStompGoomba->Play();
					enemy->SetState("SafeShell");
					enemy->SetFrame(2);
					enemy->SetMovementSpeed(300.0f);
					enemy->SetPosition(Vector2D(enemy->GetCollisionBox().x, enemy->GetCollisionBox().y - 4));
					enemy->SetStationary(true);
					character1->SetJumping(false);
					character1->SetOnPlatform(true);
					character1->SetCollidingY(true);
					character1->SetPositionY(EnemyTop - character1->GetCollisionBox().h - 5);
					character1->Jump(300.0f);
					character1->IncreaseAirKills();
				}

				else 
				{
					int scoreIncrease = 0;
					if (character1->GetAirKills() == 0)
						scoreIncrease = 100;
					else if (character1->GetAirKills() == 1)
						scoreIncrease = 200;
					else if (character1->GetAirKills() == 2)
						scoreIncrease = 400;
					else if (character1->GetAirKills() == 3)
						scoreIncrease = 500;
					else if (character1->GetAirKills() == 4)
						scoreIncrease = 800;
					else if (character1->GetAirKills() == 5)
						scoreIncrease = 1000;
					mScore = mScore + scoreIncrease;
					string score = to_string(mScore);
					string scoreAddition = to_string(scoreIncrease);
					mValueScoreText = NULL;
					mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
					mValueScoreText->LoadFromFile();
					TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(enemy->GetPosition().x, enemy->GetPosition().y - 36));
					TempScores[TempScoreCounter]->LoadFromFile();
					TempScoreCounter++;
					mStompGoomba->Play();
					enemy->SetDeathType("Fireball");
					enemy->Jump(300.0f);
					enemy->SetBeenHit(true);
					character1->SetJumping(false);
					character1->SetOnPlatform(true);
					character1->SetCollidingY(true);
					character1->SetPositionY(EnemyTop - character1->GetCollisionBox().h - 5);
					character1->Jump(300.0f);
					character1->IncreaseAirKills();
				}
			}
		}

		else
		{
			if (character1->GetSize() == "Fire")
			{
				mPowerDown->Play();
				character1->SetChangeType("F->L");
				character1->ChangeSizeDown();
			}
			if (character1->GetSize() == "Large")
			{
				mPowerDown->Play();
				character1->SetChangeType("L->S");
				character1->ChangeSizeDown();
			}
			if (character1->GetSize() == "Small")
			{
				mMarioDies->Play();
				character1->SetJumping(false);
				character1->SetOnPlatform(true);
				character1->SetCollidingY(true);
				character1->Jump(700.0f);
				character1->SetDying(true);
			}
		}
	}
}

void GameScreenLevel1::UpdateShell(Character* character1, Enemy* enemy)
{
	int CharCenterX = character1->GetCollisionBox().x + character1->GetCollisionBox().w / 2;
	int ShellCenterX = enemy->GetCollisionBox().x + enemy->GetCollisionBox().w / 2;

	if (Collisions::Instance()->Box(character1->GetCollisionBox(), enemy->GetCollisionBox()))
	{
		if (CharCenterX <= ShellCenterX)
		{
			enemy->SetFacingDirection(FACING_RIGHT);
		}

		if (CharCenterX > ShellCenterX)
		{
			enemy->SetFacingDirection(FACING_LEFT);
		}

		mStompGoomba->Play();
		enemy->SetStationary(false);
		enemy->SetChanging(true);
	}
}

void GameScreenLevel1::UpdateFireball(Fireball* fireball, Enemy* enemy)
{
	if (Collisions::Instance()->Box(fireball->GetCollisionBox(), enemy->GetCollisionBox()) && !fireball->GetBeenHit() && !enemy->GetBeenHit())
	{
		string scoreAddition = "100";
		mScore = mScore + 100;
		string score = to_string(mScore);
		mValueScoreText = NULL;
		mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(65, 40));
		mValueScoreText->LoadFromFile();
		TempScores[TempScoreCounter] = new Text(mRenderer, scoreAddition, 25, "Temp", Vector2D(enemy->GetPosition().x, enemy->GetPosition().y - 36));
		TempScores[TempScoreCounter]->LoadFromFile();
		TempScoreCounter++;
		mStompGoomba->Play();
		fireball->SetPosition(Vector2D(enemy->GetPosition().x + 8, enemy->GetPosition().y + 8));
		fireball->SetBeenHit(true);
		fireball->SetFrame(3);
		enemy->SetDeathType("Fireball");
		enemy->Jump(300.0f);
		enemy->SetBeenHit(true);
	}
}

void GameScreenLevel1::UpdateBounce(Brick* brick, int PlayerNum)
{
	if (PlayerNum == 1)
	{
		if (Characters[1]->TopHit(brick->GetCollisionBox()))
		{
			Characters[1]->SetTopBrick(brick->GetCollisionBox());
			Characters[1]->Jump(350.0f);
		}
	}
	if (PlayerNum == 2)
	{
		if (Characters[0]->TopHit(brick->GetCollisionBox()))
		{
			Characters[0]->SetTopBrick(brick->GetCollisionBox());
			Characters[0]->Jump(350.0f);
		}
	}

	for (int i = 0; i < MushroomCounter; i++)
	{
		if (Mushrooms[i]->TopHit(brick->GetCollisionBox()) && brick->GetType() == "Brick")
		{
			Mushrooms[i]->ChangeDirection();
		}
	}

	mBumpSound->Play();
	brick->Bounce();
}

void GameScreenLevel1::CheckCollidingCharacter(Character* character)
{
	int groupCounterA = 0;
	int groupCounterB = 0;
	int OnScreenBrickCounter = 0;

	int center = mPowBlock->GetCollisionBox().x + mPowBlock->GetCollisionBox().w / 2;
	if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
	{
		OnScreenBrickCounter += 1;
		character->CheckOnPlatform(mPowBlock->GetCollisionBox());
		if (!character->CheckCollidingX(mPowBlock->GetCollisionBox()))
		{
			groupCounterB++;
		}
		if (character->GetCollidingY())
		{
			if (!character->CheckCollidingY(mPowBlock->GetCollisionBox()))
			{
				groupCounterA++;
			}
		}
	}

	for (int z = 0; z < BrickCounter; z++)
	{
		int center = Bricks[z]->GetCollisionBox().x + Bricks[z]->GetCollisionBox().w / 2;
		if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
		{
			OnScreenBrickCounter += 1;
			character->CheckOnPlatform(Bricks[z]->GetCollisionBox());

			if (!character->CheckCollidingX(Bricks[z]->GetCollisionBox()))
			{
				groupCounterB++;
			}
			if (character->GetCollidingY())
			{
				if (!character->CheckCollidingY(Bricks[z]->GetCollisionBox()))
				{
					groupCounterA++;
				}
			}
		}
	}

	if (groupCounterA == OnScreenBrickCounter)
	{
		character->SetCollidingY(false);
	}
	if (groupCounterB == OnScreenBrickCounter)
	{
		character->SetCollidingX(false);
	}
}

void GameScreenLevel1::CheckCollidingMushroom(Mushroom* mushy)
{
	int groupCounterA = 0;
	int groupCounterB = 0;
	mushy->CheckOnPlatform(mPowBlock->GetCollisionBox());
	mushy->CheckCollidingX(mPowBlock->GetCollisionBox());
	if (!mushy->CheckCollidingX(mPowBlock->GetCollisionBox()))
	{
		groupCounterB++;
	}
	if (mushy->GetCollidingY())
	{
		if (!mushy->CheckCollidingY(mPowBlock->GetCollisionBox()))
		{
			groupCounterA++;
		}
	}

	for (int z = 0; z < BrickCounter; z++)
	{
		mushy->CheckOnPlatform(Bricks[z]->GetCollisionBox());

		mushy->CheckCollidingX(Bricks[z]->GetCollisionBox());

		if (!mushy->CheckCollidingX(Bricks[z]->GetCollisionBox()))
		{
			groupCounterB++;
		}
		if (mushy->GetCollidingY())
		{
			if (!mushy->CheckCollidingY(Bricks[z]->GetCollisionBox()))
			{
				groupCounterA++;
			}
		}
	}

	if (groupCounterA == BrickCounter + 1)
	{
		mushy->SetCollidingY(false);
	}
	if (groupCounterB == BrickCounter + 1)
	{
		mushy->SetCollidingX(false);
	}
}

void GameScreenLevel1::CheckCollidingFireball(Fireball* fireball)
{
	int groupCounterA = 0;
	int groupCounterB = 0;
	int OnScreenBrickCounter = 0;

	int center = mPowBlock->GetCollisionBox().x + mPowBlock->GetCollisionBox().w / 2;
	if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
	{
		OnScreenBrickCounter += 1;
		fireball->CheckOnPlatform(mPowBlock->GetCollisionBox());
		if (!fireball->CheckCollidingX(mPowBlock->GetCollisionBox()))
		{
			groupCounterB++;
		}
		if (fireball->GetCollidingY())
		{
			if (!fireball->CheckCollidingY(mPowBlock->GetCollisionBox()))
			{
				groupCounterA++;
			}
		}
	}

	for (int z = 0; z < BrickCounter; z++)
	{
		int center = Bricks[z]->GetCollisionBox().x + Bricks[z]->GetCollisionBox().w / 2;
		if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
		{
			OnScreenBrickCounter += 1;
			fireball->CheckOnPlatform(Bricks[z]->GetCollisionBox());

			if (!fireball->CheckCollidingX(Bricks[z]->GetCollisionBox()))
			{
				groupCounterB++;
			}
			if (fireball->GetCollidingY())
			{
				if (!fireball->CheckCollidingY(Bricks[z]->GetCollisionBox()))
				{
					groupCounterA++;
				}
			}
		}
	}

	if (groupCounterA == OnScreenBrickCounter)
	{
		fireball->SetCollidingY(false);
	}
	if (groupCounterB == OnScreenBrickCounter)
	{
		fireball->SetCollidingX(false);
	}
}

void GameScreenLevel1::DoScreenshake()
{
	mScreenshake = true;
	mScreenShakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;
}

void GameScreenLevel1::DeleteItemsFromList()
{
	for (int i = 0; i < FallingBrickCounter; i++)
	{
		if (FallingBricks[i]->GetDead())
		{
			for (int q = i; q < FallingBrickCounter; q++)
			{
				FallingBricks[q] = FallingBricks[q + 1];
			}
			FallingBrickCounter--;
		}
	}

	for (int i = 0; i < MushroomCounter; i++)
	{
		if (Mushrooms[i]->GetDead())
		{
			for (int q = i; q < MushroomCounter; q++)
			{
				Mushrooms[q] = Mushrooms[q + 1];
			}
			MushroomCounter--;
		}
	}

	for (int i = 0; i < BrickCounter; i++)
	{
		if (Bricks[i]->GetDead())
		{
			for (int q = i; q < BrickCounter; q++)
			{
				Bricks[q] = Bricks[q + 1];
			}
			BrickCounter--;
		}
	}

	for (int i = 0; i < CoinCounter; i++)
	{
		if (Coins[i]->GetDead())
		{
			for (int q = i; q < CoinCounter; q++)
			{
				Coins[q] = Coins[q + 1];
			}
			CoinCounter--;
		}
	}

	for (int i = 0; i < EnemyCounter; i++)
	{
		if (Enemies[i]->GetDead())
		{
			for (int q = i; q < EnemyCounter; q++)
			{
				Enemies[q] = Enemies[q + 1];
			}
			EnemyCounter--;
		}
	}

	for (int i = 0; i < FireballCounter; i++)
	{
		if (Fireballs[i]->GetDead())
		{
			for (int q = i; q < FireballCounter; q++)
			{
				Fireballs[q] = Fireballs[q + 1];
			}
			FireballCounter--;
		}
	}

	for (int i = 0; i < TempScoreCounter; i++)
	{
		if (TempScores[i]->GetDead())
		{
			for (int q = i; q < TempScoreCounter; q++)
			{
				TempScores[q] = TempScores[q + 1];
			}
			TempScoreCounter--;
		}
	}
}

void GameScreenLevel1::Render()
{
	if (!mLevelFinished)
	{
		//Draw the background
		mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), mCamera->GetCameraRect(), SDL_FLIP_NONE, 0.0f, NULL);

		//Draw the player
		for (int i = 0; i < BrickCounter; i++)
		{
			int center = Bricks[i]->getPosition().x + Bricks[i]->GetCollisionBox().w / 2;
			if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
				Bricks[i]->Render();
		}
		for (int i = 0; i < MushroomCounter; i++)
		{
			int center = Mushrooms[i]->getPosition().x + Mushrooms[i]->GetCollisionBox().w / 2;
			if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
				Mushrooms[i]->Render();
		}
		for (int i = 0; i < FallingBrickCounter; i++)
		{
			FallingBricks[i]->Render();
		}
		for (int i = 0; i < CoinCounter; i++)
		{
			int center = Coins[i]->GetCollisionBox().x + Coins[i]->GetCollisionBox().w / 2;
			if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
				Coins[i]->Render();
		}
		for (int i = 0; i < FireballCounter; i++)
		{
			int center = Fireballs[i]->GetPosition().x + Fireballs[i]->GetCollisionBox().w / 2;
			if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
				Fireballs[i]->Render();
		}
		for (int i = 0; i < EnemyCounter; i++)
		{
			int center = Enemies[i]->GetPosition().x + Enemies[i]->GetCollisionBox().w / 2;
			if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
				Enemies[i]->Render();
		}
		for (int i = 0; i < 2; i++)
		{
			Characters[i]->Render();
		}
		for (int i = 0; i < TempScoreCounter; i++)
		{
			TempScores[i]->Render(mCamera->GetCameraRect());
		}

		int center = mPowBlock->GetCollisionBox().x + mPowBlock->GetCollisionBox().w / 2;
		if (center >= mCamera->GetCameraRect().x - 100 && center <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
			mPowBlock->Render();

		int centerB = mFlag->GetPosition().x + 30;
		if (centerB >= mCamera->GetCameraRect().x - 100 && centerB <= mCamera->GetCameraRect().x + mCamera->GetCameraRect().w + 100)
			mFlag->Render();

		mScoreText->Render(mCamera->GetCameraRect());
		mValueScoreText->Render(mCamera->GetCameraRect());
		mCoinText->Render(mCamera->GetCameraRect());
		mValueCoinText->Render(mCamera->GetCameraRect());
		mCountText->Render(mCamera->GetCameraRect());
		mValueCountText->Render(mCamera->GetCameraRect());
		if (!mStarted)
		{
			mStartText->Render(mCamera->GetCameraRect());
		}
	}
	else
	{
		mLeaderboardBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, SDL_FLIP_NONE, 0.0f, NULL);
		if (mValueYouScoredText == NULL)
		{
			string score = to_string(mScore);
			mValueYouScoredText = new Text(mRenderer, score, 40, "Stat", Vector2D(SCREEN_WIDTH / 2 - 90, 70));
			mValueYouScoredText->LoadFromFile();
		}
		if (mEndScreen == "You Win")
		{
			mYouWinText->Render(mCamera->GetCameraRect());
		}
		else
		{
			mYouLoseText->Render(mCamera->GetCameraRect());
		}
		mYouScoredText->Render(mCamera->GetCameraRect());
		mValueYouScoredText->Render(mCamera->GetCameraRect());
		for (int i = 0; i < 2; i++)
		{
			Characters[i]->Render();
		}
		if (!mLeaderboardSorted)
		{
			for (int i = 0; i < 2; i++)
			{
				Characters[i]->SetEndScreen(true);
			}
			Characters[0]->SetPosition(Vector2D(0 + Characters[0]->GetCollisionBox().w * 2, FLOOR_HEIGHT - Characters[0]->GetCollisionBox().h));
			Characters[1]->SetPosition(Vector2D(0, FLOOR_HEIGHT - Characters[1]->GetCollisionBox().h));
			mLeaderboard = new Leaderboard(mName, mScore);
			mLeaderboard->SortLeaderboard();
			stringstream leaderboard;
			//vector <string> scores; // Creates vector

			ifstream infile("Leaderboard.txt"); // Open file
			if (!infile) // If file cannot be opened
			{
				cerr << "File could not be opened!";
				exit(1);
			}
			else // If file can be opened
			{
				infile.seekg(0, ios::end);
				int len = infile.tellg(); // Finds length 
				if (len == 0) // If empty
				{
					mScores.push_back("There are no scores");
				}
				else // If not empty
				{
					infile.seekg(0, ios::beg);
					string line;
					while (getline(infile, line)) // Loops through all lines
					{
						mScores.push_back(line); // Adds line to vector
					}
				}
			}
			infile.close(); // Closes file
			for (int i = 0; i < 10; i++)
			{
				LeaderboardText[i] = new Text(mRenderer, mScores[i], 40, "Stat", Vector2D(SCREEN_WIDTH / 2 - 200, i * 40 + 180));
				LeaderboardText[i]->LoadFromFile();
				//cout << mScores[i] << endl;
			}
			mLeaderboardSorted = true;
		}
		for (int i = 0; i < 10; i++)
		{
			LeaderboardText[i]->Render(mCamera->GetCameraRect());
		}
	}
}

void GameScreenLevel1::SetLevelMap(string path)
{
	int map[MAP_HEIGHT][MAP_WIDTH] = {};
	ifstream inFile(path); // Opens file
	if (!inFile) // If it cant be opened
	{
		cerr << "File could not be opened!";
		exit(1);
	}
	else // If it can be opened
	{
		string line;
		int height = -1;
		while (getline(inFile, line)) // Loops through every line in the file
		{
			height++;
			int width = -1;
			for (int i = 0; i < line.length(); i++) // Loops through every character in the line
			{
				width++;
				int linei = line[i] - '0';
				map[height][width] = linei;
				//cout << map[height][width] << " ";
			}
			//cout << endl;
		}
	}

	//Clear up any old map
	if (mLevelMap != NULL)
	{
		delete mLevelMap;
	}

	//Set the new one
	mLevelMap = new LevelMap(map);

	inFile.close();
}

bool GameScreenLevel1::SetUpLevel()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (mLevelMap->GetTileAt(i, j) == 9)
			{
				if (Characters[0] == NULL)
				{
					Characters[0] = new Character(mRenderer, mCamera, "Images/Mario_walking_sheet5.png", Vector2D(j * 48, i * 48), 1, mLevelMap);
				}
				else 
				{
					Characters[1] = new Character(mRenderer, mCamera, "Images/Luigi_walking_sheet3.png", Vector2D(j * 48, i * 48), 2, mLevelMap);
				}
				
			}
				
			if (mLevelMap->GetTileAt(i, j) == 1)
			{
				Bricks[BrickCounter] = new Brick(mRenderer, mCamera, "Images/Brick.png", Vector2D(j * 48, i * 48), "Brick", NULL);
				BrickCounter++;
			}

			if (mLevelMap->GetTileAt(i, j) == 2)
			{
				Bricks[BrickCounter] = new Brick(mRenderer, mCamera, "Images/QuestionBlock.png", Vector2D(j * 48, i * 48), "Question", NULL);
				Mushrooms[MushroomCounter] = new Mushroom(mRenderer, mCamera, mLevelMap, Bricks[BrickCounter]);
				BrickCounter++;
				MushroomCounter++;
			}

			if (mLevelMap->GetTileAt(i, j) == 3)
			{
				Bricks[BrickCounter] = new Brick(mRenderer, mCamera, "Images/QuestionBlock.png", Vector2D(j * 48, i * 48), "Question", NULL);
				Coins[CoinCounter] = new Coin(mRenderer, mCamera, mLevelMap, Bricks[BrickCounter], Vector2D((j * 48) + 9, (i * 48) + 9), "Block");
				BrickCounter++;
				CoinCounter++;
			}

			if (mLevelMap->GetTileAt(i, j) == 4)
			{
				Bricks[BrickCounter] = new Brick(mRenderer, mCamera, "Images/Brick2.png", Vector2D(j * 48, i * 48 + 11), "Brick", NULL);
				BrickCounter++;
			}

			if (mLevelMap->GetTileAt(i, j) == 5)
			{
				Coins[CoinCounter] = new Coin(mRenderer, mCamera, mLevelMap, NULL, Vector2D((j * 48)+9, (i * 48)+9), "Still");
				CoinCounter++;
			}

			if (mLevelMap->GetTileAt(i, j) == 6)
			{
				Enemies[EnemyCounter] = new Enemy(mRenderer, mCamera, mLevelMap, "Images/Goomba.png", Vector2D(j * 48, i * 48), "Goomba");
				EnemyCounter++;
			}

			if (mLevelMap->GetTileAt(i, j) == 8)
			{
				Enemies[EnemyCounter] = new Enemy(mRenderer, mCamera, mLevelMap, "Images/Koopa.png", Vector2D(j * 48, i * 48), "Koopa");
				EnemyCounter++;
			}

			if (mLevelMap->GetTileAt(i, j) == 7)
			{
				bool TubeAbove = false;
				bool TubeBelow = false;
				int frame = 0;
				if (mLevelMap->GetTileAt(i + 1, j) == 7)
				{
					TubeBelow = true;
				}
				if (mLevelMap->GetTileAt(i - 1, j) == 7)
				{
					TubeAbove = true;
				}
				if (TubeAbove)
					frame = 1;
				Bricks[BrickCounter] = new Brick(mRenderer, mCamera, "Images/Tube.png", Vector2D(j * 48, i * 48), "Tube", frame);
				BrickCounter++;
			}
		}
	}

	mPowBlock = new PowBlock(mRenderer, mCamera, mLevelMap, Vector2D(15 * 48, 15 * 48));
	mFlag = new Flag(mRenderer, mCamera, "Images/Flag.png", Vector2D(162 * 48 + 12, 3 * 48 + 8));

	mScreenshake = false;
	mBackgroundYPos = 0.0f;
	mScore = 0;
	mCoins = 0;
	mStarted = false;
	mCountdown = 250.0f;
	mICountdown = 0;
	mPrevcount = 0;
	mOutOfTimePlayed = false;
	mValueYouScoredText = NULL;
	mFlagScoreAdded = false;
	mLeaderboardSorted = false;

	LoadMusic("Music/Mario.wav");

	mPowerUpSound = new SoundEffect();
	mPowerUpSound->LoadFromFile("Sounds/PowerUp.WAV");
	mMushSpawnSound = new SoundEffect();
	mMushSpawnSound->LoadFromFile("Sounds/MushSpawn.WAV");
	mBreakingBrickSound = new SoundEffect();
	mBreakingBrickSound->LoadFromFile("Sounds/BreakBrick.WAV");
	mCoinSound = new SoundEffect();
	mCoinSound->LoadFromFile("Sounds/Coin.WAV");
	mKickShellSound = new SoundEffect();
	mKickShellSound->LoadFromFile("Sounds/KickShell.WAV");
	//mPauseSound = new SoundEffect();
	//mPauseSound->LoadFromFile("Sounds/Pause.WAV");
	mShellHitsWall = new SoundEffect();
	mShellHitsWall->LoadFromFile("Sounds/ShellHitsWall.WAV");
	mStompGoomba = new SoundEffect();
	mStompGoomba->LoadFromFile("Sounds/KickShell.WAV");
	mBumpSound = new SoundEffect();
	mBumpSound->LoadFromFile("Sounds/Bump.WAV");
	mFireballSound = new SoundEffect();
	mFireballSound->LoadFromFile("Sounds/Fireball.WAV");
	mPowerDown = new SoundEffect();
	mPowerDown->LoadFromFile("Sounds/PowerDown.WAV");
	mMarioDies = new SoundEffect();
	mMarioDies->LoadFromFile("Sounds/MarioDies.WAV");
	mRunningOutOfTime = new SoundEffect();
	mRunningOutOfTime->LoadFromFile("Sounds/OutOfTime.WAV");
	mReachedCastleSound = new SoundEffect();
	mReachedCastleSound->LoadFromFile("Sounds/StageClear.WAV");
	mGameOverSound = new SoundEffect();
	mGameOverSound->LoadFromFile("Sounds/GameOver.WAV");

	mScoreText = new Text(mRenderer, "SCORE", 40, "Stat", Vector2D(40, 10));
	mScoreText->LoadFromFile();
	string score = to_string(mScore);
	mValueScoreText = new Text(mRenderer, score, 40, "Stat", Vector2D(100, 40));
	mValueScoreText->LoadFromFile();
	mCoinText = new Text(mRenderer, "COINS", 40, "Stat", Vector2D(360, 10));
	mCoinText->LoadFromFile();
	string coins = to_string(mCoins);
	mValueCoinText = new Text(mRenderer, coins, 40, "Stat", Vector2D(420, 40));
	mValueCoinText->LoadFromFile();
	mStartText = new Text(mRenderer, "PRESS ENTER TO BEGIN", 40, "Stat", Vector2D(375, 220));
	mStartText->LoadFromFile();
	mCountText = new Text(mRenderer, "TIME", 40, "Stat", Vector2D(710, 10));
	mCountText->LoadFromFile();
	int iCountdown = int(mCountdown);
	string time = to_string(iCountdown);
	mValueCountText = new Text(mRenderer, time, 40, "Stat", Vector2D(725, 40));
	mValueCountText->LoadFromFile();
	mYouWinText = new Text(mRenderer, "YOU WIN!", 40, "Stat", Vector2D(SCREEN_WIDTH / 2 - 150, 10));
	mYouWinText->LoadFromFile();
	mYouLoseText = new Text(mRenderer, "YOU LOSE!", 40, "Stat", Vector2D(SCREEN_WIDTH / 2 - 150, 10));
	mYouLoseText->LoadFromFile();
	mYouScoredText = new Text(mRenderer, "YOU SCORED", 40, "Stat", Vector2D(SCREEN_WIDTH / 2 - 180, 40));
	mYouScoredText->LoadFromFile();

	//Load the background texture
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("Images/Background4.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}
	mLeaderboardBackgroundTexture = new Texture2D(mRenderer);
	mLeaderboardBackgroundTexture->LoadFromFile("Images/Leaderboard.png");
	return true;
}