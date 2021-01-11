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
class Ghost : public GameObject
{
private:
	//Move pacman
	int _direction;
	int _changeDirTime = 0;
	int _randTime = 0;
	int _randDir;

	//animate Pacman
	int _frameTime;
	int _currentFrameTime;

	//Constant Variables
	const float _cGhostSpeed;
	const float _cGhostFrameTime;

	//other vars
	bool _blocked;

	//route vars
	int _routeType;
	int _currentTarget;
	float _chaseDist;
	Vector2* _target;
	vector<Vector2*> _routeArray;
	Vector2* _originalPos;

	GameObject* _player;

	//Sound vars
	SoundEffect* _ghostNoise;

	//methods
	void Ghost::RandPatrol(int elapsedTime);
	void Ghost::PathPatrol(int elapsedTime);
	void Ghost::ChaseTarget(Vector2* target, int elapsedTime);

public:
	/// <summary> Constructs the Ghost class. </summary>
	Ghost();

	/// <summary> Destroys any data associated with Ghost class. </summary>
	virtual ~Ghost();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	//vars
	bool _paused;

	//my methods
	Vector2* Ghost::GetPosition();

	void Ghost::SetBlocked(bool blocked);
	void Ghost::SetGhost(int ghostNum);
	void Ghost::SetRouteType(int routeNum);
	void Ghost::SetChaseDist(int chaseDist);
	void Ghost::SetPlayerObject(GameObject* player);
	void Ghost::SetOriginalPosition(float x, float y);
	void Ghost::Restart();
};