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
#include "Ghost.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the GameController class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class BonusController : public Game
{
private:
	//Position for String
	Vector2* _stringPointsPos;

	//Data for Pause Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;

	Vector2* _centrePos;

	//Bool Vars
	bool _paused;
	bool _pKeyDown;
	bool _gameStarted;
	bool _playerKilled;
	bool _justDied;
	bool _justWon;

	//Methods
	void BonusController::Input(int elapsedTime, Input::KeyboardState* state);
	bool BonusController::Collision(Rect* colliderRect1, Rect* colliderRect2);
	void BonusController::Collision(GameObject* movingObj, GameObject* immovableObj);
	void BonusController::CheckCollisions();
	void BonusController::SpawnMunchie();
	void BonusController::SpawnCherry();
	void BonusController::SpawnBarrier(float x, float y);
	void BonusController::SpawnGhost(float xPos, float yPos, int routeType, int chaseDist, int ghostVersion);
	void BonusController::Level(string levelF);
	int BonusController::HighScore(int playerScore);
	void BonusController::DisplayHighscore(string title);
	void BonusController::Restart();
	

	//mouse input vars
	Input::MouseState* _mouseState;

	//Pacman vars
	Pacman* _pacman;

	//Ghost vars
	Ghost* _ghost;

	//Munchies vars
	Munchies* _munchie;

	//Cherry vars
	Cherry* _cherry;
	int _framesWaited;
	bool _cherryEaten;

	//Barrier vars
	Barrier* _barrier;

	//Object Arrays
	vector<Munchies*> _munchieArray;
	vector<Cherry*> _cherryArray;
	vector<Barrier*> _barrierArray;
	vector<Ghost*> _ghostArray;

	//sound vars
	SoundEffect* _munchieEatSound;
	SoundEffect* _cherryEatSound;
	SoundEffect* _deathSound;

	//score vars
	int _pointsGoal;
	int currentHighscore;

public:
	/// <summary> Constructs the BonusController class. </summary>
	BonusController(int argc, char* argv[]);

	/// <summary> Destroys any data associated with BonusController class. </summary>
	virtual ~BonusController();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};