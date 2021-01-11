#include "Munchies.h"
#include <sstream>
#include <ctime>

Munchies::Munchies() : GameObject()
{
	LoadContent();
}

Munchies::~Munchies()
{
}

void Munchies::LoadContent()
{
	// Load Munchie
	_texture = new Texture2D();
	_texture->Load("Textures/Munchie.png", true);
	_position = new Vector2(720, 540);
	_sourceRect = new Rect(0.0f, 0.0f, 12, 12);
	_colliderRect = new Rect(0.0f, 0.0f, 10, 10);

	//declare vars to animate munchie
	_frameTime = rand() % 300 + 200;

	//other vars
	_pointsValue = 10;
}

void Munchies::Update(int elapsedTime)
{
	Animate(elapsedTime);
}

void Munchies::Draw(int elapsedTime)
{
	//animate munchie
	if (_animationCount == 0)
	{
		//move source rect position
		_sourceRect->X = _sourceRect->Width * _animationCount;
	}
	else
	{
		//move source rect position
		_sourceRect->X = _sourceRect->Width * _animationCount;
	}

	// Draws Red Munchie
	SpriteBatch::Draw(_texture, _position, _sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}

int Munchies::GetPointsValue()
{
	return _pointsValue;
}