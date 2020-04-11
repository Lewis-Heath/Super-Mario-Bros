#include "Leaderboard.h"

Leaderboard::Leaderboard(string name, int score)
{
	mName = name;
	mScore = score;
}

Leaderboard::~Leaderboard()
{
}

int Leaderboard::indexofSmallestElement(int array[], int size) // Returns the smallest index of the array passed in.
{
	int index = 0;

	if (size != 1)
	{

		int n = array[0];
		for (int i = 1; i < size; i++)
		{
			if (array[i] < n)
			{
				n = array[i];
				index = i;
			}
		}
	}
	return index;
}

void Leaderboard::SortLeaderboard()
{
	int score;
	int counter = 0;
	ifstream infile("Leaderboard.txt"); // Opens file
	if (!infile) // If file cannot be opened
	{
		cerr << "File could not be opened!";
		exit(1);
	}
	else // If file is opened
	{
		string line;
		string names[10] = {}; // Creates array
		int values[10] = {}; // Creates array
		while (getline(infile, line)) // Loops through everything in file
		{
			string a;
			a = line;
			int posOfSpace = 0;
			for (int i = 0; i < a.length(); i++) // Loops through everything in the line
			{
				if (a[i] == ' ') // If there is a gap
				{
					posOfSpace = i; // Set pos of space to i
				}
			}
			if (counter == 10) // If counter is 10
				break; // Break loop
			names[counter] = line.substr(0, posOfSpace); // Sets name
			values[counter] = stoi(line.substr(posOfSpace + 1, line.length())); // Sets value
			counter++; // Increases counter
		}
		infile.close(); // Closes file
		int indexSmallestNum = indexofSmallestElement(values, 10); // Sets smallest number index
		if (values[9] < mScore) //If player score is higher than the lowest score
		{
			names[indexSmallestNum] = mName; // Adds name to array
			values[indexSmallestNum] = mScore; // Adds score to array
		}
		int n = sizeof(values) / sizeof(values[0]); // Sets size
		int temp;
		string temp2;

		// Loops through all values in values and orders them and also orders the names to correspond
		for (int i = 0; i < n; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				if (values[i] > values[j])
				{
					temp = values[i];
					temp2 = names[i];
					values[i] = values[j];
					names[i] = names[j];
					values[j] = temp;
					names[j] = temp2;
				}
			}
		}
		ofstream outFile("Leaderboard.txt", ios::trunc); // Opens file as trunc so clears it
		for (int i = 9; i > -1; i--) // Loops through array
		{
			outFile << names[i] << " " << values[i] << endl; // Adds names and values to file
		}
		outFile.close(); // Closes file
	}
}



