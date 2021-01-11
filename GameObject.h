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

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Cherry class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class GameObject
{
protected:
	// Data to represent Munchie
	Texture2D* _texture;
	Rect* _sourceRect;
	Vector2* _position;

	//Collider vars
	Rect* _colliderRect;

	//animate munchie
	int _frameTime;
	int _currentFrameTime;
	int _animationCount;

	//Methods
	virtual void Animate(int elapsedTime);

public:
	/// <summary> Constructs the GameObject class. </summary>
	GameObject();

	/// <summary> Destroys any data associated with GameObject class. </summary>
	virtual ~GameObject();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	Rect* GameObject::GetColliderRect();
	Vector2* GameObject::GetPosition();

	void GameObject::SetPosition(float x, float y);
};