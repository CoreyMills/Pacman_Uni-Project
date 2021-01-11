#include "GameObject.h"
#include <sstream>

GameObject::GameObject()
{
	LoadContent();
}

GameObject::~GameObject()
{
	// Data to represent Munchie
	delete _texture;
	delete _sourceRect;
	delete _position;

	//Collider vars
	delete _colliderRect;
}

void GameObject::LoadContent()
{
	// Load Munchie
	_texture = new Texture2D();
	_position = new Vector2(0, 0);
	_sourceRect = new Rect(0.0f, 0.0f, 0, 0);
	_colliderRect = new Rect(0.0f, 0.0f, 0, 0);

	//vars to animate munchie
	_currentFrameTime = 0;
	_frameTime = 0;
	_animationCount = 0;
}

void GameObject::Update(int elapsedTime)
{
}

void GameObject::Draw(int elapsedTime)
{

}

void GameObject::Animate(int elapsedTime)
{
	//increase frameCount
	_currentFrameTime += elapsedTime;

	if (_currentFrameTime > _frameTime)
	{
		_animationCount++;

		if (_animationCount >= 2)
		{
			_animationCount = 0;
		}

		_currentFrameTime = 0;
	}
}

Rect* GameObject::GetColliderRect()
{
	Rect* rect = new Rect;
	rect->Height = _colliderRect->Height;
	rect->Width = _colliderRect->Width;
	rect->X = _position->X;
	rect->Y = _position->Y;
	return rect;
}

Vector2* GameObject::GetPosition()
{
	return _position;
}

void GameObject::SetPosition(float x, float y)
{
	_position = new Vector2(x, y);
}