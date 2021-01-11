#include "Cherry.h"
#include <sstream>

Cherry::Cherry() : GameObject()
{
	LoadContent();
}

Cherry::~Cherry()
{
}

void Cherry::LoadContent()
{
	// Load Munchie
	_texture = new Texture2D();
	_texture->Load("Textures/Cherry.png", true);
	_position = new Vector2(240, 320);
	_sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_colliderRect = new Rect(0.0f, 0.0f, 18, 28);

	//declare vars to animate munchie
	_frameTime = rand() % 150 + 350;

	//other vars
	_pointsValue = 25;
}

void Cherry::Update(int elapsedTime)
{
	Animate(elapsedTime);
}

void Cherry::Draw(int elapsedTime)
{
	//animate munchie
	//move source rect position
	_sourceRect->X = _sourceRect->Width * _animationCount;

	// Draws Red Munchie
	SpriteBatch::Draw(_texture, _position, _sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
}

int Cherry::GetPointsValue()
{
	return _pointsValue;
}