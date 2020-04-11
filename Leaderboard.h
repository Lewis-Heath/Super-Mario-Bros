#ifndef _LEADERBOARD_H
#define _LEADERBOARD_H

#include "SDL.h"
#include "Commons.h"
#include "Constants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class Leaderboard
{
public:
	Leaderboard(string name, int score);
	~Leaderboard();

	int indexofSmallestElement(int array[], int size);
	void SortLeaderboard();

private:
	string mName;
	int mScore;
};

#endif // !_LEADERBOARD_H

