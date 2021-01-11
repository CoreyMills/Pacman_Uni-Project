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

// Declares the Speed_PowerUp class which inherits from the GameObject class.
// This allows us to overload the GameObject class methods to help us
// load content, draw and update our game.
class Speed_PowerUp : public GameObject
{
private:
	//powerUps
	float _effect;

	//Methods
	void Animate(int elapsedTime);

public:
	/// <summary> Constructs the Speed_PowerUp class. </summary>
	Speed_PowerUp();

	/// <summary> Destroys any data associated with Speed_PowerUp class. </summary>
	virtual ~Speed_PowerUp();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	float Speed_PowerUp::GetEffect();
};