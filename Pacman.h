#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "GameObject.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public GameObject
{
private:
	//Animate pacman
	int _pacmanDirection;

	//Constant Variables
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;

	//Player Vars
	Vector2* _originalPos;
	int _points = 0;
	int _player;
	bool _playerKilled;
	bool _readyToKill;

	//speed
	float _speedIncreaser;
	float _speedDecreaser;
	int _iSpeedCount;
	int _dSpeedCount;
	bool _iSpeedChanged;
	bool _dSpeedChanged;

	//other vars
	float _powerUpTimeLimit;
	bool _blocked;
	bool _bonusLevel;

	//Methods
	void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);
	void Pacman::CheckViewPortCollision();
	void Pacman::Animate(int elapsedTime);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman();

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime, Input::MouseState* mouseState);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	bool _paused;

	int Pacman::GetPoints();

	void Pacman::SetPoints(int newPoints);
	void Pacman::SetPlayerNum(int playerNum);
	void Pacman::SetBlocked(bool blocked);
	void Pacman::SetBonusLevel(bool bonusLevel);
	void Pacman::ItemCollected(int pointsGained);
	void Pacman::SetSpeedIncreaser(float speedChange);
	void Pacman::SetSpeedDecreaser(float speedChange);
	void Pacman::SetPlayerKilled(bool playerKilled);
	void Pacman::SetOriginalPosition(float x, float y);
	void Pacman::Restart();
};