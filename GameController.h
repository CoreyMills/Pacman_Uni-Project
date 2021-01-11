#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include "GameObject.h"
#include "Pacman.h"
#include "Munchies.h"
#include "Cherry.h"
#include "Barrier.h"
#include "Speed_PowerUp.h"
#include "Slowness_PowerUp.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the GameController class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class GameController : public Game
{
private:
	// Position for String
	Vector2* _stringPlayer1Points;
	Vector2* _stringPlayer2Points;

	//Data for Pause Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;

	Vector2* _centrePos;

	//Bool Vars
	bool _paused;
	bool _pKeyDown;
	bool _gameStarted;

	//Methods
	void GameController::Input(int elapsedTime, Input::KeyboardState* state);
	bool GameController::Collision(Rect* rect1, Rect* rect2);
	void GameController::Collision(GameObject* movingObj, GameObject* immovableObj);
	void GameController::CheckCollisions(int elapsedTime);
	void GameController::SpawnMunchie(int playerNum);
	void GameController::SpawnCherry(int playerNum, int framesWaited);
	void GameController::SpawnBarrier(float x, float y);
	void GameController::SpawnPowerUp(int framesWaited);
	void GameController::Level(string levelF);
	void GameController::Restart();

	void GameController::SetPowerPosition(GameObject* powerUp);
	void GameController::SetPointsGoal(int pointsGoal);

	//mouse input vars
	Input::MouseState* _mouseState;

	//Pacman vars
	Pacman* _pacman1;
	Pacman* _pacman2;

	//Munchies
	Munchies* _munchie1;
	Munchies* _munchie2;

	//Cherry
	Cherry* _cherry;

	//Barrier
	Barrier* _barrier;

	//freeSpace
	GameObject* _freeSpace;

	//PowerUp's
	vector<Speed_PowerUp*> _speedUpArray;
	Speed_PowerUp* _speedUp;

	vector<Slowness_PowerUp*> _speedDownArray;
	Slowness_PowerUp* _speedDown;

	int _currentPower;
	int _powerFramesWaited;
	int _levelToLoad;

	//cherry vars
	int _cherryFramesWaited1;
	int _cherryFramesWaited2;
	bool _cherryEaten1;
	bool _cherryEaten2;

	//Object Arrays
	vector<Munchies*> _munchieArray;
	vector<Cherry*> _cherryArray;
	vector<Barrier*> _barrierArray;
	vector<GameObject*> _freeSpaceArray1;
	vector<GameObject*> _freeSpaceArray2;

	//sound vars
	SoundEffect* _munchieEatSound;
	SoundEffect* _cherryEatSound;

	//other vars
	int _pointsGoal;

public:
	/// <summary> Constructs the GameController class. </summary>
	GameController(int argc, char* argv[], int levelToLoad, int pointsGoal);

	/// <summary> Destroys any data associated with GameController class. </summary>
	virtual ~GameController();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	void GameController::SetLevelToLoad(int levelToLoad);
};