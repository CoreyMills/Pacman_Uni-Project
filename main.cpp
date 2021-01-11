#include "GameController.h"
#include "BonusController.h"
#include<iostream>
#include <ctime>
using namespace std;

//Entry Point for Application
int main(int argc, char* argv[]) 
{
	srand(time(NULL));
	int gameChosen = 0, levelChosen = 0, pointsGoal;
	bool readyToStart = false;

	while (!readyToStart)
	{
		cout << "--Enter the number that corresponds with the level you want--" << endl;
		cout << "Pick a level: \n 1. Normal (2Player) \n 2. Bonus (1Player)" << endl;
		cin >> gameChosen;

		if (gameChosen == 1)
		{
			cout << "Pick target points: " << endl;
			cin >> pointsGoal;

			cout << "Pick a level:  \n 1.  \n 2.  \n 3." << endl;
			cin >> levelChosen;

			GameController* normalGame = new GameController(argc, argv, levelChosen, pointsGoal);
			readyToStart = true;
		}
		else if (gameChosen == 2)
		{
			BonusController* bonusGame = new BonusController(argc, argv);
			readyToStart = true;
		}
	}
}