#ifndef _GAMESCREENLEVEL1_H
#define _GAMESCREENLEVEL1_H
#include "SDL.h"
#include "Commons.h"
#include "GameScreen.h"
#include "Camera.h"
#include "Character.h"
#include "Collisions.h"
#include "Brick.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "Mushroom.h"
#include "FallingBrick.h"
#include "Coin.h"
#include "Enemy.h"
#include "Fireball.h"
#include "SoundEffect.h"
#include "Flag.h"
#include "Text.h"
#include "Leaderboard.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Leaderboard;
class Enemy;
class Character;
class Texture2D;
class Text;
class GameScreenLevel1 : GameScreen
{
private:
	Camera* mCamera;
	Texture2D* mBackgroundTexture;
	Texture2D* mLeaderboardBackgroundTexture;
	Leaderboard* mLeaderboard;
	Text* mScoreText;
	Text* mValueScoreText;
	Text* mStartText;
	Text* mCoinText;
	Text* mValueCoinText;
	Text* mCountText;
	Text* mValueCountText;
	Text* mYouWinText;
	Text* mYouLoseText;
	Text* mYouScoredText;
	Text* mValueYouScoredText;
	Collisions* Collisions;
	LevelMap* mLevelMap;
	PowBlock* mPowBlock;
	Flag* mFlag;
	void SetLevelMap(string path);
	bool SetUpLevel();
	Text* TempScores[15];
	Text* LeaderboardText[9];
	Brick* Bricks[800];
	Mushroom* Mushrooms[10];
	Fireball* Fireballs[50];
	Character* Characters[2];
	FallingBrick* FallingBricks[200];
	Coin* Coins[50];
	Enemy* Enemies[100];
	SoundEffect* mPowerUpSound;
	SoundEffect* mBreakingBrickSound;
	SoundEffect* mCoinSound;
	SoundEffect* mFireballSound;
	SoundEffect* mKickShellSound;
	SoundEffect* mMushSpawnSound;
	SoundEffect* mPauseSound;
	SoundEffect* mShellHitsWall;
	SoundEffect* mStompGoomba;
	SoundEffect* mBumpSound;
	SoundEffect* mPowerDown;
	SoundEffect* mMarioDies;
	SoundEffect* mRunningOutOfTime;
	SoundEffect* mReachedCastleSound;
	SoundEffect* mGameOverSound;
	Mix_Music* gMusic;
	bool mScreenshake;
	float mScreenShakeTime;
	float mWobble;
	float mBackgroundYPos;
	bool mStarted;
	int BrickCounter;
	int MushroomCounter;
	int FallingBrickCounter;
	int CoinCounter;
	int EnemyCounter;
	int FireballCounter;
	int TempScoreCounter;
	float TimeSinceLastFireball;
	bool FireballSpawned;
	int mScore;
	int mCoins;
	float mCountdown;
	int mICountdown;
	int mPrevcount;
	bool mOutOfTimePlayed;
	bool mLevelFinished;
	string mEndScreen;
	bool mFlagScoreAdded;
	bool mLeaderboardSorted;
	string mName;
	vector <string> mScores;
	int mLeaderboardScoreCounter;

public:
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1();

	void Render();
	void Update(float deltaTime, SDL_Event e);
	void UpdatePOWBlock(Vector2D CharOldPos, Vector2D CharNewPos, Character* character1);
	void UpdateQuestionBlock(Vector2D CharOldPos, Vector2D CharNewPos, Character* character1, Brick* brick);
	void UpdateMushroom(Character* character1, Mushroom* mushy, int index);
	void UpdateCoin(Character* character1, Coin* coin);
	void UpdateEnemy(Character* character1, Enemy* enemy);
	void UpdateShell(Character* character1, Enemy* enemy);
	void UpdateFireball(Fireball* fireball, Enemy* enemy);
	void UpdateBounce(Brick* brick, int Player1Num);
	void CheckCollidingCharacter(Character* character);
	void CheckCollidingMushroom(Mushroom* mushy);
	void CheckCollidingFireball(Fireball* fireball);
	void DoScreenshake();
	void DeleteItemsFromList();
	void LoadMusic(string path);
};


#endif // !_GAMESCREENLEVEL1_H

